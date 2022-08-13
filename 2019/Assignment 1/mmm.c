/* mmm.c
 * by Nathan Pelletier
 *
 * Started: October 17 2019
 * Finished: 
 *
 * Work goals: 
 *	must take 3 ints from command line: m, k, n  --> DONE
 *	must randomly generate doubles for matrix population --> DONE
 *	must correctly multiply two matrices --> DONE
 *	divides the work load evenly --> DONE
 *  Must work in serial and with 2 or more cores --> 
 *  Gather run time data --> DONE
 *		
 */
 
/* Programs purpose:
 * Takes three integers as matrix sizes and after filling said 
 * matrices with random doubles the program asks the user how 
 * many cores their system has. The program then divides the 
 * work up among the cores available and preforms
 * matrix multiplication. 
 * The program then prints out how long the multiplication took
 * on 1, 2, 4 and 8 cores. 
 *
 * (disclaimer: program will not work unless your system has 8 cores)
 * (disclaimer: program will not run without proper MPI 
 * compiler Installation)
 */
 
#include <mpi.h> 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

	
/* fill_matrixes_with_random_doubles(int,int,int, double*, double*) 
 *
 * requires two pointer matrices called:
 *   matrix1 
 *   matrix2
 * of size:
 *   mxk
 *   kxn
 */
int fill_matrixes_with_random_doubles(
        int m1,
        int k1,
        int n1, 
        double *matrix1, 
        double *matrix2){

    srand(time(NULL));
    for(int a = 0; a < (m1*k1); a++){
	*(matrix1 + a) = rand();
    }//for matrix1 fill
		
    for(int b = 0; b < (n1*k1); b++){
	*(matrix2 + b) = rand();
    }//for matrix2 fill
		
    return 0;
}//fill_matrixes_with_random_doubles


/* matrix_multiplication(int,int,int,int,int,double,double,double) --> int
 *
 * takes the dimensions of two matrices and multiplies them using multiple 
 * threads among a specified amount of processors.
 * 
 * REQUIRES:
 *   three pointers named matrix1, matrix2, answer
 *	 MPI must be initialized
 *
 * NOTE:
 *	 Assumes the matrix is divisible by the number of processes
 *
 * Returns:
 * 	 a clock time based on how long the slowest processor took to finish
 *   the operation 
 */
double matrix_multiplication(
            int m, 
            int k, 
            int n, 
            int comm_sz, 
            int my_rank, 
            double *matrix1, 
            double *matrix2, 
            double *answer){

    double	start_time;
    double 	end_time;
    double	time_taken;
    double	running_total = 0;
    double	temp1;
    double 	temp2;
    int		local_matrix1_size = (m*k)/comm_sz;
    int		local_answer_size = (m*n)/comm_sz;
    int 	local_m = m/comm_sz;
    double	local_matrix1[local_matrix1_size];
    double	local_answer[local_answer_size];

    start_time = MPI_Wtime();

    MPI_Scatter(matrix1, local_matrix1_size, MPI_DOUBLE,
        local_matrix1, local_matrix1_size, MPI_DOUBLE,
        0, MPI_COMM_WORLD);

    for(int g = 0; g < local_m; g++){
        for(int h = 0; h < n; h++){
            for(int i = 0; i < k; i++){
                temp1 = local_matrix1[(g*k+i)];
                temp2 = *(matrix2 + (i*n+h));
                running_total = running_total + (temp1 * temp2);
            }//local_m * n * k

            local_answer [(g*n+h)] = running_total;
            temp1 = 0;
            temp2 = 0;
            running_total = 0;
        }//local_m * n
    }//local_m

    MPI_Gather(local_answer, local_answer_size, MPI_DOUBLE,
        answer, local_answer_size, MPI_DOUBLE, 
        0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    time_taken = end_time - start_time;

    return time_taken;
}//multi_core_matrix_multiplication


int print_results(double time){
    printf("Total time taken was: %g \n", time);
    return 0;
}//print_results

/* main(int, char*) --> int
 *
 * 
 */
int main(int argc, char* argv[]){
    char *temp;
    double time;
    int m; 
    int k;
    int n;
    int comm_sz;
    int my_rank;
    double *matrix1;
    double *matrix2;
    double *answer;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank == 0){
        m = strtol(argv[1], &temp, 10); 
        k = strtol(argv[2], &temp, 10);
        n = strtol(argv[3], &temp, 10);
        matrix1 = (double*)malloc(m*k*sizeof(double));
        answer = (double*)malloc(m*n*sizeof(double));
    }//main process tasks

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //This allocation is outside the if to allow 
    //all processes to allocate memory
    matrix2 = (double*)malloc(k*n*sizeof(double));

    if(my_rank == 0){
        fill_matrixes_with_random_doubles(m,k,n, matrix1, matrix2);
    }

    // Broadcast matrix2 to all processes before multiplication
    MPI_Bcast(matrix2, n*k, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    time = matrix_multiplication(m,k,n,comm_sz,my_rank,
        matrix1,matrix2,answer);

    if(my_rank == 0){
        print_results(time);
    } 

    MPI_Finalize();

    return 0;
}//main
