#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "scanner.h"
#include "stack.h"
#include "errorCodes.h"
#define PrecTableSize 14

typedef enum {

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

}tableItem;

typedef enum{
    KeyWord,
    Term,
    NonTerm
}TermType;
// pridat veci co mam v mobile
typedef struct elmExpression{
    
    int type; // Id
    int dataType;
    bool terminal; 
}ElmExp;

int analyzeExp(TStack *expStack, TOKEN *token);
int initExpStack(TStack *expStack);
char getSign(TStack *expStack);
int convertToIndex(int value);
#endif