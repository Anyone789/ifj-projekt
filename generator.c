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
    printf("DEFVAR GF@typeOp1\n");
    printf("DEFVAR GF@typeOp2\n");
    printf("DEFVAR GF@result\n");
    printf("DEFVAR GF@nullValue\n");
    printf("JUMP main\n");
    generateReadStr();
    generateReadInt();
    generateReadFloat();
    generateWrite();
    generateInt2Float();
    generateFloat2Int();
    generateString();
    generateLenght();
    generateConcat();
    // generateSubstring();
    generateStrCmp();
    generateOrd();
    generateChr();
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

void generateIfBeginning(int ifAloneCounter)
{
    printf("POPS GF@nullValue\n");
    printf("TYPE GF@result GF@nullValue\n");
    // cmp with null
    printf("JUMPIFEQ else%d GF@result string@nil\n", ifAloneCounter);
    printf("JUMPIFEQ ifContinue%d GF@result string@bool\n", ifAloneCounter);
    printf("JUMP inIf%d\n", ifAloneCounter);
    printf("LABEL ifContinue%d\n", ifAloneCounter);
    printf("PUSHS GF@nullValue\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS else%d\n", ifAloneCounter);
    printf("LABEL inIf%d\n", ifAloneCounter);
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
    printf("POPS GF@nullValue\n");
    printf("TYPE GF@result GF@nullValue\n");
    // cmp with null
    printf("JUMPIFEQ whileEnd%d GF@result string@nil\n", whileCounter);
    printf("JUMPIFEQ whileContinue%d GF@result string@bool\n", whileCounter);
    printf("JUMP inWhile%d\n", whileCounter);
    printf("LABEL whileContinue%d\n", whileCounter);
    printf("PUSHS GF@nullValue\n");
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS whileEnd%d\n", whileCounter);
    printf("LABEL inWhile%d\n", whileCounter);
}

