#include "libStack.h"
#include <stdio.h>
#include <stdlib.h>

struct Stack init (int size)
{
    struct Stack test;

    test.stack = (int *)malloc(sizeof(int) * size);
    if (test.stack == NULL)
    {
        fprintf(stderr,"Αδυναμία παραχώρησης μνήμης.\n");
        exit(-1);
    }
    test.top = 0;

    return test;
}

void push (struct Stack *initi,int size,int numb)
{
    if (initi->top >= size)
    {
        fprintf(stderr,"Το stack είναι γεμάτο. Αδυναμία εκχώρησης του αριθμού %d\n\n",numb);
        return;
    }

    initi->stack[initi->top] = numb;
    initi->top++;
}

int pop (struct Stack *init)
{
    int forreturn;

    if (init->top <= 0)
    {
        fprintf(stderr,"To stack είναι άδειο.\n\n");
        return -1;
    }

    init->top--;
    return forreturn = init->stack[init->top];
}