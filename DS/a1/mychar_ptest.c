/*
--------------------------------------------------
Project: a1q1
File:    mychar_ptest.c
Author:  HBF
Version: 2025-08-19
--------------------------------------------------
*/

#include <stdio.h>
#include "mychar.h"

static const char tests[] = {'2', '8', 'A', 'a', 'z', 'Z', '+', '-', '(', ')', '$'};

void test_mychar(void)
{
	printf("------------------\n");
	printf("Test: mychar\n\n");

	int count = sizeof tests / sizeof *tests;

	int i;
	for (i = 0; i < count; i++)
	{
		char c = tests[i];
		int t = mytype(c);
		printf("Char\tASCII\tMyType\n");
		printf("%4c\t%5d\t%6d\n", c, c, t);

		if (t == 0)
		{
			printf("  digit_char_to_int: %d\n", digit_to_int(c));
		}
		else if (t == 1)
		{
			printf("  operator\n");
		}
		else if (t == 2)
		{
			printf("  left parenthesis: %c\n", c);
		}
		else if (t == 3)
		{
			printf("  right parenthesis: %c\n", c);
		}		
		else if (t == 4)
		{
			printf("  caseflip: %c\n", case_flip(c));
		}
		else
		{
			printf("  Not typed\n");
		}
	}
	printf("\n");
}

void test(char c)
{
	int t = mytype(c);
	printf("Char\tASCII\tMyType\n");
	printf("%c\t%d\t%d\n", c, c, t);

	if (t == 0)
	{
		printf("Square:%d\n", digit_to_int(c) * digit_to_int(c));
	}
	else if (t == 1)
	{
		printf("operator\n");
	}
	else if (t == 2)
	{
		printf("  left parenthesis: %c\n", c);
	}
	else if (t == 3)
	{
		printf("  right parenthesis: %c\n", c);
	}		
	else if (t == 4)
	{
		printf("Caseflip:%c\n", case_flip(c));
	}
	else
	{
		printf("Not typed\n");
	}
}

int main(int argc, char *args[])
{
	char c;
	if (argc <= 1)
	{
		test_mychar();
	}
	else
	{
		do
		{
			printf("Enter a character (! to quit)\n");
			scanf("%c", &c);
			if (c != '\n')
			{
				while (getc(stdin) != '\n'); 
			}
			test(c);
			if (c == '!')
				break;
		} while (1);
	}
	printf("\n");

	return 0;
}
