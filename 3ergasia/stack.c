#include <stdio.h>
#include <stdlib.h>
#include "libStack.h"

struct Stack copystack (struct Stack *);
struct StackList * copyslist (struct StackList **);
struct Oura copyoura (struct Oura *);

int main()
{
    int size=5;

    struct Stack mystack = init(size);

    for(int i=0;i<(size + 1);i++)
    {
        push(&mystack,i+1);
    }

    struct Stack cpstack = copystack(&mystack);

    int number;

    printf("======STACK======\n");
    
    printf("----Βασικό Stack----\n");
    for(int i=0;i<(mystack.size+1);i++)
    {
        number = pop(&mystack);
        if (number != -1) 
            printf("%d\n",number);
    }

    printf("----Αντίγραφο Stack----\n");
    for(int i=0;i<(mystack.size+1);i++)
    {
        number = pop(&cpstack);
        if (number != -1) 
            printf("%d\n",number);
    }

    free(mystack.stack);

    printf("\n\n\n");

    struct StackList *top = initlist();

    for(int i=0;i<(size + 1);i++)
    {
        pushlist(&top,i+1);
    }

    struct StackList *cplstack = copyslist(&top);

    printf("----Βασική Λίστα Stack----\n");
    for(int i=0;i<(size+1);i++)
    {
        number = poplist(&top);
        if (number != -1) 
            printf("%d\n",number);
    }

    printf("----Αντιγραμμένη Λίστα Stack----\n");
    for(int i=0;i<(size+1);i++)
    {
        number = poplist(&cplstack);
        if (number != -1) 
            printf("%d\n",number);
    }

    printf("\n======ΟΥΡΑ======\n");

    struct Oura queue1 = inito(size);

    for(int i=0;i<(size + 1);i++)
    {
        enqueue(&queue1,i+1);
    }

    struct Oura copyoura1 = copyoura(&queue1);

    printf("---Βασική Ουρά---\n");
    for(int i=0;i<(size + 1);i++)
    {
        number = dequeue(&queue1);
        if (number != -1)
            printf("%d\n",number);
    }

    printf("---Αντιγραμμένη Ουρά---\n");
    for(int i=0;i<(size + 1);i++)
    {
        number = dequeue(&copyoura1);
        if (number != -1)
            printf("%d\n",number);
    }

    return 0;
}

    struct Stack copystack (struct Stack *mains)
    {
        int number,elements = mains->top;
        struct Stack temp = init(elements);
        struct Stack final = init(mains->size);

        for(int i=0;i<elements;i++)
        {
            number = pop(mains);
            push(&temp,number);
        }

        for(int i=0;i<elements;i++)
        {
            number = pop(&temp);
            push(mains,number);
            push(&final,number);
        }

        free (temp.stack);

        return final;
    }

    struct StackList * copyslist (struct StackList **mains)
    {
        int number;
        struct StackList *ttemp = initlist();
        struct StackList *ffinal = initlist();

        while ((number = poplist(mains)) != -1)
        {
            pushlist(&ttemp,number);
        }

        while ((number = poplist(&ttemp)) != -1)
        {
            pushlist(mains, number);
            pushlist(&ffinal,number);
        }

        return ffinal;
    }

struct Oura copyoura (struct Oura *basic)
{
    int elements = basic->back - basic->front;
    int number;
    struct Oura final = inito(basic->size);

    for(int i=0;i<elements;i++)
    {
        number = dequeue(basic);
        enqueue(&final,number);
        enqueue(basic, number);
    }

    return final;
}