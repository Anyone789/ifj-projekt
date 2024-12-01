// expression.c
// Expression module
// Author(s):  Marián Šuľa
// Last Edit: 1.12.2024

#include "expression.h"
// Number of brackets in stack
int lBracketInStack = 0;
// Return value of expression
int returnExpValue = I;
// In if or while expression
bool inFce = false;
// Next token
TOKEN *nextToken;
// Precedence table
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
/**
 * @brief Initializes an expression stack.
 *
 * Initializes the stack and pushes an initial expression item with a terminal value
 * and type set to `tableDollar`. If the stack pointer is `NULL`, returns a syntax error.
 *
 * @param expStack A pointer to the expression stack to initialize.
 * @return `0` if successful, `SYNTAX_ERROR` if the stack pointer is `NULL`.
 */
int initExpStack(TStack *expStack)
{
    if (expStack != NULL)
    {
        stackInit(expStack);
        ElmExp *newExp = malloc(sizeof(struct elmExpression));
        if (newExp != NULL)
        {
            newExp->terminal = true;
            newExp->type = tableDollar;
        }
        stackPush(expStack, newExp);
        return 0;
    }
    else
    {
        return SYNTAX_ERROR;
    }
}
/**
 * @brief Checks the semantic compatibility of two operands.
 *
 * This function checks the data types of the left and right operands to ensure they
 * are compatible for operations. If either operand is a string, or if there is a
 * type mismatch, it will trigger an error. It also handles operands of type `T_ID`
    by verifying that the referenced variable types are compatible.
 *
 * @param lOperand A pointer to the left operand.
 * @param rOperand A pointer to the right operand.
 */
void checkSem(ElmExp *lOperand, ElmExp *rOperand)
{
    // At least one operand is string
    if (lOperand->dataType.type == T_STR || rOperand->dataType.type == T_STR)
    {
        exit(INCOMPATIBLE_TYPE_ERROR);
    }
    // Operands are not id
    if (lOperand->dataType.type != T_ID && rOperand->dataType.type != T_ID)
    {

        if (lOperand->dataType.type != rOperand->dataType.type)
        {

            if (lOperand->dataType.type == T_INT)
            {
                printf("POPS GF@op1\n");
                printf("CALL i2f\n");
                printf("PUSHS GF@op1\n");
            }
            else
            {
                printf("CALL i2f\n");
            }
            lOperand->dataType.type = T_FLOAT;
        }
    }
    else
    {
        // Loperand is id check datatypes
        if (lOperand->dataType.type == T_ID && rOperand->dataType.type != T_ID)
        {

            bstSymtable *result = symtableSearch(&symLocal, *lOperand->key);
            if (result == NULL)
            {
                exit(INCOMPATIBLE_TYPE_ERROR);
            }
            else
            {
                if (rOperand->valueInt != 0 && (((varData *)result->data)->dataType.type != rOperand->dataType.type || ((varData *)result->data)->dataType.type == T_STR))
                {

                    exit(INCOMPATIBLE_TYPE_ERROR);
                }
                else
                {
                    ((varData *)result->data)->use = true;
                    if (((varData *)result->data)->dataType.type != rOperand->dataType.type)
                    {
                        if (rOperand->dataType.type == T_INT)
                        {
                            printf("CALL i2f\n");
                        }
                    }
                }
            }
        }
        // Roperand is id check datatypes
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
                if (lOperand->valueInt != 0 && (((varData *)result->data)->dataType.type != lOperand->dataType.type || ((varData *)result->data)->dataType.type == T_STR))
                {
                    exit(INCOMPATIBLE_TYPE_ERROR);
                }
                else
                {
                    lOperand->dataType.type = T_ID;
                    lOperand->key = rOperand->key;
                    ((varData *)result->data)->use = true;
                    if (((varData *)result->data)->dataType.type != lOperand->dataType.type)
                    {
                        if (lOperand->dataType.type == T_INT)
                        {
                            printf("POPS GF@op1\n");
                            printf("CALL i2f\n");
                            printf("PUSHS GF@op1\n");
                        }
                    }
                }
            }
        }
        else
        {
            // Both operands are ids check datatypes
            bstSymtable *lResult = symtableSearch(&symLocal, *lOperand->key);
            bstSymtable *rResult = symtableSearch(&symLocal, *rOperand->key);
            if (lResult == NULL || rResult == NULL)
            {
                exit(INCOMPATIBLE_TYPE_ERROR);
            }
            else
            {
                if (((varData *)lResult->data)->dataType.type != ((varData *)rResult->data)->dataType.type || ((varData *)lResult->data)->dataType.type == T_STR || ((varData *)rResult->data)->dataType.type == T_STR)
                {

                    exit(INCOMPATIBLE_TYPE_ERROR);
                }
                else
                {
                    ((varData *)lResult->data)->use = true;
                    ((varData *)rResult->data)->use = true;
                }
            }
        }
    }
}

