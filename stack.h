// stack.h
// Interface for stack
// Author(s):  Marián Šuľa
// Last Edit: 15.11.2024
#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure of item in stack
typedef struct stackItemStructure
{
    void *value;
    struct stackItemStructure *next;
} TStackItem;

// Structure of stack
typedef struct stackStructure
{
    TStackItem *stackTop;
    int stackSize;
} TStack;

// Functions used in c files

void stackInit(TStack *stack);
void stackPush(TStack *stack, void *value);
void stackPop(TStack *stack);
int *stackTop(TStack *stack);
bool stackIsEmpty(TStack *stack);
void stackDispose(TStack *stack);
#endif