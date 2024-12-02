// parser.c
// Parser module
// Author(s): Samuel Kundrat, Marián Šuľa
// Last Edit: 1.12.2024
#include "parser.h"

// global variables definitions
bool nullType = false;
bool isConst = false;
bool assign = false;
int bracketWhileCounter = 0;
int paramCountGlob = 0;
bstSymtable *symTree;
bstSymtable *symLocal;
int ifAloneCounter = 0;
int ifInsideCount = 0;
int whileCounter = 0;
int functionCounter = 0;
/**
 * @brief Parsing rules for the IFJ24 compiler.
 * @see documentation.pdf, section "LL Table"
 * @note each line represents single rule in our language
 */
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

/**
 * @brief 2D array representing the LL(1) table for the IFJ24 compiler.
 * @see documentation.pdf, section "LL Table"
 */
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
    /* <return_list>         */ {-1, -1, -1, 32, 32, 32, 32, 32, -1, -1, -1, -1, -1, 32, -1, -1, 31, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, 34},
    /* <item>                */ {-1, -1, -1, 33, 35, 35, 35, 35, -1, -1, -1, -1, -1, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, 34},
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
/**
 * @brief Checks if all variables in the symbol table are used.
 *
 * This function traverses the binary search tree of the symbol table and checks if each variable's `use` flag is `true`.
 * If any variable is unused (`use == false`), the function exits with an error.
 *
 * @param symTree A pointer to the symbol table (BST) root.
 */
void usedVar(bstSymtable **symTree)
{
    
    if (*symTree != NULL)
    {
        if (((varData *)(*symTree)->data)->use == false)
        {
            exit(UNUSED_VARIABLE_ERROR);
        }
        if (((varData *)(*symTree)->data)->isPar == false)
        {
            generateVariables(&(*symTree)->key);
            /* code */
        }
        
        usedVar(&(*symTree)->left);
        usedVar(&(*symTree)->right);
    }
}
/**
 * @brief Checks if all functions in the symbol table are defined.
 *
 * This function recursively traverses the binary search tree of the symbol table, verifying if each function's `isDefined` flag is `true`.
 * If any function is undefined (`isDefined == false`), the function exits with an error.
 *
 * @param symTree A pointer to the symbol table (BST) root.
 */
void defFunction(bstSymtable **symTree)
{
    if (*symTree != NULL)
    {
        if (((fceData *)(*symTree)->data)->isDefined == false)
        {
            exit(UNUSED_VARIABLE_ERROR);
        }

        defFunction(&(*symTree)->left);
        defFunction(&(*symTree)->right);
    }
}

/**
 * @brief Processes a variable in the symbol table by setting its data type.
 *
 * This function sets the type and nullability of the variable's data in the symbol table.
 * It also resets the `paramTypeCounter` to 0.
 *
 * @param res A pointer to the symbol table entry for the variable.
 * @param type The type to assign to the variable.
 * @param paramTypeCounter A pointer to a counter that is reset to 0.
 */
void processVariable(bstSymtable *res, int type, int *paramTypeCounter)
{
    if (res != NULL)
    {
        ((varData *)res->data)->dataType.type = type;
        ((varData *)res->data)->dataType.isNull = nullType;
        *paramTypeCounter = 0;
    }
}
/**
 * @brief Processes a function in the symbol table by handling its parameters.
 *
 * This function processes the parameters for a function during its definition or when called,
 * updating the symbol table for the function's parameters and handling argument validation.
 *
 * @param res A pointer to the symbol table entry for the parameter variable.
 * @param resFce A pointer to the symbol table entry for the function.
 * @param functionID The function's identifier.
 * @param type The type of the parameter.
 * @param paramCount A pointer to the number of parameters processed so far.
 * @param state The current state of the function processing.
 */
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
            ((varData *)(res->data))->isPar = true;
            insertFunction(&symTree, functionID->str, (DATATYPE){nullType, false, type}, *paramCount + 1, true, false, true, varDatas);
            (*paramCount)++;
            paramCountGlob = *paramCount;
        }
        else
        {
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
                ((varData *)(res->data))->isPar = true;
            }

            (*paramCount)++;
            paramCountGlob = *paramCount;
        }
    }
}
/**
 * @brief Sets the return type for a function in the symbol table.
 *
 * This function sets the return type for a given function and marks it as defined.
 * It also generates the function's header for further processing.
 *
 * @param resFce A pointer to the symbol table entry for the function.
 * @param type The return type of the function.
 */
