Mark Guevara
704962920
CS35L
TA: Zhaowei Tan

Assignment 6: readme.txt
===================================

The provided ray tracing program works by going through each pixel
and calculating the color value based on the lighting and reflections off of
the spheres calculated in the scene.

Since each pixel is calculated independently of the others, and each requires
a large amount of calculation, it is perfect for parallelization.
The calculations are done within the main function. To start off, I moved the
bulk of the code into an external function:

void * generate_pixels(void * thread) { . . . }

This function is structured to work with pthread_create. It returns NULL
because it is a void * function, and takes an input void * thread that
tells the function which thread number it is. 

However, since previously when the code was in the main function it had 
access to the 'scene' struct, and they still need to access it, I moved it to
a global variable. Similarly, since the threads are operating in parallel,
there is no easy way to have them print the pixels in order like the original
main function does. To get around this, I created a global array:

float image_pixels[width][height][3];

This array allows each thread to write their respective pixel values 
simultaneously, and once they are finished they can all be printed in order as
is for the file format. 

I then restructured the main function to be similar to the wikipedia page
linked in the assignment (https://en.wikipedia.org/wiki/POSIX_Threads).
Each thread is declared, then processes the column numbers that are equal to
the corresponding thread number mod the thread count
(i.e. each thread n processes columns where col%nthreads == n).

When the threads finish, they are joined, then the program loops through the
array and outputs the pixel values in the correct format. 

In order to allow the file to compile, I edited the Makefile and added the
-lpthread parameter to the LDLIBS line:

LDLIBS = -lm -lpthread 

======================
Testing
======================
The results from "make clean check" are given here:

time ./srt 1-test.ppm >1-test.ppm.tmp

real    0m48.015s
user    0m48.006s
sys     0m0.001s

time ./srt 2-test.ppm >2-test.ppm.tmp

real    0m24.579s
user    0m49.008s
sys     0m0.003s

time ./srt 4-test.ppm >4-test.ppm.tmp

real    0m12.626s
user    0m49.771s
sys     0m0.003s

time ./srt 8-test.ppm >8-test.ppm.tmp

real    0m7.490s
user    0m54.724s
sys     0m0.006s

Running my own tests on higher thread counts gives the following results:

time ./srt 16-test.ppm >16-test.ppm.tmp

real    0m6.083s
user    1m23.995s
sys     0m0.005s

time ./srt 32-test.ppm >32-test.ppm.tmp

real    0m5.820s
user    1m25.309s
sys     0m0.006s

Clearly, up until 8 threads, the real processing time gives increasingly
faster results as the number of threads increases. Each time the number of
threads doubles, the real time is cut in half. Once the thread count goes
past 8, however, there are diminishing returns, as even though the total
real time does go down slightly, the user time on the CPU increases 
significantly due to the increased overhead that comes with multithreading.
The first 8 threads have roughly the same user time because the CPU can 
handle up to 8 threads at a time, but beyond that it is not worth utilizing
parallelism.

The files are identical, as running the following command shows:

diff baseline.ppm 1-test.ppm -s
Files baseline.ppm and 1-test.ppm are identical

