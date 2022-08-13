/* 
 *  cpfile.c 
 *
 *  From the Wikibooks book "A Little C Primer" (author unknown)
 *  http://en.wikibooks.org/wiki/A_Little_C_Primer/C_Command_Line_Arguments
 *
 *  Modified by J. Mohr to use EXIT_SUCCESS and EXIT_FAILURE.
 */

#include <stdio.h>
#include <stdlib.h>

// The old way to define a constant (using the preprocessor):
// #define MAX 256
//
// The new way:
const int MAX = 256;

//
//  Forward declarations.
//
//  If there are many of these, they are typically put in a
//  user-defined header file, and the header file would be
//  included above.
//
void fatal( const char * );
void fatalsys( const char * );
void copyfile( FILE *, FILE * );


int main( unsigned int argc, unsigned char *argv[] )
{
    FILE *src, *dst;

    /* Try to open source and destination files. */
    switch (argc)
    {
	case 1:          /* No parameters, use stdin-stdout. */
	    src = stdin;
	    dst = stdout;
	    break;

	case 2:          /* One parameter -- use input file & stdout. */
	    if ( ( src = fopen( argv[1], "r" )) == NULL )
	    {
		/* The original reported errors this way, but they
		 * should be directed to stderr, not to stdout.
		 * Error-reporting is simplified if we factor out
		 * these lines to a function -- see 'fatalsys' below.
		 */
		// puts( "Can't open input file.\n" );
		// exit( EXIT_FAILURE );
		fatalsys("Can't open input file");
	    }
	    dst = stdout;
	    break;

	case 3:         /* Two parameters -- use input and output files. */
	    if ( ( src = fopen( argv[1], "r" )) == NULL )
		fatalsys("Can't open input file");
	    if ( ( dst = fopen( argv[2], "w" )) == NULL )
		fatalsys("Can't open output file");
	    break;

	default:        /* Too many parameters. */
	    fatal( "Wrong parameters.\n" );

    }

    /* Copy a file from src to dst. */
    copyfile( src, dst );
   
    /* All done, close up shop. */
    fclose( src );
    fclose( dst );
    exit( EXIT_SUCCESS );
}

/* 
 * Copy one file to the next. 
 */
void copyfile( FILE * source, FILE * dest )
{
/* This is the code from the original source (cited in the
 * program header), except that I moved it out of 'main'
 * into a separate function.
 *
 ****************  WARNING  ***********************
 *
 * This code has two major flaws:
 *
 * 1. Although nothing in the documentation says so,
 *    THIS ROUTINE COPIES ONLY TEXT FILES.  It uses
 *    'fgets' and 'fputs', which work only on "strings"
 *    (null-terminated sequences of characters).
 *    Since most binary files have many zero bytes,
 *    'fgets' will fill the buffer with characters
 *    (bytes), many of which are null, but 'fputs'
 *    will only write out the characters in the 
 *    buffer up to the first null character; it
 *    does not output that null character or
 *    anything in the buffer after it. Typically,
 *    most of a binary file will NOT be copied.
 *    (To be fair to the original author, the purpose
 *    of this program was to illustrate command-line
 *    argument handling, not how to copy files, but
 *    a comment indicating that it works correctly
 *    only on text files would have been prudent.
 *
 * 2. This routine does no error checking. The 'fgets'
 *    function returns NULL in two cases: a) when an
 *    end-of-file occurs when no characters have been
 *    read and b) if an error occurs while reading.
 *    The return value of the 'fputs' function is not
 *    even checked -- it returns EOF on error.
 *
 * If you want a program that copies only text files,
 * this will work most of the time, but it is an
 * important principle that any system program --
 * and copying files is clearly a system task --
 * should check every function return value so it
 * can detect errors.
 */
/*
 *  HOW NOT TO DO IT:
 *
    char buf[MAX];
    while( ( fgets( buf, MAX, source ) ) != NULL )
    {
        fputs( buf, dest );
    }
*/

/*
 *  HOW TO COPY TEXT FILES WITH ERROR-CHECKING:
 *
    char buf[MAX];
    while( ( fgets( buf, MAX, source ) ) != NULL )
    {
        if ( fputs( buf, dest ) == EOF )
	    fatalsys( "fputs" );
    }
    // Check if 'fgets' returned NULL due to an error.
    if ( ferror( source ))
	fatalsys( "fgets" );
    // If not, just return, because we reached end-of-file.
 */

/*
 *  HOW TO COPY TEXT OR BINARY FILES
 *
 *  'getc' and 'putc' are implemented in the Standard I/O Library
 *  in a way that makes them more efficient than they appear.
 *  Although it looks like we are reading and writing our way 
 *  through the input and output files character-by character,
 *  they actually read from and write to buffers maintained by
 *  the 'stdio' library, and physical reads and writes are done
 *  in standard-sized chunks as the buffers are emptied or filled.
 *  
 *  'getc' and 'putc' may be implemented as macros that are expanded
 *  inline, so they should not be called with arguments that may have
 *  side effects.
 */
    int c;			/* Declare as int, not char
 				 * so EOF can be detected. */
    while ( ( c = getc( source )) != EOF )
	if ( putc( c, dest ) == EOF )
	    fatalsys( "putc" );

    // Check if 'getc' returned EOF due to an error.
    if ( ferror( source ))
	fatalsys( "getc" );
    // If not, just return, because we reached end-of-file.
}

/*  Print an error message relevant to a user error and exit.
 *  (Use 'fatalsys' instead if the error occurred in a system
 *  or library function.)
 */
void fatal( const char* msg )
{
    fprintf( stderr, msg );
    exit( EXIT_FAILURE );
}

/*  Produce an error message specific to a system or library function
 *  and exit.
 */
void fatalsys( const char* msg )
{
    /*  The Standard I/O function 'perror' produces a message on the 
     *  standard error output consisting of the message provided 
     *  as an argument, followed by a colon and a space, a message
     *  describing the last error encountered during a call to a 
     *  system or library function, and a newline.
     */
    perror( msg );
    exit( EXIT_FAILURE );
}
