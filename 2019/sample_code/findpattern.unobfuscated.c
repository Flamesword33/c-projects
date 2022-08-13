/*
 * findpattern.c - A sample program to illustrate the parsing of
 * command-line options.
 *
 * The program accepts a pattern as a command-line argument, then
 * prints each input line (reading from the standard input) that
 * matches the program (like 'grep').  It returns the number of
 * lines printed. (Basically this program does the same thing as
 * the Unix system utility 'grep', except for the return value,
 * which could be determined by calling 'grep' with the '-c' flag.
 * However, the point of this program is to illustrate how to parse
 * command-line options and flags so, unlike you, it doesn't have to
 * do anything meaningful in order to justify its existence.)
 *
 * If option -x (for "except") is specified, it prints each line
 * that doesn't match the pattern (like 'grep -v').
 *
 * If option -n (for "number") is specified, it precedes each
 * output line by its line number (like 'grep -n').
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
 * instead of '#define', and to make it easier to read and understand.
 * Kernighan and Ritchie write C in a very compact style, such as
 *    while ( --argc > 0 && (*++argv)[0] == '-' )
 * or
 *    while ( --lim > 0 && (c = getchar()) != EOF && c != '\n' )
 *       s[i++] = c;
 * I have broken these compact expressions into separate parts.
 * I also direct the error output to stderr.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const int MAXLINE = 1000;

/* Forward declarations */
int getline(char *, int);
void usage_exit(char *);

/* find: print lines that match pattern from 1st arg */
int main(int argc, char *argv[])
{
	char *progname = argv[0];
	char *pattern = NULL;
	char line[MAXLINE];
	long lineno = 0;
	int c;
	unsigned int found = 0;
	bool except = false;
	bool number = false;

	/*  We've kept a pointer to the program name, so decrement the count of 
	 *  arguments and make 'argv' point to the next argument to be processed.
	 */
	argc--;
	argv++;

	/*  Process the optional flag arguments ('-x', '-n', or '-xn').
	 *  Check that there is another argument to process, and then
	 *  check if it's a flag argument (i.e., its first character is '-').
	 */
	/*  These four expressions are identical in effect.
	 *  Which is clearest?
	 *
	 *     (*argv)[0]
	 *     *argv[0]
	 *     argv[0][0]
	 *     **argv
	 *
	 *  Note that they are only equivalent because 'argv' is a pointer
	 *  to an array of pointers to strings, so *argv and argv[0] are
	 *  the same -- they both return the first string in the array of
	 *  strings (which are pointers to arrays of characters).
	 */
	while (argc > 0 && (*argv)[0] == '-')
	{
		char *argstr = argv[0];	/* Get the current argument. */
		argstr++;				/* Point to the character after the
								   leading '-' of the option arg. */
		/* Get the next character. If it's the null character ('\0'), 
		 * the expression will be interpreted as false.
		 */
		while (c = *argstr)
		{
			switch (c)
			{
				case 'x':
					except = true;
					break;
				case 'n':
					number = true;
					break;
				default:
					fprintf(stderr, "%s: illegal option %c\n", progname, c);
					usage_exit(progname);
			}
			argstr++;			/* Advance to the next character. */
		}

		/* Advance to the next command-line argument. */
		argc--;
		argv++;
	}

	/* Because we have decremented the argument counter for each
	 * option argument processed, we should have only one argument
	 * left (a pattern string).
	 */
	if (argc != 1)
		usage_exit(progname);

	/* We have stepped the 'argv' pointer along the array of
	 * command-line arguments until it points to the user-specified
	 * pattern.  Let's dereference the 'argv' pointer now and give 
	 * the string it points to a meaningful name.
	 */
	pattern = *argv;

	while (getline(line, MAXLINE) > 0)
	{
		lineno++;

		/* If the pattern is a substring of the current line,
		 * then we want to print the line unless the 'except'
		 * flag is set (and vice versa).
		 */
		if ((strstr(line, pattern) != NULL) != except)
		{
			if (number)
				printf("%ld: ", lineno);
			printf("%s", line);
			found++;
		}
	}

	exit(found);
}


/* getline: get line into s, return length 
 * (Kernighan and Ritchie, p. 69)
 *
 * This is essentially equivalent to the Standard I/O Library's
 * 'fgets' function, but it returns an integer indicating the
 * number of characters read.
 */
int getline(char s[], int lim)
{
	int c, i;

	i = 0;
	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		s[i++] = c;

	/* I have added this check to see if getchar() returned EOF 
	 * due to an error (rather than reaching end-of-file).
	 */
	if (c == EOF && ferror(stdin))
	{
		perror("getchar");
		exit(EXIT_FAILURE);
	}

	if (c == '\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}

void usage_exit(char *progname)
{
	fprintf(stderr, "Usage: %s [-x] [-n] <pattern>\n", progname);
	exit(EXIT_FAILURE);
}
