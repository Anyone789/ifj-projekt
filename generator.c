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

// void generateFunctionHead(bstSymtable *symtable)
// {
//     printf("LABEL %s\n", symtable->key);
//     printf("CREATEFRAME\n");
//     printf("PUSHFRAME\n");

//     for (int i = 0; i < ((fceData *)symtable->data)->paramCount; i++)
//     {
//         printf("DEFVAR LF@%s", ((fceData*)symtable->data)->params[0].);
//     }
// }