#include "generator.h"

void generateVariables(DSTRING *name)
{

    printf("DEFVAR LF@%s\n", name->str);
}

void assigneVariables(DSTRING *name)
{
    printf("POPS LF@%s\n", name->str);
}

void generateHeader()
{
    printf(".IFJcode24\n");
    printf("JUMP main\n");
}

void generateFunctionHead(bstSymtable *symtable)
{
    printf("LABEL %s\n", symtable->key);
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    for (int i = ((fceData *)symtable->data)->paramCount - 1; i >= 0; i--)
    {
        printf("DEFVAR LF@%s\n", ((fceData *)symtable->data)->params[i].name);
        printf("POPS LF@%s\n", ((fceData *)symtable->data)->params[i].name);
    }
}
void generateFunctionReturn(DSTRING *functionID)
{
    printf("POPFRAME\n");
    if (strcmp(functionID->str, "main") != 0)
    {
        printf("RETURN\n");

        return;
    }
    else
    {
        printf("EXIT int@0\n");
    }
}

void generateFunctionCall(bstSymtable *symtableGlobal, bstSymtable *symtableLocal)
{
}
