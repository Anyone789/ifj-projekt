// stack.c
// Stack module
// Author(s): Marián Šuľa
// Last Edit: 16.11.2024

#include "stack.h"
#include "errorCodes.h"

/**
 * @brief Initializes a stack.
 *
 * Sets the stack's top pointer to `NULL` and its size to 0, preparing it for use.
 *
 * @param stack A pointer to the stack to initialize.
 */
void stackInit(TStack *stack)
{
    stack->stackTop = NULL;
    stack->stackSize = 0;
}
/**
 * @brief Checks if the stack is empty.
 *
 * Determines whether the stack contains any elements by checking if the top pointer is `NULL`.
 *
 * @param stack A pointer to the stack to check.
 * @return `true` if the stack is empty, `false` otherwise.
 */
bool stackIsEmpty(TStack *stack)
{
    return (stack->stackTop == NULL);
}
/**
 * @brief Pushes a value onto the stack.
 *
 * Allocates memory for a new stack item, assigns the value, and places it at the top of the stack. 
 * If memory allocation fails, the program exits with `INTERNAL_ERROR`.
 *
 * @param stack A pointer to the stack to push the value onto.
 * @param value A pointer to the value to push onto the stack.
 */
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
/**
 * @brief Removes the top item from the stack.
 *
 * If the stack is not empty, removes the top item, updates the stack's top pointer,
 * decreases the stack size, and frees the memory of the removed item.
 *
 * @param stack A pointer to the stack to pop the top item from.
 */
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
/**
 * @brief Retrieves the value of the top item on the stack.
 *
 * Returns a pointer to the value of the top stack item. Assumes the stack is not empty.
 *
 * @param stack A pointer to the stack.
 * @return A pointer to the value of the top item on the stack.
 */
int *stackTop(TStack *stack)
{
    return (stack->stackTop->value);
}
/**
 * @brief Disposes of the stack by freeing all items.
 *
 * Pops all items from the stack, freeing their memory. The stack itself remains uninitialized.
 *
 * @param stack A pointer to the stack to dispose of.
 */
void stackDispose(TStack *stack)
{
    while (!stackIsEmpty(stack))
    {
        stackPop(stack);
    }
}