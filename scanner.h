// scanner.h
// Interface for the scanner
// Author(s): Václav Bergman, Tomáš Hrbáč
// Last Edited: 30.11.2024

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "dstring.h"

#define HORIZONTAL_TAB 9
#define LINE_FEED 10
#define CARRIAGE_RETURN 13

// Enum of token types
typedef enum tokenType
{
    T_KEYWORD, T_ERROR, T_ID, T_FX_ID, T_BUILT_IN_FX_ID,
    T_INT, T_STR, T_FLOAT, 
    T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_EQ, T_NE, T_LT, T_LE, T_GT, T_GE, T_ASSIGN,
    T_SEMICOL, T_COLON, T_L_BRACE, T_R_BRACE, T_L_SQ_BRACKET, T_R_SQ_BRACKET,
    T_L_BRACKET, T_R_BRACKET, T_QUESTION_MK, T_PIPE, T_DOT, T_COMMA, T_COM,
    T_EOF, T_UNDEFINED
} TOKEN_TYPE;

// Array of keywords
extern const char* keywords[];

// Enum of FSM states
typedef enum states
{
    INITIAL, IDENTIFIER, FX_IDENTIFIER, IMPORT, STRING_START, STRING_END, ESCAPE_SEQ,
    INTEGER, INT_ZERO, FLOAT_DP_START, FLOAT_DP_END, FLOAT_EX_START, FLOAT_EX_MID, FLOAT_EX_END, 
    NOT_EQUAL, LESS_THAN, GREATER_THAN, ESCAPE_SEQ_HEX,
    ASSIGN, PLUS, MINUS, DIVIDE, MULTIPLY, SEMICOLON, COLON, LEFT_SQ_BRACKET, RIGHT_SQ_BRACKET,
    LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE, PIPE, DOT, COMMA, QUESTION_MARK, COMMENT,
    NO_LINEFEED, END_OF_FILE, ERROR
} STATES;

// Enum for attribute type
// I, F, DSTR correspond to i, f, dStr of TOKEN_ATTRIBUTE struct
// NONE indicates TOKEN_ATTRIBUTE is not set
typedef enum attributeType
{
    NONE, I, F, DSTR
} ATTRIBUTE_TYPE;

// Union for storing token value
// i -> integer
// f -> float
// dStr -> dynamic string
typedef union tokenAtribute
{
    int i;
    double f;
    DSTRING *dStr;
} TOKEN_ATTRIBUTE;

// Structure representing the token
// type -> token type
// attribute -> value of the token, some tokens have NONE (eg. T_ASSIGN)
// current_attribute -> indicates which element of attribute struct is set
typedef struct tokenStruct
{
    TOKEN_TYPE type;
    TOKEN_ATTRIBUTE attribute;
    ATTRIBUTE_TYPE current_attribute;
} TOKEN;

/*
    @brief Sets a file from which the scanner will get tokens
    @param *file pointer to a file
    @return NONE
*/
void setSourceFile(FILE *file);

/*
    @brief Performs a scanning operation on the source file and returns one token
    @param NONE
    @return Pointer to a token
*/
TOKEN *getToken();

/*
    @brief Destroys a token
    @param *token pointer to a token
    @return NONE
*/
void destroyToken(TOKEN *token);

#endif
