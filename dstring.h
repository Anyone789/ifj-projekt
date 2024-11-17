// dstring.h
// Module for manipulating with strings dynamically
// Author(s): Václav Bergman, Tomáš Hrbáč
// Last Edited: 31.10.2024

#ifndef DSTRING_H
#define DSTRING_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "errorCodes.h"
#include <stdbool.h>
// Base length of dynamic string
#define DSTRING_BASE_LEN 8 

// Data type representing dynamic string
typedef struct dstring
{
  char* str;		      // Pointer to a string
  int length;	        // Current string length
  int allocSize;	    // Amount of bytes allocated
} DSTRING;

DSTRING *dStringCreate();

void dStringDestroy(DSTRING *dStr);

void dStringClear(DSTRING *dStr);

void dStringAddChar(DSTRING *dStr, char c);

void dStringCopy(DSTRING *dest, DSTRING *src);

int dStringCmp(DSTRING *dStr1, DSTRING *dStr2);

int dStringCmpConst(DSTRING *dStr, const char *cStr);

char *dStringGetStr(DSTRING *dStr);

int dStringGetLength(DSTRING *dStr);

int dStringToInt(DSTRING *dStr);

double dStringToDouble(DSTRING *dStr);

#endif