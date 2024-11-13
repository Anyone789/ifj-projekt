// header file for parser for ifj24
// authors: Samuel Kundrat
// created: 11.11.2024
// edited: 13.11.2024
#include "parser.h"

// array of rules needed for parsing may be not completed, but it is what we need

// ProductionRule llRules[] =
//     {
//         // eof and epsilon rules are temporary
//         {nLlProgram, {"nLlPrologue", "nLlProgramBody", "EOF"}},
//         {nLlPrologue, {"tLlConst", "tLlIfj", "tLlEqual", "@", "import", "tLlLeftRoundBracket", "\"ifj24.zig\"", "tLlRightRoundBracket", "tLlSemicolon"}},
//         {nLlProgramBody, {"nLlFunctDef", "nLlProgramBody"}},
//         {nLlProgramBody, {"nLlFunctCall", "nLlProgramBody"}},
//         {nLlProgramBody, {""}},
//         {nLlFunctDef, {"tLlPub", "tLlFn", "tLlFunctId", "tLlLeftRoundBracket", "nLlParamList", "tLlRightRoundBracket", "nLlType", "tLlLeftCurlyBracket", "nLlFunctBody", "tLlRightCurlyBracket"}},
//         {nLlParamList, {"tLlId", "tLlColon", "nLlType", "nLlParams"}},
//         {nLlParamList, {""}},
//         {nLlType, {"tLlInt"}},
//         {nLlType, {"tLlFloat"}},
//         {nLlType, {"tLlString"}},
//         {nLlType, {""}},
//         {nLlParams, {"tLlComma"}, {"nLlParamList"}},
//         {nLlParams, {""}},
//         {nLlFunctCall, {"nLlFunctId", "tLlLeftRoundBracket", "nLlParamsCallList", "tLlRightRoundBracket"}},
//         {nLlParamCallList, {"tLlId", "nLlParamCall"}},
//         {nLlParamCallList, {""}},
//         {nLlParamsCall, {"tLlComma", "nLlParamCallList"}},
//         {nLlParamsCall, {""}},
//         {nLlFunctBody, {"nLlStatement", "nLlFunctBody"}},
//         {nLlFunctBody, {""}},
//         {nLlStatement, {"nLlConditionStatement"}},
//         {nLlStatement, {"nLlLoopStatement"}},
//         {nLlStatement, {"nLlReturnStatement"}},
//         {nLlStatement, {"nLlFunctCall"}},
//         {nLlStatement, {"nLlVarDefStatement"}},
//         {nLlStatement, {"nLlConstDefStatement"}},
//         {nLlStatement, {"nLlVarInitStatement"}},
//         {nLlConditionStatement, {"tLlIf", "tLlLeftRoundBracket", "EXP", "tLlRightRoundBracket", "nLlNoNullBody", "tLlLeftCurlyBracket", "nLlFunctBod", "tLlRightCurlyBracket", "else", " LlLeftCurlyBracket", "nLlFunctBod", "tLlRightCurlyBracket"}},
//         {nLlLoopStatement, {"tLlWhile", "tLlLeftRoundBracket", "EXP", "tLlRightRoundBracket", "nLlNoNullBody", "tLlLeftCurlyBracket", "nLlFunctBod", "tLlRightCurlyBracket"}},
//         {nLlReturnStatement, {"tLlReturn", "nLlReturnList"}},
//         {nLlReturnList, {""}},
//         {nLlReturnList, {"nLlItem"}},
//         {nLlItem, {"nLlFunctCall"}},
//         {nLlItem, {"tLlId"}},
//         {nLlItem, {"EXP"}},
//         {nLlVarDefStatement, {"tLlId", "tLlColon", "nLlType", "tLlEqual", "nLlItem", "tLlSemicolon"}},
//         {nLlConstDefStatement, {"tLlId", "tLlColon", "nLlType", "tLlEqual", "nLlItem", "tLlSemicolon"}},
//         {nLlVarInitStatement, {"tLlId", "tLlEqual", "nLlItem", "tLlSemicolon"}}

// };

