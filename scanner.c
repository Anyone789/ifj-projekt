// Module for lexical analyser
// Author(s): Tomáš Hrbáč, Václav Bergman
// Last Edit: 30.11.2024

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "dstring.h"
#include "errorCodes.h"

FILE *src;  // Pointer to a file to be scanned

const char *importStr = "import";   // Auxiliary string for matching scanned token to import keyword

// Array of keywords
const char* keywords[] = {
    "const", "else", "fn", "if", "i32", "f64", 
    "null", "pub", "return","u8", "[]u8", "var",
    "void", "while" 
};

void setSourceFile(FILE *file)
{
  src = file;
}

TOKEN *createToken()
{
    TOKEN *newToken = malloc(sizeof(TOKEN));

    if (newToken == NULL)
    {
        fprintf(stderr, "Token allocation error");
        exit(INTERNAL_ERROR);
    }

    newToken->type = T_UNDEFINED;
    newToken->current_attribute = NONE;

    return newToken;
}

void destroyToken(TOKEN *token)
{
    if (token->current_attribute == DSTR)
    {
        dStringDestroy(token->attribute.dStr);
    }
    free(token);
}

bool isdStringKeyword(DSTRING *dStr)
{
    bool isKeyword = false;
    for (unsigned i = 0; i < (sizeof(keywords) / sizeof(keywords[0])); i++)
    {
        if (dStringCmpConst(dStr, keywords[i]) == 0)
        {
            isKeyword = true;
            break;
        }     
    }
    return isKeyword;
}

