#include "expression.h"
// vyriesit zatvorky
int dolarValue = tableDollar;
// nastav premennu inFce na true ked sme vo while alebo v if inak bude false
// bool inFce = false;
bool lBracketInStack = false;
int returnExpValue = I;

bool inFce = false;
// na oddelenie v stacku
// v scanner treba urobit tak aby ked uz ma zistene ze aky to je typ, tak konci
int sep = 44;
TOKEN *nextToken;
char precTable[PrecTableSize][PrecTableSize] = {
    //        *    /    +    -    ==  !=    <    >    <=  >=    (    )    ID   $
    /* * */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* / */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* + */ {'<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* - */ {'<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* == */ {'<', '<', '<', '<', '-', '-', '-', '-', '-', '-', '<', '>', '<', '>'},
    /* != */ {'<', '<', '<', '<', '-', '-', '-', '-', '-', '-', '<', '>', '<', '>'},
    /* < */ {'<', '<', '<', '<', '-', '-', '-', '-', '-', '-', '<', '>', '<', '>'},
    /* > */ {'<', '<', '<', '<', '-', '-', '-', '-', '-', '-', '<', '>', '<', '>'},
    /* <= */ {'<', '<', '<', '<', '-', '-', '-', '-', '-', '-', '<', '>', '<', '>'},
    /* >= */ {'<', '<', '<', '<', '-', '-', '-', '-', '-', '-', '<', '>', '<', '>'},
    /* ( */ {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '>'},
    /* ) */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '-', '>', '-', '>'},
    /* ID */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '-', '>', '-', '>'},
    /* $ */ {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '-', '<', '#'},
};

int initExpStack(TStack *expStack)
{
    if (expStack != NULL)
    {
        stackInit(expStack);
        ElmExp *newExp = malloc(sizeof(struct elmExpression));
        if (newExp != NULL)
        {
            newExp->terminal = true;
            newExp->type = dolarValue;
        }
        stackPush(expStack, newExp);
        return 0;
    }
    else
    {
        return SYNTAX_ERROR;
    }
}

void checkSem(ElmExp *lOperand, ElmExp *rOperand)
{
    if (lOperand->dataType.type != T_ID && rOperand->dataType.type != T_ID)
    {

        if (lOperand->dataType.type != rOperand->dataType.type)
        {
            if (lOperand->dataType.type == T_STR || rOperand->dataType.type == T_STR)
            {
                // printf("%d", INCOMPATIBLE_TYPE_ERROR);
                exit(INCOMPATIBLE_TYPE_ERROR);
            }
            else
            {
                lOperand->dataType.type = T_FLOAT;
            }
        }
    }
    else
    {
        // search for id adn cmpr
        if (lOperand->dataType.type == T_ID && rOperand->dataType.type != T_ID)
        {

            bstSymtable *result = symtableSearch(&symLocal, *lOperand->key);
            if (result == NULL)
            {
                ("%d", INCOMPATIBLE_TYPE_ERROR);
                exit(INCOMPATIBLE_TYPE_ERROR);
            }
            else
            {
                if (((varData *)result->data)->dataType.type != rOperand->dataType.type)
                {
                    // printf("%d", INCOMPATIBLE_TYPE_ERROR);
                    // printf("NO");
                    exit(INCOMPATIBLE_TYPE_ERROR);
                }
                else
                {
                    // printf("OK");
                    ((varData *)result->data)->use = true;
                }
            }
        }
        else if (rOperand->dataType.type == T_ID && lOperand->dataType.type != T_ID)
        {
            bstSymtable *result = symtableSearch(&symLocal, *rOperand->key);
            if (result == NULL)
            {
                printf("%d", INCOMPATIBLE_TYPE_ERROR);
                exit(INCOMPATIBLE_TYPE_ERROR);
            }
            else
            {
                if (((varData *)result->data)->dataType.type != lOperand->dataType.type)
                {
                    // printf("%d", INCOMPATIBLE_TYPE_ERROR);
                    // printf("NO");
                    exit(INCOMPATIBLE_TYPE_ERROR);
                }
                else
                {
                    lOperand->dataType.type = T_ID;
                    lOperand->key = rOperand->key;
                    // printf("OK");
                    ((varData *)result->data)->use = true;
                }
            }
        }
        else
        {

            bstSymtable *lResult = symtableSearch(&symLocal, *lOperand->key);
            bstSymtable *rResult = symtableSearch(&symLocal, *rOperand->key);
            if (lResult == NULL || rResult == NULL)
            {
                // printf("%d", INCOMPATIBLE_TYPE_ERROR);
                exit(INCOMPATIBLE_TYPE_ERROR);
            }
            else
            {
                if (((varData *)lResult->data)->dataType.type != ((varData *)rResult->data)->dataType.type)
                {
                    // printf("%d", INCOMPATIBLE_TYPE_ERROR);
                    // printf("NO");
                    exit(INCOMPATIBLE_TYPE_ERROR);
                }
                else
                {
                    // printf("OK");
                    ((varData *)lResult->data)->use = true;
                    ((varData *)rResult->data)->use = true;
                }
            }
        }
    }
}