/**
 * @brief Validates operands and operator for a binary operation.
 *
 * Ensures that the operands are identifiers, the operator matches the expected type,
 * and checks their semantic compatibility. Exits on syntax errors.
 *
 * @param expStack A pointer to the expression stack.
 * @param operator The expected operator type.
 * @return `0` if successful, exits on error.
 */
int binCheck(TStack *expStack, int operator)
{
    TStackItem *stackItem = expStack->stackTop;
    ElmExp *rOperand = ((ElmExp *)(stackItem->value));
    if (rOperand->type != tableIdentifier || rOperand->terminal == true)
    {
        exit(SYNTAX_ERROR);
    }
    ElmExp *op = ((ElmExp *)(stackItem->next->value));
    if (op->type != operator|| op->terminal == false)
    {
        exit(SYNTAX_ERROR);
    }
    ElmExp *lOperand = ((ElmExp *)(stackItem->next->next->value));
    if (lOperand->type != tableIdentifier || lOperand->terminal == true)
    {
        exit(SYNTAX_ERROR);
    }
    checkSem(lOperand, rOperand);

    return 0;
}
/**
 * @brief Analyzes and processes an expression from the token stream.
 *
 * This function analyzes an expression by pushing tokens onto the stack, performing reductions,
 * and checking semantic and syntax errors. It processes the tokens until the end of the expression is reached.
 *
 * @param expStack A pointer to the expression stack.
 * @param token The current token to process.
 * @return `0` if successful, exits on error.
 */
