/*
 *  A sample program to show how to check the number of command-line
 *  arguments and to open a file assuming that the (only) command-line
 *  argument is a filename.
 *
 *  From an online C programming tutorial (author unknown):
 *  http://www.cprogramming.com/tutorial/c/lesson14.html
 *
 *  Modified by J. Mohr to print error output to stderr
 *  and exit with EXIT_FAILURE or EXIT_SUCCESS.
 */

#include <stdlib.h>
#include <stdio.h>

int main ( int argc, char *argv[] )
{
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename", argv[0] );
    }
    else 
    {
        // We assume argv[1] is a filename to open
        FILE *file = fopen( argv[1], "r" );

        /* fopen returns 0, the NULL pointer, on failure */
        if ( file == 0 )
        {
            fprintf( stderr, "Could not open file\n" );
            exit( EXIT_FAILURE );
        }
        else 
        {
            int x;
            /* read one character at a time from file, stopping at EOF, which
               indicates the end of the file.  Note that the idiom of "assign
               to a variable, check the value" used below works because
               the assignment statement evaluates to the value assigned. */
            while  ( ( x = fgetc( file ) ) != EOF )
            {
                printf( "%c", x );
            }
            fclose( file );
        }
    }
    exit( EXIT_SUCCESS );
}
