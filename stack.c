#include "stack.h"

// inicializacia zasobnika
void stackInit(TStack *stack){
    stack->stackTop = NULL;
    stack->stackSize = 0;
}
// zistenie prazdneho zasobnika
bool stackIsEmpty(TStack *stack){
    return (stack->stackTop == NULL);
}
// vlozenie prvku do zasobnika
void stackPush(TStack *stack, void *value){
    TStackItem *newItem = (TStackItem *)malloc(sizeof(struct stackItemStructure));
    newItem->value = value;
    newItem->next = stack->stackTop;
    stack->stackTop = newItem;
    stack->stackSize++;
}
// odobratie prvku zo zasobnika
void stackPop(TStack *stack){
    TStackItem *item;
    if (!stackIsEmpty(stack))
    {
        item = stack->stackTop;
        stack->stackTop = stack->stackTop->next;
        stack->stackSize--;
        free(item);
    }
}
// ziskanie hodnoty vrcholu zasobnika
int *stackTop(TStack *stack){
    return (stack->stackTop->value);
}
// odstranenie prvkov zo zasobnika
void stackDispose(TStack *stack){
    while (!stackIsEmpty(stack))
    {
        stackPop(stack);
    }
}