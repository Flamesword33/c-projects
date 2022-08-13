/* parser.h
   Header file for lexical scanner in front.c
*/

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define EOF_TOKEN 0
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

//To print tokens by name instead of number
#define TOKEN_STRING {"EOF_TOKEN","","","","","","","","","","INT_LIT",\
                      "IDENT","","","","","","","","","ASSIGN_OP","ADD_OP",\
                      "SUB_OP","MULT_OP","DIV_OP","LEFT_PAREN","RIGHT_PAREN"}

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();