ProductionRule llRules[] =
    {
        // eof and epsilon rules are temporary
        {nLlProgram, {nLlPrologue, nLlProgramBody, EOF}},
        //{nLlPrologue, {tLlConst, tLlIfj, tLlEqual, @, import, tLlLeftRoundBracket, ifj24.zig, tLlRightRoundBracket, tLlSemicolon}},
        // neviem toto pravidlo je zatial take
        {nLlProgramBody, {nLlFunctDef, nLlProgramBody}},
        {nLlProgramBody, {nLlFunctCall, nLlProgramBody}},
        {nLlProgramBody, {}},
        {nLlFunctDef, {tLlPub, tLlFn, tLlFunctId, tLlLeftRoundBracket, nLlParamList, tLlRightRoundBracket, nLlType, tLlLeftCurlyBracket, nLlFunctBody, tLlRightCurlyBracket}},
        {nLlParamList, {tLlId, tLlColon, nLlType, nLlParams}},
        {nLlParamList, {}},
        {nLlType, {tLlInt}},
        {nLlType, {tLlFloat}},
        {nLlType, {tLlString}},
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
        {nLlReturnStatement, {tLlReturn, nLlReturnList}},
        {nLlReturnList, {}},
        {nLlReturnList, {nLlItem}},
        {nLlItem, {nLlFunctCall}},
        {nLlItem, {tLlId}},
        {nLlItem, {EXP}},
        {nLlVarDefStatement, {tLlId, tLlColon, nLlType, tLlEqual, nLlItem, tLlSemicolon}},
        {nLlConstDefStatement, {tLlId, tLlColon, nLlType, tLlEqual, nLlItem, tLlSemicolon}},
        {nLlVarInitStatement, {tLlId, tLlEqual, nLlItem, tLlSemicolon}}};

// 2D array of LL1 table for ifj24 compile
int llTable[TERMINAL_COUNT][NON_TERMINAL_COUNT] = {
    //                           c   p   f   F   I   i   f   s  if   w   r   v   ,   (   )   =   ;   {   }   e   :   $   |  ifj
    /* <program>             */ {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <prologue>            */ {1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <program_body>        */ {-1, 2, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1},
    /* <funct_def>           */ {-1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <param_list>          */ {-1, -1, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <type>                */ {-1, -1, -1, -1, -1, 8, 9, 10, -1, -1, -1, -1, 11, -1, 11, 11, -1, 11, -1, -1, -1, -1, -1, -1},
    /* <params>              */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, -1, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <funct_call>          */ {-1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14},
    /* <param_call_list>     */ {-1, -1, -1, -1, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <params_call>         */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 17, -1, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <funct_body>          */ {19, -1, -1, 19, 19, -1, -1, -1, 19, 19, 19, 19, -1, -1, -1, -1, -1, -1, 20, -1, -1, -1, -1, 19},
    /* <statement>           */ {26, -1, -1, 24, 27, -1, -1, -1, 21, 22, 23, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 24},
    /* <condition_statement> */ {-1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <loop_statement>      */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, 29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <return_statement>    */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <return_list>         */ {-1, -1, -1, 32, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, -1, -1, -1, -1, -1, -1, 32},
    /* <item>                */ {-1, -1, -1, 33, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33},
    /* <var_def_statement>   */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 36, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <const_def_statement> */ {37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <var_init_statement>  */ {-1, -1, -1, -1, 38, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    /* <noNull_body>         */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, -1, -1, -1, -1, 40, -1},
    /* <param_list>          */ {-1, -1, -1, 41, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 42},
};

int parsIt()
{
    TStack *parserStack;
    TOKEN *token;
    stackInit(parserStack);
    // push for the first NONTERMINAL <program> => beginning of the parsing PHAZE
    stackPush(parserStack, nLlProgram);
    if ()
    {
        /* code */
    }

    token = getToken();
}

void pushRule(TStack *parserStack, int rule)
{
    int i = 0;
    while (llRules[rule].production[i] != 0)
    {
        stackPush(parserStack, llRules[rule].production[i]);
        i++;
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

        break;
        // opytaj sa ne error token type ci tu mozes dat ze to exitne na scanner erroro nejaky alebo ako sa to vola
    case T_ERROR:
        exit(LEXICAL_ERROR);
    case T_ID:
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
    case T_L_BRACKET:
        return tLlLeftRoundBracket;
        break;
    case T_R_BRACKET:
        return tLlRightRoundBracket;
        break;
    case T_QUESTION_MK:
        return tLlQuestionMark;
    // neviem co s botkou prekonzultovat na meetingu
    case T_DOT:
        break;
    case T_COM:
        return tLlComma;
        break;
    // opytat sa na import
    case T_IMPORT:
        break;
    case T_EOF:
        return tLlEndOfFile;
    case T_UNDEFINED:
        break;
    default:
        // if token will be anything else than ll table symbol or something unexpected it ll be lexical error just for now
        exit(1);
        break;
    }
}

int getItemFromStackAndConvert(TStack *parserStack)
{
}