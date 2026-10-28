#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

struct Search
{
	int found;
	int counter;
};

int *createpin(int );
int *createcopy(int *,int );
void printpin(int *,int );
struct Search seiriaki(int *,int ,int );
void mergesort(int *,int ,int );
void merge(int *,int ,int ,int );
struct Search diadiki(int *,int ,int ,int );
int *pinmerge(int *,int );
void inputf(FILE *,int *,int );
void outputf(FILE *,int *,int );
void comparison(int *,int );

int main()
{
	int size;
	FILE *ifp = fopen("forone.txt","r"),*ofp = fopen("forone.txt","w");
	if (ifp == NULL || ofp == NULL)
	{
		fprintf(stderr,"Αδυναμίας ανοίγματος αρχείου");
		exit(-5);
	}

    fprintf(stdout,"Δώσε μέγεθος πίνακα: ");
    scanf("%d",&size);

	int *pin = createpin(size);

	printpin(pin,size);

	fprintf(stdout,"Δώσε αριθμό για αναζήτηση: ");
	int numb;
	scanf("%d",&numb);

	struct Search seir = seiriaki(pin,size,numb);

	if (seir.found == FALSE)
		fprintf(stdout,"Δεν υπάρχει ο αριθμός στον πίνακα\n");
	else
		fprintf(stdout,"Ο αριθμός %d εμφανίζεται στον πίνακα\n", numb);

	int *copy = createcopy(pin,size);

	printpin(copy,size);

	mergesort(copy,0,size-1);

	printpin(copy,size);

        fprintf(stdout,"Δώσε αριθμό για αναζήτηση: ");
        scanf("%d",&numb);

    struct Search diad = diadiki(copy,0,size-1,numb);

        if (diad.found == FALSE)
                fprintf(stdout,"Δεν υπάρχει ο αριθμός στον πίνακα\n");
        else
                fprintf(stdout,"Ο αριθμός %d εμφανίζεται στον πίνακα\n", numb);

	int *doublepinakas = pinmerge(copy,size);

	printpin(doublepinakas,size*2);

	outputf(ofp,pin,size);
	fclose(ofp);

	rewind(ifp);
	inputf(ifp,copy,size);
	fclose(ifp);
	printpin(copy,size);

	comparison(copy,size);

	return 0;
}

int *createpin(int sizepin)
{
	int *pinakas = (int *)malloc(sizeof(int) * sizepin);
	if (pinakas == NULL)
	{
		fprintf(stderr,"Αδυναμία παραχώρησης μνήμης");
		exit(-1);
	}

	for (int i=0;i<sizepin;i++)
	{
		do
		{
			*(pinakas + i) = rand()%500+1;
		}
		while (*(pinakas + i) < 30);
	}

	return pinakas;
}

int *createcopy(int *pinakas,int sizepin)
{
	int *antigrafo = (int *)malloc(sizeof(int) * sizepin);

        for (int i=0;i<sizepin;i++)
        {
		*(antigrafo + i) = *(pinakas + i);
        }

	return antigrafo;
}

void printpin(int *pinakas,int sizepin)
{
	for (int i=0;i<sizepin;i++)
	{
		fprintf(stdout,"%d ",*(pinakas + i));
	}

	printf("\n");
}

struct Search seiriaki(int *pinakas,int sizepin,int num)
{
	struct Search p1;
	p1.counter = 0;
	p1.found = FALSE;
	int i=0;

	while (p1.found == FALSE && i<sizepin)
	{
		p1.counter++;
		if (*(pinakas + i) == num)
			p1.found = TRUE;
		i++;
	}

	return p1;
}

void mergesort(int *pinakas,int left,int right)
{
	if (left < right)
	{
		int mid = left + (right - left) / 2;

		mergesort(pinakas,left,mid);
		mergesort(pinakas,mid+1,right);

		merge(pinakas,left,mid,right);
	}
}

