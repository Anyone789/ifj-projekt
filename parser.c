// header file for parser for ifj24
// authors: Samuel Kundrat
// created: 11.11.2024
// edited: 17.11.2024

#include "parser.h"

bool nullType = false;
bool isConst = false;
bool assign = false;
bstSymtable *symTree;
bstSymtable *symLocal;

// PARSING RULES
ProductionRule llRules[] =
    {
        {nLlProgram, {nLlPrologue, nLlProgramBody, EOF}},
        {nLlPrologue, {tLlConst, tLlIfj, tLlEqual, tLlImport, tLlLeftRoundBracket, tLlString, tLlRightRoundBracket, tLlSemicolon}},
        {nLlProgramBody, {nLlFunctDef, nLlProgramBody}},
        {nLlProgramBody, {nLlFunctCall, nLlProgramBody}},
        {nLlProgramBody, {}},
        {nLlFunctDef, {tLlPub, tLlFn, tLlFunctId, tLlLeftRoundBracket, nLlParamList, tLlRightRoundBracket, nLlType, tLlLeftCurlyBracket, nLlFunctBody, tLlRightCurlyBracket}},
        {nLlParamList, {tLlId, tLlColon, nLlType, nLlParams}},
        {nLlParamList, {}},
        {nLlType, {tLlInt}},
        {nLlType, {tLlFloat}},
        {nLlType, {tLlleftSquareBracket, tLlrightSquareBracket, tLlString}},
        {nLlType, {}},
        {nLlParams, {tLlComma, nLlParamList}},
        {nLlParams, {}},
        {nLlFunctCall, {nLlFunctId, tLlLeftRoundBracket, nLlParamCallList, tLlRightRoundBracket, tLlSemicolon}},
        {nLlParamCallList, {tLlId, nLlParamsCall}},
        {nLlParamCallList, {nLlType, nLlParamsCall}},
        {nLlParamsCall, {tLlComma, nLlParamCallList}},
        {nLlParamsCall, {}},
        {nLlFunctBody, {nLlStatement, nLlFunctBody}},
        {nLlFunctBody, {}},
        {nLlStatement, {nLlConditionStatement}},
        {nLlStatement, {nLlLoopStatement}},
        {nLlStatement, {nLlReturnStatement}},
        {nLlStatement, {nLlFunctCall}},
        {nLlStatement, {nLlVarDefStatement}},
        {nLlStatement, {nLlConstDefStatement}},
        {nLlStatement, {nLlVarInitStatement}},
        {nLlConditionStatement, {tLlIf, tLlLeftRoundBracket, EXP, tLlRightRoundBracket, nLlNoNullBody, tLlLeftCurlyBracket, nLlFunctBody, tLlRightCurlyBracket, tLlElse, tLlLeftCurlyBracket, nLlFunctBody, tLlRightCurlyBracket}},
        {nLlLoopStatement, {tLlWhile, tLlLeftRoundBracket, EXP, tLlRightRoundBracket, nLlNoNullBody, tLlLeftCurlyBracket, nLlFunctBody, tLlRightCurlyBracket}},
        {nLlReturnStatement, {tLlReturn, nLlReturnList}},
        {nLlReturnList, {tLlSemicolon}},
        {nLlReturnList, {nLlItem}},
        {nLlItem, {nLlFunctCall}},
        {nLlItem, {tLlNull, tLlSemicolon}},
        {nLlItem, {EXP, tLlSemicolon}},
        {nLlVarDefStatement, {tLlVar, tLlId, nLlDefType, tLlEqual, nLlItem}},
        {nLlConstDefStatement, {tLlConst, tLlId, nLlDefType, tLlEqual, nLlItem}},
        {nLlVarInitStatement, {tLlId, tLlEqual, nLlItem}},
        {nLlNoNullBody, {}},
        {nLlNoNullBody, {tLlPipe, tLlId, tLlPipe}},
        {nLlFunctId, {tLlFunctId}},
        {nLlFunctId, {tLlIfj, tLlDot, tLlFunctId}},
        {nLlDefType, {tLlColon, nLlType}},
        {nLlDefType, {}},
        {nLlType, {tLlVoid}},
        {nLlType, {tLlString}},
};

