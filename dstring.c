// dstring.c
// Module for manipulating with strings dynamically
// Author(s): Václav Bergman, Tomáš Hrbáč
// Last Edited: 30.11.2024

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dstring.h"
#include "errorCodes.h"

DSTRING *dStringCreate()
{
    DSTRING *dStr = malloc(sizeof(DSTRING));
    if (dStr == NULL)
    {
        fprintf(stderr, "Dynamic string allocation error");
        exit(INTERNAL_ERROR);
    }

    dStr->str = malloc(DSTRING_BASE_LEN);
    if (dStr->str == NULL)
    {
        fprintf(stderr, "Dynamic string allocation error");
        exit(INTERNAL_ERROR);
    }

    dStr->str[0] = '\0';
    dStr->length = 0;
    dStr->allocSize = DSTRING_BASE_LEN;

    return dStr;
}

void dStringDestroy(DSTRING *dStr)
{
    free(dStr->str);
    free(dStr);
}

void dStringClear(DSTRING *dStr)
{
   dStr->str[0] = '\0';
   dStr->length = 0;
}

void dStringAddChar(DSTRING *dStr, char c)
{
    // Test for string size overflow
    if ((dStr->length + 1) >= dStr->allocSize)
    {
        // String size gets realocated to original length + base length
        dStr->str = realloc(dStr->str, dStr->length + DSTRING_BASE_LEN);
        if (dStr == NULL)
        {
            fprintf(stderr, "Dynamic string reallocation error");
            exit(INTERNAL_ERROR);
        }
        
        dStr->allocSize = dStr->length + DSTRING_BASE_LEN;
    }

    dStr->str[dStr->length] = c;
    dStr->length++;
    dStr->str[dStr->length] = '\0';
}

void dStringCopy(DSTRING *dest, DSTRING *src)
{
   if ((src->length + 1) > dest->allocSize)
   {
      free(dest->str);
      dest->str = malloc(src->allocSize);
      if (dest->str == NULL)
      {
        fprintf(stderr, "Dynamic string allocation error");
        exit(INTERNAL_ERROR);
      }
   }

   strcpy(dest->str, src->str);
   dest->length = src->length;
}

int dStringCmp(DSTRING *dStr1, DSTRING *dStr2)
{
   return strcmp(dStr1->str, dStr2->str);
}

int dStringCmpConst(DSTRING *dStr, const char *cStr)
{
   return strcmp(dStr->str, cStr);
}

char *dStringGetStr(DSTRING *dStr)
{
   return dStr->str;
}

int dStringGetLength(DSTRING *dStr)
{
   return dStr->length;
}

int dStringToInt(DSTRING *dStr)
{
    return atoi(dStr->str);
}

double dStringToDouble(DSTRING *dStr)
{
    return atof(dStr->str);
}

void dStringAddIntIFJcode24Format(DSTRING *dStr, int i)
{
    // Leading backslash
    dStringAddChar(dStr, '\\');
    // Digit at 10^2 position
    dStringAddChar(dStr, ((i / 100) % 10) + '0');
    // Digit at 10^1 position
    dStringAddChar(dStr, ((i / 10) % 10) + '0');
    // Digit at 10^0 position
    dStringAddChar(dStr, (i % 10) + '0');
}
