// scanner.h
// Interface for the scanner
// Author(s): Václav Bergman, Tomáš Hrbáč
// Last Edited: 1.11.2024

#ifndef SCANNER_H
#define SCANNER_H

#include "dstring.h"

// Enum of token types
typedef enum tokenType
{
    T_KEYWORD, T_ERROR, T_ID, T_FX_ID, T_BUILT_IN_FX_ID,
    T_INT, T_STR, T_FLOAT, 
    T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_EQ, T_NE, T_LT, T_LE, T_GT, T_GE, T_ASSIGN,
    T_SEMICOL, T_COLON, T_L_BRACE, T_R_BRACE, T_L_SQ_BRACKET, T_R_SQ_BRACKET,
    T_L_BRACKET, T_R_BRACKET, T_QUESTION_MK, T_DOT, T_COM, T_IMPORT,
    T_EOF, T_UNDEFINED
} TOKEN_TYPE;

// String equivalents of token types
extern const char *TOKEN_TYPE_STRING[];

// Array of keywords
extern const char* keywords[];

// Enum of FSM states
typedef enum states
{
    INITIAL, IDENTIFIER, FX_IDENTIFIER, IMPORT, STRING_START, STRING_END, ESCAPE_SEQ,
    INTEGER, INT_ZERO, FLOAT_DP_START, FLOAT_DP_END, FLOAT_EX_START, FLOAT_EX_MID, FLOAT_EX_END, 
    NOT_EQUAL, LESS_THAN, GREATER_THAN,
    ASSIGN, PLUS, MINUS, DIVIDE, MULTIPLY, SEMICOLON, COLON, LEFT_SQ_BRACKET, RIGHT_SQ_BRACKET,
    LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE, DOT, QUESTION_MARK, COMMENT, NO_LINEFEED,
    END_OF_FILE
} STATES;

typedef enum attributeType
{
    NONE, I, F, DSTR
} ATTRIBUTE_TYPE;

// Union for storing token value
typedef union tokenAtribute
{
    int i;
    double f;
    DSTRING *dStr;
} TOKEN_ATTRIBUTE;

// The token
typedef struct tokenStruct
{
    TOKEN_TYPE type;
    TOKEN_ATTRIBUTE attribute;
    ATTRIBUTE_TYPE current_attribute;
} TOKEN;


void setSourceFile(FILE *file);

TOKEN *getToken();

void destroyToken(TOKEN *token);

#endif
