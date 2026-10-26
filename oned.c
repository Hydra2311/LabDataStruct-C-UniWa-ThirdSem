#include <stdio.h>
#include <stdlib.h>

int *createpin(int );
int *createcopy(int *,int );
void printpin(int *,int );
int seiriaki(int *,int ,int );
void mergesort(int *,int ,int );
void merge(int *,int ,int ,int );

int main()
{
        int size = 20;

	int *pin = createpin(size);

	printpin(pin,size);

	fprintf(stdout,"Δώσε αριθμό για αναζήτηση: ");
	int numb;
	scanf("%d",&numb);

	int nquant = seiriaki(pin,size,numb);

	if (nquant == 0)
		fprintf(stdout,"Δεν υπάρχει ο αριθμός στον πίνακα\n");
	else
		fprintf(stdout,"Ο αριθμός %d εμφανίζεται στον πίνακα %d φορές\n", numb,nquant);

	int *copy = createcopy(pin,size);

	printpin(copy,size);

	mergesort(copy,0,size-1);

	printpin(copy,size);

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

int seiriaki(int *pinakas,int sizepin,int num)
{
	int quantity = 0;

	for (int i=0;i<sizepin;i++)
	{
		if (*(pinakas + i) == num)
			quantity++;
	}

	return quantity;
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