// 2D array of LL1 table for ifj24 compile
int llTable[NON_TERMINAL_COUNT][TERMINAL_COUNT] = {
    //                           c   p   f   F   I   i   f   s  if   w   r   v   ,   (   )   =   ;   {   }   e   :   $   |  ifj  [   ]  v null
    /* <program>             */ {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <prologue>            */ {1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <program_body>        */ {-1, 2, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1},
    /* <funct_def>           */ {-1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <param_list>          */ {-1, -1, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <type>                */ {-1, -1, -1, -1, -1, 8, 9, 46, -1, -1, -1, -1, 11, -1, 11, 11, -1, 11, -1, -1, -1, -1, -1, -1, 10, -1, 45, -1},
    /* <params>              */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, -1, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <funct_call>          */ {-1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1, -1},
    /* <param_call_list>     */ {-1, -1, -1, -1, 15, 16, 16, 16, -1, -1, -1, -1, 16, -1, 16, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <params_call>         */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 17, -1, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <funct_body>          */ {19, -1, -1, 19, 19, -1, -1, -1, 19, 19, 19, 19, -1, -1, -1, -1, -1, -1, 20, -1, -1, -1, -1, 19, -1, -1, -1, -1},
    /* <statement>           */ {26, -1, -1, 24, 27, -1, -1, -1, 21, 22, 23, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 24, -1, -1, -1, -1},
    /* <condition_statement> */ {-1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <loop_statement>      */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, 29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <return_statement>    */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <return_list>         */ {-1, -1, -1, 32, 32, 32, 32, 32, -1, -1, -1, -1, -1, -1, -1, -1, 31, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, -1},
    /* <item>                */ {-1, -1, -1, 33, 35, 35, 35, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, 34},
    /* <var_def_statement>   */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 36, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <const_def_statement> */ {37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <var_init_statement>  */ {-1, -1, -1, -1, 38, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <noNull_body>         */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1},
    /* <param_list>          */ {-1, -1, -1, 41, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 42, -1, -1, -1, -1},
    /* <def_type>            */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 44, -1, -1, -1, -1, 43, -1, -1, 42, -1, -1, -1, -1},
};

int parsIt(TStack *parserStack)
{
    stackInit(parserStack);
    pushRule(parserStack, llTable[0][0]);
    parserIn(parserStack);
    return 1;
}

void pushRule(TStack *parserStack, int rule)
{
    int i = MAX_RULE_ITEMS - 1;
    // loop that pushes whole right side of rule to the stack without 0
    while (i >= 0)
    {
        if ((intptr_t)llRules[rule].production[i] == 0)
        {
            // condition to push terminal with enum = 0 to the stack
            if ((rule == 1 && i == 0) || (rule == 37 && i == 0))
            {
                stackPush(parserStack, (void *)(intptr_t)llRules[rule].production[i]);
                i--;
                continue;
            }
            else
            {
                i--;
                continue;
            }
        }

        stackPush(parserStack, (void *)(intptr_t)llRules[rule].production[i]);
        i--;
    }
}

int convertTokenToIndex(TOKEN *token)
{
    // decoder from token enumerator to parser enumerator
    switch (token->type)
    {
    case T_KEYWORD:
        if (strcmp(token->attribute.dStr->str, "const") == 0)
        {
            isConst = true;
            return tLlConst;
        }
        else if (strcmp(token->attribute.dStr->str, "pub") == 0)
        {
            return tLlPub;
        }
        else if (strcmp(token->attribute.dStr->str, "fn") == 0)
        {
            return tLlFn;
        }
        else if (strcmp(token->attribute.dStr->str, "if") == 0)
        {
            return tLlIf;
        }
        else if (strcmp(token->attribute.dStr->str, "while") == 0)
        {
            return tLlWhile;
        }
        else if (strcmp(token->attribute.dStr->str, "return") == 0)
        {
            return tLlReturn;
        }
        else if (strcmp(token->attribute.dStr->str, "var") == 0)
        {
            return tLlVar;
        }
        else if (strcmp(token->attribute.dStr->str, "else") == 0)
        {
            return tLlElse;
        }
        else if (strcmp(token->attribute.dStr->str, "import") == 0)
        {
            return tLlImport;
        }
        else if (strcmp(token->attribute.dStr->str, "i32") == 0)
        {
            return tLlInt;
        }
        else if (strcmp(token->attribute.dStr->str, "u8") == 0)
        {
            return tLlString;
        }
        else if (strcmp(token->attribute.dStr->str, "f64") == 0)
        {
            return tLlFloat;
        }
        else if (strcmp(token->attribute.dStr->str, "void") == 0)
        {
            return tLlVoid;
        }
        else if (strcmp(token->attribute.dStr->str, "null") == 0)
        {
            return tLlNull;
        }
        else if (strcmp(token->attribute.dStr->str, "NULL") == 0)
        {
            return tLlNull;
        }
        else
        {
            exit(SYNTAX_ERROR);
        }

        break;
    case T_ERROR:
        exit(LEXICAL_ERROR);
    case T_ID:
        if (strcmp(token->attribute.dStr->str, "ifj") == 0)
        {
            return tLlIfj;
        }

        return tLlId;
        break;
    case T_FX_ID:

        return tLlFunctId;
        break;
    case T_BUILT_IN_FX_ID:
        return tLlIfj;
        break;
    case T_INT:
        return tLlInt;
        break;
    case T_FLOAT:
        return tLlFloat;
        break;
    case T_STR:
        return tLlString;
        break;
    case T_PLUS:
    case T_MINUS:
    case T_MUL:
    case T_DIV:
    case T_NE:
    case T_LT:
    case T_LE:
    case T_GT:
    case T_GE:
    case T_EQ:
        exit(SYNTAX_ERROR);
    case T_ASSIGN:
        return tLlEqual; // equel = "=" not "==" name isnt the best but it is what it is
        break;
    case T_SEMICOL:
        return tLlSemicolon;
        break;
    case T_COLON:
        return tLlColon;
        break;
    case T_L_BRACE:
        return tLlLeftCurlyBracket;
        break;
    case T_R_BRACE:
        return tLlRightCurlyBracket;
        break;
    case T_L_SQ_BRACKET:
        return tLlleftSquareBracket;
        break;
    case T_R_SQ_BRACKET:
        return tLlrightSquareBracket;
        break;
    case T_L_BRACKET:
        return tLlLeftRoundBracket;
        break;
    case T_R_BRACKET:
        return tLlRightRoundBracket;
        break;
    case T_QUESTION_MK:
        return tLlQuestionMark;
        break;
    case T_DOT:
        return tLlDot;
        break;
    case T_COMMA:
        return tLlComma;
        break;
    case T_PIPE:
        return tLlPipe;
    case T_EOF:
        return tLlDollar;
    case T_UNDEFINED:
        exit(LEXICAL_ERROR);
        break;
    default:
        exit(SYNTAX_ERROR);
        break;
    }
}

void parserIn(TStack *parserStack)
{
    DSTRING *ID;
    ID = dStringCreate();
    TOKEN *token;
    int literal = 99;
    int top;

    token = getToken();
    literal = convertTokenToIndex(token);

    // loop for whole parsing phaze, loops till EOF or error
    while (literal != EOF)
    {
        printf("%d*******\n", assign);
        if (literal == tLlId && assign == false)
        {
            printf("********NA VSTUPE JE TOKEN ID************\n");
            ID = token->attribute.dStr;
            printf("%s\n", ID->str);
        }
        // top is top value from the stack
        top = *(int *)(parserStack->stackTop);
        printf("som tu\n");
        printf("top: %d\n", top);
        printf("literal: %d\n", literal);
        stackPrint(parserStack);

        // condition which sets global variable to tru to let exprssion parsing now that we are parsing if condition expression
        if (literal == tLlIf)
        {
            inFce = true;
        }

        if (literal == tLlEqual)
        {
            assign = true;
            bstSymtable *res = symtableSearch(&symLocal, *ID);
            if (res != NULL)
            {
                ((varData *)res->data)->initialized = true;
            }
        }

        printf("==================76==========\n");
        // terminal part
        if ((top >= 0 && top <= TERMINAL_COUNT - 1) || (top <= -1 && top >= -8))
        {

            printf("NA VSTUPE JE TERMINAL\n");
            printf("top: %d\n", top);
            printf("literal: %d\n", literal);
            // END OF PARSING
            if (top == -1 && literal == 21)
            {
                stackPop(parserStack);
                printf("END");
                return;
            }
            else if (top == literal)
            {
                stackPop(parserStack);
                printf("po pope je stack\n");
                stackPrint(parserStack);
                // parserIn(parserStack);
                // continue;
            }
            else
            {
                printf("syntax error");
                exit(SYNTAX_ERROR);
            }

            token = getToken();
            literal = convertTokenToIndex(token);
            printf("sdads");
            if (top == tLlVar || top == tLlConst)
            {
                if (symtableSearch(&symLocal, *token->attribute.dStr) == NULL)
                {
                    insertVariables(token->attribute.dStr->str, (DATATYPE){false, false, token->type}, false, isConst, false, false, &symLocal);
                }
                else
                {
                    printf("majko");

                    exit(REDEFINITION_ERROR);
                    /* code */
                }
                isConst = false;
            }

            if (literal == tLlSemicolon)
            {
                assign = false;
            }
        }
        // none terminal phaze, pops nonterminal from stack and pushes right side of specific rule to it
        else if (top >= 100 && top <= 122)
        {
            printf("-------------top: %d\n", top);
            printf("literal: %d\n", literal);

            // null type part
            // this condition checks if there is ? before data type and sets global variable nullType to true to let semantic analizator let know
            if (top == 105 && literal == 28)
            {
                nullType = true;
                printf("ID moze byt aj NULL\n");
                token = getToken();
                literal = convertTokenToIndex(token);
                // checker if user isnt using null or void with null typr indicator
                if (literal == 26 || literal == 27)
                {
                    printf("void alebo null nemoze byt null");
                    exit(SYNTAX_ERROR);
                }
            }
            else
            {
                printf("ID moze byt aj NULL v else\n");
                nullType = false;
            }

            if ((llTable[top % 100][literal]) == 38)
            {
                bstSymtable *res = symtableSearch(&symLocal, *ID);
                if (res == NULL)
                {
                    exit(UNDEFINED_VARIABLE_ERROR);
                }
                if (((varData *)res->data)->constant == true)
                {
                    exit(REDEFINITION_ERROR);
                }
            }

            if ((llTable[top % 100][literal]) == 9 || (llTable[top % 100][literal]) == 8)
            {
                bstSymtable *res = symtableSearch(&symLocal, *ID);
                printf("jhash %s", ID->str);
                if (res == NULL)
                {
                    exit(UNDEFINED_VARIABLE_ERROR);
                }
                else
                {

                    if (strcmp(token->attribute.dStr->str, "i32") == 0)
                    {
                        ((varData *)res->data)->dataType.type = T_INT;
                    }

                    else if (strcmp(token->attribute.dStr->str, "f64") == 0)
                    {
                        ((varData *)res->data)->dataType.type = T_FLOAT;
                    }
                }
            }
            if ((llTable[top % 100][literal]) == 10)
            {
                bstSymtable *res = symtableSearch(&symLocal, *ID);
                if (res == NULL)
                {
                    exit(UNDEFINED_VARIABLE_ERROR);
                }
                else
                {
                    printf("kjsabkj%d", token->type);
                    if (token->type == T_L_SQ_BRACKET)
                    {
                        ((varData *)res->data)->dataType.type = T_STR;
                    }
                }
            }

            printf("NA VSTUPE JE NETERMINAL ALEBO SPECIAL\n");
            printf("top: %d\n", top);
            printf("literal: %d\n", literal);
            printf("pravidlo: %d\n", (llTable[top % 100][literal]));
            stackPop(parserStack);
            printf("po pope je stack\n");
            stackPrint(parserStack);
            if ((llTable[top % 100][literal]) == -1)
            {
                printf("syntax error");
                exit(SYNTAX_ERROR);
            }

            pushRule(parserStack, llTable[top % 100][literal]);
            continue;
        }
        // expression part
        // uses double pop, because we want to get rid of the exp terminal from stack and first item after it, because expression takes one more token at the end
        else if (top == -10)
        {
            stackPop(parserStack);
            stackPop(parserStack);
            printf("bude exprssion:   %d\n", inFce);
            TStack expStack;
            analyzeExp(&expStack, token);

            if (inFce == false)
            {
                bstSymtable *result = symtableSearch(&symLocal, *ID);
                if (result == NULL)
                {
                    printf("sadsadass");
                    exit(SYNTAX_ERROR);
                }
                else
                {
                    printf("alles gut\n%d, %d", ((varData *)result->data)->dataType.type, returnExpValue);
                    if (((varData *)result->data)->dataType.type == T_ID)
                    {
                        ((varData *)result->data)->dataType.type = returnExpValue;
                        /* code */
                    }
                    else if (((varData *)result->data)->dataType.type != returnExpValue)
                    {
                        exit(INCOMPATIBLE_TYPE_ERROR);
                    }

                    //*ID = NULL;
                }
            }

            token = getToken();
            // printf("padla expression");
            // return;
            literal = convertTokenToIndex(token);
            //  stackPush(parserStack, (void *)(intptr_t)tLlRightRoundBracket);
            stackPrint(parserStack);
            printf("token po EXP: %d\n", literal);
            assign = false;
        }
    }
}

// uncomment main for testing

int main(int argc, char **argv)
{
    FILE *src;

    if ((src = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "The file cannot be opened.");
        exit(0);
    }

    setSourceFile(src);
    TStack parserStack;
    symtableInit(&symTree);
    symtableInit(&symLocal);
    parsIt(&parserStack);
    DSTRING *str = dStringCreate();
    dStringAddString(str, "a");
    bstSymtable *resLocal = symtableSearch(&symLocal, *str);
    if (resLocal != NULL)
    {
        printf("\nFound in locals %d %d\n", ((varData *)resLocal->data)->dataType.type, isConst);
    }
    else
    {
        printf("Not found in locals\n");
    }

    return 0;
}