/* question5.c
 * by Nathan Pelltier
 * Febuary 19, 2021
 *
 * Program opens sacrifice.txt in parent directory
 * program creates 1 child process
 * Both processes then attempt to write to same file
 * 
 * Output:
 * Let's see what this does?!
 * Let's see what this does?!
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
FILE* open_file(){
	FILE *out_file = fopen("./sacrifice.txt", "w");
	return out_file;
}//open_file

int write_to_file(FILE* sacrifice){
	fprintf(sacrifice, "Let's see what this does?!\n");
	return 0;
}//write_to_file

int main(int argc, char* argv){
	FILE *sacrifice = open_file();
	fork();
	write_to_file(sacrifice);
	return 0;
}//main
