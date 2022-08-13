/*  shmcount - Parent and child processes increment a counter
 *             in shared memory.  
 *
 *  This example illustrates the use System V shared memory.
 *  It does not use semaphores to avoid the critical section
 *  problem of updating a shared variable.
 *
 *  J. Mohr
 *  2005.03.06
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


const int SHM_KEY = IPC_PRIVATE; /* for parent-child sharing only */
const int SHM_SIZE = sizeof( int );
const int SHM_MODE = SHM_R | SHM_W; /* open for read/write access */
const int COUNT_MAX = 5000000;
//const int COUNT_MAX = 10;	/* for testing */

const int DELAY_COUNT = 10;

void fatalsys( char* );


int main()
{
   int shmid;			/* a shared memory descriptor (ID) */
   int * shmptr;		/* address of shared memory */
   pid_t pid;
   int i, j;			/* loop control variables */

   /* Creating shared memory is like opening a file. */
   shmid = shmget( SHM_KEY, SHM_SIZE, SHM_MODE );
   if ( shmid == -1 )
      fatalsys("shmget failed");

   /* Attach the shared memory to the virtual address space. 
      The 0 arguments let the OS decide where to map it.
    */
   shmptr = (int *) shmat( shmid, 0, 0 );	
   if ( shmptr == (int *) -1 )
      fatalsys("shmat failed");

   /* Create a child process to access the same shared memory. */
   if ( ( pid = fork()) < 0 )
      fatalsys("fork failed");

   /* BOTH PARENT AND CHILD EXECUTE THE FOLLOWING CODE */
   for ( i = 0; i < COUNT_MAX; i++ )
   {
      /* This is the straightforward way to do it, but is
	 unlikely to result in an error due to a race condition. */
      (*shmptr)++;

      /* This is the way to test whether a critical section exists at all.
	 We exaggerate the timing of the race condition. */
      //int val = *shmptr;
      //for ( j = 0; j < DELAY_COUNT; j++ )
      //	 ;			/* do nothing */
      //*shmptr = ++val;
   }

   /* Only the parent does this. */
   if ( pid > 0 )
   {
      /* Wait for the child to complete. */
      if ( wait( NULL ) < 0 )
	 fatalsys("wait failed");

      /* Print the result. */
      printf("Shared value is %ld.\n", *shmptr );
   }

   /* Detach the shared memory from the virtual address space. */
   if ( shmdt( (void *) shmptr ) == -1 )
      fatalsys("shmdt failed");

   
   /* Remove the shared memory segment set from the system. 
      This is also only done by the parent.
    */
   if ( pid > 0 )
      if ( shmctl( shmid, IPC_RMID, 0 ) == -1 )
	 fatalsys("shmctl failed");
   
   exit( EXIT_SUCCESS );
}


void fatalsys( char* msg )
{
	perror( msg );
	exit( EXIT_FAILURE );
}
