// inicializacia zasobnika
#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// struktura prvku v zasobniku, zatial bude value int, predpokladam, ze to bude asi nejaka struktura

typedef struct stackItemStructure
{
    void *value;
    struct stackItemStructure *next; 
}TStackItem;

// struktura zasobinka

typedef struct stackStructure
{
   TStackItem *stackTop;
   int stackSize;
}TStack;

// deklaracia funkcii
void stackInit(TStack *stack);
void stackPush(TStack *stack, void *value);
void stackPop(TStack *stack);
int *stackTop(TStack *stack);
bool stackIsEmpty(TStack *stack);
void stackDispose(TStack *stack);
#endif