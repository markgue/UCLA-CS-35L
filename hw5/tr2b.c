/* Mark Guevara
*  704962920
*  CS35L
*  TA: Zhaowei Tan
*
*  Assignment 5: tr2b.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	


int main(int argc, char **argv)
{
	/* Check that there are 2  */
	if (argc != 3)
	{
		fprintf(stderr, "Error: invalid arguments; Use: tr2b [from] [to]\n");
		exit(1);
	}
	/* Save the inputs */
	char *from = argv[1];
	char *to = argv[2];

	/* Check that the inputs are the same length */
	if (strlen(from) != strlen(to))
	{
		fprintf(stderr, "Error: arguments must be the same length\n");
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
				fprintf(stderr, "Error: \"from\" must have no repeated characters\n");
				exit(1);
			}
		}
	}

	char inputChar;

	while (!feof(stdin))
	{
		inputChar = getchar();
		/* Test for error */
		if (ferror(stdin))
		{
			fprintf(stderr, "Error: cannot read file\n");
			exit(1);
		}
		/* Exit when the file ends */
		if (feof(stdin))
		{
			break;
		}

		int inFrom = 0;
		/* Test if the char is in from */
		for (i = 0; i < fromlen; i++)
		{
			if (from[i] == inputChar)
			{
				inFrom = 1;
				break;
			}
		}
		if (inFrom)
			putchar(to[i]);
		else
			putchar(inputChar);
	}

	return 0;
}