// built in functions
void generateReadStr()
{
    printf("LABEL readstr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@str\n");
    printf("READ LF@str string\n");
    printf("PUSHS LF@str\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateReadInt()
{
    printf("LABEL readi32\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@int\n");
    printf("READ LF@int int\n");
    printf("PUSHS LF@int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateReadFloat()
{
    printf("LABEL readf64\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@float\n");
    printf("READ LF@float float\n");
    printf("PUSHS LF@float\n");
    printf("POPFRAME\n");
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

    printf("JUMPIFEQ write_null LF@type string@nil\n");

    printf("JUMPIFEQ write_int LF@type string@int\n");

    printf("JUMPIFEQ write_float LF@type string@float\n");

    printf("JUMPIFEQ write_string LF@type string@string\n");

    printf("EXIT int@4\n");

    printf("LABEL write_null\n");
    printf("WRITE string@null\n");
    printf("JUMP write_end\n");

    printf("LABEL write_int\n");
    printf("WRITE LF@term\n");
    printf("JUMP write_end\n");

    printf("LABEL write_float\n");
    printf("WRITE LF@term\n");
    printf("JUMP write_end\n");

    printf("LABEL write_string\n");
    printf("WRITE LF@term\n");
    printf("JUMP write_end\n");

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

void generateSubstring()
{
    printf("LABEL substring\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@str\n");
    printf("DEFVAR LF@i\n");
    printf("DEFVAR LF@j\n");
    printf("DEFVAR LF@result\n");
    printf("DEFVAR LF@temp_char\n");
    printf("DEFVAR LF@length\n");

    printf("POPS LF@j\n");
    printf("POPS LF@i\n");
    printf("POPS LF@str\n");

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

    printf("MOVE LF@result string@\n");
    printf("LABEL substring_loop\n");
    printf("LT LF@cond6 LF@i LF@j\n");
    printf("JUMPIFEQ substring_end LF@cond6 bool@false\n");
    printf("GETCHAR LF@temp_char LF@str LF@i\n");
    printf("CONCAT LF@result LF@result LF@temp_char\n");
    printf("ADD LF@i LF@i int@1\n");
    printf("JUMP substring_loop\n");

    printf("LABEL substring_end\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

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
    printf("DEFVAR LF@s1\n");
    printf("DEFVAR LF@s2\n");
    printf("DEFVAR LF@char_s1\n");
    printf("DEFVAR LF@char_s2\n");
    printf("DEFVAR LF@len1\n");
    printf("DEFVAR LF@len2\n");
    printf("DEFVAR LF@i\n");

    printf("POPS LF@s1\n");
    printf("POPS LF@s2\n");

    printf("STRLEN LF@len1 LF@s1\n");
    printf("STRLEN LF@len2 LF@s2\n");

    printf("PUSHS LF@len1\n");
    printf("PUSHS LF@len2\n");
    printf("LTS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS THEN_GREATER\n");

    printf("PUSHS LF@len1\n");
    printf("PUSHS LF@len2\n");
    printf("GTS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS THEN_LESS\n");
    printf("MOVE LF@i int@0\n");
    printf("MOVE LF@result int@0\n");

    printf("LABEL strcmp_loop\n");
    printf("PUSHS LF@i\n");
    printf("PUSHS LF@len1\n");
    printf("LTS\n");
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS strcmp_end\n");

    printf("PUSHS LF@i\n");
    printf("PUSHS LF@len2\n");
    printf("LTS\n");
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS strcmp_end\n");

    printf("GETCHAR LF@char_s1 LF@s1 LF@i\n");
    printf("GETCHAR LF@char_s2 LF@s2 LF@i\n");
    printf("PUSHS LF@char_s1\n");
    printf("PUSHS LF@char_s2\n");
    printf("JUMPIFNEQS strcmp_char_diff\n");

    printf("ADD LF@i LF@i int@1\n");
    printf("JUMP strcmp_loop\n");

    printf("LABEL strcmp_char_diff\n");
    printf("PUSHS LF@char_s1\n");
    printf("PUSHS LF@char_s2\n");
    printf("LTS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS THEN_GREATER\n");

    printf("PUSHS LF@char_s1\n");
    printf("PUSHS LF@char_s2\n");
    printf("GTS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS THEN_LESS\n");

    printf("LABEL strcmp_end\n");
    printf("PUSHS LF@len1\n");
    printf("PUSHS LF@len2\n");
    printf("LTS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS strcmp_s1_shorter\n");

    printf("PUSHS LF@len1\n");
    printf("PUSHS LF@len2\n");
    printf("GTS\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS strcmp_s2_shorter\n");

    printf("MOVE LF@result int@0\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL strcmp_s1_shorter\n");
    printf("MOVE LF@result int@-1\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL strcmp_s2_shorter\n");
    printf("MOVE LF@result int@1\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL THEN_GREATER\n");
    printf("MOVE LF@result int@1\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL THEN_LESS\n");
    printf("MOVE LF@result int@-1\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateOrd()
{
    printf("LABEL ord\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@result\n");
    printf("DEFVAR LF@len\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("DEFVAR LF@s\n");
    printf("DEFVAR LF@i\n");

    printf("POPS LF@i\n");
    printf("POPS LF@s\n");

    printf("STRLEN LF@len LF@s\n");

    printf("LT LF@tmp_bool int@0 LF@i\n");
    printf("JUMPIFEQ ord_invalid LF@tmp_bool bool@false\n");

    printf("LT LF@tmp_bool LF@i LF@len\n");
    printf("JUMPIFEQ ord_continue LF@tmp_bool bool@true\n");

    printf("LABEL ord_invalid\n");
    printf("MOVE LF@result int@0\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL ord_continue\n");
    printf("STRI2INT LF@result LF@s LF@i\n");
    printf("PUSHS LF@result\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateChr()
{
    printf("LABEL chr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@i\n");
    printf("DEFVAR LF@char\n");
    printf("DEFVAR LF@result\n");
    printf("DEFVAR LF@tmp_bool\n");

    printf("POPS LF@i\n");
    printf("LT LF@tmp_bool LF@i int@0\n");
    printf("JUMPIFEQ chr_invalid LF@tmp_bool bool@true\n");

    printf("LT LF@tmp_bool int@256 LF@i\n");
    printf("JUMPIFEQ chr_continue LF@tmp_bool bool@false\n");

    printf("LABEL chr_invalid\n");
    printf("MOVE LF@result nil@nil\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL chr_continue\n");
    printf("INT2CHAR LF@char LF@i\n");
    printf("MOVE LF@result LF@char\n");
    printf("PUSHS LF@result\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}
