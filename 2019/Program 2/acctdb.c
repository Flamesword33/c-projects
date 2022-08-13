/* acctdb.c
 * by Nathan Pelletier
 * published: March 15, 2019
 */

/* This is a parent process that must
 *   use UNIX control system calls
 *		(fork, exec, wait, getpid, exit)
 *   use file I/O system calls
 *		(open, close, lseek, read, write)
 *   create multiple child processes
 *		one for each .txt file present
 *	 preforms one last read of binary file to
 *		print everyones id and total
 *		calculate and print total number of transactions
 *
 *	 It should take a binary file(serves as the data base)
 *   It should take additional files as transaction.txt
 *
 *   If no filename is provided then the program should terminate 
 *		with an error message and error condition
 *	 If the file is corrupted or cannot be created then
 *		an error message is written to the standard error stream
 *		the program should terminate without output
 *
 *	 WARNING: DO NOT USE O_TRUNC 
 *		DO NOT INITIALIZE EVERY BALLANCE TO 0
 *
 * COULD NOT GET execlp WORKING IN TIME. DO NOT KNOW IF acctdb_child works.
 * 
 */
 
 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define ERROR -1
#define CHILD 0


typedef struct Account{ 
	int transactions;
	double ballance;
} account;

char* second_program_path = "/home/npelleti/acctdb_child";


// COPIED FROM JOHNATHAN MHOR FROM ECLASS 2019//
/*  Produce an error message specific to a system or library function
 *  and exit.
 */
void fatal_system_error( const char* msg ){
    /*  The Standard I/O function 'perror' produces a message on the
     *  standard error output consisting of the message provided
     *  as an argument, followed by a colon and a space, a message
     *  describing the last error encountered during a call to a
     *  system or library function, and a newline.
     */
    perror( msg );
    exit( EXIT_FAILURE );
}//fatal_system_error


/* child_processes_left(int *child_ids)
 *
 * looks through an array for process id's and waits for them to finish
 */
void child_processes_left(int *child_ids){
	while (*child_ids != NULL){
		wait(*child_ids);
		child_ids++;
	}//while child processes are left
}//child_processes_left


/* start_children(int argc, char* argv[])
 *
 * starts up child processes, catalogs them in an array 
 * and waits for them to close
 */
void start_children(int argc, char* argv[]){
	int i;
	int pid;
	int error_detector;
	int child_ids[argc];
	const char* fd;
	
	for(i = 1; i <= argc; i++){
		pid = fork();
		switch(pid){
			case ERROR :
				fatal_system_error("Failed to fork correctly");
			case CHILD :
				printf("a child is created");
				fd = open(second_program_path, O_RDWR | O_CREAT);
				error_detector = execlp(fd, "acctdb_child", 
                        argv[1], argv[i], NULL);
				if(error_detector == ERROR){
					fatal_system_error("Failed to run execlp");
				}
			default : //parent
				printf("The parent runs free");
				child_ids[i - 1] = pid; //starts array at 0
		}//switch	
				
	}//for to start each child
	//(one per file present past the database)
	
	child_processes_left(child_ids);
	
}//start_children


/* print_binary_file(int file)
 * 
 * prints out the database in format 
 * account number | transactions | ballance
 */
void print_binary_file(int file) {
	account temp_account;
	int file_detector = 1;
	int counter = 0;
	int total_transactions = 0;
	double total_ballance = 0;
	lseek(file, 0, SEEK_SET);
	
	printf("   Summary of Accounts\n");
	
	while(file_detector != EOF){
		counter++;
		file_detector = read(file, &temp_account, sizeof(struct Account));
		if(file_detector > 0){
			printf("%d %d %f", counter, temp_account.transactions, 
                    temp_account.ballance);
			printf("\n");
			total_transactions = total_transactions + 
                    temp_account.transactions;
			total_ballance = total_ballance + temp_account.ballance;
		}//if the line isn't empty
	}//while the read command is not at the end of file
	
	printf("      ----  --------- \n");
	printf("        %d  %f \n", total_transactions, total_ballance);
}//print_binary_file


/* main(int argc, char* argv[])
 *
 * checks the binary file, starts the children and
 * prints out the child output
 *
 * uses method(s):
 *   check_binary_file(int file)
 *	 start_children(int argc, char* argv[])
 *	 print_binary_file(int file)
 */
int main(int argc, char* argv[]){
	int database;
	
	if(argv[1] == NULL){
		database = open(argv[1], O_CREAT, O_RDONLY);
	}//if file doesn't exist
	else {
		database = open(argv[1], O_RDONLY);
	}//else file exists
	
	start_children(argc, argv);
	
	print_binary_file(database);
	
	return EXIT_SUCCESS;
}//main




