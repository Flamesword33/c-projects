/*
 * acctdb 
 * 
 * Create child processes to read sequences of account transactions (deposits
 * and withdrawals) from a text file on the standard input and update a binary
 * data file (the accounts database) accordingly.
 * 
 * Each transaction consists of an account number and a transaction amount
 * (a positive or negative real number).  Account numbers are non-negative integers.
 * The accounts database consists of a set of records, indexed by account number,
 * consisting of a count of the number of transactions performed and the balance.
 * 
 * After all transactions have been processed, the parent process prints a
 * summary of transaction counts and account balances for all accounts with
 * a non-zero transaction count.
 * 
 * The name of the binary data file is given as a command-line argument.
 * If no other arguments are provided, the main process prints a summary of
 * the current account balances.  If the names of transaction files are provided
 * as additional command-line arguments, the parent process spawns a child process
 * for each transaction file.
 *
 *  J. Mohr
 *  2005.02.27; rev. 2019-01-21
 */

#include <sys/wait.h>

#include "acctdb.h"

int main(int argc, char *argv[])
{
	int db_fd;						/* file descriptor */
	int flags;
	pid_t pid;
	int retval;						/* system call return value */
	int status;
    int semid;						/* a semaphore descriptor (ID) */
    char *semid_str;				/* semaphore ID as a string */
    unsigned semid_str_len;
	int child_processes = 0;
	struct acct_record acct_data;	/* account data record */
	int record_size = sizeof (struct acct_record);
	int acct_num = 0;
	int total_transactions = 0;		/* sum of transaction counts in the database */
	double total_balance = 0.0;		/* sum of account balances in the database */

	/* Check that a filename was given as an argument. */
	if (argc < 2)
	{
		fprintf(stderr, "%s: a file name must be provided as a"
				" command-line argument.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Try to open the specified data file; create it if it doesn't exist. */
	if ((db_fd = open(argv[1], O_RDWR | O_CREAT, PERMS)) == -1)
		fatalsys("Unable to open database file");

	/* Set close-on-exec flag for database file descriptor so that child
	 * processes won't have the file open twice.  (Each child needs to open
	 * the database file separately so it has its own file pointer.)
	 * 
	 * Students have not be told how to do this, and it's not covered in
	 * "Beginning Linux Programming".
	 * 
	 * This code is from Stevens, APUE, p. 376, with modifications.
	 */
	if ((flags = fcntl(db_fd, F_GETFD, 0)) < 0)
		fatalsys("fcntl F_GETFD error");
	flags |= FD_CLOEXEC;
	if (fcntl(db_fd, F_SETFD, flags) < 0)
		fatalsys("fcntl F_SETFD error");
	
	/* Create a semaphore and initialize it to 1. */
	semid = createsem( 1 );
	
	/* Convert the semaphore ID to a string to pass it to child processes. */
	semid_str_len =  snprintf(NULL, 0, "%d", semid);
	if ((semid_str = malloc(semid_str_len + 1)) == NULL)
		fatalsys("malloc failed");
	snprintf(semid_str, semid_str_len + 1, "%d", semid);
	
	/* Create one child process for each transaction file named on the
	 * command line.
	 */
	for (int i = 2; i < argc; i++ ) {
		if ((pid = fork()) < 0)
			fatalsys("fork error");
		else if (pid == 0)	// child process
		{
			/* Child announces its birth. */
			printf("Child %d: %d\n", i-1, getpid());
			fflush(stdout);
	
			/* Have the child execute the transaction processing program. */
			if ((execlp("transaction_processor", "transaction_processor",
					argv[1], argv[i], semid_str, (void *) 0)) < 0)
				fatalsys("exec error");
		}
		// else parent process continues here
		child_processes++;
	}
	
	/* Wait for child processes to finish and display exit status. */
	for (int i = 0; i < child_processes; i++)
	{
		if ((pid = wait(&status)) < 0)
			fatalsys("wait error");
		
		printf("Child process %d: ", pid);
		exit_status(status);
	}
	if (child_processes > 0)
		printf("\n\n");

	deletesem(semid);
	
	/* Display the results, with a total of the balances in the file. */
	
	/* Not necessary to seek to start of file if child processes have
	   each opened the database file independently.
	*//*
	if (lseek(db_fd, 0, SEEK_SET) == -1)
		fatalsys("Seek to start of file failed");
	*/

	// Read account records from database file until EOF reached.
	printf("   Summary of Accounts\n");
 	while ((retval = read(db_fd, &acct_data, record_size)) != 0)
	{
		if (retval == -1)
			fatalsys("Read failed");
		else if (acct_data.transaction_count != 0)
		{
			printf("%5d  %5d  %10.2f\n", acct_num, acct_data.transaction_count,
				   acct_data.balance);
			total_transactions += acct_data.transaction_count;
			total_balance += acct_data.balance;
		}

		acct_num++;
	}

	printf("        ----   ---------\n");
	printf("       %5d  %10.2f\n", total_transactions, total_balance);

	exit(EXIT_SUCCESS);
}
