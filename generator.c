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
void generateFunctionReturn()
{
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateFunctionCall(bstSymtable *symtableGlobal, bstSymtable *symtableLocal)
{
}
