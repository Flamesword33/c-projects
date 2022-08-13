/* acctdb_child.c
 * by Nathan Pelletier
 * published: March 15, 2019
 */

/* This is a child process that must
 *   write binary data to a file
 *
 *   process:
 *		transaction files from a .txt file
 *		a line as a transaction
 *
 *   keep track of:
 *		the number of transactions processed 
 *
 *   update:
 *		a simple database 
 *		account balances as doubles 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define ERROR -1

typedef struct Account{ 
	int transactions;
	double ballance;
} account;

// COPIED FROM JOHNATHAN MHOR FROM ECLASS 2019//
/*  Print an error message relevant to a user error and exit.
 *  (Use 'fatalsys' instead if the error occurred in a system
 *  or library function.)
 */
void fatal( const char* msg, int file, int lock_number ){
    fprintf( stderr, msg );
    exit( EXIT_FAILURE );
}

/* acquire_lock(int file, int lock_number)
 * 
 * creates a lock on a place in the file
 */
void acquire_lock(int file, int lock_number){
	lseek(file, (lock_number) * (sizeof(account)), SEEK_SET);
	lockf(file, F_LOCK, sizeof(struct Account));
}//acquire_lock

/* release_lock(int file, int lock_number)
 *
 * releases the lock on the file at a given point
 */
void release_lock(int file, int lock_number){
	lseek(file, (lock_number) * (sizeof(account)), SEEK_SET);
	lockf(file, F_ULOCK, sizeof(struct Account));
}//release_lock


/* update_account_ballance(int account_num, double database_ballance, 
 *                         int database)
 * 
 * acquires a lock on a given account number and proceeds to read its contents
 * The program then adds the results from the transaction to the database 
 * and updates the database
 * The program then releases the lock
 *
 * uses method(s):
 *   acquire lock(int account_number)
 *	 release_lock(int account_number)
 */
void update_account_ballance(int account_num, double db_ballance, 
        int database){
	int error_check;
	account temp_account;
	int did_it_read;
	
	acquire_lock(database, account_num);
	
	error_check = lseek(database, (account_num) * (sizeof(account)), 
            SEEK_SET);
	if(error_check == ERROR){
		fatal("Failed first lseek", database, account_num);
	}//if couldn't seek 
	did_it_read = read(database, &temp_account, sizeof(struct Account));
	
	if(did_it_read == ERROR) {
		fatal("Failed to read database.", database, account_num);
	}//if read an error
	
	else if(did_it_read){
		temp_account.ballance = db_ballance + temp_account.ballance;
		temp_account.transactions++;
	}//if it read correctly
	
	else{
		temp_account.ballance = db_ballance;
		temp_account.transactions = 1;
	}//else the file contains nothing there
	
	error_check = lseek(database, (account_num) * (sizeof(account)),
            SEEK_SET);
	if(error_check == ERROR){
		fatal("Failed second seek", database, account_num);
	}//if couldn't seek (hint at lack of paralleleism
	
	error_check = write(database, &temp_account, sizeof(struct Account));
	if(error_check == ERROR){
		fatal("Failed to write to database", database, account_num);
	}//if error with write
	
	release_lock(database, account_num);
}//update_account_ballance


/* main(int argc, char* argv[])
 * 
 * argv[1] must be a binary database in format int double
 * argv[2] must be a .txt file containing int double format
 *
 * Main opens said files and begins scanning the .txt file for
 * account numbers and balances
 * 
 * uses method(s):
 * 	 void update_account_ballance(int, double, int)
 */
int main(int argc, char* argv[]){
	FILE *child_file;
	int database;
	int account_num;
	double db_ballance;
	printf("I can't believe we made it here");
	
	child_file = fopen(argv[2], "r");
	database = open(argv[1], O_RDWR);
	
	while (fscanf(child_file, "%d", "%f", &account_num, &db_ballance)){
		update_account_ballance(account_num, db_ballance, database);
	}//while
	
	exit( EXIT_SUCCESS );
}//main