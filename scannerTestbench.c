// Testbench for scanner.c
// Author(s): Václav Bergman
// Last Edit: 19.11.2024

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "scanner.h"

// String equivalents for token types
const char *TOKEN_TYPE_STRING[] = {
    "T_KEYWORD", "T_ERROR", "T_ID", "T_FX_ID", "T_BUILT_IN_FX_ID",
    "T_INT", "T_STR", "T_FLOAT", 
    "T_PLUS", "T_MINUS", "T_MUL", "T_DIV",
    "T_EQ", "T_NE", "T_LT", "T_LE", "T_GT", "T_GE", "T_ASSIGN",
    "T_SEMICOL", "T_COLON", "T_L_BRACE", "T_R_BRACE", "T_L_SQ_BRACKET", "T_R_SQ_BRACKET",
    "T_L_BRACKET", "T_R_BRACKET", "T_QUESTION_MK", "T_PIPE", "T_DOT", "T_COMMA", "T_COM",
    "T_EOF", "T_UNDEFINED"
};

int main(int argc, char **argv)
{
    if(argc > 2)
    {
        fprintf(stderr, "Too many arguments!");
        exit(0);
    }
    if(argc == 1)
    {
        fprintf(stderr, "Few arguments!");
        exit(0);
    }
    else
    {
        FILE *src;
        
        if ((src = fopen(argv[1], "r")) == NULL)
        {
            fprintf(stderr, "The file cannot be opened.");
            exit(0);
        }

        setSourceFile(src);

        TOKEN *token;

        int tokenCnt = 1;
        bool stop = false;
        do
        {
            token = getToken();
            printf("Token No: %d Type: %s Attribute: ", tokenCnt, TOKEN_TYPE_STRING[token->type]);
            
            switch (token->current_attribute)
            {
                case DSTR:
                {
                    printf("%s\n", dStringGetStr(token->attribute.dStr));
                    break;
                }
                case I:
                {
                    printf("%d\n", token->attribute.i);
                    break;
                }
                case F:
                {
                    printf("%f\n", token->attribute.f);
                    break;
                }
                case NONE:
                {
                    printf("NONE\n");
                    break;
                }
                default: break;
            }

            if (token->type == T_ERROR || token->type == T_EOF)
            {
                stop = true;
            }

            destroyToken(token);

            tokenCnt++;

        }while (!stop);

        fclose(src);
    }

    return 0;
}