

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


int thread_count;

void* Thread_sum(void* rank); /* thread function */

int main(int argc, char* argv[]){
	
}//main

void* Thread_sum(void*){
	long my_rank = (long) rank;
	double factor;
	double my_sum = 0.0;
	long long i;
	//n is the number of terms in function
	long long my_n = n/thread_count;
	long long my_first_i = my_n * my_rank;
	long long my_last_i = my_first_i + my_n;
	
	/* This block guarantees that our code is always adding or subtracting
	in the correct place */
	if(my_first_i % 2 == 0){
		factor = 1.0;
	}
	else{
		factor = -1.0;
	}
	
	for(i = my_first_i; i < my_last_i; i++){
		my_sum += factor/(2 * i + 1);
		factor = -factor;
	}
	
	while(flag != my_rank){
		
	}
	sum += my_sum;
	flag = (flag + 1) % thread_count;
	
	return NULL;
	
}//Thread_sum