// header file for parser for ifj24
// authors: Samuel Kundrat
// created: 11.11.2024
// edited: 17.11.2024

#include "parser.h"

bool nullType = false;
bool isConst = false;
bool assign = false;
int paramCountGlob = 0;
bstSymtable *symTree;
bstSymtable *symLocal;
int ifAloneCounter = 0;
int ifInsideCount = 0;

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
    /* <return_list>         */ {-1, -1, -1, 32, 32, 32, 32, 32, -1, -1, -1, -1, -1, -1, -1, -1, 31, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, 34},
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
void usedVar(bstSymtable **symTree)
{
    if (*symTree != NULL)
    {
        // printf("%s", ((DSTRING *)(*symTree)->key)->str);
        if (((varData *)(*symTree)->data)->use == false)
        {
            exit(UNUSED_VARIABLE_ERROR);
        }

        usedVar(&(*symTree)->left);
        usedVar(&(*symTree)->right);
    }
}
void defFunction(bstSymtable **symTree)
{
    if (*symTree != NULL)
    {
        // printf("%s", ((DSTRING *)(*symTree)->key)->str);
        if (((fceData *)(*symTree)->data)->isDefined == false)
        {
            exit(UNUSED_VARIABLE_ERROR);
        }

        defFunction(&(*symTree)->left);
        defFunction(&(*symTree)->right);
    }
}

void processVariable(bstSymtable *res, int type, int *paramTypeCounter)
{
    if (res != NULL)
    {
        ((varData *)res->data)->dataType.type = type;
        ((varData *)res->data)->dataType.isNull = nullType;
        *paramTypeCounter = 0;
    }
}
void processFunction(bstSymtable *res, bstSymtable *resFce, DSTRING *functionID, int type, int *paramCount, int state)
{
    if (resFce != NULL && state == nLlParamList)
    {
        if (((fceData *)(resFce->data))->isDefined)
        {
            varData *varDatas = malloc(sizeof(varData) * 1);
            if (varDatas == NULL)
            {
                exit(EXIT_FAILURE);
            }
            varDatas[0].dataType = (DATATYPE){nullType, false, type};
            varDatas[0].name = res->key;
            // printf("ISNULL%d %s\n", nullType, resFce->key);
            insertFunction(&symTree, functionID->str, (DATATYPE){nullType, false, type}, *paramCount + 1, true, false, true, varDatas);
            (*paramCount)++;
            paramCountGlob = *paramCount;
        }
        else
        {
            // printf("\nParCount%d, %d\n", ((fceData *)(resFce->data))->paramCount, *paramCount);
            if (((fceData *)(resFce->data))->paramCount > 0)
            {
                if ((int)(((fceData *)resFce->data)->params[*paramCount].dataType.type) != type)
                {
                    exit(WRONG_ARGUMENTS_ERROR);
                }
                if (((fceData *)resFce->data)->paramCount <= *paramCount)
                {
                    exit(WRONG_ARGUMENTS_ERROR);
                }
                ((fceData *)(resFce->data))->params[*paramCount].name = res->key;
            }

            (*paramCount)++;
            paramCountGlob = *paramCount;
        }
    }
}
void setFunctionReturnType(bstSymtable *resFce, int type)
{
    if (resFce != NULL)
    {
        ((fceData *)resFce->data)->returnType.type = type;
        ((fceData *)resFce->data)->returnType.isNull = nullType;
        ((fceData *)(resFce->data))->isDefined = true;
        generateFunctionHead(resFce);
    }
}
void handleTokenType(bstSymtable *res, bstSymtable *resFce, TOKEN *token, DSTRING *functionID, int *paramCount, int *paramTypeCounter, int state)
{
    if (token->type == T_L_SQ_BRACKET)
    {
        if (*paramTypeCounter == 1)
        {
            processFunction(res, resFce, functionID, T_STR, paramCount, state);
            processVariable(res, T_STR, paramTypeCounter);
        }
        else
        {
            setFunctionReturnType(resFce, T_STR);
        }
    }
    else if (strcmp(token->attribute.dStr->str, "i32") == 0)
    {
        if (*paramTypeCounter == 1)
        {
            processFunction(res, resFce, functionID, T_INT, paramCount, state);
            processVariable(res, T_INT, paramTypeCounter);
        }
        else
        {
            setFunctionReturnType(resFce, T_INT);
        }
    }
    else if (strcmp(token->attribute.dStr->str, "f64") == 0)
    {
        if (*paramTypeCounter == 1)
        {
            processFunction(res, resFce, functionID, T_FLOAT, paramCount, state);
            processVariable(res, T_FLOAT, paramTypeCounter);
        }
        else
        {
            setFunctionReturnType(resFce, T_FLOAT);
        }
    }
    else
    {
        if (*paramTypeCounter == 1)
        {
            processVariable(res, T_KEYWORD, paramTypeCounter);
        }
        else
        {
            setFunctionReturnType(resFce, T_KEYWORD);
        }
    }
}

