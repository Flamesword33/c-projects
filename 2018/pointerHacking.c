#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int passRegular(int x){
	x = 33;        //creates a local change
	return x;
}

void passByPointer(int *x){
	*x = 99;        //creates a non local change
}                   //can use to print memory in hex

int main(){
	int x = 0;
	printf("Start x = %d\n", x);
	
	x = passRegular(x);
	printf("Now x = %d\n", x);
	
	passByPointer(&x);
	printf("Now x = %d\n", x);
	
	return 0;
}//main