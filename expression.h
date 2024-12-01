// expression.h
// Interface for expression
// Author(s):  Marián Šuľa
// Last Edit: 1.12.2024
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "scanner.h"
#include "stack.h"
#include "errorCodes.h"
#include "symtable.h"
#define PrecTableSize 14

extern bool inFce;
extern bstSymtable *symTree;
extern bstSymtable *symLocal;
// Enum of all symbols in table
typedef enum
{

    tableMultiply,
    tableDivide,
    tablePlus,
    tableMinus,
    tableEqual,
    tableNotEqual,
    tableLess,
    tableLessEqual,
    tableGreat,
    tableGreatEqual,

    tableLeftPar,
    tableRightPar,

    tableIdentifier,
    tableDollar,
    tableOther,

} tableItem;

// Structure of expression element
typedef struct elmExpression
{

    int type;
    DATATYPE dataType;
    bool terminal;
    DSTRING *key;
    int valueInt;
    float valueFloat;
} ElmExp;
// Functions used in c files
int analyzeExp(TStack *expStack, TOKEN *token);
int initExpStack(TStack *expStack);
char getSign(TStack *expStack);
int convertToIndex(int value);
int reduce(TStack *expStack);
#endif