/* Mark Guevara
*  704962920
*  CS35L
*  TA: Zhaowei Tan
*  
*  Assignment 4: sfrob.c
*/
#include <stdio.h>
#include <stdlib.h>

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

int frobcmp_qsort(const void* a, const void* b)
{
    /* Since qsort passes a void*, and qsort needs a char*, use casting to switch around types */
    const char* aa = *(const char**)a;
    const char* bb = *(const char**)b;
    return frobcmp(aa, bb);
}

void freeall(char** a, int n)
{
	int i;
	for (i = 0; i < n; i++)
		free(a[i]);
	free(a);
}

int main(void) 
{
    char inputChar;
    char *currWord;
    char **wordList;
    int currWordLen = 0, wordListLen = 0;
    currWord = (char*)malloc(sizeof(char));
    wordList = (char**)malloc(sizeof(char*));

    while (!feof(stdin))
    {
        inputChar = getchar();
        /* Test for error */
        if (ferror(stdin))
        {
            fprintf(stderr, "Error: cannot read file");
			freeall(wordList, wordListLen);
            exit(1);
        }
        if (feof(stdin))
        {
            /* The file may not end in a space, so make sure to add the last word */
            if (currWordLen == 0)
                break;
            /* Grow the word */
            currWord = (char*)realloc(currWord, (currWordLen + 1) * sizeof(char));
            /* Test for error */
            if (currWord == NULL)
            {
                fprintf(stderr, "Error: Cannot allocate memory");
				freeall(wordList, wordListLen);
				exit(1);
            }
            currWord[currWordLen] = ' ';
            currWordLen++;

            wordList = (char**)realloc(wordList, (wordListLen + 1) * sizeof(char*));
            /* Test for error */
            if (wordList == NULL)
            {
                fprintf(stderr, "Error: Cannot allocate memory");
				freeall(wordList, wordListLen);
                exit(1);
            }
            /* Add currWord to wordList */
            wordList[wordListLen] = currWord;
            wordListLen++;

            break;
        }

        /* Don't do anything if there is no word to be formed (i.e. multiple spaces in a row) */
        if ((inputChar == ' ') && (currWordLen == 0))
            continue;

        /* Grow the word */
        currWord = (char*)realloc(currWord, (currWordLen + 1) * sizeof(char));
        /* Test for error */
        if (currWord == NULL)
        {
            fprintf(stderr, "Error: Cannot allocate memory");
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
                fprintf(stderr, "Error: Cannot allocate memory");
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
                fprintf(stderr, "Error: Cannot allocate memory");
				freeall(wordList, wordListLen);
                exit(1);
            }
            currWordLen = 0;
        }
    } /* End while */

    qsort(wordList, wordListLen, sizeof(char*), frobcmp_qsort);

    /* print the output */
    int i, j;
    for (i = 0; i < wordListLen; i++)
    {
        for (j = 0; ; j++)
        {
            printf("%c", wordList[i][j]);
            if (wordList[i][j] == ' ')
                break;
        }
    }
	freeall(wordList, wordListLen);
	exit(0);
}