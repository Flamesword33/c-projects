#include <cstdlib>
#include <cstdio>
#include <mpi.h>

/* 
wants printed in format:
Process 1, local_n = 256, local_a = 512, local_b = 1024, My_sum = ___, Global_sum = 0
.
.
.

Our first file will be Trapzoidal rule in MPI as per normal --> DONE
Second file will be the same but using MPI_Reduce() --> THIS FILE
Third file will be with MPI_Allreduce() 


NOTE you may only have send/ recieve or reduce not both
*/

const int MAX_STRING = 100;

/* f(x) --> int
 * 
 * any function to do the trapezoidal function on
 */
double f(double x){
	double y;
	y = x * x;
	return y;
}//f(x)

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) {
	double estimate; 
	double x;
	int i;
	
	estimate = (f(left_endpt) + f(right_endpt))/2.0;
	
	for(i = 1; i <= trap_count - 1; i++){
		x = left_endpt + i * base_len;
		estimate += f(x);
	} //for
	
	estimate = estimate * base_len;
	
	return estimate;
	
}//Trap

int main(void) {
	int my_rank;
	int comm_sz;
	int n = 1024;
	int local_n;
	int process;
	double a = 0.0;
	double b = 3.0;
	double h;
	double local_a;
	double local_b;
	double local_int;
	double total_int;
	char result[MAX_STRING];
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	h = (b-a)/n;			/* h is the same for all processes	*/
	local_n = n/comm_sz;	/* So is the number of trapezoids	*/
	
	local_a = a + my_rank * local_n * h; //each core runs through all code
	local_b = local_a + local_n * h;
	local_int = Trap(local_a, local_b, local_n, h);
	
	if(my_rank == 0){
		total_int = local_int;

		MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		
		printf("With n = %d trapezoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
		
		printf("Process %d, local_n = %d, local_a = %f, ", my_rank, local_n, local_a);  
		printf("local_b = %f, My sum = %f, Global_sum = %f \n", local_b, local_int, total_int);
		
		for(process = 1; process < comm_sz; process++){
			MPI_Recv(&result, MAX_STRING, MPI_CHAR, process, 0,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%s\n", result);
		}//for
		
	}//if main process
	//to make the code faster we must make this statement far more complicated
	else {
		sprintf(result, "Process %d, local_n = %d, local_a = %f, local_b = %f, My sum = %f, Global_sum = %f", my_rank, local_n, local_a, local_b, local_int, total_int);

		MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		MPI_Send(&result, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}//else worker processes
	
	MPI_Finalize();
	return 0;
}//main

//note to self: the f() function is my designed function