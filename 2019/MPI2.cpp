
 #include "mpi.h" 
 #include <stdio.h> 
 #include <string.h> 
 int main(int argc, char **argv) { 
    char idstr[32]; 
    char buff[128]; 
    int numprocs; 
    int myid; 
    int i; 

    MPI_Status stat; 

    /*********************************************
      Begin program
     *********************************************/

    MPI_Init(&argc,&argv);                 // Initialize
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);  // Get # processors
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);      // Get my rank (id)

    if( myid == 0 ) {  // Master
       printf("WE have %d processors\n", numprocs); 

       for( i = 1; i < numprocs; i++) { 
          sprintf(buff, "Hello %d", i); 
          MPI_Send(buff, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD); 
       }//for 

       for( i = 1; i < numprocs; i++) { 
          MPI_Recv(buff, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD, &stat); 
          printf("%s\n", buff); 
       }//for  
    }//if 
    else {  // Slave

       MPI_Recv(buff, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat); 

       sprintf(idstr, "Hello 0, Processor %d is present and accounted for !", 
		myid); 
       MPI_Send(buff, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD); 
    }//else 

    MPI_Finalize(); 
 }//main