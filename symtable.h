// symtable.h
// Interface for symbol table module
// Author(s):  Marián Šuľa
// Last Edit: 20.11.2024

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include "scanner.h"
#include "errorCodes.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

// enum for function and variable
typedef enum
{
    var,
    fce
} nodeType;

extern const char *TOKEN_TYPE_STRING[];

extern const char *keywords[];

// Structure of node
typedef struct symtable
{
    char *key;
    void *data;
    int height;
    nodeType dataType;    
    struct symtable *left;  
    struct symtable *right; 
} bstSymtable;
// Structure of datatype 
typedef struct dataType
{
    bool isNull;
    bool isVoid;
    TOKEN_TYPE type;
} DATATYPE;
// Structure of variable in nodes
typedef struct var
{
    DATATYPE dataType;
    char *name;
    bool initialized;
    bool constant; 
    bool isPar;  
    bool use;    
} varData;
// Structure of function in nodes
typedef struct fce
{
    DATATYPE returnType; 
    int paramCount;     
    bool isDefined;     
    bool buildIn;
    varData *params;
    bstSymtable **locals;
} fceData;
// Functions used in c files

void symtableInit(bstSymtable **symTree);
bstSymtable *symtableSearch(bstSymtable **symTree, DSTRING key);
void symtableInsertVar(bstSymtable **symTree, DSTRING key, void *data);
void symtableInsertFce(bstSymtable **symTree, DSTRING key, void *data);
void symtableInsertBuildInFce(bstSymtable **symTree);
void symtableDelete(bstSymtable **symTree, DSTRING key);
void symtableDispose(bstSymtable **symTree);
void insertVariables(const char *name, DATATYPE dataType, bool initialized, bool constant, bool isPar, bool use, bstSymtable **local);
void insertFunction(bstSymtable **symTree, const char *name, DATATYPE returnType, int newParamCount, bool isDefined, bool buildIn, bool hasLocals, varData *newVarDatas);
void dStringAddString(DSTRING *dstr, const char *str);
#endif