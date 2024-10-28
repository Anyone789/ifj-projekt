// Module for lexical analyser
// Author(s): Tomáš Hrbáč
// Last Edit: 28.10.2024


#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "dstring.h"
#include "dstring.c"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

FILE *src;
TOKEN token;

void setSourceFile(FILE *file)
{
  src = file;
}

void initToken(void)
{
	token.atribute.dStr = dStringCreate();
	dStringClear(token.atribute.dStr);
}

int getToken(TOKEN *arg) 
{
	int c;
	initToken();
	while(c != EOF)
	{
		dStringClear(token.atribute.dStr);
		c = getc(src);
	    if(c == '_' || isalpha(c))
		{
			c = getc(src);
            if(c == '_' || isalnum(c))
            {
                ungetc(c, src);
                for(unsigned i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
                {
                    if(dStringCmpConst(token.atribute.dStr, keywords[i]) == 0)
                        token.type = KEYWORD;
                }
                token.type = ID;
            }
            break;
		}
        else if(c == '[')
        {
            token.type = L_SQ_BRACKET;
            break;
        }
        else if(c == ']')
        {
            token.type = R_SQ_BRACKET;
            break;
        }
        else if(c == '(')
        {
            token.type = L_BRACKET;
            break;
        }
        else if(c == ')')
        {
            token.type = R_BRACKET;
            break;
        }
        else if(c == '{')
        {
            token.type = L_BRACE;
            break;
        }
        else if(c == '}')
        {
            token.type = R_BRACE;
            break;
        }
        else if(c == '?')
        {
            token.type = QUESTION_MK;
            break;
        }
        else if(c == '.')
        {
            token.type = DOT;
            break;
        }
        else if (c == ';')
        {
            token.type = SEMI_COL;
            break;
        }
        else if(c == '@')
        {
            bool isImport = true;

            int i = 0;
            int nextChar = getc(src);
            if(nextChar != keywords[0][i])
            {
                isImport = false;
                ungetc(nextChar, src);
                break;
            }

            if(isImport)
            {
                token.type = KEYWORD;
            }       
            else
            {
                token.type = ERROR;
            }            
        }
        // Strings
        else if(c == '"')
        {
            token.type = STR;
            while((c = getc(src)) != '"' && c != EOF)
            {
                dStringAddChar(token.atribute.dStr, c);
            }
            if(c == EOF)
            {
                token.type = ERROR;
            }
            break;
        }
        // Operators
        else if(c == '+')
        {
            token.type = PLUS;
            break;
        }
        else if(c == '-')
        {
            token.type = MINUS;
            break;
        }
        else if(c == '*')
        {
            token.type = MUL;
            break;
        }
        else if(c == '/')
        {
            token.type = DIV;
            break;
        }
        else if(c == '=')
        {
            c = getc(src);
            if(c == '=')
            {
                token.type = EQ;
            }
            else
            {
                ungetc(c, src);
                token.type = ASSIGN;
            }
            break;
        }
        else if(c == '!')
        {
            c = getc(src);
            if(c == '=')
            {
                token.type = NE;
            }
            else
            {
                ungetc(c, src);
                token.type = ERROR;
            }
            break;
        }
        else if(c == '<')
        {
            c = getc(src);
            if(c == '=')
                token.type = LE;
            else
            {
                ungetc(c, src);
                token.type = LT;
            }
            break;
        }
        else if(c == '>')
        {
            c = getc(src);
            if(c == '=')
                token.type = GE;
            else
            {
                ungetc(c, src);
                token.type = GT;
            }
            break;
        }
        // Numbers
        else if(isdigit(c))
        {
            token.type = INT;
            do {
                dStringAddChar(token.atribute.dStr, c);
                c = getc(src);
            } while(isdigit(c));

            if(c == '.') // Float
            { 
                token.type = FLOAT;
                dStringAddChar(token.atribute.dStr, c);
                c = getc(src);
                while(isdigit(c))
                {
                    dStringAddChar(token.atribute.dStr, c);
                    c = getc(src);
                }
            }
            if(c == 'e' || c == 'E') // Float with exponent
            { 
                token.type = FLOAT;
                dStringAddChar(token.atribute.dStr, c);
                c = getc(src);
                if(c == '+' || c == '-')
                {
                    dStringAddChar(token.atribute.dStr, c);
                    c = getc(src);
                }
                while(isdigit(c))
                {
                    dStringAddChar(token.atribute.dStr, c);
                    c = getc(src);
                }
            }
            ungetc(c, src);
            break;
        }        
        // Comments
        else if(c == '/') {
            int nextChar = getc(src);

            if(nextChar == '/') {
                // Single-line comment
                while ((c = getc(src)) != '\n' && c != EOF);
                continue;
            }
        }

        else 
        {
            token.type = ERROR;
            break;
        }
    }
	*arg = token;
    return (token.type == ERROR) ? EXIT_FAILURE : EXIT_SUCCESS;
}
