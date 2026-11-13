#ifndef LIBSTACK_H_
#define LIBSTACK_H_

struct Stack
{
    int *stack;
    int top;
    int size;
};

struct StackList
{
    struct StackList *next;
    int number;
};

struct Oura
{
    int *oura;
    int front;
    int back;
    int size;
};

struct Stack init (int );
void push (struct Stack *,int );
int pop (struct Stack *);

struct StackList * initlist ();
void pushlist (struct StackList **,int );
int poplist (struct StackList **);

struct Oura inito (int );
void enqueue (struct Oura *,int );
int dequeue (struct Oura *);

#endif