int analyzeExp(TStack *expStack, TOKEN *token)
{
    // Init stack
    initExpStack(expStack);
    nextToken = token;
    if (nextToken->type == 1)
    {
        exit(LEXICAL_ERROR);
    }

    if (nextToken->type == T_L_BRACKET)
    {
        lBracketInStack++;
    }
    char sign;
    while (1)
    {
        // Get sign
        sign = getSign(expStack);
        // Push to stack rule
        if (sign == '<')
        {
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
        // Use reduce stack rule
        else if (sign == '>')
        {
            if (reduce(expStack) == SYNTAX_ERROR)
            {
                exit(SYNTAX_ERROR);
            }
        }
        // Reduce brackets in stack
        else if (sign == '=')
        {
            ElmExp *newExp = malloc(sizeof(struct elmExpression));
            newExp = expStack->stackTop->value;
            stackPop(expStack);
            stackPop(expStack);
            stackPush(expStack, newExp);
            nextToken = getToken();
            lBracketInStack--;
            if (nextToken->type == 1)
            {
                exit(LEXICAL_ERROR);
            }
        }
        // End of expression
        else if (sign == '#')
        {

            returnExpValue = ((ElmExp *)(expStack->stackTop->value))->dataType.type;
            if (returnExpValue == T_ID)
            {
                bstSymtable *resul = symtableSearch(&symLocal, *((ElmExp *)(expStack->stackTop->value))->key);
                if (resul == NULL)
                {
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
        // Error
        else
        {

            exit(SYNTAX_ERROR);
        }
    }
    stackDispose(expStack);
    lBracketInStack = 0;
    return 0;
}
/**
 * @brief Analyzes and processes an expression from the token stream.
 *
 * This function analyzes an expression by pushing tokens onto the stack, performing reductions,
 * and checking semantic and syntax errors. It processes the tokens until the end of the expression is reached.
 *
 * @param expStack A pointer to the expression stack.
 * @param token The current token to process.
 * @return `0` if successful, exits on error.
 */
char getSign(TStack *expStack)
{
    int stackInput = nextToken->type;
    TStackItem *stackTopValue = expStack->stackTop;
    // Finding terminals in stack
    while (((ElmExp *)(stackTopValue->value))->terminal != true)
    {
        stackTopValue = stackTopValue->next;
    }
    return precTable[((ElmExp *)(stackTopValue->value))->type][convertToIndex(stackInput)];
}
/**
 * @brief Performs reductions on the expression stack based on operator precedence and types.
 *
 * This function processes the top element of the expression stack, checking its type, and performing
 * the corresponding operation. It handles various types of operations (arithmetic, comparison, etc.)
 * and prints the appropriate bytecode instructions. It also ensures that type compatibility and syntax
 * are correct during the reductions.
 *
 * @param expStack A pointer to the expression stack, which holds the operands and operators for reduction.
 * @return `SYNTAX_ERROR` if there is a syntax error during reduction; otherwise, the function returns nothing.
 */
int reduce(TStack *expStack)
{
    TStackItem *stackItem = expStack->stackTop;
    while (((ElmExp *)(stackItem->value))->terminal != true)
    {
        stackItem = stackItem->next;
    }
    // Use role from operand/id in stack
    switch (((ElmExp *)(stackItem->value))->type)
    {
    case tableIdentifier:
        ((ElmExp *)(stackItem->value))->terminal = false;
        // Generate
        if (((ElmExp *)(stackItem->value))->dataType.type == T_INT)
        {
            printf("PUSHS int@%d\n", ((ElmExp *)(stackItem->value))->valueInt);
        }
        else if (((ElmExp *)(stackItem->value))->dataType.type == T_FLOAT)
        {

            printf("PUSHS float@%a\n", ((ElmExp *)(stackItem->value))->valueFloat);
        }
        else if (((ElmExp *)(stackItem->value))->dataType.type == T_STR)
        {
            printf("PUSHS string@%s\n", ((ElmExp *)(stackItem->value))->key->str);
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
        // Generate
        printf("MULS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tablePlus:
        if (binCheck(expStack, tablePlus) != 0)
        {
            return SYNTAX_ERROR;
        }
        // Generate
        printf("ADDS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableMinus:
        if (binCheck(expStack, tableMinus) != 0)
        {
            return SYNTAX_ERROR;
        }
        // Generate
        printf("SUBS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableDivide:
        if (binCheck(expStack, tableDivide) != 0)
        {
            return SYNTAX_ERROR;
        }
        // Generate
        printf("DIVS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableEqual:
        if (binCheck(expStack, tableEqual) != 0)
        {
            return SYNTAX_ERROR;
        }
        if (!inFce)
        {
            exit(INCOMPATIBLE_TYPE_ERROR);
        }
        // Generate
        printf("EQS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableNotEqual:
        if (binCheck(expStack, tableNotEqual) != 0)
        {
            return SYNTAX_ERROR;
        }
        if (!inFce)
        {
            exit(INCOMPATIBLE_TYPE_ERROR);
        }
        // Generate
        printf("EQS\n");
        printf("NOTS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableLess:
        if (binCheck(expStack, tableLess) != 0)
        {
            return SYNTAX_ERROR;
        }
        if (!inFce)
        {
            exit(INCOMPATIBLE_TYPE_ERROR);
        }
        // Generate
        printf("LTS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableLessEqual:
        if (binCheck(expStack, tableLessEqual) != 0)
        {
            return SYNTAX_ERROR;
        }
        if (!inFce)
        {
            exit(INCOMPATIBLE_TYPE_ERROR);
        }
        // Generate
        printf("POPS GF@op1\n");
        printf("POPS GF@op2\n");
        printf("LT GF@result GF@op1 GF@op2\n");
        printf("PUSHS GF@result\n");
        printf("EQ GF@result GF@op1 GF@op2\n");
        printf("PUSHS GF@result\n");
        printf("ORS\n");

        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableGreat:
        if (binCheck(expStack, tableGreat) != 0)
        {
            return SYNTAX_ERROR;
        }
        if (!inFce)
        {
            exit(INCOMPATIBLE_TYPE_ERROR);
        }
        // Generate
        printf("GTS\n");
        stackPop(expStack);
        stackPop(expStack);
        break;

    case tableGreatEqual:
        if (binCheck(expStack, tableGreatEqual) != 0)
        {
            return SYNTAX_ERROR;
        }
        if (!inFce)
        {
            exit(INCOMPATIBLE_TYPE_ERROR);
        }
        // Generate
        printf("POPS GF@op1\n");
        printf("POPS GF@op2\n");
        printf("GT GF@result GF@op1 GF@op2\n");
        printf("PUSHS GF@result\n");
        printf("EQ GF@result GF@op1 GF@op2\n");
        printf("PUSHS GF@result\n");
        printf("ORS\n");

        stackPop(expStack);
        stackPop(expStack);
        break;
    case tableLeftPar:
        return SYNTAX_ERROR;
        break;
    }
}

/**
 * @brief Converts a token type to an index for stack management or precedence.
 *
 * This function maps token types (e.g., operators, identifiers, brackets) to indices used in expression evaluation or stack operations.
 *
 * @param value The token type (e.g., operator, identifier).
 * @return An index representing the token type for processing.
 */
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

        return tableLeftPar;
    case T_R_BRACKET:
        if (inFce == true && lBracketInStack == 0)
        {
            return tableDollar;
        }
        return tableRightPar;
    case T_SEMICOL:
        return tableDollar;
    }
    return tableOther;
}