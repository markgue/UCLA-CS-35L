/* Mark Guevara
*  704962920
*  CS35L
*  TA: Zhaowei Tan
*  
*  Assignment 4: sfrob.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>


int frobcmp(char const *a, char const *b)
{
    /* Loop over all of the two words until their end */
    while (1)
    {
        if ((*a == ' ') && (*b == ' '))
        {
            return 0;
        }
        /* memfrob uses an XOR with 00101010 = 42 to transform */ 
        else if ((*a == ' ') || (*a ^ 42) < (*b ^ 42))
        {
            return -1;
        }
        else if ((*b == ' ') || (*a ^ 42) > (*b ^ 42))
        { 
            return 1;
        }
        a++;
        b++;
    }
}

int frobcmp_f(char const *a, char const *b)
{
	if ((*a ^ 42) > UCHAR_MAX || (*a ^ 42) < 0 || 
	    (*b ^ 42) > UCHAR_MAX || (*b ^ 42) < 0)
	{
	write(2, "Error: characters incompatable with -f command", 47);
		exit(1);
	}
	/* Loop over all of the two words until their end */
	while (1)
	{
		if ((*a == ' ') && (*b == ' '))
		{
			return 0;
		}
		/* memfrob uses an XOR with 00101010 = 42 to transform */
		else if ((*a == ' ') || toupper(*a ^ 42) < toupper(*b ^ 42))
		{
			return -1;
		}
		else if ((*b == ' ') || toupper(*a ^ 42) > toupper(*b ^ 42))
		{
			return 1;
		}
		a++;
		b++;
	}
}

int frobcmp_qsort(const void* a, const void* b)
{
    /* Since qsort passes a void*, and qsort needs a char*, use casting to switch around types */
    const char* aa = *(const char**)a;
    const char* bb = *(const char**)b;
    return frobcmp(aa, bb);
}

int frobcmp_f_qsort(const void* a, const void* b)
{
	/* Since qsort passes a void*, and qsort needs a char*, use casting to switch around types */
	const char* aa = *(const char**)a;
	const char* bb = *(const char**)b;
	return frobcmp_f(aa, bb);
}

void freeall(char** a, int n)
{
	int i;
	for (i = 0; i < n; i++)
		free(a[i]);
	free(a);
}

int main(int argc, const char * argv[])
{
	/* Parse the input arguments for -f */
	int fOption = 0;
	if (argc > 2)
	{
		write(2, "Invalid arguments; only one allowed (-f)", 41);
		exit(1);
	}
	if (argc == 2)
	{
		if (strcmp(argv[1], "-f") != 0) 
		{
			write(2, "Invalid arguments; only one allowed (-f)", 41);
			exit(1);
		}
		else
		{
			fOption = 1;
		}
	}
	
	int bufferSize = 1;
	char inputChar;
	char *currWord;
	char **wordList;
	char *wordBuffer;
	int currWordLen = 0, wordListLen = 0;

	/* stat struct from sys/stat.h */
	struct stat infile;
	/* Check that the file is readable */
	if (fstat(0, &infile) < 0)
	{
		write(2, "Error accessing file", 21);
		exit(1);
	}	
	if (S_ISREG(infile.st_mode))
	{
		bufferSize = infile.st_size;
		wordBuffer = (char*)malloc(bufferSize * sizeof(char));
		read(0, wordBuffer, bufferSize);
		if (wordBuffer == NULL)
		{
			write(2, "Error: Cannot allocate memory", 30);
			free(wordBuffer);
			exit(1);
		}
	}
	else
	{
		char* temp;
		wordBuffer = (char*)malloc(sizeof(char));
		while (read(0, temp, sizeof(char)) > 0)
		{
			wordBuffer[bufferSize - 1] = *temp;
			bufferSize++;
			wordBuffer = realloc(wordBuffer, bufferSize * sizeof(char));
			if (wordBuffer == NULL)
			{
				write(2, "Error: Cannot allocate memory", 30);
				free(wordBuffer);
				exit(1);
			}
		}
	}

	currWord = (char*)malloc(sizeof(char));
	wordList = (char**)malloc(sizeof(char*));

	/* Parse the file */
    for (int i = 0; i < bufferSize; i++)
	{
		inputChar = wordBuffer[i];
		
		if (inputChar == 0)
			break;
		/* Don't do anything if there is no word to be formed (i.e. multiple spaces in a row) */
        if ((inputChar == ' ') && (currWordLen == 0))
            continue;

        /* Grow the word */
        currWord = (char*)realloc(currWord, (currWordLen + 1) * sizeof(char));
        /* Test for error */
        if (currWord == NULL)
        {
            write(2, "Error: Cannot allocate memory", 30);
			freeall(wordList, wordListLen);
            exit(1);
        }

        currWord[currWordLen] = inputChar;
        currWordLen++;

        if ((inputChar == ' '))
        {
            wordList = (char**)realloc(wordList, (wordListLen + 1) * sizeof(char*));
            /* Test for error */
            if (wordList == NULL)
            {
                write(2, "Error: Cannot allocate memory", 30);
				freeall(wordList, wordListLen);
                exit(1);
            }
            /* Add currWord to wordList */
            wordList[wordListLen] = currWord;
            wordListLen++;

            /* reset currWord to be empty */
            currWord = (char*)malloc(sizeof(char));
            /* Test for error */
            if (currWord == NULL)
            {
                write(2, "Error: Cannot allocate memory", 30);
				freeall(wordList, wordListLen);
                exit(1);
            }
            currWordLen = 0;
        }
    } /* End while */

	/* The file may not end in a space, so make sure to add the last word */
	if (currWordLen != 0)
	{
		/* Grow the word */
		currWord = (char*)realloc(currWord, (currWordLen + 1) * sizeof(char));
		/* Test for error */
		if (currWord == NULL)
		{
			write(2, "Error: Cannot allocate memory", 30);
			freeall(wordList, wordListLen);
			exit(1);
		}
		currWord[currWordLen] = ' ';
		currWordLen++;

		wordList = (char**)realloc(wordList, (wordListLen + 1) * sizeof(char*));
		/* Test for error */
		if (wordList == NULL)
		{
			write(2, "Error: Cannot allocate memory", 30);
			freeall(wordList, wordListLen);
			exit(1);
		}
		/* Add currWord to wordList */
		wordList[wordListLen] = currWord;
		wordListLen++;
	}

	if (fOption)
	{
		qsort(wordList, wordListLen, sizeof(char*), frobcmp_f_qsort);
	}
	else
	{
		qsort(wordList, wordListLen, sizeof(char*), frobcmp_qsort);
	}

    /* print the output */
    int i, j;
    for (i = 0; i < wordListLen; i++)
    {
        for (j = 0; ; j++)
        {
            write(1, &wordList[i][j], sizeof(char));
            if (wordList[i][j] == ' ')
                break;
        }
    }
	freeall(wordList, wordListLen);
	exit(0);
}