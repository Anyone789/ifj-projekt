// header file for parser for ifj24
// authors: Samuel Kundrat
// created: 11.11.2024
// edited: 12.11.2024
#include "parser.h"

// array of rules needed for parsing may be not completed, but it is what we need

ProductionRule llRules[] =
    {
        // eof and epsilon rules are temporary
        {nLlProgram, {"nLlPrologue", "nLlProgramBody", "EOF"}},
        {nLlPrologue, {"tLlConst", "tLlIfj", "tLlEqual", "@", "import", "tLlLeftRoundBracket", "\"ifj24.zig\"", "tLlRightRoundBracket", "tLlSemicolon"}},
        {nLlProgramBody, {"nLlFunctDef", "nLlProgramBody"}},
        {nLlProgramBody, {"nLlFunctCall", "nLlProgramBody"}},
        {nLlProgramBody, {""}},
        {nLlFunctDef, {"tLlPub", "tLlFn", "tLlFunctId", "tLlLeftRoundBracket", "nLlParamList", "tLlRightRoundBracket", "nLlType", "tLlLeftCurlyBracket", "nLlFunctBody", "tLlRightCurlyBracket"}},
        {nLlParamList, {"tLlId", "tLlColon", "nLlType", "nLlParams"}},
        {nLlParamList, {""}},
        {nLlType, {"tLlInt"}},
        {nLlType, {"tLlFloat"}},
        {nLlType, {"tLlString"}},
        {nLlType, {""}},
        {nLlParams, {"tLlComma"}, {"nLlParamList"}},
        {nLlParams, {""}},
        {nLlFunctCall, {"nLlFunctId", "tLlLeftRoundBracket", "nLlParamsCallList", "tLlRightRoundBracket"}},
        {nLlParamCallList, {"tLlId", "nLlParamCall"}},
        {nLlParamCallList, {""}},
        {nLlParamsCall, {"tLlComma", "nLlParamCallList"}},
        {nLlParamsCall, {""}},
        {nLlFunctBody, {"nLlStatement", "nLlFunctBody"}},
        {nLlFunctBody, {""}},
        {nLlStatement, {"nLlConditionStatement"}},
        {nLlStatement, {"nLlLoopStatement"}},
        {nLlStatement, {"nLlReturnStatement"}},
        {nLlStatement, {"nLlFunctCall"}},
        {nLlStatement, {"nLlVarDefStatement"}},
        {nLlStatement, {"nLlConstDefStatement"}},
        {nLlStatement, {"nLlVarInitStatement"}},
        {nLlConditionStatement, {"tLlIf", "tLlLeftRoundBracket", "EXP", "tLlRightRoundBracket", "nLlNoNullBody", "tLlLeftCurlyBracket", "nLlFunctBod", "tLlRightCurlyBracket", "else", " LlLeftCurlyBracket", "nLlFunctBod", "tLlRightCurlyBracket"}},
        {nLlLoopStatement, {"tLlWhile", "tLlLeftRoundBracket", "EXP", "tLlRightRoundBracket", "nLlNoNullBody", "tLlLeftCurlyBracket", "nLlFunctBod", "tLlRightCurlyBracket"}},
        {nLlReturnStatement, {"tLlReturn", "nLlReturnList"}},
        {nLlReturnList, {""}},
        {nLlReturnList, {"nLlItem"}},
        {nLlItem, {"nLlFunctCall"}},
        {nLlItem, {"tLlId"}},
        {nLlItem, {"EXP"}},
        {nLlVarDefStatement, {"tLlId", "tLlColon", "nLlType", "tLlEqual", "nLlItem", "tLlSemicolon"}},
        {nLlConstDefStatement, {"tLlId", "tLlColon", "nLlType", "tLlEqual", "nLlItem", "tLlSemicolon"}},
        {nLlVarInitStatement, {"tLlId", "tLlEqual", "nLlItem", "tLlSemicolon"}}

};

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