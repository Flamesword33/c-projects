#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0


int main(){
	char c;
	FILE *file;
	file = fopen("C:/Users/Nathan Pelletier/desktop/c/simp.txt", "r");
	
	if (file){
		while (fscanf(file, "%c", &c) != EOF){
			printf("%c", c);
			
			//DO WHAT YOU WANT HERE
			
		}//while
		
		fclose(file);
	}//if
	
	return 0;
}//main