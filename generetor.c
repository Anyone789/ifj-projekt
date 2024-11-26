#include "generator.h"

void generateVariables(DSTRING *name)
{

    printf("DEFVAR LF@%s", name->str);
}

void assigneVariables(DSTRING *name)
{
    printf("POPS LF@%s", name->str);
}