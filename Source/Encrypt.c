#include <stdlib.h>
#include "str.c"
#include "StrList.c"

#define JOLLY '#'

void Commands(char **, char **, int);
char * Encrypt(char *);
char * Decrypt(char *);
int intLen(int);

int main (int argc, char * argv[])
{
	char * commands[3] = {NULL, "F", NULL};

	Commands(commands, argv, argc);

	if (argc == 1)
		printf("Encrypter, type 'help'\n");

	else if (strCompare(commands[1], "T", 0))
	{
		printf("%s\n", Decrypt(commands[0]));
	}
	else if (strCompare(commands[0], "help", 0))
	{
		printf("Encrypt <STRING>\n");
		printf("OPTIONS:\n");
		printf("-d | Decrypt\n");
		printf("Use 'Encrypt (-d) <STRING> > FILENAME' to print output to file\n ");
	}
	else
	{
		printf("%s\n", Encrypt(commands[0]));
	}
	return 0;
}

void Commands(char * commands[], char * input[], int len)
{
	list * tmp = NULL;
	int i;
	int index;
	int appending = 0;

	for (i = 1; i < len; i++)
	{
		if (strCompare(input[i], "-d", 0))
			commands[1] = strCopy("T");
		else if (strCompare(input[i], "help", 0))
			commands[0] = strCopy("help");
		else if (appending)
		{
			tmp = list_appendCh(tmp , ' ');
			tmp = list_append(tmp, input[i]);
		}
		else
		{
			tmp = list_append(tmp, input[i]);
			appending = 1;
		}
	}
	if (tmp)
		commands[0] = toStr(tmp);
}

char * Encrypt(char str[])
{
	char * nu_str;
	char * tmp;
	int len1;
	int len2;
	int increment = 1;
	int i,j,k;

	len1 = 0;
	len2 = 0;

	for (i = 0; str[i] != '\0'; i++)
	{
		len1 = len1 + intLen(str[i]+increment);
		len2++;
		increment++;
	}

	increment = 1;

	if (nu_str = malloc(sizeof(char)*(len1+len2+1)))
	{
		for (i = 0, j = 0; str[i] != '\0'; i++)
		{
			tmp = intToStr(str[i]+increment);
			for (k = 0; tmp[k] != '\0'; k++, j++)
				nu_str[j] = tmp[k];
			increment++;
			nu_str[j] = JOLLY;
			j++;
			free(tmp);	
		}
		nu_str[j] = '\0';
	}
	return nu_str;
}

char * Decrypt(char str[])
{
	char * nu_str;
	int tmp;
	int increment;
	int len1;
	int len2;
	int dec;
	int i,j;
	
	len1 = 0;
	len2 = 0;

	for (i = 0; str[i] != '\0'; i++)
	{
		len1++;
		if (str[i] == JOLLY)
			len2++;
	}

	increment = 1;

	if (nu_str = malloc(sizeof(char)*len2+1))
	{
		increment = increment * len2;
		for (i = len1-2, j = len2-1; i >= 0; i--, j--)
		{
			tmp = 0;
			dec = 1;
			for ( ; str[i] != JOLLY && i >= 0; i--)
			{
				tmp = tmp + (str[i] - '0') * dec;
				dec = dec * 10;	
			}
			nu_str[j] = tmp - increment;
			increment--;
		}
		nu_str[len2] = '\0';
	}
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
