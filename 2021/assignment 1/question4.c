/* question4.c
 * by Nathan Pelletier
 * Febuary 19, 2021
 *
 * Creates a single process (fork())
 * Then the second program sends a execute command to 
 *   /bin/ls
 * 
 * Output:
 * [npelleti@localhost assignment1]$ gcc question4.c
 * [npelleti@localhost assignment1]$ ./a.out
 * a.out  input.txt  question3.c  question4.c  question5.c  sacrifice.txt
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> 
#include <unistd.h> 

#define PARENT 0

int execute_other_program(int is_parent){
	if(is_parent != PARENT){
		//child code
		execl("/bin/ls", "/root", NULL);
	}//if child process
	
	return 0;
}//execute_other_program

int force_parent_to_wait(int is_parent){
	if(is_parent == PARENT){
		wait(NULL);
	}//if parent 
}//force_parent_to_wait
 
int main(int argc, char* argv[]){
	int pid_1 = 0;
	
	pid_1 = fork();
	execute_other_program(pid_1);
	force_parent_to_wait(pid_1);
	
	return 0;
}//main
