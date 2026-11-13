/* (Για το ερώτημα 1 απά τα προαιρετικά όπου υπάρχει μία ερώτηση)

    Ερώτηση : Γιατί δεν χρειάζεται παρόμοια λειτουργικότητα η υλοποίηση του 2ου ερωτήματος;
    
    Απάντηση : Εφόσον η λίστα από nodes που αποτελούν το stack δεν είναι περιορισμένου
    μεγέθους αλλά μπορεί να μεταβληθεί αφαιρώντας ή προσθέτοντας, εάν κάθε φορά
    που αφαιρούμε ένα node κάνουμε και free το memory που κρατάει τότε στο τέλος
    δεν θα μας μέινει κάποιο άδειο κουτί χώρου όπως στον στατικό πίνακα. Επομένως
    στον στατικό πίνακα αφού θα έχουμε τελειώσει με την χρήση του θα πρέπει να
    διαχειριστούμε την μνήμη που μένει, ενώ στην λίστα διαχειρίζεται κατευθείαν.*/

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
    test.size = size;

    return test;
}

void push (struct Stack *initi,int numb)
{
    if (initi->top >= initi->size)
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
        fprintf(stdout,"Το stack είναι άδειο. Να διαγραφεί(1/0)? ");
        int choice;
        scanf("%d",&choice);
        if (choice == 1)
        {
            free(init->stack);
            init->stack = NULL;
            return -1;
        }
        else
        {
            return -1;
        }
    }

    init->top--;
    return forreturn = init->stack[init->top];
}

struct StackList * initlist()
{
    return NULL;
}

void pushlist (struct StackList **top,int numb)
{
    struct StackList *newpart = (struct StackList *)malloc(sizeof(struct StackList));
    if (newpart == NULL)
    {
        fprintf(stderr,"Αδυναμία παραχώρησης μνήμης.\n");
        exit(-1);        
    }

    newpart->number = numb;

    newpart->next = *top;

    *top = newpart;
}

int poplist (struct StackList **top)
{
    int number;

    if (*top == NULL)
    {
        fprintf(stderr,"Το stack είναι άδειο\n");
        return -1;
    }

    struct StackList *temp = *top;

    number = temp->number;

    *top = temp->next;

    free(temp);
    return number;
}

struct Oura inito (int size)
{
    struct Oura base;
    base.oura = (int *)malloc(sizeof(int) * size);
    if (base.oura == NULL)
    {
        fprintf(stderr,"Αδυναμία παραχώρησης μνήμης\n");
        exit(-1);
    }

    base.size = size;
    base.front = 0;
    base.back = 0;

    return base;
}

void enqueue (struct Oura *foradd,int number)
{
    if (foradd->back >= foradd->size)
    {
        if (foradd->front == 0)
        {
            fprintf(stdout,"Η ουρά έχει γεμίσει\n");
            return;
        }
        int j=0;
        for(int i=foradd->front;i<foradd->back;i++)
        {
            foradd->oura[j] = foradd->oura[i];
            j++;
        }
        foradd->front = 0;
        foradd->back = j;
    }
    foradd->oura[foradd->back] = number;
    foradd->back++;
}

int dequeue (struct Oura *fordel)
{
    if (fordel->front == fordel->back)
    {
        if (fordel->oura == NULL) 
            return -1;

        fprintf(stdout,"Άδειασε η ουρά. Να διαγραφεί(1/0)?\n");
        int choice;
        scanf("%d",&choice);
        if (choice == 1)
        {
            free(fordel->oura);
            fordel->oura = NULL;
            return -1;
        }
        else
        {
            return -1;
        }
    }
    int number = fordel->oura[fordel->front];
    fordel->front++;

    return number;
}