void merge(int *pinakas,int left,int mid,int right)
{
	int i,j,k;

	int n1 = mid - left + 1;
	int n2 = right - mid;

	int *pin1 = (int *)malloc(n1 * sizeof(int));
	int *pin2 = (int *)malloc(n2 *sizeof(int));

	for (i=0;i<n1;i++)
	{
		*(pin1+i) = *(pinakas+left+i);
	}
	for (j=0;j<n2;j++)
	{
		*(pin2+j) = *(pinakas+mid+1+j);
	}

	i = 0;j = 0;k = left;

	while (i < n1 && j < n2)
	{
		if (*(pin1 + i) <= *(pin2 + j))
		{
			*(pinakas + k) = *(pin1 + i);
			i++;
		}
		else
		{
			*(pinakas + k) = *(pin2 + j);
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		*(pinakas + k) = *(pin1 + i);
		i++;
		k++;
	}

        while (j < n2)
        {
                *(pinakas + k) = *(pin2 + j);
                j++;
                k++;
        }

	free(pin1);
	free(pin2);
}

struct Search diadiki(int *pinakas,int low,int high,int num)
{
	struct Search k1;
	k1.counter = 0;
	k1.found = FALSE;

	if (low > high)
	{
		return k1;
	}

	int mid = (high+low)/2;

	if (num > *(pinakas+mid))
	{
		k1 = diadiki(pinakas,mid+1,high,num);
		k1.counter++;
		return k1;
	}
	else if (num == *(pinakas + mid))
	{
		k1.counter = 1;
		k1.found = TRUE;
		return k1;
	}
	else
	{
		k1 = diadiki(pinakas,low,mid-1,num);
		k1.counter++;
		return k1;
	}
}

int *pinmerge(int *pinakas,int size)
{
	int *newpin = createpin(size);
	mergesort(newpin,0,size-1);
	printpin(newpin,size);
	int *doublepin = createpin(size*2);

	int i=0,j=0,k=0;
	while (i < size && j < size)
	{
		if (*(pinakas+i) < *(newpin+j))
		{
			*(doublepin+k) = *(pinakas+i);
			i++;
		}
		else
		{
			*(doublepin+k) = *(newpin+j);
			j++;
		}
		k++;
	}

	while (i < size)
	{
		*(doublepin+k) = *(pinakas+i);
		i++;
		k++;
	}

	while (j < size)
	{
		*(doublepin+k) = *(newpin+j);
		j++;
		k++;
	}

	return doublepin;
}

void inputf(FILE *ifp,int *pinakas,int size)
{
	if (pinakas == NULL)
	{
		pinakas = (int *)malloc(sizeof(int) * size);
		if (pinakas == NULL)
		{
			fprintf(stderr,"Αδυναμία παραχώρησης μνήμης");
			exit(-1);
		}
	}

	int number,i = 0;
	while (i < size && fscanf(ifp, "%d", &number) == 1)
	{
		*(pinakas+i) = number;
		i++;
	}
}

void outputf(FILE *ofp,int *pinakas,int size)
{
	for (int i = 0;i<size;i++)
	{
		fprintf(ofp,"%d ",*(pinakas+i));
	}
}

void comparison (int *pinakas,int size)
{
	int sc=0,dc=0;

	for(int i=0;i<size;i++)
	{
		struct Search seir = seiriaki(pinakas,size,*(pinakas+i));

		sc = sc + seir.counter;
	}

	mergesort(pinakas,0,size-1);
	for(int i=0;i<size;i++)
	{
		struct Search diad = diadiki(pinakas,0,size-1,*(pinakas+i));

		dc = dc + diad.counter;
	}

	if (dc > sc)
	{
		float difference = ((float)(dc-sc) / sc) * 100.0;
		fprintf(stdout,"Ο αλγόριθμος της σειριακής αναζήτησης ήταν %.2f%% καλύτερος για μέγεθος %d πίνακα\n",difference,size);
	}
	else if (dc == sc)
	{
		fprintf(stdout,"Οι αλγόριθμοι ήταν ίδιο αποδοτικοί για μέγεθος %d πίνακα\n",size);
	}
	else
	{
		float difference = ((float)(sc-dc) / dc) * 100.0;
		fprintf(stdout,"Ο αλγόριθμος της δυαδικής αναζήτησης ήταν %.2f%% καλύτερος για μέγεθος %d πίνακα\n",difference,size);
	}
}