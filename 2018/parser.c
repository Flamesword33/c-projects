/* Lexical scanner for simple arithmetic expressions:

     <expr> ::= <term> {(+ | -) <term>}
     <term> ::= <factor> {(* | /) <factor>}
     <factor> ::= id | int_constant | ( <expr> )

   From Robert W. Sebesta, Concepts of Programming
   Languages, 10e, Chapter 4.

   Works as a front-end for a recursive-descent
   parser from the same chapter.
*/

#include <stdio.h>
#include <ctype.h>

/* global declarations */

/* Define SCANNER_ONLY as 1 if you want to test the
   scanner (lexical analyzer) independently of the parser. */
#define SCANNER_ONLY 0

#include "parser.h"

/* variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int nextToken;
FILE *in_fp;

char* tokenString[] = TOKEN_STRING;

/****************************************************/
#if SCANNER_ONLY
/* Define main() only if you want to test the scanner
   independently of the parser.
 ALLOWS FOR TWO MAINS   */

/* main driver */
int main() {

  /* open the input data file and process contents */
  if ((in_fp = fopen("parser.in","r")) == NULL)
    fprintf(stderr, "ERROR - cannot open 'front.in'\n");
  else {
    getChar();
    do {
      lex();
    } while (nextToken != EOF_TOKEN);
  }
  return 0;
}

#endif

/****************************************************/
/* lookup - a function to lookup operators and parentheses
            and return the token */
int lookup(char ch){
  switch(ch){
      case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;

      case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;

      case '+':
        addChar();
        nextToken = ADD_OP;
        break;

      case '-':
        addChar();
        nextToken = SUB_OP;
        break;

      case '*':
        addChar();
        nextToken = MULT_OP;
        break;

      case '/':
        addChar();
        nextToken = DIV_OP;
        break;

      default:
        addChar();
        nextToken = EOF;
        break;
  }
  return nextToken;
}

/****************************************************/
/* addChar - a function to add next char to lexeme */
void addChar(){
  if (lexLen <= 98){
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  } 
  else{
    fprintf(stderr, "Error - lexeme too long \n");
  }
}

/****************************************************/
/* getChar - a function get the next character of
             input and determine its character class */
void getChar(){
  if ((nextChar = getc(in_fp)) != EOF){
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else charClass = UNKNOWN;
  } 
  else charClass = EOF;
}

/****************************************************/
/* getNonBlank - a function to call getChar until it
                 returns a non-whitespace character */
void getNonBlank(){
  while (isspace(nextChar))
    getChar();
}

/****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex(){
  lexLen = 0;
  getNonBlank();
  switch (charClass){
    case LETTER:
      /* parse identifiers */
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT){
        addChar();
        getChar();
      }
      nextToken = IDENT;
      break;
    case DIGIT:
      /* parse integer literals */
      addChar();
      getChar();
      while (charClass == DIGIT){
        addChar();
        getChar();
      }
      nextToken = INT_LIT;
      break;
    case UNKNOWN:
      /* parenthese and operators */
      lookup(nextChar);
      getChar();
      break;
    case EOF:
      /* EOF */
      nextToken = EOF_TOKEN;
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
      break;
  } /* end of switch */
  printf("Token is: %s, Lexeme is %s\n",
	 tokenString[nextToken], lexeme);
  return nextToken;
} /* end lex */
