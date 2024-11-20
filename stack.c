#include "stack.h"
#include "errorCodes.h"

// inicializacia zasobnika
void stackInit(TStack *stack)
{
    stack->stackTop = NULL;
    stack->stackSize = 0;
}
// zistenie prazdneho zasobnika
bool stackIsEmpty(TStack *stack)
{
    return (stack->stackTop == NULL);
}
// vlozenie prvku do zasobnika
void stackPush(TStack *stack, void *value)
{
    TStackItem *newItem = (TStackItem *)malloc(sizeof(struct stackItemStructure)); // ak toto zliha treba pridat internal error
    if (newItem == NULL)
    {
        exit(INTERNAL_ERROR);
    }

    newItem->value = value;
    newItem->next = stack->stackTop;
    stack->stackTop = newItem;
    stack->stackSize++;
}
// odobratie prvku zo zasobnika
void stackPop(TStack *stack)
{
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
int *stackTop(TStack *stack)
{
    return (stack->stackTop->value);
}
// odstranenie prvkov zo zasobnika
void stackDispose(TStack *stack)
{
    while (!stackIsEmpty(stack))
    {
        stackPop(stack);
    }
}
// void stackPrint(TStack *stack)
// {
//     TStackItem *current = stack->stackTop;
//     int position = stack->stackSize;

//     printf("\nStack (top to bottom):\n\n");
//     printf(" ┌───────┐\n");

//     while (current != NULL)
//     {
//         // Cast value back to int for printing
//         int value = (int)current->value;

//         // Print each stack item with its position and value
//         printf(" │ %5d │ <- position %d\n", value, position);
//         printf(" |-------|\n");
//         current = current->next;
//         position--;
//     }

//     printf(" └───────┘\n");
//     printf(" Stack size: %d\n", stack->stackSize);
// }