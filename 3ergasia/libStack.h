#ifndef LIBSTACK_H_
#define LIBSTACK_H_

struct Stack
{
    int *stack;
    int top;
};

struct Stack init (int );
void push (struct Stack *,int ,int );
int pop (struct Stack *);

#endif