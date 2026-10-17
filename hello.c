#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#define TRUE 1
#define FALSE 0
#define ROWS 9
#define COL 9

char ** fill(FILE *);
int checkvalid(char **);

int main(int argc,char *argv[])
{
	setlocale(LC_ALL, "");

	if (argc != 2)
	{
		fprintf(stderr, "Λάθος παράμετροι");
		exit(-1);
	}

	FILE *ifp;
	ifp = fopen(argv[1],"r");

	if (ifp == NULL)
	{
		fprintf(stderr, "Αδυναμία ανοίγματος αρχείου");
		exit(-2);
	}

	char **sudoku;

	sudoku = fill(ifp);

	for(int i=0;i<9;i++)
	{
		fprintf(stdout,"\n");
		for(int j=0;j<9;j++)
		{
			printf("|");
			fprintf(stdout,"%c",sudoku[i][j]);
		}
		printf("|");
		fprintf(stdout,"\n-------------------");
	}
	printf("\n");

	int valid = checkvalid(sudoku);
	if (valid > 0)
	{
		fprintf(stdout,"Είναι sudoku\n");
	}
	else if (valid == -1)
	{
		fprintf(stderr,"Λάθος σε οριζόντιο\n");
		exit(-6);
	}
	else if (valid == -2)
	{
		fprintf(stderr,"Λάθος σε κάθετο\n");
		exit(-7);
	}
		
	return 0;
}

char ** fill (FILE *input)
{
	char **pin;
	
	pin = (char **)malloc(sizeof(char *) * ROWS);
	if (pin == NULL)
	{
		fprintf(stderr,"Αδυναμία παραχώρησης μνήμης\n");
		exit(-3);
	}
	for (int rows = 0;rows < 9;rows++)
	{
		pin[rows] = (char *)malloc(sizeof(char) * COL);
		if (pin[rows] == NULL)
		{
			fprintf(stderr,"Αδυναμία παραχώρησης μνήμης\n");
			exit (-3);
		}
	}

	char ch;

	int rows = 0,columns = 0;

	while ((ch = fgetc(input)) != EOF)
	{
		if (ch >= '0' && ch <= '9')
		{	
			if (ch == '0')
			{
				pin[rows][columns] = ' ';
			}
			else
			{
				pin[rows][columns] = ch;
			}
			columns++;
			if (columns == 9)
			{
				rows++;
				columns = 0;
			}
		}
		else if (ch == '\n')
		{
			continue;
		}
		else
		{
			fprintf(stderr,"Δεν είναι σωστές η είσοδος στην σειρά %d και στην στήλη %d\n",rows+1,columns+1);
			exit(-5);
		}
	}

	return pin;	
}

int checkvalid(char **check)
{
	int *list = (int *)malloc(sizeof(int) * 10);

	for (int i = 0;i < ROWS;i++)
	{
		for (int j = 0;j < 10;j++)
		{
			list[j]=0;
		}
		for (int k = 0;k < COL;k++)
		{
			if (isdigit(check[i][k]))
			{
				int knum = check[i][k] - 48;
				list[knum]++;
				if (list[knum] > 1)
				{
					return -1;
				}
			}
		}
	}

	for (int k = 0;k < COL;k++)
	{
		for (int j = 0;j < 10;j++)
		{
			list[j]=0;
		}
		for (int i = 0;i < ROWS;i++)
		{
			if (isdigit(check[i][k]))
			{
				int inum = check[i][k] - 48;
				list[inum]++;
				if (list[inum] > 1)
				{
					return -2;
				}
			}
		}
	}

	return TRUE;
}
