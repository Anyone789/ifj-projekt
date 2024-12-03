// Testbench for scanner.c
// Author(s): Václav Bergman
// Last Edit: 1.11.2024

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "scanner.h"

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        fprintf(stderr, "Too many arguments!");
        exit(0);
    }
    if (argc == 1)
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
            default:
                break;
            }

            if (token->type == T_ERROR || token->type == T_EOF)
            {
                stop = true;
            }

            destroyToken(token);

            tokenCnt++;

        } while (!stop);

        fclose(src);
    }

    return 0;
}