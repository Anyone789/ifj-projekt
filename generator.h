#ifndef GENERATOR_H
#define GENERATOR_H

#include "stack.h"
#include "errorCodes.h"
#include "scanner.h"
#include "expression.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void generateVariables(DSTRING *name);
void assigneVariables(DSTRING *name);
void generateHeader();
void generateFunctionHead(bstSymtable *symtable);
void generateFunctionReturn(DSTRING *functionID);
void generateFunctionEnd(bstSymtable *symtable);
void generateFunctionCall(bstSymtable *symtableGlobal, bstSymtable *symtableLocal);
void generateIfBeginning(int ifAloneCounter);
void generateIfEnd(int ifAloneCounter, int ifInsideCount);
void generateElse(int ifAloneCounter, int ifInsideCount);
void generateWhileBeginning(int whileCounter);
// built in functions

/**
 * @brief Generates code for reading an integer from the standard input.
 *
 * This function generates code that reads an integer from the standard input.
 * The input value is stored in a temporary variable.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateReadInt();

/**
 * @brief Generates code for reading a string from the standard input.
 *
 * This function generates code that reads a string from the standard input.
 * The input string is stored in a temporary variable.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateReadStr();

/**
 * @brief Generates code for reading a floating-point number from the standard input.
 *
 * This function generates code that reads a floating-point number from the standard input.
 * The input value is stored in a temporary variable.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateReadFloat();

/**
 * @brief Generates code for writing the value of a variable to the standard output.
 *
 * This function generates code that writes the value of a variable (could be of type string, integer, or float)
 * to the standard output.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateWrite();

/**
 * @brief Generates code for converting an integer to a floating-point number.
 *
 * This function generates code to convert an integer into its floating-point representation.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateInt2Float();

/**
 * @brief Generates code for converting a floating-point number to an integer.
 *
 * This function generates code to convert a floating-point number to its integer representation,
 * truncating the decimal part.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateFloat2Int();

/**
 * @brief Generates code for converting a string literal to a dynamic string representation.
 *
 * This function generates code that converts a string literal (or constant) to a dynamic string representation.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateString();

/**
 * @brief Generates code for getting the length of a string.
 *
 * This function generates code to calculate the length of a string and stores it in a variable.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateLenght();

/**
 * @brief Generates code for concatenating two strings.
 *
 * This function generates code that concatenates two strings into a single string.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateConcat();

/**
 * @brief Generates code for extracting a substring from a given string.
 *
 * This function generates code that extracts a substring from a given string using specified start and end indices.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateSubstring();

/**
 * @brief Generates code for comparing two strings lexicographically.
 *
 * This function generates code that compares two strings lexicographically and returns:
 * -1 if the first string is less than the second,
 * 0 if they are equal,
 * 1 if the first string is greater than the second.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateStrCmp();

/**
 * @brief Generates code for getting the ordinal value (ASCII code) of a character in a string.
 *
 * This function generates code to obtain the ordinal value (ASCII code) of a character at a given index in a string.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateOrd();

/**
 * @brief Generates code for converting an ASCII value to a character string.
 *
 * This function generates code that converts an integer ASCII value to a single-character string.
 *
 * @return void
 *
 * @note This function does not return any value. It only generates code.
 */
void generateChr();

#endif