/* Mark Guevara
*  704962920
*  CS35L
*  TA: Zhaowei Tan
*
*  Assignment 5: tr2u.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	/* Check that there are 2  */
	if (argc != 3)
	{
		write(2, "Error: invalid arguments; Use: tr2u [from] [to]\n", 49);
		exit(1);
	}
	/* Save the inputs */
	char *from = argv[1];
	char *to = argv[2];

	/* Check that the inputs are the same length */
	if (strlen(from) != strlen(to))
	{
		write(2, "Error: arguments must be the same length\n", 42);
		exit(1);
	}

	/* Check for any repeated chars in from */
	size_t i, j, fromlen = strlen(from);
	for (i = 0; i < fromlen; i++)
	{
		for (j = i + 1; j < fromlen; j++)
		{
			if (from[i] == from[j])
			{
				write(2, "Error: \"from\" must have no repeated characters\n", 48);
				exit(1);
			}
		}
	}

	char inputChar[1];

	while (read(0, inputChar, 1) > 0)
	{
		int inFrom = 0;
		/* Test if the char is in from */
		for (i = 0; i < fromlen; i++)
		{
			if (from[i] == *inputChar)
			{
				inFrom = 1;
				break;
			}
		}
		if (inFrom)
			write(1, to + i, 1);
		else
			write(1, inputChar, 1);
	}

	return 0;
}