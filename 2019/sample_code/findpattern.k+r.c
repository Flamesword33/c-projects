/*
 * find.c - A sample program to illustrate the parsing of
 * command-line options.
 *
 * The program accepts a pattern as a command-line argument, then
 * prints each input line (reading from the standard input) that
 * matches the program (like 'grep').
 *
 * If option -x (for "except") is specified, it prints each line
 * that doesn't match the pattern.
 *
 * If option -n (for "number") is specified, it precedes each
 * output line by its line number.
 *
 * Optional arguments are permitted in any order and can be combined,
 * as in:
 *    find -nx <pattern>
 *
 * From "The C Programming Language," 2e, by Brian Kernighan and
 * Dennis Ritchie (Prentice Hall, 1988), p. 117.
 *
 * Modified by J. Mohr to modernize the code a little, including
 * through the use of boolean variables and by using 'const'
 * instead of '#define'.  I also direct the error output to
 * stderr exit with EXIT_SUCCESS or EXIT_FAILURE.  (The original
 * program returned the number of matching lines found or -1).
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const int MAXLINE = 1000;

/* Forward declaration */
int getline( char *line, int max );

/* find: print lines that match pattern from 1st arg */
int main( int argc, char *argv[] )
{
   char * progname = argv[0];
   char line[ MAXLINE ];
   long lineno = 0;
   int c;
   bool except = false;
   bool number = false;
   bool found = false;

   while ( --argc > 0 && (*++argv)[0] == '-' )
      while ( c = *++argv[0] )
         switch (c)
         {
         case 'x':
            except = true;
            break;
         case 'n':
            number = true;
            break;
         default:
            fprintf( stderr, "%s: illegal option %c\n",
               progname, c );
            exit( EXIT_FAILURE );
         }

   if ( argc != 1 )
   {
      fprintf( stderr, "Usage: %s [-x] [-n] <pattern>\n",
         progname );
      exit( EXIT_FAILURE );
   }
   else
      while ( getline( line, MAXLINE ) > 0 )
      {
         lineno++;
         if ( (strstr( line, *argv ) != NULL ) != except )
         {
            if ( number )
               printf("%ld: ", lineno );
            printf("%s", line );
            found = true;
         }
      }
   if ( found )
      exit( EXIT_SUCCESS );
   exit( EXIT_FAILURE );
}


/* getline: get line into s, return length */
/* Kernighan and Ritchie, p. 69. */
int getline( char s[], int lim )
{
   int c, i;

   i = 0;
   while ( --lim > 0 && (c = getchar()) != EOF && c != '\n' )
      s[i++] = c;
   if (c == '\n')
      s[i++] = c;
   s[i] = '\0';
   return i;
}

