// dstring.c
// Module for manipulating with strings dynamically
// Author(s): Václav Bergman, Tomáš Hrbáč
// Last Edited: 31.10.2024

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dstring.h"
#include "errorCodes.h"

// Dynamic string constructor
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

// Dynamic string destructor
void dStringDestroy(DSTRING *dStr)
{
    free(dStr->str);
    free(dStr);
}

// Clears contents of a dynamic string
void dStringClear(DSTRING *dStr)
{
   dStr->str[0] = '\0';
   dStr->length = 0;
}

// Adds one character to a dynamic string
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

// Makes a copy of src to dest
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

// Compares two dynamic strings
int dStringCmp(DSTRING *dStr1, DSTRING *dStr2)
{
   return strcmp(dStr1->str, dStr2->str);
}

// Compares a dynamic string with a constant string
int dStringCmpConst(DSTRING *dStr, const char *cStr)
{
   return strcmp(dStr->str, cStr);
}

// Returns contents of a dynamic string
char *dStringGetStr(DSTRING *dStr)
{
   return dStr->str;
}

// Returns length of a dynamic string
int dStringGetLength(DSTRING *dStr)
{
   return dStr->length;
}

// Converts dynamic string to int
int dStringToInt(DSTRING *dStr)
{
    return atoi(dStr->str);
}

// Converts dynamic string to double
double dStringToDouble(DSTRING *dStr)
{
    return atof(dStr->str);
}