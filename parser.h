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

#define TERMINAL_COUNT 24
#define NON_TERMINAL_COUNT 22
#define MAX_RULE_ITEMS 12

#define tLlleftSquareBracket -1
#define tLlrightSquareBracket -2
#define tLlQuestionMark -3
#define tLlEndOfFile -4
#define EXP -5

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
    @brief  function that gets item from top of the stack and converts it to index
    *parserStack  stack to push
    @return index
 */
int getItemFromStackAndConvert(TStack *parserStack);
#endif
