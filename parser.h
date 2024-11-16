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
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TERMINAL_COUNT 27
#define NON_TERMINAL_COUNT 25
#define MAX_RULE_ITEMS 12

#define tLlQuestionMark -4
#define tLlZigImport -5
#define tLlImport -6
#define EPS -7
#define tLlDot -8
#define tLlComm -9
#define EXP -10

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
    tLlleftSquareBracket,
    tLlrightSquareBracket,
    tLlVoid,

} Terminal;

// enumerator for all non-terminals in LL1 table
typedef enum
{
    nLlProgram = 100,
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
    nLlDefType,

} NonTerminals;

// structure for single rule for LL1 table
//  nonTerminal -> rightSide of rule
// example nLlProgram ->  <nLlPrologue> <nLlProgramBody>
typedef struct
{
    NonTerminals nonTerminal;
    char production[MAX_RULE_ITEMS];
} ProductionRule;

/*
    @brief that pushes right side of the rule to the stack
    @param *parserStack  stack to push
    @param rule  index of rule in LL1 table
    @return NONE
*/
void pushRule(TStack *parserStack, int rule);
/*
    @brief  function that converts token from scanner to index in LL table
    @param *token token to convert to index in
    @return index of token in LL table,
    @return -1 when [
    @return -2 when ]
    @return -3 when ?
    @return -4 when EOF = \0
 */
int convertTokenToIndex(TOKEN *token);
/*
    @brief  function that parses whole file
    @param  *parserStack  stack to push to
    @return none
 */
void parserIn(TStack *parserStack);
#endif
