/* question3.c
 * by Nathan Pelletier
 * Febuary 18 2021
 *
 * Reads input.txt from same folder
 * Creates four children processes
 * Each child reads a line from the file and prints itoa
 * Each child prints its process id
 * Each child then terminates 
 * 
 * OUTPUT:
 * [npelleti@localhost assignment1]$ gcc question3.c
 * [npelleti@localhost assignment1]$ ./a.out
 * Child 1 reads this line
 * Procces 1 ID: 3287 
 * Child 2 reads this line
 * Procces 2 ID: 3288 
 * Child 3 reads this line
 * [npelleti@localhost assignment1]$ Procces 3 ID: 3289 
 * Child 4 reads this lineProcces 4 ID: 3290 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>   //added cause the tutorial site had it
#include <unistd.h>    //and im not sure which library is needed for fork() and wait()

#define PARENT 0

int force_parent_to_wait(int is_parent){
	if(is_parent == PARENT){
		wait(NULL);
	}
	return 0;
}//force_parent_to_wait

int give_children_line(char *line_1, char *line_2, char *line_3, char *line_4, 
		int pid_1, int pid_2, int pid_3, int pid_4){
			
	if(pid_1 > 0){
		printf(line_1);
		printf("Procces 1 ID: %i \n", pid_1);
	}//if process 1
	if(pid_2 > 0){
		printf(line_2);
		printf("Procces 2 ID: %i \n", pid_2);
	}//if process 2
	if(pid_3 > 0){
		printf(line_3);
		printf("Procces 3 ID: %i \n", pid_3);
	}//if process 3
	if(pid_4 > 0){
		printf(line_4);
		printf("Procces 4 ID: %i \n", pid_4);
	}//if process 4
	
	return 0;
}//give_children_line

int create_children(char *line_1, char *line_2, char *line_3, char *line_4){
	int pid_1 = -1; //set to -1 to id child process even if not process 1
	int pid_2 = -1; //that whay i can pass 1 pid instead of 4
	int pid_3 = -1;
	int pid_4 = -1;
	
	
	//create four children here//
	pid_1 = fork();
	
	if(pid_1 == 0){
		//this structure is to prevent child processes from creating child processes
		//could also do fork(); fork(); and rememeber combined pid but i feel this is more 
		//readable
		pid_2 = fork(); 
	}
	if(pid_2 == 0){
		pid_3 = fork(); 
	}
	if(pid_3 == 0){
		pid_4 = fork();
	}
	//create four children here//
	
	give_children_line(line_1, line_2, line_3, line_4, pid_1, pid_2, pid_3, pid_4);
	force_parent_to_wait(pid_1);
	return 0;
}//create_children

FILE* get_input(){
	FILE *in_file = fopen("./input.txt", "r");
	return in_file;
}//get_input

int main(int argc, char *argv[]){
	char *line_1 = NULL;
	char *line_2 = NULL;
	char *line_3 = NULL;
	char *line_4 = NULL;
    size_t len = 0;
	
	FILE *input = get_input();
	
	getline(&line_1, &len, input);
	getline(&line_2, &len, input);
	getline(&line_3, &len, input);
	getline(&line_4, &len, input);
	
	fclose(input);
	
	create_children(line_1, line_2, line_3, line_4);
	return 0;
}//main
