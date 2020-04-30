#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "str.c"
#include "StrList.c"

#define SIZE 200

char * Encrypt(char *);
char * Decrypt(char *);
void Commands(char **, char **, int);
char * inputFile(char *);
void outputFile(char *, char *);
int intLen(int);

int main (int argc, char * argv[])
{
	char * commands[6] = {NULL, "F", "F", NULL, NULL, "F"};
	char * encryption;
	
	//Process input commands
	Commands(commands, argv, argc);

	//Help screen	
	if (commands[2][0] == 'T')
	{
		printf("String encrypter\n");
		printf("encrypt <STRING>\n");
		printf("OPTIONS:\n");
		printf("-d | Decrypt a string\n");
		printf("encrypt -d <STRING>\n");
		printf("---\n");
		printf("-o | Output to file\n");
		printf("encrypt <STRING> -o <FILENAME>\n");
		printf("---\n");
		printf("-in | Take the first row of a file as string input\n");
		printf("encrypt -in <FILENAME>\n");
		printf("---\n");
		printf("-s | Prevent encryption printing on screen\n");
		printf("encrypt <STRING> -s -o <FILENAME>\n");
		printf("---\n");
		printf("-h | Shows this screen\n");
	}
	//Input from command line
	if (commands[0])
	{	
		if (commands[1][0] == 'F')
			encryption = Encrypt(commands[0]);
		else if (commands[1][0] == 'T')
			encryption = Decrypt(commands[0]);
	
		if (commands[5][0] == 'F')
			printf("%s\n", encryption);
		//File output
		if (commands[3])
			outputFile(commands[3], encryption);
	}
	//Input from file
	else if (commands[4])
	{
		char * row;
				
		row = inputFile(commands[4]);
		
		if (row)
		{	
			if (commands[1][0] == 'F')
				encryption = Encrypt(row);
			else if (commands[1][0] == 'T')
				encryption = Decrypt(row);

			if (commands[5][0] == 'F')
				printf("%s\n", encryption);
			//File output
			if (commands[3])
				outputFile(commands[3], encryption);
		}
	}
	else
		printf("Encrypter, type '-h'\n");

	return 0;
}

char * inputFile(char fileName[])
{
	FILE * fptr;
	char row[SIZE];

	if (fptr = fopen(fileName, "r"))
	{
		fgets(row, SIZE, fptr);
		fclose(fptr);
		return strCopy(row);
	}
	else
		printf("FILE OPENING ERROR\n");

	return NULL;
}

void outputFile(char fileName[], char str[])
{
	FILE * fptr;

	if (fptr = fopen(fileName, "w"))
	{
		fprintf(fptr, "%s\n", str);
		fclose(fptr);
	}
	else
		printf("FILE WRITING ERROR\n");
}

void Commands(char * commands[], char * input[], int len)
{
	list * tmp = NULL;
	int check;
	int out;
	int in;
	int i;
	
	check = 0;
	out = 0;
	in = 0;

	for (i = 1; i < len; i++)
	{
		if (strCompare(input[i], "-d", 0))
			commands[1] = strCopy("T");
		
		else if (strCompare(input[i], "-h", 0))
			commands[2] = strCopy("T");
		
		else if (strCompare(input[i], "-o", 0))
			out = 1;

		else if (strCompare(input[i], "-in", 0))
			in = 1;
		
		else if (strCompare(input[i], "-s", 0))
			commands[5] = strCopy("T");

		else if (out)
		{
			commands[3] = strCopy(input[i]);
			out = 0;
		}

		else if (in)
		{
			commands[4] = strCopy(input[i]);
			in = 0;
		}

		else if (check)
		{
			tmp = list_appendCh(tmp, ' ');
			tmp = list_append(tmp, input[i]);
		}
		else
		{
			tmp = list_append(tmp, input[i]);
			check = 1;
		}
	}
	if (tmp)
		commands[0] = toStr(tmp);
}

char * Encrypt(char str[])
{
	char * nu_str = NULL;
	char * tmp;
	int len;
	int indexLen;
	int pass;
	int i,j,k;

	len = 0;
	indexLen = 0;

	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] >= 64 && str[i] % 2 != 0)
		{
			len = len + intLen(str[i]) + 2;
			indexLen++;
		}
		else
			len = len + 2;
	}

	if (nu_str = malloc(sizeof(char)*(len+1)))
	{
		for (i = 0, j = 0; str[i] != '\0'; i++)
			if (str[i] >= 64 && str[i] % 2 != 0)
			{
				tmp = intToStr(i);
				for (k = 0; tmp[k] != '\0'; k++, j++)
					nu_str[j] = tmp[k];
				nu_str[j] = (rand() % 14) + 33;
				j++;
			}
		
		if (j > 0)
			nu_str[j-1] = (rand() % 37) + 58;
		
		pass = 0;

		for (i = 0; str[i] != '\0'; j++)
		{
			if (!pass)
			{
				if (str[i] >= 64)
					nu_str[j] = str[i] / 2;
				
				else
					nu_str[j] = str[i] * 2;
				
				pass = 1;
				i++;
			}
			else
			{
				nu_str[j] = (rand() % (123 - 32 + 1)) + 32;
				pass = 0;
			}
		}
		return nu_str;
	}
	else
		printf("ALLOCATION ERROR\n");

	return nu_str;
}

char * Decrypt(char str[])
{
	char * nu_str = NULL;
	int * indexes = NULL;
	int len;
	int indexLen;
	int indexEnd;
	int dec;
	int pass;
	int i,j,k;
	
	len = 0;
	indexLen = 0;

	for (i = 0; str[i] < 58 && str[i] != '\0'; i++)
	{
		if (str[i] < 48)
			indexLen++;
	}

	indexEnd = i;
	indexLen++;
	len = 0;

	for (i = i+1; str[i] != '\0'; i++)
		len++;
	
	if (indexEnd)
	{
		if (indexes = malloc(sizeof(int)*indexLen))
		{
			dec = 1;

			for (i = 0; i < indexLen; i++)
				indexes[i] = 0;

			for (i = indexEnd-1, j = indexLen-1; i >= 0; i--)
			{
				if (str[i] < 48)
				{
					dec = 1;
					j--;
				}
				else
				{
					indexes[j] = indexes[j] + (str[i] - '0') * dec;
					dec = dec * 10;
				}
			}
		}
		else
			printf("ALLOCATION ERROR\n");
	}
	if (nu_str = malloc(sizeof(char)*(len+1)))
	{
		pass = 0;
		if (indexEnd)
			i = indexEnd+1;
		else
			i = 0;
		for (j = 0, k = 0; str[i] != '\0'; i++)
		{
			if (!pass)
			{
				if (indexes && j == indexes[k])
				{
					nu_str[j] = str[i] * 2 + 1;
					k++;
				}
				else if (str[i] >= 64)
				{
					nu_str[j] = str[i] / 2;
				}
				else
					nu_str[j] = str[i] * 2;
				j++;
				pass = 1;
			}
			else
				pass = 0;
		}
		free(indexes);
		return nu_str;
	}
	else
		printf("ALLOCATION ERROR\n");

	return nu_str;
}

//Returns the number of digits in a number
int intLen(int num)
{
	int len = 0;
	
	if (num == 0)
		return 1;

	while (num != 0)
	{
		num = num / 10;
		len++;
	}
	return len;
}