TOKEN *getToken() 
{
	int c;
    TOKEN *token = createToken();
    STATES state = INITIAL;
    bool tokenScanned = false;

	while (!tokenScanned)
	{
		c = getc(src);
 
        switch (state)
        {
            case INITIAL:
            {
                if (c == '_' || isalpha(c))
                {
                    state = IDENTIFIER;
                    token->type = T_ID;
                    token->current_attribute = DSTR;
                    token->attribute.dStr = dStringCreate();
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (c == '[')
                {
                    state = LEFT_SQ_BRACKET;
                    token->type = T_L_SQ_BRACKET;
                }
                else if (c == ']')
                {
                    state = RIGHT_SQ_BRACKET;
                    token->type = T_R_SQ_BRACKET;
                }
                else if (c == '(')
                {
                    state = LEFT_BRACKET;
                    token->type = T_L_BRACKET;
                }
                else if (c == ')')
                {
                    state = RIGHT_BRACKET;
                    token->type = T_R_BRACKET;
                }
                else if (c == '{')
                {
                    state = LEFT_BRACE;
                    token->type = T_L_BRACE;
                }
                else if (c == '}')
                {
                    state = RIGHT_BRACE;
                    token->type = T_R_BRACE;
                }
                else if (c == '?')
                {
                    state = QUESTION_MARK;
                    token->type = T_QUESTION_MK;
                }
                else if (c == '|')
                {
                    state = PIPE;
                    token->type = T_PIPE;
                }
                else if (c == '.')
                {
                    state = DOT;
                    token->type = T_DOT;
                }
                else if (c == ',')
                {
                    state = COMMA;
                    token->type = T_COMMA;
                }
                else if (c == ';')
                {
                    state = SEMICOLON;
                    token->type = T_SEMICOL;
                }
                else if (c == ':')
                {
                    state = COLON;
                    token->type = T_COLON;
                }
                else if (c == '!')
                {
                    state = NOT_EQUAL;
                    token->type = T_NE;
                }
                else if (c == '<')
                {
                    state = LESS_THAN;
                    token->type = T_LT;
                }
                else if (c == '>')
                {
                    state = GREATER_THAN;
                    token->type = T_GT;
                }
                else if (c == '=')
                {
                    state = ASSIGN;
                    token->type = T_ASSIGN;
                }
                else if (c == '@')
                {
                    state = IMPORT;
                    token->type = T_KEYWORD;
                    token->current_attribute = DSTR;
                    token->attribute.dStr = dStringCreate();
                }
                else if (c == '+')
                {
                    state = PLUS;
                    token->type = T_PLUS;
                }
                else if (c == '-')
                {
                    state = MINUS;
                    token->type = T_MINUS;
                }
                else if (c == '*')
                {
                    state = MULTIPLY;
                    token->type = T_MUL;
                }
                else if (c == '/')
                {
                    state = DIVIDE;
                    token->type = T_DIV;
                }
                else if (isdigit(c))
                {
                    if (c == '0')
                    {
                        state = INT_ZERO;
                    }
                    else
                    {
                        state = INTEGER;
                    }
                    token->type = T_INT;
                    token->attribute.dStr = dStringCreate();
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (c == '\"')
                {
                    state = STRING_START;
                    token->type = T_STR;
                    token->current_attribute = DSTR;
                    token->attribute.dStr = dStringCreate();
                }
                else if (c == EOF)
                {
                    state = END_OF_FILE;
                    token->type = T_EOF;
                }
                else if (!isspace(c))
                {
                    state = ERROR;
                    token->type = T_ERROR;
                }
                
                // Following states are terminal, no more scanning required to decide token type
                if (\
                state == LEFT_SQ_BRACKET || state == RIGHT_SQ_BRACKET || state == LEFT_BRACKET ||\
                state == RIGHT_BRACKET || state == LEFT_BRACE || state == RIGHT_BRACE ||\
                state == QUESTION_MARK || state == PIPE || state == DOT || state == COMMA ||\
                state == SEMICOLON || state == COLON || state == PLUS || state == MINUS ||\
                state == MULTIPLY || state == END_OF_FILE || state == ERROR\
                )
                {
                    tokenScanned = true;
                }

                break;
            }
            case IDENTIFIER:
            {
                if (c == '_' || isalnum(c))
                {
                    dStringAddChar(token->attribute.dStr, c);
                }
                else
                {
                    if(c == '(')
                    {
                        token->type = T_FX_ID;
                    }
                    if(isdStringKeyword(token->attribute.dStr))
                    {
                        token->type = T_KEYWORD;
                    }
                    ungetc(c, src);
                    tokenScanned = true;
                }

                break;
            }
            case IMPORT:
            {
                static size_t importCnt = 0;

                //printf("importCnt = %lu, c = %c, importStr[importCnt] = %c \n", importCnt, c, importStr[importCnt]);

                if (importCnt < strlen(importStr) && c == importStr[importCnt])
                {
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (importCnt == strlen(importStr) && c == '(')
                {
                    ungetc(c, src);
                    tokenScanned = true;
                }
                else
                {
                    token->type = T_ERROR;
                    token->current_attribute = NONE;
                    dStringDestroy(token->attribute.dStr);
                    tokenScanned = true;
                }

                importCnt++;

                break;
            }
            case NOT_EQUAL:
            {
                if (c != '=')
                {
                    token->type = T_ERROR;
                }

                tokenScanned = true;

                break;
            }
            case LESS_THAN:
            {
                if (c == '=')
                {
                    token->type = T_LE;
                }
                else
                {
                    ungetc(c, src);
                }

                tokenScanned = true;

                break;
            }
            case GREATER_THAN:
            {
                if (c == '=')
                {
                    token->type = T_GE;
                }
                else
                {
                    ungetc(c, src);
                }

                tokenScanned = true;

                break;
            }
            case ASSIGN:
            {
                if (c == '=')
                {
                    token->type = T_EQ;
                }
                else
                {
                    ungetc(c, src);
                }

                tokenScanned = true;

                break;
            }
            case DIVIDE:
            {
                if (c == '/')
                {
                    state = COMMENT;
                }
                else
                {
                    ungetc(c, src);
                    tokenScanned = true;
                }

                break;
            }
            case COMMENT:
            {
                if (c == '\\')
                {
                    state = NO_LINEFEED;
                }
                // c == EOF for dealing with 'comment and EOF on the same line' edge case
                else if (c == '\n' || c == EOF)
                {
                    state = INITIAL;
                }

                break;
            }
            case NO_LINEFEED:
            {
                if (c != '\n')
                {
                    ungetc(c, src);
                }
                state = COMMENT;

                // Different code for handling '\' symbol in comment
                /*
                if (c == '\n')
                {
                    state = COMMENT;
                }
                else
                {
                    token->type = T_ERROR;
                    tokenScanned = true;
                }
                */

                break;
            }
            case INT_ZERO:
            {
                if (c == '.')
                {
                    token->type = T_FLOAT;
                    state = FLOAT_DP_START;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (c == 'e' || c == 'E')
                {
                    token->type = T_FLOAT;
                    state = FLOAT_EX_START;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else
                {
                    dStringDestroy(token->attribute.dStr);

                    if (isalnum(c))
                    {
                        token->type = T_ERROR;
                    }
                    else
                    {
                        token->current_attribute = I;
                        token->attribute.i = 0;
                        ungetc(c, src);
                    }

                    tokenScanned = true;
                }

                break;
            }
            case INTEGER:
            {
                if (isdigit(c))
                {
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (c == '.')
                {
                    token->type = T_FLOAT;
                    state = FLOAT_DP_START;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (c == 'e' || c == 'E')
                {
                    token->type = T_FLOAT;
                    state = FLOAT_EX_START;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (isalpha(c))
                {
                    token->type = T_ERROR;
                    dStringDestroy(token->attribute.dStr);
                    tokenScanned = true;
                }
                else
                {
                    int dStrInt = dStringToInt(token->attribute.dStr);
                    dStringDestroy(token->attribute.dStr);
                    token->current_attribute = I;
                    token->attribute.i = dStrInt;
                    ungetc(c, src);
                    tokenScanned = true;
                }

                break;
            }
            case FLOAT_DP_START:
            {
                if (isdigit(c))
                {
                    state = FLOAT_DP_END;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else
                {
                    token->type = T_ERROR;
                    dStringDestroy(token->attribute.dStr);
                    tokenScanned = true;
                }

                break;
            }
            case FLOAT_DP_END:
            {
                if (isdigit(c))
                {
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (c == 'e' || c == 'E')
                {
                    state = FLOAT_EX_START;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else
                {
                    double dStrDouble = dStringToDouble(token->attribute.dStr);
                    dStringDestroy(token->attribute.dStr);
                    token->current_attribute = F;
                    token->attribute.f = dStrDouble;
                    ungetc(c, src);
                    tokenScanned = true;
                }

                break;
            }
            case FLOAT_EX_START:
            {
                if (c == '+' || c == '-')
                {
                    state = FLOAT_EX_MID;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else if (isdigit(c))
                {
                    state = FLOAT_EX_END;
                    dStringAddChar(token->attribute.dStr, c);
                }
                else
                {
                    token->type = T_ERROR;
                    dStringDestroy(token->attribute.dStr);
                    tokenScanned = true;
                }

                break;
            }
            case FLOAT_EX_MID:
            {
                if (isdigit(c))
                {
                    dStringAddChar(token->attribute.dStr, c);
                    state = FLOAT_EX_END;
                }
                else
                {
                    token->type = T_ERROR;
                    dStringDestroy(token->attribute.dStr);
                    tokenScanned = true;
                }

                break;
            }
            case FLOAT_EX_END:
            {
                if (isdigit(c))
                {
                    dStringAddChar(token->attribute.dStr, c);
                }
                else
                {
                    double dStrDouble = dStringToDouble(token->attribute.dStr);
                    dStringDestroy(token->attribute.dStr);
                    token->current_attribute = F;
                    token->attribute.f = dStrDouble;
                    ungetc(c, src);
                    tokenScanned = true;
                }

                break;
            }
            case STRING_START:
            {
                // Escape sequences, hash symbol and control characters (0 - 32) get converted
                // to \XXX format
                if (c == '\\')
                {
                    state = ESCAPE_SEQ;
                }
                // Dealing with unterminated string ending with EOF
                // and with single line string split to multiple lines
                else if (c == EOF || c == '\n')
                {
                    token->type = T_ERROR;
                    token->current_attribute = NONE;
                    dStringDestroy(token->attribute.dStr);
                    tokenScanned = true;
                }
                else if ((c >= 0 && c <= 32) || c == '#')
                {
                    dStringAddIntIFJcode24Format(token->attribute.dStr, c);
                }
                else if (c == '\"')
                {
                    tokenScanned = true;
                }
                else
                {
                    dStringAddChar(token->attribute.dStr, c);
                }

                break;
            }
            case ESCAPE_SEQ:
            {
                state = STRING_START;
                
                // If c is specified in hex format
                if (c == 'x')
                {
                    state = ESCAPE_SEQ_HEX;
                }
                else
                {
                    // Allowed escape sequences
                    if (c == 'n')
                    {
                        dStringAddIntIFJcode24Format(token->attribute.dStr, LINE_FEED);
                    }
                    else if (c == 'r')
                    {
                        dStringAddIntIFJcode24Format(token->attribute.dStr, CARRIAGE_RETURN);
                    }
                    else if (c == 't')
                    {
                        dStringAddIntIFJcode24Format(token->attribute.dStr, HORIZONTAL_TAB);
                    }
                    else if (c == '\\' || c == '\"' || c == '\'')
                    {
                        dStringAddIntIFJcode24Format(token->attribute.dStr, c);
                    }
                    else
                    {
                        token->type = T_ERROR;
                        token->current_attribute = NONE;
                        dStringDestroy(token->attribute.dStr);
                        tokenScanned = true;
                    }
                }
                
                break;
            }
            case ESCAPE_SEQ_HEX:
            {
                // Variable for storing escape sequence in integer format
                static int escapeSequence = -1;
                
                if (c >= '0' && c <= '9')
                {
                    // Converting c to int value 0 - 9
                    c -= '0';
                    // escapeSequence was not modified -> c contains hex value at position 16^1
                    if (escapeSequence == -1)
                    {
                        // Setting c to escapeSequence multiplied by it's weight
                        escapeSequence = c * 16;
                    }
                    // escape sequence was modified -> c contains hex value at position 16^0
                    else
                    {
                        state = STRING_START;
                        escapeSequence += c;
                        
                        // Returning converted escape sequence to source file
                        ungetc(escapeSequence, src);
                        if (\
                        escapeSequence == 'n' || escapeSequence == 'r' || escapeSequence == 't' ||\
                        escapeSequence == '\\' || escapeSequence == '\"' || escapeSequence == '\''\
                        )
                        {
                            ungetc('\\', src);
                        }
                        // Resetting escape sequence variable
                        escapeSequence = -1;
                    }
                    
                }
                else if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
                {
                    // Converting c to int value 10 - 15
                    c = tolower(c) - 'a' + 10;
                    // escapeSequence was not modified -> c contains hex value at position 16^1
                    if (escapeSequence == -1)
                    {
                        // Setting c to escapeSequence multiplied by it's weight
                        escapeSequence = c * 16;
                    }
                    // escape sequence was modified -> c contains hex value at position 16^0
                    else
                    {
                        state = STRING_START;
                        escapeSequence += c;
                        // returning converted escape sequence to source file
                        ungetc(escapeSequence, src);
                        if (\
                        escapeSequence == 'n' || escapeSequence == 'r' || escapeSequence == 't' ||\
                        escapeSequence == '\\' || escapeSequence == '\"' || escapeSequence == '\''\
                        )
                        {
                            ungetc('\\', src);
                        }
                        // Resetting escape sequence variable
                        escapeSequence = -1;
                    }
                }
                // c does not contain valid hex digit
                else
                {
                    token->type = T_ERROR;
                    token->current_attribute = NONE;
                    dStringDestroy(token->attribute.dStr);
                    tokenScanned = true;
                }

                break;
            }
            default: break;
        }
    }
	
    return token;
}

