/*
 * transaction_processor 
 * 
 * Each child process reads sequences of account transactions (deposits and
 * withdrawals) from a text file on the standard input and updates a binary
 * data file (the accounts database) accordingly.
 * 
 * Each transaction consists of an account number and a transaction amount
 * (a positive or negative real number).  Account numbers are non-negative integers.
 * The accounts database consists of a set of records, indexed by account number,
 * consisting of a count of the number of transactions performed and the balance.
 * 
 * The names of the binary data file and the transaction file are given as 
 * command-line arguments.
 *
 *  J. Mohr
 *  2019-01-21
 */

#include "acctdb.h"

/* Loop count for delay loop to demonstrate non-atomic file access. */
const int DELAY_COUNT = 1000000;
//const int DELAY_COUNT = 0;

int main(int argc, char *argv[])
{
	int db_fd;					// file descriptor of binary database file
	FILE *fp;					// transaction file pointer
	int retval;					// system call return value
	unsigned acct_num;			// account number
	double amount;				// a transaction amount
	struct acct_record acct_data;	// account data record
	unsigned record_size = sizeof (struct acct_record);
	int delay;

	if (argc < 3)
	{
		fprintf(stderr,
				"%s: Unexpected number of command-line arguments (%d).\n",
				argv[0], argc);
		exit(EXIT_FAILURE);
	}

	/* Try to open the specified data file; create it if it doesn't exist. */
	if ((db_fd = open(argv[1], O_RDWR | O_CREAT, PERMS)) == -1)
		fatalsys("Unable to open database file");

	/* Open the transaction file for reading. */
	if ((fp = fopen(argv[2], "r")) == NULL)
		fatalsys("Unable to open transaction file");

	/* Repeatedly read an account number and transaction amount
	   from the standard input and update the data file accordingly. */
	while (fscanf(fp, "%u", &acct_num) != EOF)
	{
		if (fscanf(fp, "%lf", &amount) == EOF)
			fatal("Unexpected EOF when reading transaction amount.");

		if (lseek(db_fd, acct_num * record_size, SEEK_SET) == -1)
			fatalsys("Seek failed");

		if ((retval = read(db_fd, &acct_data, record_size)) == -1)
			fatalsys("Read failed");
		else if (retval == 0)
		{
			acct_data.transaction_count = 1;
			acct_data.balance = amount;
		}
		else
		{
			acct_data.transaction_count++;
			acct_data.balance += amount;
		}

		/* DELAY LOOP TO DEMONSTRATE NON-ATOMICITY OF FILE ACCESS */
		for (delay = 0; delay < DELAY_COUNT; delay++)
			;

		/* The read advanced the file pointer, so seek again. */
		if (lseek(db_fd, acct_num * record_size, SEEK_SET) == -1)
			fatalsys("Seek failed");

		if (write(db_fd, &acct_data, record_size) != record_size)
			fatalsys("Write failed");
	}

	exit(EXIT_SUCCESS);
}
