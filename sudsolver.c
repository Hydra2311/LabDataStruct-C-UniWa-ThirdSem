#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <locale.h>
#define TRUE 1
#define FALSE 0
#define ROWS 9
#define COL 9

char ** fill(FILE *);
int checkvalid(char **,WINDOW *);
int checkrows(char **,WINDOW *);
int checkcols(char **,WINDOW *);
int check3x3(char **,WINDOW *);
int recursion(char **,int ,int ,WINDOW *);
char **solve(char **,WINDOW *);	

int main(int argc,char *argv[])
{
	setlocale(LC_ALL, "");

	if (argc != 2)
	{
		fprintf(stderr, "Λάθος παράμετροι. Πρέπει %s και ένα file .txt\n", argv[0]);
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

	initscr();
    cbreak();
	noecho(); 
	curs_set(0);  

    WINDOW *menuwin = newwin(LINES,COLS,0,0);  
    refresh();
	box(menuwin,0,0);

    attron(A_BOLD);
    mvwprintw(menuwin,1,COLS/2-5,"SUDOKU");
    attroff(A_BOLD);
    mvwprintw(menuwin,LINES-2,COLS-25,"Πατήστε q για έξοδο");

    wrefresh(menuwin);

    keypad(menuwin,TRUE);

	int choice, highlighted = 0,numbofch = 2;
	const char *choices[] = {"Έλεγξε", "Λύσε"};

	do
    {
		if (sudoku != NULL)
		{
			for(int i=0;i<9;i++)
			{
				mvwprintw(menuwin, i + 3, 2, "|");
				for(int j=0;j<9;j++)
				{
					mvwprintw(menuwin,i+3,(j * 2) + 3,"%c|",sudoku[i][j]);
				}
			}
		}	

        for(int i = 0;i < numbofch;i++)
        {
            if (i == highlighted)
            {
                wattron(menuwin,A_REVERSE);
            }
            mvwprintw(menuwin, 13 + i, 2, "%s", choices[i]);
            wattroff(menuwin,A_REVERSE);
        }

		wrefresh(menuwin);
     	choice = wgetch(menuwin);

        switch (choice)
        {
            case KEY_UP:
            {
                highlighted--;
                if(highlighted == -1)
                {
                    highlighted = 0;
                }
                break;
            }
            case KEY_DOWN:
            {
                highlighted++;
                if(highlighted == numbofch)
                {
                    highlighted = numbofch-1;
                }
                break;
            }
            default:
            {
                break;
            }
        }

		if (choice == 10)
		{
			mvwprintw(menuwin, 11, 2, "                                      ");

			switch (highlighted)
			{
				case 0:
				{
					mvwprintw(menuwin, 14, 10, "                          ");
					int valid = checkvalid(sudoku,menuwin);
					if (valid)
						mvwprintw(menuwin,13,12,"Είναι sudoku");
					wrefresh(menuwin);
					break;
				}
				case 1:
				{
					mvwprintw(menuwin,13,12,"                            ");
					char **solved_sudoku = solve(sudoku,menuwin);
					if (solved_sudoku == NULL)
					{
						mvwprintw(menuwin, 14, 10, "Αδύνατη η επίλυση");
						break;
					}
					else
					{
						mvwprintw(menuwin, 14, 10, "Επιτυχής επίλυση");
						sudoku = solved_sudoku;
						for(int i=0;i<9;i++)
						{
							mvwprintw(menuwin, i + 3, 2, "|");
							for(int j=0;j<9;j++)
							{
								mvwprintw(menuwin,i+3,(j * 2) + 3,"%c|",sudoku[i][j]);
							}
						}

						wrefresh(menuwin);
						break;
					}
				}
			}
		}
	}
	while(choice != 'q' && choice != 'Q' && choice != ';');
	/*Έχω χρησιμοποιήσει και το ; στον έλεγχο, για να μπορείς κάποιος να βγει
	ακόμα και αν πατάει το q στα ελληνικά.*/

	/*char **sudoku;

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
        printf("\n");*/

	endwin();
	if (sudoku != NULL)
	{
		for (int l = 0;l<9;l++)
		{
			free(sudoku[l]);
		}
		free(sudoku);
	}
	fclose(ifp);

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
			fprintf(stderr,"Δεν είναι σωστή η είσοδος στην σειρά %d και στην στήλη %d\n",rows+1,columns+1);
			exit(-5);
		}
	}

	return pin;	
}

int checkvalid(char **check,WINDOW *menuwin)
{
/*	int *list = (int *)malloc(sizeof(int) * 10);

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
*/
	if ((checkrows(check,menuwin)) && (checkcols(check,menuwin)) && (check3x3(check,menuwin)))
		return TRUE;
	else
		return FALSE;
}

int checkrows(char **check,WINDOW *menuwin)
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
					mvwprintw(menuwin,13,12,"Λάθος στην οριζόντιο [%d], ο αριθμός %d",i+1,knum);
					return FALSE;
				}
			}
		}
	}

	free(list);
	return TRUE;
}

int checkcols(char **check,WINDOW *menuwin)
{
	int *list = (int *)malloc(sizeof(int) * 10);

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
					mvwprintw(menuwin,13,12,"Λάθος στην κάθετο [%d], ο αριθμός %d",k+1,inum);
					return FALSE;
				}
			}
		}
	}

	free(list);
	return TRUE;
}

int check3x3(char **check,WINDOW *menuwin)
{
	int *list = (int *)calloc(10,sizeof(int));

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
							mvwprintw(menuwin,13,12,"Λάθος στo 3x3 [%d][%d], ο αριθμός %d",h+1,g+1,num);
							return FALSE;
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

/*Μέσω ενός αναδρομικού αλγοριθμού, δοκιμάζονται σε κάθε κενό κελί οι αριθμοί από το 1 εώς το 9
και γίνεται έλεγχος του sudoku. Μετά γίνεται κάλεσμα της ίδιας συνάρτησης για το επόμενο κελί και κάνει το ίδιο μέχρι να πέσει 
σε κάποιο κελί που δεν έχει κανέναν έγκυρο αριθμό, τότε αδειάζει το κελί και πηγαίνει πίσω, μετά στο προηγούμενο κάλεσμα της συνάρτησης
συνεχίζεται η for επανάληψη και δοκιμάζει άλλους αριθμούς. Αυτό τα πίσω και μπροστά βήματα γίνονται μέχρι, είτε να φτάσουμε το row
να γίνει 9, που σημαίνει ότι έχουμε ελέγξει όλο τον πίνακα από 0->8, είτε να μην υπάρχει κανένας αριθμός που να μπορέσει να μπει ώστε να 
βγει sudoku*/

int recursion(char **sudoku,int row,int col,WINDOW *menuwin)
{
	if (row == ROWS)
	{
		return TRUE;
	}

	if (col == COL)
	{
		return recursion(sudoku,row+1,0,menuwin);
	}

	if (isdigit(sudoku[row][col]))
	{
		return recursion(sudoku,row,col+1,menuwin);
	}

	for (int i = 1;i < 10;i++)
	{
		sudoku[row][col] = i + 48;
		if (checkvalid(sudoku,menuwin) == TRUE)
		{
			if (recursion(sudoku,row,col+1,menuwin) == TRUE)
			{
				mvwprintw(menuwin,13,12,"                                       ");
				return TRUE;
			}
		}
	}

	sudoku[row][col] = ' ';
	mvwprintw(menuwin,13,12,"                                       ");
	return FALSE;
}


char **solve (char **sudoku,WINDOW *menuwin)
{
	if (recursion(sudoku,0,0,menuwin))
		return sudoku;
	else 
		return NULL;
}

