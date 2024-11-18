// heafer file for parser for ifj24
// authors: Samuel Kundrat
// created: 11.11.2024
// edited: 17.11.2024

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

#define TERMINAL_COUNT 29
#define NON_TERMINAL_COUNT 26
#define MAX_RULE_ITEMS 12

// #define tLlZigImport -5
#define tLlImport -6
// #define EPS -7
#define tLlDot -8
// #define tLlComm -9
#define EXP -10

// global variable to let know sematic alysis that variable can be also null
extern bool nullType;

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
    tLlNull,
    tLlQuestionMark,

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
    nLlNull,

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
    @return void This function does not return a value.
*/
void pushRule(TStack *parserStack, int rule);
/*
    @brief  function that converts token from scanner to index in LL table and adds specific tokens to symtebl for semantic analysis
    @param *token token to convert to index in
    @return index of token in LL table based on enumrator,
    @return -6 when TOKEN = T_KEYWOR.atribute = import
    @return -8 when TOKEN  = T_DO
 */
int convertTokenToIndex(TOKEN *token);
/**
 * @brief Parses tokens from the input and processes them based on LL(1) grammar rules.
 *
 * This function takes a stack as input, processes tokens from the input using
 * the LL(1) parsing method, and modifies the stack accordingly. It handles
 * terminals, non-terminals, special cases, and expressions. Errors in syntax
 * result in termination of the program with a syntax error code.
 *
 * @param parserStack Pointer to the stack used for parsing. The stack contains
 *        grammar rules and tokens for processing.
 *
 * @details
 * - Reads tokens using `getToken()`.
 * - Converts tokens to indexes using `convertTokenToIndex()`.
 * - Uses an LL table (`llTable`) for processing grammar rules.
 * - Handles expressions through the `analyzeExp()` function when encountering
 *   expression-related non-terminals.
 * - If a syntax error is encountered, the program exits with `SYNTAX_ERROR`.
 * - Special cases:
 *   - Handles `nullType` when encountering the `NULL` token.
 *   - Toggles `inFce` when processing an `if` statement token (`tLlIf`).
 *
 * @return void This function does not return a value.
 *
 */
void parserIn(TStack *parserStack);

#endif
