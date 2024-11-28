#include "generator.h"

void generateVariables(DSTRING *name)
{

    printf("DEFVAR LF@%s\n", name->str);
}

void assigneVariables(DSTRING *name)
{
    printf("POPS LF@%s\n", name->str);
}

void generateHeader()
{
    printf(".IFJcode24\n");
    printf("DEFVAR GF@op1\n");
    printf("DEFVAR GF@op2\n");
    printf("DEFVAR GF@result\n");
    printf("JUMP main\n");
}

void generateFunctionHead(bstSymtable *symtable)
{
    printf("LABEL %s\n", symtable->key);
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    for (int i = ((fceData *)symtable->data)->paramCount - 1; i >= 0; i--)
    {
        printf("DEFVAR LF@%s\n", ((fceData *)symtable->data)->params[i].name);
        printf("POPS LF@%s\n", ((fceData *)symtable->data)->params[i].name);
    }
}
void generateFunctionReturn(DSTRING *functionID)
{
    printf("POPFRAME\n");
    if (strcmp(functionID->str, "main") != 0)
    {
        printf("RETURN\n");

        return;
    }
    else
    {
        printf("EXIT int@0\n");
    }
}

void generateIfBeginning(int ifAloneCounter, int ifInsideCount)
{
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS else%d%d\n", ifAloneCounter, ifInsideCount);
}

void generateElse(int ifAloneCounter, int ifInsideCount)
{
    generateIfEnd(ifAloneCounter, ifInsideCount);
    printf("LABEL else%d%d\n", ifAloneCounter, ifInsideCount);
}

void generateIfEnd(int ifAloneCounter, int ifInsideCount)
{
    printf("JUMP elseEnd%d%d\n", ifAloneCounter, ifInsideCount);
}

void generateWhileBeginning(int whileCounter)
{
    printf("LABEL whileBegin%d\n", whileCounter);
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS whileEnd%d\n", whileCounter);
}

// built in functions
void generateReadStr()
{
    printf("LABEL readStr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@str\n");
    printf("READ LF@str string\n");
    printf("PUSHS LF@str\n");
    printf("RETURN\n");
}

void generateReadInt()
{
    printf("LABEL readInt\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@int\n");
    printf("READ LF@int int\n");
    printf("PUSHS LF@int\n");
    printf("RETURN\n");
}

void generateReadFloat()
{
    printf("LABEL readFloat\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@float\n");
    printf("READ LF@float float\n");
    printf("PUSHS LF@float\n");
    printf("RETURN\n");
}

