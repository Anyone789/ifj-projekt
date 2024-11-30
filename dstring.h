// dstring.h
// Module for manipulating with strings dynamically
// Author(s): Václav Bergman, Tomáš Hrbáč
// Last Edited: 30.11.2024

#ifndef DSTRING_H
#define DSTRING_H

// Base length of dynamic string
#define DSTRING_BASE_LEN 8 

// Data type representing dynamic string
typedef struct dstring
{
  char* str;		    // Pointer to a string
  int length;	        // Current string length
  int allocSize;	    // Amount of bytes allocated
} DSTRING;

/*
    @brief Creates a DSTRING
    @param NONE
    @return Pointer to a DSTRING
*/
DSTRING *dStringCreate();

/*
    @brief Frees a DSTRING
    @param *dStr pointer to a DSTRING
    @return NONE
*/
void dStringDestroy(DSTRING *dStr);

/*
    @brief Clears DSTRING contents
    @param *dStr pointer to a DSTRING
    @return NONE
*/
void dStringClear(DSTRING *dStr);

/*
    @brief Adds one character to a DSTRING
    @param *dStr pointer to a DSTRING
    @param c Character
    @return NONE
*/
void dStringAddChar(DSTRING *dStr, char c);

/*
    @brief Copies contents of src DSTRING to dest DSTRING 
    @param *dest pointer to the destination
    @param *src pointer to the source
    @return NONE
*/
void dStringCopy(DSTRING *dest, DSTRING *src);

/*
    @brief Compares two DSTRINGs
    @param *dStr1 pointer to a DSTRING
    @param *dStr2 pointer to a DSTRING
    @return 0 when dStr1->str == dStr2->str\
    @return >0 when dStr1->str > dStr2->str\
    @return <0 when dStr1->str < dStr2->str
*/
int dStringCmp(DSTRING *dStr1, DSTRING *dStr2);

/*
    @brief Compares a DSTRING to a const string
    @param *dStr pointer to a DSTRING
    @param *cStr pointer to a const string
    @return 0 when dStr1->str == cStr\
    @return >0 when dStr1->str > cStr\
    @return <0 when dStr1->str < cStr
*/
int dStringCmpConst(DSTRING *dStr, const char *cStr);

/*
    @brief Returns pointer to DSTRING contents
    @param *dStr pointer to a DSTRING
    @return Pointer to char
*/
char *dStringGetStr(DSTRING *dStr);

/*
    @brief Returns DSTRING length
    @param *dStr pointer to a DSTRING
    @return int
*/
int dStringGetLength(DSTRING *dStr);

/*
    @brief Returns contents of a DSTRING converted to int
    @param *dStr pointer to a DSTRING
    @return int
*/
int dStringToInt(DSTRING *dStr);

/*
    @brief Returns contents of a DSTRING converted to double
    @param *dStr pointer to a DSTRING
    @return double
*/
double dStringToDouble(DSTRING *dStr);

/*
    @brief Appends integer to DSTRING in IFJcode24 format \xyz, where xyz is decimal number 000-255
    @param *dStr pointer to a DSTRING
    @param int integer to be added
    @return NONE
*/
void dStringAddIntIFJcode24Format(DSTRING *dStr, int i);

#endif