int binCheck(TStack *expStack, int operator)
{
    TStackItem *stackItem = expStack->stackTop;
    ElmExp *rOperand = ((ElmExp *)(stackItem->value));
    if (rOperand->type != tableIdentifier || rOperand->terminal == true)
    {
        // printf("\nRoperand\n");
        exit(SYNTAX_ERROR);
    }
    ElmExp *op = ((ElmExp *)(stackItem->next->value));
    if (op->type != operator|| op->terminal == false)
    {
        // printf("\nOperator\n");
        exit(SYNTAX_ERROR);
    }
    ElmExp *lOperand = ((ElmExp *)(stackItem->next->next->value));
    if (lOperand->type != tableIdentifier || lOperand->terminal == true)
    {
        // printf("\nLoperand\n");
        exit(SYNTAX_ERROR);
    }
    checkSem(lOperand, rOperand);

    return 0;
}

int analyzeExp(TStack *expStack, TOKEN *token)
{
    // ked doskusam, tak odkomentovat
    initExpStack(expStack);
    // printf("%d", ((ElmExp *)(expStack->stackTop->value))->type);
    nextToken = token;
    // nextToken = getToken();
    if (nextToken->type == 1)
    {
        exit(LEXICAL_ERROR);
    }

    char sign;
    while (1)
    {
        // printf("\nstack size:%d\n", expStack->stackSize);
        // printf("\n%d\n", nextToken->current_attribute);
        sign = getSign(expStack);

        if (sign == '<')
        {
            // printf("<");
            ElmExp *newExp = malloc(sizeof(struct elmExpression));
            if (newExp != NULL)
            {
                newExp->terminal = true;
                newExp->type = convertToIndex(nextToken->type);
                newExp->dataType.type = nextToken->type;
                if (nextToken->current_attribute == DSTR)
                {

                    newExp->key = nextToken->attribute.dStr;
                }
                else if (nextToken->current_attribute == I)
                {
                    newExp->valueInt = nextToken->attribute.i;
                }
                else if (nextToken->current_attribute == F)
                {
                    newExp->valueFloat = nextToken->attribute.f;
                }
            }

            stackPush(expStack, newExp);
            nextToken = getToken();
            if (nextToken->type == 1)
            {
                exit(LEXICAL_ERROR);
            }
        }
        else if (sign == '>')
        {
            // printf(">");
            if (reduce(expStack) == SYNTAX_ERROR)
            {
                exit(SYNTAX_ERROR);
            }
        }
        else if (sign == '=')
        {
            ElmExp *newExp = malloc(sizeof(struct elmExpression));
            newExp = expStack->stackTop->value;
            stackPop(expStack);
            stackPop(expStack);
            stackPush(expStack, newExp);
            nextToken = getToken();
            if (nextToken->type == 1)
            {
                exit(LEXICAL_ERROR);
            }
            // printf("=");

            // redukcia zatvoriek
        }
        else if (sign == '#')
        {
            // printf("end");
            returnExpValue = ((ElmExp *)(expStack->stackTop->value))->dataType.type;
            if (returnExpValue == T_ID)
            {
                bstSymtable *resul = symtableSearch(&symLocal, *((ElmExp *)(expStack->stackTop->value))->key);
                if (resul == NULL)
                {
                    printf("saa");
                    exit(UNDEFINED_VARIABLE_ERROR);
                }
                returnExpValue = ((varData *)resul->data)->dataType.type;
                ((varData *)resul->data)->use = true;
                if (returnExpValue != T_INT && returnExpValue != T_FLOAT && returnExpValue != T_STR)
                {
                    exit(GENERIC_SEMANTIC_ERROR);
                }
            }
            break;
        }
        else
        {
            // printf("error");
            //  stackDispose(expStack);
            exit(SYNTAX_ERROR);
        }
    }
    return 0;
    stackDispose(expStack);
}

char getSign(TStack *expStack)
{
    int stackInput = nextToken->type;
    TStackItem *stackTopValue = expStack->stackTop;
    while (((ElmExp *)(stackTopValue->value))->terminal != true)
    {
        stackTopValue = stackTopValue->next;
    }
    // printf("\nprec[%d][%d]\n", ((ElmExp *)(stackTopValue->value))->type, convertToIndex(stackInput));
    return precTable[((ElmExp *)(stackTopValue->value))->type][convertToIndex(stackInput)];
}

