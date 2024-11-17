// symtable.h
// Interface for symbol table module
// Author(s): Václav Bergman, Marián Šuľa
// Last Edit: 13.11.2024

#ifndef SYMTABLE_H
#define SYMTABLE_H


#include "scanner.h"


extern const char *TOKEN_TYPE_STRING[];  // Declare extern
extern const char* keywords[];  
typedef enum {
    var,
    fce
} nodeType;

// Uzel stromu
typedef struct symtable {
  char  *key;                     // klíč
  void *data;                   // hodnota
  int height;                   // vyska stromu
  nodeType dataType;            // typ uzla
  struct symtable *left;       // levý potomek
  struct symtable *right;      // pravý potomek
} bstSymtable;
//struktura premennej
typedef struct var {
    int dataType;
    bool initialized;
    bool constant;      // If variable is const type
    bool isPar;         // If variable is function parameter (const == true)
    bool use;           // If variable is used during run time
} varData;


typedef struct fce {
    int returnType;      // return value
    int paramCount;     // param caunt
    bool isDefined;     // is function defined
    bool buildIn;
    varData *params;
    bstSymtable **locals;
} fceData;

void symtableInit(bstSymtable **symTree);
bstSymtable* symtableSearch(bstSymtable **symTree, DSTRING key);
void symtableInsertVar(bstSymtable **symTree, DSTRING key, void *data);
void symtableInsertFce(bstSymtable **symTree, DSTRING key, void *data);
void symtableInsertBuildInFce(bstSymtable **symTree);
void symtableDelete(bstSymtable **symTree, DSTRING key);
void symtableDispose(bstSymtable **symTree);
void insertVariables(const char *name, int dataType, bool initialized, bool constant, bool isPar, bool use, bstSymtable **local);
#endif