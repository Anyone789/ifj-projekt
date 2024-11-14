// header file for parser for ifj24
// authors: Samuel Kundrat
// created: 11.11.2024
// edited: 13.11.2024
#include "parser.h"

ProductionRule llRules[] =
    {
        // eof and epsilon rules are temporary
        {nLlProgram, {nLlPrologue, nLlProgramBody, EOF}},
        {nLlPrologue, {tLlConst, tLlIfj, tLlEqual, tLlImport, tLlLeftRoundBracket, tLlId, tLlRightRoundBracket, tLlSemicolon}},
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
        {nLlFunctCall, {nLlFunctId, tLlLeftRoundBracket, nLlParamCallList, tLlRightRoundBracket}},
        {nLlParamCallList, {tLlId, nLlParamsCall}},
        {nLlParamCallList, {}},
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
        {nLlReturnStatement, {tLlReturn, nLlReturnList, tLlSemicolon}},
        {nLlReturnList, {}},
        {nLlReturnList, {nLlItem}},
        {nLlItem, {nLlFunctCall}},
        {nLlItem, {tLlId}},
        {nLlItem, {EXP}},
        {nLlVarDefStatement, {tLlVar, tLlId, nLlDefType, tLlEqual, nLlItem, tLlSemicolon}},
        {nLlConstDefStatement, {tLlConst, tLlId, nLlDefType, tLlEqual, nLlItem, tLlSemicolon}},
        {nLlVarInitStatement, {tLlId, tLlEqual, nLlItem, tLlSemicolon}},
        {nLlNoNullBody, {}},
        {nLlNoNullBody, {tLlPipe, tLlId, tLlPipe}},
        {nLlFunctId, {tLlFunctId}},
        {nLlFunctId, {tLlIfj, tLlDot, tLlFunctId}},
        {nLlDefType, {tLlColon, nLlType}},
        {nLlDefType, {}},
        {nLlType, {tLlVoid}},
};

// 2D array of LL1 table for ifj24 compile
int llTable[NON_TERMINAL_COUNT][TERMINAL_COUNT] = {
    //                           c   p   f   F   I   i   f   s  if   w   r   v   ,   (   )   =   ;   {   }   e   :   $   |  ifj  [   ]  v
    /* <program>             */ {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <prologue>            */ {1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <program_body>        */ {-1, 2, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1},
    /* <funct_def>           */ {-1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <param_list>          */ {-1, -1, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <type>                */ {-1, -1, -1, -1, -1, 8, 9, -1, -1, -1, -1, -1, 11, -1, 11, 11, -1, 11, -1, -1, -1, -1, -1, -1, 10, -1, 45},
    /* <params>              */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, -1, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <funct_call>          */ {-1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1},
    /* <param_call_list>     */ {-1, -1, -1, -1, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <params_call>         */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 17, -1, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <funct_body>          */ {19, -1, -1, 19, 19, -1, -1, -1, 19, 19, 19, 19, -1, -1, -1, -1, -1, -1, 20, -1, -1, -1, -1, 19, -1, -1, -1},
    /* <statement>           */ {26, -1, -1, 24, 27, -1, -1, -1, 21, 22, 23, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 24, -1, -1, -1},
    /* <condition_statement> */ {-1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <loop_statement>      */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, 29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <return_statement>    */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <return_list>         */ {-1, -1, -1, 32, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1},
    /* <item>                */ {-1, -1, -1, 33, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1},
    /* <var_def_statement>   */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 36, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <const_def_statement> */ {37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <var_init_statement>  */ {-1, -1, -1, -1, 38, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <noNull_body>         */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, -1, -1, -1, -1, 40, -1, -1, -1, -1},
    /* <param_list>          */ {-1, -1, -1, 41, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 42, -1, -1, -1},
    /* <def_type>            */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 44, -1, -1, -1, -1, 43, -1, -1, 42, -1, -1, -1},
};

int parsIt(TStack *parserStack)
{
    // TOKEN *token;
    // int top;
    // int index;

    stackInit(parserStack);
    // pushes EOF to the stack bacause we skipped first rule, because every programm in ifj24 has to start with prologue
    // stackPush(parserStack, (void *)(intptr_t)EOF);

    //  push for the first NONTERMINAL <program> => beginning of the parsing PHAZE
    // stackPush(parserStack, (void *)(intptr_t)nLlProgram); // Cast enum to integer (intptr_t) to store in stack
    // top = *(int *)(parserStack->stackTop);

    // stackPop(parserStack);
    //  printf("P%d.\n", stackIsEmpty(parserStack));
    //  token = getToken();
    //  index = convertTokenToIndex(token);
    pushRule(parserStack, llTable[0][0]);

    // index = convertTokenToIndex(token);
    // printf(" | %d je index pravidlo je: %d\n ", index, llTable[top % 100][index]);
    // top = *(int *)(parserStack->stackTop);
    // printf("Top je: %d\n", top);

    // stackPrint(parserStack);

    parserIn(parserStack);
    return 1;
}

void pushRule(TStack *parserStack, int rule)
{
    int i = MAX_RULE_ITEMS - 1;
    while (i >= 0)
    {

        if ((intptr_t)llRules[rule].production[i] == 0)
        {
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
    switch (token->type)
    {
    case T_KEYWORD:
        if (strcmp(token->attribute.dStr->str, "const") == 0)
        {
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
        // if (strcmp(token->attribute.dStr->str, "ifj24.zig") == 0)
        // {
        //     return tLlZigImport;
        // }
        return tLlId;
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
        // built in function
    case T_DOT:
        return tLlDot;
        break;
    case T_COM:
        return tLlComma;
        break;
    // netusim co je import
    case T_IMPORT:
        break;
    case T_EOF:
        return tLlDollar;
    case T_UNDEFINED:
        exit(LEXICAL_ERROR);
        break;
    default:
        // if token will be anything else than ll table symbol or something unexpected it ll be lexical error just for now
        exit(SYNTAX_ERROR);
        break;
    }
}

void parserIn(TStack *parserStack)
{
    TOKEN *token;
    int literal = 99;
    int top;

    token = getToken();
    literal = convertTokenToIndex(token);

    while (literal != EOF)
    {

        top = *(int *)(parserStack->stackTop);
        printf("som tu\n");
        printf("top: %d\n", top);
        printf("literal: %d\n", literal);
        stackPrint(parserStack);

        printf("==================76==========\n");
        if ((top >= 0 && top <= TERMINAL_COUNT - 1) || (top <= -1 && top >= -8))
        {

            printf("NA VSTUPE JE TERMINAL\n");
            printf("top: %d\n", top);
            printf("literal: %d\n", literal);
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
        }
        else if (top >= 100 && top <= 122)
        {

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
        else if (literal == -10)
        {
            // zavolat funkciu analyze expression
            // urobit vlastny stack
            // funkcia vracia 0 ak je dobre a != 0 ak je chyba
            // nastav globalnu premennu bool inFce = true; ak je to expression v if alebo while = true, inak false
            // zatvorka za exprssion mozno het mozno nie to iste ; ak je priradenie
            printf("bude exprssion\n");
        }
    }
}

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
    parsIt(&parserStack);

    return 0;
}