/* Serial_Matrix_Multiplication.c
 * by Nathan Pelletier
 *
 * This file is a quick and dirty proof of consept to make two matrixes 
 * inputed by the user (At first will be just in the code)
 * The program will then preform matrix multiplication on them.
 * Finally the program will print out the result
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

	int matrix1[10][10];
	int matrix2[10][10];
	int answer[10][10];

int main(int argc, char *argv[]){
	input_matrix();

	multiply_matrix();
	
	print_matrix();
	
	return 0;
}//main

//for now it makes matrix1 == matrix2 
int input_matrix(){
	int count = 1;
	for(int a = 0; a < 10; a++){
		for(int b = 0; b < 10; b++){
			matrix1[a][b] = count;
			matrix2[a][b] = count;
			count++;
		}//for 
	}//for
	return 0;
}//input_matrix

int multiply_matrix(){
	int running_total = 0;
	for(int c = 0; c < 10; c++){
		for(int d = 0; d < 10; d++){
			for(int e = 0; e < 10; e++){
				running_total = running_total + (matrix1[c][e] * matrix2[e][d]);
			}//for (goes through the rows)
			answer[c][d] = running_total;
			running_total = 0;
		}//for (finds the coordinates)
	}//for
	return 0;
}//multiply_matrix

int print_matrix(){
	for(int f = 0; f < 10; f++){
		for(int g = 0; g < 10; g++){
			printf("%d ", answer[f][g]);
		}//for 
		printf("\n");
	}//for
	return 0;
}//print_matrix