void pushRule(TStack *parserStack, int rule)
{
    int i = MAX_RULE_ITEMS - 1;
    // loop that pushes whole right side of rule to the stack without 0
    while (i >= 0)
    {
        if (llRules[rule].production[i] == 0)
        {
            // condition to push terminal with enum = 0 to the stack
            if ((rule == 1 && i == 0) || (rule == 37 && i == 0))
            {
                stackPush(parserStack, (void *)llRules[rule].production[i]);
                i--;
                continue;
            }
            else
            {
                i--;
                continue;
            }
        }

        stackPush(parserStack, (void *)llRules[rule].production[i]);
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
            if (ifInsideCount == 0)
            {
                //printf("sadsas\n");
                ifAloneCounter++;
            }
            ifInsideCount++;
            // printf("else: %d, if: %d\n", elseCounter, ifCounter);
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
    DSTRING *functionID;
    DSTRING *functionIDCurrent;
    functionID = dStringCreate();
    DSTRING *currentID;
    currentID = dStringCreate();
    TOKEN *token;
    TOKEN *tokenBefore = token;
    int literal = 99;
    int top;
    int paramTypeCounter = 0;
    int paramCount = 0;
    int paramCountList = 0;
    NonTerminals state;
    bool fceCall = false;
    bool varDef = false;
    token = getToken();
    literal = convertTokenToIndex(token);
    bool ifj = false;
    bool unusedFce = false;
    bool isReturn = false;

    bool ifBody = false;
    bool elseBody = false;
    int elseCount = 0;
    // loop for whole parsing phaze, loops till EOF or error
    generateHeader();
    while (literal != EOF)
    {

        // printf("%d*******\n", assign);

        if (literal == tLlId && (!assign || (assign && fceCall)))
        {
            if (strcmp(token->attribute.dStr->str, "ifj") != 0)
            {
                // printf("********NA VSTUPE JE TOKEN ID************\n");
                ID = token->attribute.dStr;
                // printf("%s\n", ID->str);
                paramTypeCounter = 1;
            }
        }
        if (literal == tLlIfj)
        {
            // printf("padlo ifj\n");
            ifj = true;
        }

        // printf("state:%d literal:%d token:%d\n", state, literal, token->type);
        if (literal == tLlFunctId)
        {
            // printf("********NA VSTUPE JE TOKEN ID************\n");
            functionID = token->attribute.dStr;
            // printf("%s\n", functionID->str);
        }

        if (literal == tLlElse)
        {

            generateElse(ifAloneCounter, ifInsideCount);
            ifBody = false;
            elseBody = true;
            elseCount++;
        }
        if (literal == tLlIf)
        {
            elseBody = false;
            // elseCount--;
        }

        // top is top value from the stack
        top = *(int *)(parserStack->stackTop);
        // printf("som tu\n");
        // printf("top: %d\n", top);
        // printf("literal: %d\n", literal);
        // stackPrint(parserStack);

        // condition which sets global variable to tru to let exprssion parsing now that we are parsing if condition expression
        if (literal == tLlIf || literal == tLlWhile)
        {

            inFce = true;
            ifBody = true;
        }
        if (literal == tLlLeftCurlyBracket || literal == tLlPipe)
        {
            inFce = false;
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

        // printf("==================76==========\n");

        // terminal part
        if ((top >= 0 && top <= TERMINAL_COUNT - 1) || (top <= -1 && top >= -8))
        {

            // printf("NA VSTUPE JE TERMINAL\n");
            // printf("top: %d\n", top);
            // printf("literal: %d\n", literal);
            // END OF PARSING

            if (literal == tLlReturn)
            {
                isReturn = true;
            }

            if (top == -1 && literal == 21)
            {
                stackPop(parserStack);
                // printf("END");
                defFunction(&symTree);
                DSTRING *str = dStringCreate();
                dStringAddString(str, "main");
                bstSymtable *resLocal = symtableSearch(&symTree, *str);

                if (resLocal == NULL)
                {
                    exit(MISSING_MAIN);
                }
                else
                {
                    if (((fceData *)(resLocal->data))->paramCount != 0 || ((fceData *)(resLocal->data))->returnType.type != 0)
                    {
                        exit(WRONG_ARGUMENTS_ERROR);
                    }
                }
                return;
            }
            else if (top == literal)
            {
                stackPop(parserStack);
                // printf("po pope je stack\n");
                // stackPrint(parserStack);
            }
            else
            {
                // printf("syntax error");
                exit(SYNTAX_ERROR);
            }

            // printf("sdads");
            tokenBefore = token;
            token = getToken();
            literal = convertTokenToIndex(token);
            // adding variables to localTree

            if ((top == tLlVar || top == tLlConst || top == tLlLeftRoundBracket || top == tLlComma || top == tLlPipe) && token->type == T_ID)
            {
                if (inFce == false)
                {
                    if (symtableSearch(&symLocal, *token->attribute.dStr) == NULL)
                    {
                        // printf("%djshag %s\n", nullType, token->attribute.dStr->str);
                        if (top == tLlPipe)
                        {
                            // printf("CAUUU");
                            bstSymtable *res = symtableSearch(&symLocal, *ID);
                            // printf("ID IF %s %d", ID->str, ((varData*)res->data)->dataType.type);
                            if (res == NULL)
                            {
                                exit(GENERIC_SEMANTIC_ERROR);
                            }

                            insertVariables(token->attribute.dStr->str, (DATATYPE){false, false, ((varData *)res->data)->dataType.type}, false, isConst, false, false, &symLocal);
                        }
                        else
                        {
                            insertVariables(token->attribute.dStr->str, (DATATYPE){nullType, false, token->type}, false, isConst, false, false, &symLocal);
                            if (strcmp(token->attribute.dStr->str, "ifj") != 0 && (state != nLlParams && state != nLlFunctDef))
                            {
                                // printf("**%d**\n", state);

                                generateVariables(token->attribute.dStr);
                            }
                        }

                        varDef = true;
                    }
                    else
                    {
                        if (!fceCall)
                        {

                            exit(REDEFINITION_ERROR);
                            /* code */
                        }

                        /* code */
                    }
                    isConst = false;
                }
            }
            if (top == tLlVar || top == tLlConst)
            {
                currentID = token->attribute.dStr;
            }

            // adding functions to globalTree
            if (top == tLlFn)
            {
                bstSymtable *resFce = symtableSearch(&symTree, *token->attribute.dStr);
                if (resFce == NULL)
                {
                    insertFunction(&symTree, token->attribute.dStr->str, (DATATYPE){nullType, false, token->type}, 0, true, false, true, NULL);
                    functionIDCurrent = token->attribute.dStr;
                }
                else
                {
                    if (((fceData *)(resFce->data))->isDefined == true)
                    {

                        exit(REDEFINITION_ERROR);
                        /* code */
                    }
                    functionIDCurrent = token->attribute.dStr;
                    // ((fceData *)(resFce->data))->isDefined = true;
                    /* code */
                }
                paramCountGlob = 0;
            }

            if (literal == tLlSemicolon)
            {
                if (fceCall)
                {
                    bstSymtable *res = symtableSearch(&symTree, *functionID);

                    if (res != NULL)
                    {
                        if (paramCountList != ((fceData *)(res->data))->paramCount)
                        {
                            // printf("dasd%d %d", paramCountList, ((fceData *)(res->data))->paramCount);
                            exit(WRONG_ARGUMENTS_ERROR);
                        }
                        /* code */
                    }
                    printf("CALL %s\n", res->key);

                    if (assign == true)
                    {
                        printf("POPS LF@%s\n", currentID->str);
                    }
                }
                // printf("kajshdkjhsa%d\n", paramCount);
                paramCount = 0;
                assign = false;
                ifj = false;
                paramCountList = 0;
                unusedFce = false;
                varDef = false;
                isReturn = false;
                fceCall = false;
                isConst = false;
            }
            // printf("call:%d assign:%d literal:%d ifj:%d\n", fceCall, assign, literal, ifj);
            if (fceCall && assign && literal == tLlLeftRoundBracket && ifj)
            {
                bstSymtable *resFce = symtableSearch(&symTree, *functionID);
                bstSymtable *res = symtableSearch(&symLocal, *ID);

                if (resFce == NULL || res == NULL)
                {
                    // printf("NULL fceID:%s id:%s\n", functionID->str, ID->str);
                    // exit(UNDEFINED_VARIABLE_ERROR);
                }
                else
                {

                    // if (((fceData *)resFce->data)->returnType.type != ((varData *)res->data)->dataType.type)
                    // {
                    //     exit(INCOMPATIBLE_TYPE_ERROR);
                    // }
                    // printf("fceID:%s id:%s\n", functionID->str, ID->str);
                    if (((varData *)res->data)->dataType.type == T_ID)
                    {
                        ((varData *)res->data)->dataType.type = ((fceData *)resFce->data)->returnType.type;
                        ((varData *)res->data)->dataType.isNull = ((fceData *)resFce->data)->returnType.isNull;
                    }
                    else if ((((fceData *)resFce->data)->returnType.type != ((varData *)res->data)->dataType.type) ||
                             (((fceData *)resFce->data)->returnType.isNull != ((varData *)res->data)->dataType.isNull))
                    {
                        exit(INCOMPATIBLE_TYPE_ERROR);
                    }
                }
            }
        }
        // none terminal phaze, pops nonterminal from stack and pushes right side of specific rule to it
        else if (top >= 100 && top <= 122)
        {
            // printf("-------------top: %d\n", top);
            // printf("literal: %d\n", literal);

            // null type part
            // printf("pravidlo bude: %d\n", (llTable[top % 100][literal]));
            // this condition checks if there is ? before data type and sets global variable nullType to true to let semantic analizator let know
            if (top == 105 && literal == 28)
            {
                nullType = true;
                // printf("ID moze byt aj NULL\n");
                tokenBefore = token;
                token = getToken();
                literal = convertTokenToIndex(token);
                // checker if user isnt using null or void with null typr indicator
                if (literal == 26 || literal == 27)
                {
                    // printf("void alebo null nemoze byt null");
                    exit(SYNTAX_ERROR);
                }
            }
            else
            {
                // printf("ID moze byt aj NULL v else\n");
                nullType = false;
            }

            if ((llTable[top % 100][literal]) == 38)
            {
                currentID = ID;
                bstSymtable *res = symtableSearch(&symLocal, *ID);

                if (res == NULL)
                {
                    exit(UNDEFINED_VARIABLE_ERROR);
                    printf("sasasa");
                }
                else
                {
                    ((varData *)res->data)->use = true;
                }
                if (((varData *)res->data)->constant == true)
                {

                    exit(REDEFINITION_ERROR);
                }
            }
            // printf("counter je : %d\n", paramTypeCounter);
            if ((llTable[top % 100][literal]) == 9 || (llTable[top % 100][literal]) == 8 || (llTable[top % 100][literal]) == 10 || (llTable[top % 100][literal]) == 45)
            {
                bstSymtable *res = symtableSearch(&symLocal, *ID);
                bstSymtable *resFce = symtableSearch(&symTree, *functionID);
                // printf("ende%d\n", token->type);
                //  printf("jhash %s", ID->str);
                if (res == NULL && resFce == NULL)
                {
                    exit(UNDEFINED_VARIABLE_ERROR);
                    printf("saasas");
                }
                else
                {
                    if (token->type != T_INT && token->type != T_FLOAT && token->type != T_STR)
                    {
                        handleTokenType(res, resFce, token, functionID, &paramCount, &paramTypeCounter, state);
                        // printf("khjgf %d %d", paramCount, token->type);
                    }
                }
            }

            if ((llTable[top % 100][literal]) == 15 || (llTable[top % 100][literal]) == 16)
            {
                // printf("\ncall\n");
                bstSymtable *resFce = symtableSearch(&symTree, *functionID);
                bstSymtable *res = symtableSearch(&symLocal, *ID);
                // printf("ID%s", ID->str);
                if (resFce == NULL)
                {
                    // printf("\nunusedFce\n");
                    unusedFce = true;
                }

                if (res == NULL && assign)
                {

                    exit(UNDEFINED_VARIABLE_ERROR);
                    printf("asasas");
                }
                else if (token->type == T_INT || token->type == T_FLOAT || token->type == T_STR)
                {

                    if (!unusedFce)
                    {
                        if (((fceData *)resFce->data)->paramCount > 0 && (token->type == T_INT || token->type == T_FLOAT || token->type == T_STR))
                        {
                            // printf("\n%d\n", paramCountList);
                            if (((fceData *)resFce->data)->params[paramCountList].dataType.type != token->type)
                            {
                                if (((fceData *)resFce->data)->params[paramCountList].dataType.type != T_KEYWORD)
                                {
                                    exit(WRONG_ARGUMENTS_ERROR);
                                    /* code */
                                }
                            }
                            if (((fceData *)resFce->data)->paramCount <= paramCountList)
                            {
                                exit(WRONG_ARGUMENTS_ERROR);
                            }
                            paramTypeCounter = 0;
                            paramCountList++;
                        }
                        else
                        {

                            if (token->type != T_R_BRACKET || token->type != T_COMMA)
                            {
                                exit(WRONG_ARGUMENTS_ERROR);
                            }
                            paramTypeCounter = 0;
                        }
                    }
                    else
                    {
                        // printf("%d unused", unusedFce);
                        varData *varDatas;
                        varDatas = malloc(sizeof(varData) * 1); // Replace ARRAY_SIZE with the required size
                        if (varDatas == NULL)
                        {
                            // fprintf(stderr, "Memory allocation failed\n");
                            exit(EXIT_FAILURE);
                        }

                        varDatas[0].dataType = (DATATYPE){nullType, false, token->type};
                        insertFunction(&symTree, functionID->str, (DATATYPE){nullType, false, token->type}, paramCountList + 1, false, false, true, varDatas);
                        paramCountList++;
                    }
                }

                else
                {

                    if (!unusedFce)
                    {
                        if (((fceData *)resFce->data)->paramCount > 0 && token->type == T_ID)
                        {

                            /* code */
                            if (((fceData *)resFce->data)->params[paramCountList].dataType.type != ((varData *)res->data)->dataType.type || ((fceData *)resFce->data)->params[paramCountList].dataType.isNull != ((varData *)res->data)->dataType.isNull)
                            {
                                // printf("id:%s, fceID:%s count%d\n", ID->str, functionID->str, paramCountList);

                                // printf("params rozny typ param:%d, var:%d\n", ((fceData *)resFce->data)->params[paramCountList].dataType.type, ((varData *)res->data)->dataType.type);
                                if (((fceData *)resFce->data)->params[paramCountList].dataType.type != T_KEYWORD)
                                {
                                    exit(WRONG_ARGUMENTS_ERROR);
                                    /* code */
                                }
                            }
                            // printf("ADASDid:%d, count%d\n",((fceData *)resFce->data)->paramCount, paramCountList);
                            if (((fceData *)resFce->data)->paramCount <= paramCountList)
                            {
                                // printf("params pocet\n");
                                exit(WRONG_ARGUMENTS_ERROR);
                            }
                            printf("PUSHS LF@%s\n", res->key);
                            ((varData *)res->data)->use = true;
                            paramTypeCounter = 0;
                            paramCountList++;
                        }
                        else
                        {
                            // printf("\nTU%d\n", token->type);

                            if (token->type != T_R_BRACKET)
                            {
                                // printf("params 0\n");
                                exit(WRONG_ARGUMENTS_ERROR);
                            }
                            ((varData *)res->data)->use = true;
                            paramTypeCounter = 0;
                        }
                        // printf("\n%d\n", paramCountList);
                    }
                    else
                    {
                        // printf("%d unused", token->type);
                        if (token->type == T_R_BRACKET)
                        {
                            insertFunction(&symTree, functionID->str, (DATATYPE){nullType, false, token->type}, 0, false, false, true, NULL);
                        }
                        else
                        {

                            varData *varDatas;
                            varDatas = malloc(sizeof(varData) * 1); // Replace ARRAY_SIZE with the required size
                            if (varDatas == NULL)
                            {
                                // fprintf(stderr, "Memory allocation failed\n");
                                exit(EXIT_FAILURE);
                            }

                            varDatas[0].dataType = (DATATYPE){((varData *)res->data)->dataType.isNull, false, ((varData *)res->data)->dataType.type};
                            insertFunction(&symTree, functionID->str, (DATATYPE){nullType, false, token->type}, paramCountList + 1, false, false, true, varDatas);
                            ((varData *)res->data)->use = true;
                            printf("PUSHS LF@%s\n", res->key);
                            paramCountList++;
                        }
                    }
                }
            }
            if ((llTable[top % 100][literal]) == 34)
            {
                if (isReturn)
                {
                    bstSymtable *res = symtableSearch(&symTree, *functionIDCurrent);

                    if (res == NULL)
                    {
                        exit(80);
                    }
                    else
                    {
                        if (!((fceData *)res->data)->returnType.isNull)
                        {
                            exit(90);
                        }
                    }
                }
                else
                {
                    bstSymtable *res = symtableSearch(&symLocal, *ID);
                    if (res == NULL)
                    {
                        exit(80);
                    }
                    else
                    {
                        if (!((varData *)res->data)->dataType.isNull)
                        {
                            exit(90);
                        }
                    }
                }
            }

            if ((llTable[top % 100][literal]) == 5)
            {

                symtableDispose(&symLocal);
            }
            // printf("%d", state);
            if (state == nLlFunctCall)
            {

                fceCall = true;
            }
            // printf("state:%d literal:%d token:%d %d\n", fceCall, assign, token->type, ifj);
            if (fceCall && assign && (token->type == 3))
            {
                bstSymtable *resFce = symtableSearch(&symTree, *functionID);
                bstSymtable *res = symtableSearch(&symLocal, *ID);

                if (resFce == NULL || res == NULL)
                {
                    // printf("NULL fceID:%s id:%s\n", functionID->str, ID->str);
                    // exit(UNDEFINED_VARIABLE_ERROR);
                }
                else
                {

                    // if (((fceData *)resFce->data)->returnType.type != ((varData *)res->data)->dataType.type)
                    // {
                    //     exit(INCOMPATIBLE_TYPE_ERROR);
                    // }
                    // printf("fceID:%s id:%s\n", functionID->str, ID->str);
                    if (((fceData *)resFce->data)->isDefined == true)
                    {
                        if (((varData *)res->data)->dataType.type == T_ID)
                        {
                            ((varData *)res->data)->dataType.type = ((fceData *)resFce->data)->returnType.type;
                            ((varData *)res->data)->dataType.isNull = ((fceData *)resFce->data)->returnType.isNull;
                        }
                        else if ((((fceData *)resFce->data)->returnType.type != ((varData *)res->data)->dataType.type) ||
                                 (((fceData *)resFce->data)->returnType.isNull != ((varData *)res->data)->dataType.isNull))
                        {
                            // printf("%d %d", ((fceData *)resFce->data)->returnType.type, ((varData *)res->data)->dataType.type);
                            exit(INCOMPATIBLE_TYPE_ERROR);
                        }
                    }
                }
            }
            if ((llTable[top % 100][literal]) == 18)
            {
                bstSymtable *res = symtableSearch(&symTree, *functionIDCurrent);
                // printf("Function %s %d %d\n", functionIDCurrent->str, paramCountGlob, ((fceData *)(res->data))->paramCount);
                if (res != NULL)
                {
                    if (paramCountGlob != ((fceData *)(res->data))->paramCount)
                    {
                        // printf("dasda");
                        exit(WRONG_ARGUMENTS_ERROR);
                    }
                    /* code */
                }
            }

            if ((llTable[top % 100][literal]) == 18 || (llTable[top % 100][literal]) == 20)
            {

                // fceCall = false;
            }
            if ((llTable[top % 100][literal]) == 33)
            {
                paramTypeCounter = 0;
            }
            if ((llTable[top % 100][literal]) == 19 || (llTable[top % 100][literal]) == 20)
            {
                // printf("kajshdhsa%d\n", paramCount);
                paramCount = 0;
            }
            if ((llTable[top % 100][literal]) == 20)
            {
               //printf("%d %d %d\n", elseCount, elseBody, ifInsideCount);
                if (ifBody == false && ifInsideCount == 0)
                {
                    generateFunctionReturn(functionIDCurrent);
                    usedVar(&symLocal);
                }
                else
                {
                    if (ifBody == false && ifInsideCount > 0 && elseBody)
                    {
                        // printf("else: %d, if: %d\n", elseCounter, ifCounter);
                        printf("LABEL elseEnd%d%d\n", ifAloneCounter, ifInsideCount);
                        elseCount--;
                        ifInsideCount--;
                    }else if(elseCount == ifInsideCount && !elseBody){
                        printf("LABEL elseEnd%d%d\n", ifAloneCounter, ifInsideCount);
                        elseCount--;
                        ifInsideCount--;
                    }
                    elseBody = false;
                }
            }

            // if ((llTable[top % 100][literal]) == 31)
            // {
            //     exit(GENERIC_SEMANTIC_ERROR);
            // }

            if (top != nLlType && top != nLlItem)
            {
                state = top;
            }
            // printf("\nSTAV BUDE%d\n", state);

            stackPop(parserStack);
            // printf("po pope je stack\n");
            // stackPrint(parserStack);
            if ((llTable[top % 100][literal]) == -1)
            {
                // printf("syntax error");
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
            // printf("bude exprssion:   %d\n", inFce);
            paramTypeCounter = 0;
            // printf("counter je : %d\n", paramTypeCounter);
            TStack expStack;
            analyzeExp(&expStack, token);

            if (state == nLlReturnList)
            {
                bstSymtable *res = symtableSearch(&symTree, *functionIDCurrent);
                if (res == NULL)
                {
                    exit(INTERNAL_ERROR);
                }
                // printf("sgda%s\n", functionIDCurrent->str);
                if (returnExpValue != (int)(((fceData *)res->data)->returnType.type))
                {
                    exit(INCORRECT_RETURN_ERROR);
                }
            }

            if (inFce == false)
            {
                if (state != nLlReturnList)
                {
                    bstSymtable *result = symtableSearch(&symLocal, *ID);
                    if (result == NULL)
                    {
                        // printf("sadsadass");
                        exit(SYNTAX_ERROR);
                    }
                    else
                    {
                        // printf("alles gut\n%d, %d", ((varData *)result->data)->dataType.type, returnExpValue);
                        if (((varData *)result->data)->dataType.type == T_ID)
                        {
                            ((varData *)result->data)->dataType.type = returnExpValue;
                        }
                        else if ((int)(((varData *)result->data)->dataType.type) != returnExpValue)
                        {
                            exit(INCOMPATIBLE_TYPE_ERROR);
                        }
                        assigneVariables(ID);
                    }
                }
            }
            if (inFce == true)
            {
                generateIfBeginning(ifAloneCounter, ifInsideCount);
            }

            tokenBefore = token;
            token = getToken();
            literal = convertTokenToIndex(token);
            // stackPrint(parserStack);
            // printf("token po EXP: %d\n", literal);
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
    symtableInsertBuildInFce(&symTree);
    parsIt(&parserStack);
    DSTRING *str = dStringCreate();
    dStringAddString(str, "main");
    bstSymtable *resLocal = symtableSearch(&symTree, *str);
    if (resLocal != NULL)
    {
        // printf("\nFound in locals %d \n", ((fceData *)resLocal->data)->returnType.type);
    }
    else
    {
        // printf("Not found in locals\n");
    }

    return 0;
}