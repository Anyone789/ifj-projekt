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

typedef enum
{
    KeyWord,
    Term,
    NonTerm
} TermType;
// pridat veci co mam v mobile
typedef struct elmExpression
{

    int type; // Id
    DATATYPE dataType;
    bool terminal;
    DSTRING *key;
} ElmExp;

int analyzeExp(TStack *expStack, TOKEN *token);
int initExpStack(TStack *expStack);
char getSign(TStack *expStack);
int convertToIndex(int value);
int reduce(TStack *expStack);
#endif