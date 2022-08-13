#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/* File:     
 *     pth_mat_vect.c  
 *
 * Purpose:  
 *     Computes a parallel matrix-vector product.  Matrix
 *     is distributed by block ms.  Vectors are distributed by 
 *     blocks.
 *
 * Input:
 *     m, n: order of matrix
 *     A, x: the matrix and the vector to be multiplied
 *
 * Output:
 *     y: the product vector
 *
 * Compile:  gcc -g -Wall -o vectMatMul vectMatMul.c -lpthread
 * Usage:
 *     ./vectMatMul <thread_count>
 *
 * Notes:  
 *     1.  Local storage for A, x, y is dynamically allocated.
 *     2.  Number of threads (thread_count) should evenly divide both 
 *         m and n.  The program doesn't check for this.
 *     3.  We use a 1-dimensional array for A and compute subscripts
 *         using the formula A[i][j] = A[i*n + j]
 *     4.  Distribution of A, x, and y is logical:  all three are 
 *         globally shared.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variables */
int     thread_count;
int     m, n;
double** A;
double** x;
double** y;
double time_spent;

/* Serial functions */
void Usage(char* prog_name);
void printMatrix(char* title, double ** ptr, int m, int n);
double ** allocateMatrix(int m, int n);
void destroyMatrix(double ** ptr, double m, double n);
void initMatrix(int m, int n, double ** ptr);


/* Parallel function */
void *Pth_mat_vect(void* rank);

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
long       thread;
pthread_t* thread_handles;


   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   printf("Enter m and n\n");
   scanf("%d%d", &m, &n);

   A = allocateMatrix(m,n);
   x = allocateMatrix(n,1);
   y = allocateMatrix(m,1);
   
  // Read_matrix("Enter the matrix", A, m, n);
   initMatrix(m, n, A);
   initMatrix(n, 1, x);
   initMatrix(m, 1, y);
   
    
   
   clock_t begin = clock();
   //GET_TIME(start);
   
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_mat_vect, (void*) thread);
   
      
     
 
   



   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
 
  clock_t end = clock();

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   
   // printMatrix("y-Vector", y, m, 1);
   
   printf("Elapsed: %f seconds\n", time_spent);
  //  elapsed = finish - start;
    // printf("The code to be timed took %e seconds\n", elapsed);
 

//destroyMatrix
   destroyMatrix(A,m,n);
   destroyMatrix(x,n,1);
   destroyMatrix(y,m,1);
   free(thread_handles);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */



/*------------------------------------------------------------------
 * Function:       Pth_mat_vect
 * Purpose:        Multiply an mxn matrix by an nx1 vector
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count; 
   int my_first_m = my_rank*local_m;
   int my_last_m = my_first_m + local_m - 1;
   printf("Thread %ld of %d - start: %d - end: %d\n", my_rank,  thread_count, my_first_m, my_last_m); 
   
   for (i = my_first_m; i <= my_last_m; i++) {
      y[i][0] = 0.0;
      for (j = 0; j < n; j++)
          y[i][0] += A[i][j]*x[j][0];
   }

   return NULL;
}  /* Pth_mat_vect */


/*------------------------------------------------------------------
 * Function:    printMatrix
 * Purpose:     Print the matrix
 * In args:     title, ptr, m, n
 */
void printMatrix( char* title, double** ptr, int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", ptr[i][j]);
      printf("\n");
   }
}  /* printMatrix */


/*------------------------------------------------------------------
 * Function:    allocateMatrix
 * Purpose:     allocate 2d matrix of size m rows and n columns
 * In args:     title, m, n
 */
double ** allocateMatrix(int m, int n)
{
    double ** ptr = (double **) malloc(sizeof(double *)*m);
    for(int i = 0; i < m; i++)
    {
        ptr[i] = (double *) malloc(sizeof(double)*n);
    }
    return ptr;
}
/*------------------------------------------------------------------
 * Function:    destroyMatrix
 * Purpose:     deallocate 2d matrix of size m rows and n columns
 * In args:     ptr, m, n
 */

void destroyMatrix(double ** ptr, double m, double n)
{
    for(int i = 0; i < m; i++)
    {
        free(ptr[i]);
    }
    free(ptr);
}
/*------------------------------------------------------------------
 * Function:    initMatrix
 * Purpose:     initialize a 2d matrix of size m rows and n columns to     random double numbers 
 * In args:     ptr, m, n
 */
void initMatrix(int m, int n, double ** ptr)
{
    int i, j;
    srand(time(NULL));
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            ptr[i][j]=rand();

        }
    }
}
 
 
