/* 
 * acctdb.h
 * 
 * Defines a structure to represent an account record, with a
 * transaction count and a balance, as well as functions that
 * will be used by both the parent and child processes.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>	// permission bits
#include <fcntl.h>

#include "semaphore.h"

void exit_status( int status );	// defined in exitstat.c

/* database file permissions */
const int PERMS = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

struct acct_record {
	int transaction_count;
	double balance;
};

void fatal(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

void fatalsys(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}


