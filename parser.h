// parser.h
// Interface for parser
// Author(s): Samuel Kundrat
// Last Edit: 17.11.2024
#ifndef PARSER_H
#define PARSER_H

#include "stack.h"
#include "errorCodes.h"
#include "scanner.h"
#include "expression.h"
#include "generator.h"
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
extern int returnExpValue;
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
    // nLlNull,

} NonTerminals;

/*
    structure for single rule for LL1 table
    nonTerminal -> rightSide of rule
    example nLlProgram ->  <nLlPrologue> <nLlProgramBody>
 */
typedef struct
{
    NonTerminals nonTerminal;
    char production[MAX_RULE_ITEMS];
} ProductionRule;

/**
 * @brief Pushes the right-hand side of a grammar rule onto the parser stack.
 *
 * @param *parserStack Pointer to the stack where the rule's right side will be pushed.
 * @param rule Index of the rule in the LL(1) table.
 * @return void This function does not return a value.
 */

void pushRule(TStack *parserStack, int rule);
/**
 * @brief Converts a token from the scanner into an index in the LL(1) table
 *        and handles specific tokens by adding them to the symbol table for semantic analysis.
 *
 * @param *token Pointer to the token to be converted to an index in the LL table.
 * @return Index of the token in the LL table based on its enumerator.
 * @return -6 If TOKEN is T_KEYWORD and its attribute is "import".
 * @return -8 If TOKEN is T_DOT.
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