int reduce(TStack *expStack)
{
    TStackItem *stackItem = expStack->stackTop;
    while (((ElmExp *)(stackItem->value))->terminal != true)
    {
        stackItem = stackItem->next;
    }
    switch (((ElmExp *)(stackItem->value))->type)
    {
    case tableIdentifier:
        ((ElmExp *)(stackItem->value))->terminal = false;
        if (((ElmExp *)(stackItem->value))->dataType.type == T_INT)
        {
            printf("PUSHS int@%d\n", ((ElmExp *)(stackItem->value))->valueInt);
            /* code */
        }
        else if (((ElmExp *)(stackItem->value))->dataType.type == T_FLOAT)
        {

            printf("PUSHS float@%a\n", ((ElmExp *)(stackItem->value))->valueFloat);
        }
        else
        {

            printf("PUSHS LF@%s\n", ((ElmExp *)(stackItem->value))->key->str);
        }
        break;
    case tableMultiply:
        if (binCheck(expStack, tableMultiply) != 0)
        {
            return SYNTAX_ERROR;
        }
        printf("MULS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tablePlus:
        if (binCheck(expStack, tablePlus) != 0)
        {
            return SYNTAX_ERROR;
        }
        printf("ADDS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableMinus:
        if (binCheck(expStack, tableMinus) != 0)
        {
            return SYNTAX_ERROR;
        }
        printf("SUBS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableDivide:
        if (binCheck(expStack, tableDivide) != 0)
        {
            return SYNTAX_ERROR;
        }
        printf("DIVS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableEqual:
        if (binCheck(expStack, tableEqual) != 0)
        {

            return SYNTAX_ERROR;
        }
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableNotEqual:
        if (binCheck(expStack, tableNotEqual) != 0)
        {
            return SYNTAX_ERROR;
        }
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableLess:
        if (binCheck(expStack, tableLess) != 0)
        {
            return SYNTAX_ERROR;
        }
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableLessEqual:
        if (binCheck(expStack, tableLessEqual) != 0)
        {
            return SYNTAX_ERROR;
        }
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableGreat:
        if (binCheck(expStack, tableGreat) != 0)
        {
            return SYNTAX_ERROR;
        }
        stackPop(expStack);
        stackPop(expStack);
        break;

    case tableGreatEqual:
        if (binCheck(expStack, tableGreatEqual) != 0)
        {
            return SYNTAX_ERROR;
        }
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableLeftPar:
        return SYNTAX_ERROR;
        break;
    }
}

int convertToIndex(int value)
{
    switch (value)
    {
    case T_ID:
    case T_INT:
    case T_STR:
    case T_FLOAT:
        return tableIdentifier;

    case T_PLUS:
        return tablePlus;
    case T_MINUS:
        return tableMinus;
    case T_MUL:
        return tableMultiply;
    case T_DIV:
        return tableDivide;
    case T_EQ:
        return tableEqual;
    case T_NE:
        return tableNotEqual;
    case T_LT:
        return tableLess;
    case T_LE:
        return tableLessEqual;
    case T_GT:
        return tableGreat;
    case T_GE:
        return tableGreatEqual;
    case T_L_BRACKET:
        lBracketInStack = true;
        return tableLeftPar;
    case T_R_BRACKET:
        // treba odkomentovat
        if (inFce == true && lBracketInStack == false)
        {
            return tableDollar;
        }
        return tableRightPar;
    case T_SEMICOL:
        return tableDollar;
    }
    return tableOther;
}
// testovanie
// int main(int argc, char **argv)
// {
//     if (argc > 2)
//     {
//         fprintf(stderr, "Too many arguments!");
//         exit(0);
//     }
//     if (argc == 1)
//     {
//         fprintf(stderr, "Few arguments!");
//         exit(0);
//     }
//     else
//     {
//         FILE *src;

//         if ((src = fopen(argv[1], "r")) == NULL)
//         {
//             fprintf(stderr, "The file cannot be opened.");
//             exit(0);
//         }
//         setSourceFile(src);
//         TStack stack;
//         TOKEN *token;

//         symtableInit(&symTree);
//         symtableInsertBuildInFce(&symTree);
//         insertVariables("x", (DATATYPE){false, false, T_INT}, true, false, false, false, &symTree);
//         insertVariables("y", (DATATYPE){false, false, T_INT}, true, false, false, false, &symTree);
//         int i = analyzeExp(&stack, token);
//         printf("%d\n", i);
//         printf("return: %d", returnExpValue);
//     }

//     return 0;
// }