#include <cstdlib>
#include <cstdio>
#include <mpi.h>

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
		for(process = 1; process < comm_sz; process++){
			MPI_Recv(&local_int, 1, MPI_DOUBLE, process, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
			total_int += local_int;
		}//for
		printf("With n = %d trapezoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
	}//if main process
	//to make the code faster we must make this statement far more complicated
	else {
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}//else worker processes
}//main

//note to self: the Trap() function is my designed function