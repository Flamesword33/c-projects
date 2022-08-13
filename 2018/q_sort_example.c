#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int SIZE_OF_VALUES = 6;
int values[] = {92, 34, 100, 100, 2, 10};

int compare(const void *a, const void *b){
	return(*(int*)a <= *(int*)b);
}//compare

int main(){
	int n;
	
	printf("Before sorting the list is: \n");
	for(n = 0; n < SIZE_OF_VALUES; n++) {
		printf("%d ", values[n]);
	}//for
	
	qsort(values, SIZE_OF_VALUES, sizeof(int), compare);
	printf("\nAfter sorting the list is: \n");
	for(n = 0; n < SIZE_OF_VALUES; n++) {
		printf("%d ", values[n]);
	}//for
}//main