void generateWrite()
{
    printf("LABEL write\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@term\n");
    printf("DEFVAR LF@type\n");

    printf("POPS LF@term\n");

    printf("TYPE LF@type LF@term\n");

    // Kontrola na null hodnotu
    printf("JUMPIFEQ write_null LF@type string@nil\n");

    // Kontrola na int hodnotu
    printf("JUMPIFEQ write_int LF@type string@int\n");

    // Kontrola na float hodnotu
    printf("JUMPIFEQ write_float LF@type string@float\n");

    // Kontrola na string hodnotu
    printf("JUMPIFEQ write_string LF@type string@string\n");

    // Ak je typ neznámy, ukončíme s chybou
    printf("EXIT int@4\n");

    // Spracovanie null hodnoty
    printf("LABEL write_null\n");
    printf("WRITE string@null\n");
    printf("JUMP write_end\n");

    // Spracovanie int hodnoty
    printf("LABEL write_int\n");
    printf("WRITE LF@term\n");
    printf("JUMP write_end\n");

    // Spracovanie float hodnoty
    printf("LABEL write_float\n");
    printf("WRITE LF@term\n");
    printf("JUMP write_end\n");

    // Spracovanie string hodnoty
    printf("LABEL write_string\n");
    printf("WRITE LF@term\n");
    printf("JUMP write_end\n");

    // Koniec write funkcie
    printf("LABEL write_end\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateInt2Float()
{
    printf("LABEL i2f\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@int\n");
    printf("DEFVAR LF@float\n");

    printf("POPS LF@int\n");

    printf("INT2FLOAT LF@float LF@int\n");

    printf("PUSHS LF@float\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateFloat2Int()
{
    printf("LABEL f2i\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@float\n");
    printf("DEFVAR LF@int\n");

    printf("POPS LF@float\n");

    printf("FLOAT2INT LF@int LF@float\n");

    printf("PUSHS LF@int\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateString()
{
    printf("LABEL string\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@term\n");
    printf("DEFVAR LF@result\n");

    printf("POPS LF@term\n");

    printf("MOVE LF@result LF@term\n");

    printf("PUSHS LF@result\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateLenght()
{
    printf("LABEL length\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@str\n");
    printf("DEFVAR LF@length\n");

    printf("POPS LF@str\n");

    printf("STRLEN LF@length LF@str\n");

    printf("PUSHS LF@length\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateConcat()
{
    printf("LABEL concat\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@str1\n");
    printf("DEFVAR LF@str2\n");
    printf("DEFVAR LF@result\n");

    printf("POPS LF@str2\n");
    printf("POPS LF@str1\n");

    printf("CONCAT LF@result LF@str1 LF@str2\n");

    printf("PUSHS LF@result\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
}

// ten global frame asi nieje najlpsi tuna netusim robil to cetak
void generateSubstring()
{
    printf("LABEL substring\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    // Definice proměnných
    printf("DEFVAR LF@str\n");       // Vstupní řez
    printf("DEFVAR LF@i\n");         // Počáteční index
    printf("DEFVAR LF@j\n");         // Konečný index
    printf("DEFVAR LF@result\n");    // Podřez
    printf("DEFVAR LF@temp_char\n"); // Dočasný znak
    printf("DEFVAR LF@length\n");    // Délka vstupního řezu

    // Získání vstupů ze zásobníku
    printf("POPS LF@j\n");
    printf("POPS LF@i\n");
    printf("POPS LF@str\n");

    // Kontrola indexů
    printf("STRLEN LF@length LF@str\n");
    printf("LT LF@cond1 LF@i int@0\n");
    printf("JUMPIFEQ substring_null LF@cond1 bool@true\n");
    printf("LT LF@cond2 LF@j int@0\n");
    printf("JUMPIFEQ substring_null LF@cond2 bool@true\n");
    printf("GT LF@cond3 LF@i LF@j\n");
    printf("JUMPIFEQ substring_null LF@cond3 bool@true\n");
    printf("GE LF@cond4 LF@i LF@length\n");
    printf("JUMPIFEQ substring_null LF@cond4 bool@true\n");
    printf("GT LF@cond5 LF@j LF@length\n");
    printf("JUMPIFEQ substring_null LF@cond5 bool@true\n");

    // Vytvoření podřetězce
    printf("MOVE LF@result string@\n");
    printf("LABEL substring_loop\n");
    printf("LT LF@cond6 LF@i LF@j\n");
    printf("JUMPIFEQ substring_end LF@cond6 bool@false\n");
    printf("GETCHAR LF@temp_char LF@str LF@i\n");
    printf("CONCAT LF@result LF@result LF@temp_char\n");
    printf("ADD LF@i LF@i int@1\n");
    printf("JUMP substring_loop\n");

    // Ukončení
    printf("LABEL substring_end\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    // Vrácení null při neplatných indexech
    printf("LABEL substring_null\n");
    printf("PUSHS nil@nil\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateStrCmp()
{
    printf("LABEL strcmp\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@result\n");
    printf("DEFVAR LF@i\n");
    printf("DEFVAR LF@char_s1\n");
    printf("DEFVAR LF@char_s2\n");

    // Inicializace indexu
    printf("DEFVAR LF@i\n");
    printf("MOVE LF@i int@0\n");

    // Smyčka pro porovnání
    printf("LABEL strcmp_loop\n");
    printf("GETCHAR LF@char_s1 LF@s1 LF@i\n");
    printf("GETCHAR LF@char_s2 LF@s2 LF@i\n");
    printf("JUMPIFNEQ strcmp_end\n");

    // Pokud oba znaky jsou stejné, pokračuj
    printf("ADD LF@i LF@i int@1\n");
    printf("JUMP strcmp_loop\n");

    // Pokud znaky nejsou stejné
    printf("LABEL strcmp_end\n");
    printf("JUMPIFGT LF@char_s1 LF@char_s2 THEN_GREATER\n");
    printf("JUMPIFLT LF@char_s1 LF@char_s2 THEN_LESS\n");

    // Pokud jsou znaky stejné, pokračujeme až do konce
    printf("DEFVAR LF@result\n");
    printf("MOVE LF@result int@0\n");
    printf("RETURN\n");

    printf("LABEL THEN_GREATER\n");
    printf("MOVE LF@result int@1\n");
    printf("RETURN\n");

    printf("LABEL THEN_LESS\n");
    printf("MOVE LF@result int@-1\n");
    printf("RETURN\n");
}

void generateOrd()
{
    printf("LABEL ord\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@char\n");
    printf("DEFVAR LF@result\n");

    // Získání znaku podle indexu
    printf("GETCHAR LF@char LF@s LF@i\n");

    // Kontrola platnosti indexu
    printf("JUMPIFNULL ord_invalid\n");

    // Vrátíme ASCII hodnotu znaku
    printf("STRLEN LF@len LF@s\n");
    printf("JUMPIFLT ord_invalid LF@i LF@len\n");

    printf("ORD LF@result LF@char\n");
    printf("RETURN\n");

    printf("LABEL ord_invalid\n");
    printf("MOVE LF@result int@0\n");
    printf("RETURN\n");
}

void generateChr()
{
    printf("LABEL chr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@char\n");
    printf("DEFVAR LF@result\n");

    // Pokud je hodnota mimo platný rozsah (0-255), vrátíme null
    printf("JUMPIFLT chr_invalid LF@i int@0\n");
    printf("JUMPIFGT chr_invalid LF@i int@255\n");

    // Vytvoření jednoznakového řetězce
    printf("CHAR LF@char LF@i\n");
    printf("MOVE LF@result LF@char\n");
    printf("RETURN\n");

    printf("LABEL chr_invalid\n");
    printf("MOVE LF@result nil@nil\n");
    printf("RETURN\n");
}
