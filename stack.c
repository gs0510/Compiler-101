/*
        BATCH 10
        Gargi Sharma 2013A7PS161P
        Ayushi Agarwal 2013A7PS065P

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct stack
{
    struct stackNode* top;
};

struct stackNode
{
    char string[30];
    struct stackNode* next;
};

struct stack* createStack()
{
    struct stack *s=(struct stack*)malloc(sizeof(struct stack));
    if(!s)
    return NULL;
    s->top=NULL;
    return s;
}

void push(struct stack* s, char* data)
{
    struct stackNode* temp;
    temp =(struct stackNode*)malloc(sizeof(struct stackNode));
    if(!temp) return ;
    strcpy(temp->string,data);
    temp->next=s->top;
    s->top=temp;
}

int isEmptyStack(struct stack* s)
{
    return s->top==NULL;
}

void pop(struct stack* s)
{
    char strtemp[30];
    struct stackNode* temp;
    temp =malloc(sizeof(struct stackNode));
    if (isEmptyStack(s))
    return;
    temp= s->top;
    s->top=s->top->next;
    strcpy(strtemp,temp->string);
    free(temp);
}

void printStack(struct stack* s)
{
    struct stackNode* tempst;
    tempst =malloc(sizeof(struct stackNode));
    tempst=s->top;
    while(tempst)
    {
        printf("TopElement:%s\n",tempst->string);
        tempst=tempst->next;
    }
}