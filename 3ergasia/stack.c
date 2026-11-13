#include <stdio.h>
#include <stdlib.h>
#include "libStack.h"

int main()
{
    int size=5;

    struct Stack mystack = init(size);

    for(int i=0;i<(size + 1);i++)
    {
        push(&mystack,size,i+1);
    }

    int number;
    
    for(int i=0;i<(size+1);i++)
    {
        number = pop(&mystack);
        if (number != -1) printf("%d\n",number);
    }

    free(mystack.stack);

    return 0;
}