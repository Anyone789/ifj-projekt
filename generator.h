#ifndef GENERATOR_H
#define GENERATOR_H

#include "stack.h"
#include "errorCodes.h"
#include "scanner.h"
#include "expression.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void generateVariables(DSTRING *name);
void assigneVariables(DSTRING *name);
void generateHeader();
void generateFunctionHead(bstSymtable *symtable);
void generateFunctionReturn(DSTRING *functionID);
void generateFunctionEnd(bstSymtable *symtable);
void generateFunctionCall(bstSymtable *symtableGlobal, bstSymtable *symtableLocal);
#endif