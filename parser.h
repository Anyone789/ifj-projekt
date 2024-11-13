// heafer file for parser for ifj24
// authors: Samuel Kundrat
// created: 11.11.2024
// edited: 12.11.2024

#ifndef PARSER_H
#define PARSER_H

#include "stack.h"
#include "errorCodes.h"
#include "scanner.h"
#include "expression.h"

#define TERMINAL_COUNT 24
#define NON_TERMINAL_COUNT 22
#define MAX_RULE_ITEMS 12

// enumerator for all terminals in LL1 table
typedef enum
{
    tLlConst,
    tLlPub,
    tLlFn,
    tLlFunctId,
    tLlId,
    tLlInt,
    tLlFloat,
    tLlString,
    tLlIf,
    tLlWhile,
    tLlReturn,
    tLlVar,
    tLlComma,
    tLlLeftRoundBracket,
    tLlRightRoundBracket,
    tLlEqual,
    tLlSemicolon,
    tLlLeftCurlyBracket,
    tLlRightCurlyBracket,
    tLlElse,
    tLlColon,
    tLlDollar,
    tLlPipe,
    tLlIfj,

} Terminal;

// enumerator for all non-terminals in LL1 table
typedef enum
{
    nLlProgram,
    nLlPrologue,
    nLlProgramBody,
    nLlFunctDef,
    nLlParamList,
    nLlType,
    nLlParams,
    nLlFunctCall,
    nLlParamCallList,
    nLlParamsCall,
    nLlFunctBody,
    nLlStatement,
    nLlConditionStatement,
    nLlLoopStatement,
    nLlReturnStatement,
    nLlReturnList,
    nLlItem,
    nLlVarDefStatement,
    nLlConstDefStatement,
    nLlVarInitStatement,
    nLlNoNullBody,
    nLlFunctId,

} NonTerminals;

// structure for single rule for LL1 table
//  nonTerminal -> rightSide of rule
// example nLlProgram ->  <nLlPrologue> <nLlProgramBody>
typedef struct
{
    NonTerminals nonTerminal;
    char *production[MAX_RULE_ITEMS];
} ProductionRule;
#endif
