// scanner.h
// Interface for the scanner
// Author(s): Václav Bergman
// Last Edited: 27.10.2024

#ifndef SCANNER_H
#define SCANNER_H


// Enum of token types
typedef enum
{
    KEYWORD, ERROR, ID, USER_FX_ID, BUILT_IN_FX_ID,
    INT, STR, FLOAT, 
    PLUS, MINUS, MUL, DIV,
    EQ, NE, LT, LE, GT, GE, ASSIGN,
    SEMI_COL, L_BRACE, R_BRACE, L_SQ_BRACKET, R_SQ_BRACKET,
    L_BRACKET, R_BRACKET, QUESTION_MK, DOT
} TOKEN_TYPE;

// Union for storing token value
typedef union
{
    int i;
    float f;
    char *str;
    // ptr *kwrd - Pointer to a keyword in keyword table?
} TOKEN_ATTRIBUTE;

// The token
typedef struct tokenStruct
{
    TOKEN_TYPE type;
    TOKEN_ATTRIBUTE atribute;
} TOKEN;


// Temporary array of keywords. May be used in the future.
const char* keywords[]
    = { "const", "else", "fn", "if", "i32", "f64", 
        "null", "pub", "return","u8", "[]u8", "var",
        "void", "while" };

#endif
