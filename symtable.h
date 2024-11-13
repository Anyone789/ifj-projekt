// symtable.h
// Interface for symbol table module
// Author(s): Václav Bergman
// Last Edit: 13.11.2024

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>

// Separate enums for variables and functions

// Variable node
typedef struct variableNode
{
    char *name;         // Name is used as a key
    // ENUM type - same as with functions
    bool initialized;
    bool constant;      // If variable is const type
    bool isPar;         // If variable is function parameter (const == true)
    bool use;           // If variable is used during run time
    // bool compTime - if variable's value is known at compile time
    struct variableNode *left;
    struct variableNode *right;
} VARIABLE_NODE;

// Function node
typedef struct functionNode
{
    char *name;         // Name is used as a key
    // ENUM returnType - use keywords or new enum? ... not all keywords are valid return types
    struct variableNode *localSymtable;
    struct functionNode *left;
    struct functionNode *right;
} FUNCTION_NODE;

// Symtable constructor
// Return value: Success - ptr to a symtable, failure - NULL
FUNCTION_NODE **createSymtable();

// Symtable destructor
void destroySymtable(FUNCTION_NODE **symtable);

// Create function node
// Return value: Success - ptr to a function node, failure - NULL
FUNCTION_NODE *createFunctionNode(char *name); // returnType

// Add function to symtable
void addFunctionToSymtable(FUNCTION_NODE **symtable, FUNCTION_NODE *functionNode);

// Create variable node
// Return value: Success - ptr to a variable node, failure - NULL
VARIABLE_NODE *createVariableNode(char *name, bool initialized, bool constant, bool isPar, bool use); // type, compTime

// Add variable node to function's local symtable
void addVariableNodeToLocalSymtable(FUNCTION_NODE *functionNode, VARIABLE_NODE *variableNode);

// Function for batch adding built in functions

#endif
