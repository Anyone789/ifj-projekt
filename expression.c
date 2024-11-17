#include "expression.h"
// vyriesit zatvorky
int dolarValue = tableDollar;
// nastav premennu inFce na true ked sme vo while alebo v if inak bude false

bool lBracketInStack = false;
// na oddelenie v stacku
// v scanner treba urobit tak aby ked uz ma zistene ze aky to je typ, tak konci
int sep = 44;

bool inFce = false;

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
        exit(INTERNAL_ERROR);
    }
}

int binCheck(TStack *expStack, int operator)
{
    TStackItem *stackItem = expStack->stackTop;
    ElmExp *rOperand = ((ElmExp *)(stackItem->value));
    if (rOperand->type != tableIdentifier || rOperand->terminal == true)
    {
        printf("\nRoperand\n");
        exit(SYNTAX_ERROR);
    }
    ElmExp *op = ((ElmExp *)(stackItem->next->value));
    if (op->type != operator|| op->terminal == false)
    {
        printf("\nOperator\n");
        exit(SYNTAX_ERROR);
    }
    ElmExp *lOperand = ((ElmExp *)(stackItem->next->next->value));
    if (lOperand->type != tableIdentifier || lOperand->terminal == true)
    {
        printf("\nLoperand\n");
        exit(SYNTAX_ERROR);
    }

    return 0;
}
// jedina funkcia ktoru bude volat parser ked bude
int analyzeExp(TStack *expStack, TOKEN *token)
{
    printf("%d\n", inFce);
    // ked doskusam, tak odkomentovat
    initExpStack(expStack);
    printf("%d", ((ElmExp *)(expStack->stackTop->value))->type);
    nextToken = token;
    // nextToken = getToken();
    //  pridat do parseru ked budes volat token
    //  printf("%s, %d", nextToken->attribute.dStr->str, nextToken->type); // toto je slovo co da ti token
    if (nextToken->type == 1)
    {
        exit(LEXICAL_ERROR);
    }
    char sign;
    while (1)
    {
        printf("\nstack size:%d\n", expStack->stackSize);

        sign = getSign(expStack);

        if (sign == '<')
        {
            printf("<");
            ElmExp *newExp = malloc(sizeof(struct elmExpression));
            if (newExp != NULL)
            {
                newExp->terminal = true;
                newExp->type = convertToIndex(nextToken->type);
                newExp->dataType = nextToken->current_attribute;
                printf("\n%d\n", nextToken->current_attribute);
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
            printf(">");
            TStackItem *stackItem = expStack->stackTop;
            while (((ElmExp *)(stackItem->value))->terminal != true)
            {
                stackItem = stackItem->next;
            }
            switch (((ElmExp *)(stackItem->value))->type)
            {
            case tableIdentifier:
                ((ElmExp *)(stackItem->value))->terminal = false;
                break;
            case tableMultiply:
                if (binCheck(expStack, tableMultiply) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tablePlus:
                if (binCheck(expStack, tablePlus) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableMinus:
                if (binCheck(expStack, tableMinus) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableDivide:
                if (binCheck(expStack, tableDivide) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableEqual:
                if (binCheck(expStack, tableEqual) != 0)
                {

                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableNotEqual:
                if (binCheck(expStack, tableNotEqual) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableLess:
                if (binCheck(expStack, tableLess) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableLessEqual:
                if (binCheck(expStack, tableLessEqual) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableGreat:
                if (binCheck(expStack, tableGreat) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;

            case tableGreatEqual:
                if (binCheck(expStack, tableGreatEqual) != 0)
                {
                    exit(SYNTAX_ERROR);
                }
                stackPop(expStack);
                stackPop(expStack);
                break;
            case tableLeftPar:
                exit(SYNTAX_ERROR);
                break;
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
            printf("=");

            // redukcia zatvoriek
        }
        else if (sign == '#')
        {
            printf("end");
            break;
        }
        else
        {
            printf("error");
            // stackDispose(expStack);
            exit(SYNTAX_ERROR);
        }
    }
    inFce = false;
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
    printf("\nprec[%d][%d]\n", ((ElmExp *)(stackTopValue->value))->type, convertToIndex(stackInput));
    return precTable[((ElmExp *)(stackTopValue->value))->type][convertToIndex(stackInput)];
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
//         int i = analyzeExp(&stack, token);
//         printf("%d", i);
//     }

//     return 0;
// }