void setFunctionReturnType(bstSymtable *resFce, int type)
{
    if (resFce != NULL)
    {
        ((fceData *)resFce->data)->returnType.type = type;
        ((fceData *)resFce->data)->returnType.isNull = nullType;
        ((fceData *)(resFce->data))->isDefined = true;
        functionCounter++;
        generateFunctionHead(resFce);
        printf("JUMP GENVARS%d\n", functionCounter);
        printf("LABEL FUNCTIONSTART%d\n", functionCounter);
    }
}
/**
 * @brief Handles token types and processes them based on context.
 *
 * This function processes tokens based on their type and the current state. It handles the processing
 * of function parameters or return types, depending on the type of the token. It differentiates
 * between various types such as strings, integers, floats, and keywords.
 *
 * @param res A pointer to the symbol table entry for the current variable.
 * @param resFce A pointer to the symbol table entry for the current function.
 * @param token The current token to process.
 * @param functionID The ID of the function being processed.
 * @param paramCount The number of parameters in the function.
 * @param paramTypeCounter A counter for the parameter type.
 * @param state The current state of the parser (e.g., parameter list or return type).
 */
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

            return tLlIf;
        }
        else if (strcmp(token->attribute.dStr->str, "while") == 0)
        {
            whileCounter++;
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
    // opytat sa ne meeting ************************************************
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
        return tLlEqual; // equel = "=" not "=="
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
    // declarations and definitions of necessary variables
    DSTRING *ID;
    ID = dStringCreate();
    TStack generatorStack;
    stackInit(&generatorStack);
    char *print;
    DSTRING *functionID;
    DSTRING *functionIDCurrent = NULL;
    functionID = dStringCreate();
    DSTRING *currentID;
    currentID = dStringCreate();
    TOKEN *token;
    int literal = 99;
    int top = -100;
    int paramTypeCounter = 0;
    int paramCount = 0;
    int paramCountList = 0;
    NonTerminals state;
    bool fceCall = false;
    token = getToken();
    literal = convertTokenToIndex(token);
    bool ifj = false;
    bool unusedFce = false;
    bool isReturn = false;
    bool whileBody = false;
    bool ifBody = false;
    bool elseBody = false;

    // generates 3-address code for progeamm start and all of the built in functions
    generateHeader();
    // loop for whole parsing phaze, loops till EOF or error
    while (literal != EOF)
    {

        // Save id token to ID variable
        if (literal == tLlId && (!assign || (assign && fceCall)))
        {
            if (strcmp(token->attribute.dStr->str, "ifj") != 0)
            {
                ID = token->attribute.dStr;
                paramTypeCounter = 1;
            }
        }
        // Sets ifj to true
        if (literal == tLlIfj)
        {
            ifj = true;
        }
        // Save functionid token to functionID variable
        if (literal == tLlFunctId)
        {
            functionID = token->attribute.dStr;
        }
        // Sets variable on else token
        if (literal == tLlElse)
        {

            ifBody = false;
            elseBody = true;
            whileBody = false;
        }
        // Sets variable on if token
        if (literal == tLlIf)
        {
            inFce = true;
            ifBody = true;
            elseBody = false;
            whileBody = false;
        }
        // Sets variable on while token
        if (literal == tLlWhile)
        {
            inFce = true;
            ifBody = false;
            whileBody = true;
        }

        // top is top value from the stack
        top = *(int *)(parserStack->stackTop);
        // 3-address code generation for if statement
        if (top == tLlIf)
        {

            ifAloneCounter++;
            // malloc for 3-address code generation
            char *print1 = (char *)malloc(100 * sizeof(char));
            if (print1 == NULL)
            {
                exit(INTERNAL_ERROR);
            }
            // print to print1
            sprintf(print1, "LABEL elseEnd%d\n", ifAloneCounter);
            // pushes print1 to stack
            stackPush(&generatorStack, (void *)print1);
            char *print = (char *)malloc(100 * sizeof(char));
            if (print == NULL)
            {
                exit(INTERNAL_ERROR);
            }
            ifInsideCount++;
            sprintf(print, "JUMP elseEnd%d\nLABEL else%d\n", ifAloneCounter, ifAloneCounter);
            stackPush(&generatorStack, (void *)print);
        }

        // 3-address code generation for while loop
        else if (top == tLlWhile)
        {
            printf("LABEL whileBegin%d\n", whileCounter);
            char *print = (char *)malloc(100 * sizeof(char));
            if (print == NULL)
            {
                exit(INTERNAL_ERROR);
            }
            sprintf(print, "JUMP whileBegin%d\nLABEL whileEnd%d\n", whileCounter, whileCounter);
            stackPush(&generatorStack, (void *)print);
        }

        // condition which sets global variable to tru to let exprssion parsing now that we are parsing if condition expression
        // Sets inFce to false
        if (literal == tLlLeftCurlyBracket || literal == tLlPipe)
        {
            inFce = false;
        }
        // Sets variable to initialized
        if (literal == tLlEqual)
        {
            assign = true;
            bstSymtable *res = symtableSearch(&symLocal, *ID);
            if (res != NULL)
            {
                ((varData *)res->data)->initialized = true;
            }
        }

        // terminal part
        if ((top >= 0 && top <= TERMINAL_COUNT - 1) || (top <= -1 && top >= -8))
        {
            if (literal == tLlReturn)
            {
                isReturn = true;
            }

            // END OF PARSING
            // Sets isReturn to true
            if (top == -1 && literal == 21)
            {
                stackPop(parserStack);
                // Generate
                generateFunctionReturn(functionIDCurrent);
                printf("LABEL GENVARS%d\n", functionCounter);
                // Checks if vars were used
                usedVar(&symLocal);
                // Checks if all used funtions were defined
                printf("JUMP FUNCTIONSTART%d\n", functionCounter);
                defFunction(&symTree);
                // Checks if main was in the file with right arguments and return value
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
            }
            else
            {
                exit(SYNTAX_ERROR);
            }

            // next token getter
            token = getToken();
            literal = convertTokenToIndex(token);

            // Adding variables to localTree
            if ((top == tLlVar || top == tLlConst || top == tLlLeftRoundBracket || top == tLlComma || top == tLlPipe) && token->type == T_ID)
            {
                if (inFce == false)
                {
                    if (symtableSearch(&symLocal, *token->attribute.dStr) == NULL)
                    {
                        if (top == tLlPipe)
                        {
                            bstSymtable *res = symtableSearch(&symLocal, *ID);
                            if (res == NULL)
                            {
                                exit(GENERIC_SEMANTIC_ERROR);
                            }
                            // Insert variable in pileLines |variable|
                            insertVariables(token->attribute.dStr->str, (DATATYPE){false, false, ((varData *)res->data)->dataType.type}, false, isConst, false, false, &symLocal);
                            // generate
                            printf("PUSHS LF@%s\n", ID->str);
                            //printf("DEFVAR LF@%s\n", token->attribute.dStr->str);
                            printf("POPS LF@%s\n", token->attribute.dStr->str);
                        }
                        else
                        {
                            // Insert variable to local symtable
                            insertVariables(token->attribute.dStr->str, (DATATYPE){nullType, false, token->type}, false, isConst, false, false, &symLocal);
                            if (strcmp(token->attribute.dStr->str, "ifj") != 0 && (state != nLlParams && state != nLlFunctDef))
                            {
                                // generate
                                //generateVariables(token->attribute.dStr);
                            }
                        }
                    }
                    else
                    {
                        if (!fceCall)
                        {

                            exit(REDEFINITION_ERROR);
                        }
                    }
                    isConst = false;
                }
            }
            // Sets currentId to token Id
            if (top == tLlVar || top == tLlConst)
            {
                currentID = token->attribute.dStr;
            }

            // Adds functions to globalTree
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
                    }
                    functionIDCurrent = token->attribute.dStr;
                }
                paramCountGlob = 0;
            }
            // Sets variables, checks params of called function
            if (literal == tLlSemicolon)
            {
                if (fceCall)
                {
                    bstSymtable *res = symtableSearch(&symTree, *functionID);

                    if (res != NULL)
                    {
                        if (paramCountList != ((fceData *)(res->data))->paramCount)
                        {
                            exit(WRONG_ARGUMENTS_ERROR);
                        }
                    }
                    // Generate
                    printf("CALL %s\n", res->key);

                    if (assign == true)
                    {
                        // Generate
                        printf("POPS LF@%s\n", currentID->str);
                    }
                }
                // reset of all variables
                paramCount = 0;
                assign = false;
                ifj = false;
                paramCountList = 0;
                unusedFce = false;
                isReturn = false;
                fceCall = false;
                isConst = false;
            }
            // Calling buildin function
            if (fceCall && assign && literal == tLlLeftRoundBracket && ifj)
            {
                bstSymtable *resFce = symtableSearch(&symTree, *functionID);
                bstSymtable *res = symtableSearch(&symLocal, *ID);

                if (resFce == NULL || res == NULL)
                {
                }
                else
                {
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
            // null type part
            // printf("pravidlo bude: %d\n", (llTable[top % 100][literal]));
            // this condition checks if there is ? before data type and sets global variable nullType to true to let semantic analizator let know
            if (top == 105 && literal == 28)
            {
                nullType = true;
                token = getToken();
                literal = convertTokenToIndex(token);
                // checker if user isnt using null or void with null typr indicator
                if (literal == 26 || literal == 27)
                {
                    exit(SYNTAX_ERROR);
                }
            }
            else
            {
                nullType = false;
            }
            // Checks if varaible is in local symtable in initialization
            if ((llTable[top % 100][literal]) == 38)
            {
                currentID = ID;
                bstSymtable *res = symtableSearch(&symLocal, *ID);

                if (res == NULL)
                {
                    exit(UNDEFINED_VARIABLE_ERROR);
                }
                else
                {
                    ((varData *)res->data)->use = true;
                }
                // If variable is const, then error
                if (((varData *)res->data)->constant == true)
                {

                    exit(REDEFINITION_ERROR);
                }
            }
            // Handling type of variable in declaration and in function definition
            if ((llTable[top % 100][literal]) == 9 || (llTable[top % 100][literal]) == 8 || (llTable[top % 100][literal]) == 10 || (llTable[top % 100][literal]) == 45)
            {
                bstSymtable *res = symtableSearch(&symLocal, *ID);
                bstSymtable *resFce = symtableSearch(&symTree, *functionID);

                if (res == NULL && resFce == NULL)
                {
                    exit(UNDEFINED_VARIABLE_ERROR);
                }
                else
                {
                    if (token->type != T_INT && token->type != T_FLOAT && token->type != T_STR)
                    {
                        handleTokenType(res, resFce, token, functionID, &paramCount, &paramTypeCounter, state);
                    }
                }
            }
            // Calling function
            if ((llTable[top % 100][literal]) == 15 || (llTable[top % 100][literal]) == 16)
            {

                bstSymtable *resFce = symtableSearch(&symTree, *functionID);
                bstSymtable *res = symtableSearch(&symLocal, *ID);

                if (resFce == NULL)
                {
                    unusedFce = true;
                }
                if (!assign && ((fceData *)resFce->data)->returnType.type != T_KEYWORD)
                {
                    exit(INCORRECT_RETURN_ERROR);
                }
                if (res == NULL && assign)
                {

                    exit(UNDEFINED_VARIABLE_ERROR);
                }
                else if (token->type == T_INT || token->type == T_FLOAT || token->type == T_STR)
                {
                    // calling defined function, checking params types, not IDs
                    if (!unusedFce)
                    {
                        if (((fceData *)resFce->data)->paramCount > 0 && (token->type == T_INT || token->type == T_FLOAT || token->type == T_STR))
                        {
                            if (((fceData *)resFce->data)->params[paramCountList].dataType.type != token->type)
                            {
                                if (((fceData *)resFce->data)->params[paramCountList].dataType.type != T_KEYWORD)
                                {
                                    exit(WRONG_ARGUMENTS_ERROR);
                                }
                            }
                            if (((fceData *)resFce->data)->paramCount <= paramCountList)
                            {
                                exit(WRONG_ARGUMENTS_ERROR);
                            }
                            // 3-address code genartion for variables
                            if (token->type == T_INT)
                            {
                                printf("PUSHS int@%d\n", token->attribute.i);
                            }
                            else if (token->type == T_FLOAT)
                            {
                                printf("PUSHS float@%a\n", token->attribute.f);
                            }
                            else
                            {
                                printf("PUSHS string@%s\n", token->attribute.dStr->str);
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
                    // calling undefined function, adding function to global symtable for future checking, not IDs
                    else
                    {
                        varData *varDatas;
                        varDatas = malloc(sizeof(varData) * 1);
                        if (varDatas == NULL)
                        {
                            exit(EXIT_FAILURE);
                        }

                        varDatas[0].dataType = (DATATYPE){nullType, false, token->type};
                        insertFunction(&symTree, functionID->str, (DATATYPE){nullType, false, token->type}, paramCountList + 1, false, false, true, varDatas);
                        // 3-address code generation for variables
                        if (token->type == T_INT)
                        {
                            printf("PUSHS int@%d\n", token->attribute.i);
                        }
                        else if (token->type == T_FLOAT)
                        {
                            printf("PUSHS float@%a\n", token->attribute.f);
                        }
                        else
                        {
                            printf("PUSHS string@%s\n", token->attribute.dStr->str);
                        }
                        paramCountList++;
                    }
                }

                else
                {
                    // calling defined function, checking params types, IDs
                    if (!unusedFce)
                    {
                        if (((fceData *)resFce->data)->paramCount > 0 && token->type == T_ID)
                        {

                            if (((fceData *)resFce->data)->params[paramCountList].dataType.type != ((varData *)res->data)->dataType.type || ((fceData *)resFce->data)->params[paramCountList].dataType.isNull != ((varData *)res->data)->dataType.isNull)
                            {

                                if (((fceData *)resFce->data)->params[paramCountList].dataType.type != T_KEYWORD)
                                {
                                    exit(WRONG_ARGUMENTS_ERROR);
                                }
                            }

                            if (((fceData *)resFce->data)->paramCount <= paramCountList)
                            {

                                exit(WRONG_ARGUMENTS_ERROR);
                            }
                            printf("PUSHS LF@%s\n", res->key);
                            ((varData *)res->data)->use = true;
                            paramTypeCounter = 0;
                            paramCountList++;
                        }
                        else
                        {

                            if (token->type != T_R_BRACKET)
                            {

                                exit(WRONG_ARGUMENTS_ERROR);
                            }
                            ((varData *)res->data)->use = true;
                            paramTypeCounter = 0;
                        }
                    }
                    // calling undefined function, adding function to global symtable for future checking, IDs
                    else
                    {

                        if (token->type == T_R_BRACKET)
                        {
                            insertFunction(&symTree, functionID->str, (DATATYPE){nullType, false, token->type}, 0, false, false, true, NULL);
                        }
                        else
                        {

                            varData *varDatas;
                            varDatas = malloc(sizeof(varData) * 1);
                            if (varDatas == NULL)
                            {
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
            // netusim ?????
            if ((llTable[top % 100][literal]) == 34)
            {
                if (isReturn)
                {
                    bstSymtable *res = symtableSearch(&symTree, *functionIDCurrent);

                    if (res == NULL)
                    {
                        exit(INTERNAL_ERROR);
                    }
                    else
                    {
                        if (!((fceData *)res->data)->returnType.isNull)
                        {
                            exit(INCORRECT_RETURN_ERROR);
                        }
                        printf("PUSHS nil@nil\n");
                    }
                }
                else
                {
                    bstSymtable *res = symtableSearch(&symLocal, *ID);
                    if (res == NULL)
                    {
                        exit(INTERNAL_ERROR);
                    }
                    else
                    {
                        if (!((varData *)res->data)->dataType.isNull)
                        {
                            exit(INCOMPATIBLE_TYPE_ERROR);
                        }
                        printf("PUSHS nil@nil\n");
                        printf("POPS LF@%s\n", ID->str);
                    }
                }
            }
            // Return of function
            if ((llTable[top % 100][literal]) == 5)
            {
                if (functionIDCurrent != NULL)
                {
                    // Generate
                    generateFunctionReturn(functionIDCurrent);
                    printf("LABEL GENVARS%d\n", functionCounter);
                    // Checks variable usage
                    usedVar(&symLocal);
                    printf("JUMP FUNCTIONSTART%d\n", functionCounter);
                }
                // dispose local symtable
                symtableDispose(&symLocal);
            }
            // Sets fceCall to true
            if (state == nLlFunctCall)
            {

                fceCall = true;
            }
            // calling function, checking return type with variable type, else seting var type
            if (fceCall && assign && (token->type == 3))
            {
                bstSymtable *resFce = symtableSearch(&symTree, *functionID);
                bstSymtable *res = symtableSearch(&symLocal, *ID);

                if (resFce == NULL || res == NULL)
                {
                }
                else
                {

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

                            exit(INCOMPATIBLE_TYPE_ERROR);
                        }
                    }
                }
            }
            // Checks if params of function
            if ((llTable[top % 100][literal]) == 18)
            {
                bstSymtable *res = symtableSearch(&symTree, *functionIDCurrent);

                if (res != NULL)
                {
                    if (paramCountGlob != ((fceData *)(res->data))->paramCount)
                    {
                        exit(WRONG_ARGUMENTS_ERROR);
                    }
                }
            }
            // Sets paramTypeCounter to 0
            if ((llTable[top % 100][literal]) == 33)
            {
                paramTypeCounter = 0;
            }
            // Sets paramCount to 0
            if ((llTable[top % 100][literal]) == 19 || (llTable[top % 100][literal]) == 20)
            {
                paramCount = 0;
            }
            // Generate
            if ((llTable[top % 100][literal]) == 20)
            {
                if (generatorStack.stackTop != NULL)
                {

                    printf("%s", ((char *)generatorStack.stackTop->value));
                    stackPop(&generatorStack);
                }
            }
            // Sets state
            if (top != nLlType && top != nLlItem)
            {
                state = top;
            }

            stackPop(parserStack);
            if ((llTable[top % 100][literal]) == -1)
            {
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

            paramTypeCounter = 0;

            TStack expStack;
            // Calling function for analyzing expression
            analyzeExp(&expStack, token);
            // Checks return type of function with return
            if (state == nLlReturnList)
            {
                bstSymtable *res = symtableSearch(&symTree, *functionIDCurrent);
                if (res == NULL)
                {
                    exit(INTERNAL_ERROR);
                }
                if (returnExpValue != (int)(((fceData *)res->data)->returnType.type))
                {
                    exit(INCORRECT_RETURN_ERROR);
                }
            }
            // Generate
            if (inFce == false)
            {
                if (state != nLlReturnList)
                {
                    bstSymtable *result = symtableSearch(&symLocal, *ID);
                    if (result == NULL)
                    {
                        exit(SYNTAX_ERROR);
                    }
                    else
                    {
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
            // Generate
            if (inFce == true)
            {
                if (whileBody)
                {
                    generateWhileBeginning(whileCounter);
                }
                if (ifBody)
                {

                    generateIfBeginning(ifAloneCounter);
                }
            }

            token = getToken();
            literal = convertTokenToIndex(token);
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
    return 0;
}