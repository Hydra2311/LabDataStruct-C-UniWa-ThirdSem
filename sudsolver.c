#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <locale.h>
#define TRUE 1
#define FALSE 0
#define ROWS 9
#define COL 9

FILE *checkparams(int ,char **);
char **fill(FILE *);
void printSud(char **, WINDOW *);
int checkvalid(char **,WINDOW *);
int checkrows(char **,WINDOW *);
int checkcols(char **,WINDOW *);
int check3x3(char **,WINDOW *);
int recursion(char **,int ,int ,WINDOW *);
char **solve(char **,WINDOW *);	

int main(int argc,char *argv[])
{
	setlocale(LC_ALL, "");
	FILE *ifp;

	ifp = checkparams(argc,argv);

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
		printSud(sudoku,menuwin);

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
					}
					else
					{
						mvwprintw(menuwin, 14, 10, "Επιτυχής επίλυση");
						sudoku = solved_sudoku;
						printSud(sudoku,menuwin);

						wrefresh(menuwin);
					}
				}
			}
		}
	}
	while(choice != 'q' && choice != 'Q' && choice != ';');
	/*Έχω χρησιμοποιήσει και το ; στον έλεγχο, για να μπορείς κάποιος να βγει
	ακόμα και αν πατάει το q στα ελληνικά.*/

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

FILE *checkparams(int argc,char **argv)
{
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

	int counter = 0; char ch;
	while ((ch = fgetc(ifp)) != EOF)
	{
		if (ch >= '0' && ch <= '9')
		{
			counter++;
		}
	}

	if (counter != 81)
	{
		fprintf(stderr,"Λάθος αριθμός Sudoku, πρέπει να είναι 9x9\n");
		exit(-8);
	}

	rewind (ifp);
	return ifp;
}

void printSud(char **sudoku,WINDOW *menuwin)
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

/*Συνάρτηση που καλεί όλες τις άλλες συναρτήσεις ελέγχου έτσι ώστε να επιστρέψει
ένα τελικό απότελεσμα εάν ή όχι είναι valid sudoku.*/

int checkvalid(char **check,WINDOW *menuwin)
{
	if ((checkrows(check,menuwin)) && (checkcols(check,menuwin)) && (check3x3(check,menuwin)))
		return TRUE;
	else
		return FALSE;
}

/*Με την χρήση μονοδιάστατου πίνακα (list), ελέγχεται εάν ένας αριθμός εμφανίζεται
παραπάνω από μία φορά σε μία σειρά. Αυτό γίνεται εφόσον το list χρησιμοποιείται ως μετρητής
του κάθε αριθμού που δέχεται σαν index.*/

int checkrows(char **check,WINDOW *menuwin)
{
	int *list = (int *)malloc(sizeof(int) * 10);
	if (list == NULL)
	{
		fprintf(stderr,"Αδυναμία παραχώρησης μνήμης\n");
		exit(-3);
	}

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
					free(list);
					return FALSE;
				}
			}
		}
	}

	free(list);
	return TRUE;
}

/*Με την χρήση μονοδιάστατου πίνακα (list), ελέγχεται εάν ένας αριθμός εμφανίζεται
παραπάνω από μία φορά σε μία στήλη. Αυτό γίνεται εφόσον το list χρησιμοποιείται ως μετρητής
του κάθε αριθμού που δέχεται σαν index.*/

int checkcols(char **check,WINDOW *menuwin)
{
	int *list = (int *)malloc(sizeof(int) * 10);
	if (list == NULL)
	{
		fprintf(stderr,"Αδυναμία παραχώρησης μνήμης\n");
		exit(-3);
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
					mvwprintw(menuwin,13,12,"Λάθος στην κάθετο [%d], ο αριθμός %d",k+1,inum);
					free(list);
					return FALSE;
				}
			}
		}
	}

	free(list);
	return TRUE;
}

/*Για κάθε κουτί ελέγχεται εάν επαναλαμβάνεται κάποιος αριθμός. Η επανάληψη h 
και η επανάληψη g καθορίζουν σε ποια σειρά ή στήλη είμαστε. Το 3x3 ελέγχεται σειρά
ανά σειρά, εκεί χρησιμεύουν οι επαναλήψεις l και j. Με την χρήση δεύτερου μονοδιάστατου
πίνακα (list) τοποθετούμε τους αριθμούς που συναντάμε ως index του πίνακα
με σκοπό να μετρήσουμε το πόσες φορές έχει εμφανιστεί.*/

int check3x3(char **check,WINDOW *menuwin)
{
	int *list = (int *)calloc(10,sizeof(int));
	if (list == NULL)
	{
		fprintf(stderr,"Αδυναμία παραχώρησης μνήμης\n");
		exit(-3);
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
							mvwprintw(menuwin,13,12,"Λάθος στo 3x3 [%d][%d], ο αριθμός %d",h+1,g+1,num);
							free(list);
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

/*Εάν η recursion γυρίσει true τότε επιστρέφει τον γεμισμένο πίνακα, αλλιώς 
επιστρέφει NULL.*/

char **solve (char **sudoku,WINDOW *menuwin)
{
	if (recursion(sudoku,0,0,menuwin))
		return sudoku;
	else 
		return NULL;
}