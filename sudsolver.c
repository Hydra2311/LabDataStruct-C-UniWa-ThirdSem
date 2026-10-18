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
int recursion(char **,int ,int );
char **solve(char **);	

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
	else
	{
		fprintf(stderr,"Λάθος σε 3x3\n");
		exit(-8);
	}

	printf("\n\n\n");

	sudoku = solve(sudoku);

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
	for (int p = 0;p  < 10;p++)
        {
        	list[p] = 0;
        }

	for (int h = 0;h < 3;h++)
	{
		for (int g = 0;g < 3;g++)
		{
			int i = (h * 3);
			for (int l = 0;l < 3;l++)
			{
				int k = (g * 3);
				for (int j = 0;j < 3;j++)
				{
					if (isdigit(check[i][k]))
					{	
						int num = check[i][k] - 48;
						list[num]++;
						if (list[num] > 1)
						{
							return -3;
						}
					}	
					k++;
				}
				i++;
			}
	                for (int p = 0;p  < 10;p++)
	                {
        	                list[p] = 0;
                	}
		}
	}

	free(list);

	return TRUE;
}

int recursion(char **sudoku,int row,int col)
{
	if (row == ROWS)
	{
		return TRUE;
	}

	if (col == COL)
	{
		return recursion(sudoku,row+1,0);
	}

	if (isdigit(sudoku[row][col]))
	{
		return recursion(sudoku,row,col+1);
	}

	for (int i = 1;i < 10;i++)
	{
		sudoku[row][col] = i + '0';
		if (checkvalid(sudoku) == TRUE)
		{
			if (recursion(sudoku,row,col+1) == TRUE)
			{
				return TRUE;
			}
		}
	}

	sudoku[row][col] = ' ';
	return FALSE;
}


char **solve (char **sudoku)
{
	if (recursion(sudoku,0,0))
		return sudoku;
	else 
		return NULL;
}

