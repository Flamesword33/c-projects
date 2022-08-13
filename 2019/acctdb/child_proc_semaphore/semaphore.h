/*  semaphore.h	
 *
 *  This example uses Unix System V semaphores, not POSIX semaphores.
 *
 *  J. Mohr
 *  2005.03.06; rev. 2019-03-10
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define PERM_FILE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

void fatalsys( const char* msg );

const int IPC_KEY = IPC_PRIVATE; /* for parent-child sharing only */

union semun
{
	int val;		// value for SETVAL
	struct semid_ds *buf;	// buffer for IPC_STAT & IPC_SET
	unsigned short *array;	// array for GETALL & SETALL
	struct seminfo *__buf;	// buffer for IPC_INFO
};

/* Create a single semaphore and initialize it. */
int createsem( int initval )
{
    union semun semarg;

   /* Create a semaphore set with a single semaphore in it,
    * and with permissions set to typical values for a file.
    * 
    * Include IPC_CREAT and IPC_EXCL flags so that a semaphore
    * will be created but will fail if it already exists.
    */
   int semid = semget( IPC_KEY, 1, PERM_FILE | IPC_CREAT | IPC_EXCL );
   if ( semid == -1 )
      fatalsys("semget failed");

   /* Unfortunately, initializing the semaphore is a separate operation.
      This initializes our sole semaphore (semaphore 0) to 1.
    */
   semarg.val = initval;
   if ( semctl( semid, 0, SETVAL, semarg ) == -1 )
      fatalsys("Failed to initialize semaphore");

   return semid;
}


/* Dijkstra's 'p()' operation:  decrements a binary semaphore.
   This operation is often called 'wait()', but that name clashes
   with an existing system call for a parent to wait for a child.
 */
void p( int semid )
{
   /* SEM_UNDO handles the case of a process that terminates
    * without releasing its resource.
    */
   struct sembuf semoparg = { 0, -1, SEM_UNDO };

   if ( semop( semid, &semoparg, 1 ) == -1 )
      fatalsys("failed to decrement semaphore");
}


/* Dijkstra's 'v()' operation:  increments a binary semaphore. 
   This operation is often called 'signal()', but that name clashes
   with an existing system call for sending a signal to a process.
*/
void v( int semid )
{
   struct sembuf semoparg = { 0, 1, SEM_UNDO };

   if ( semop( semid, &semoparg, 1 ) == -1 )
      fatalsys("failed to increment semaphore");
}

void deletesem( int semid )
{
   union semun semarg;

   if ( semctl( semid, 0, IPC_RMID, semarg ) == -1 )
      fatalsys("Failed to delete semaphore");
}

/* UNCOMMENT this function if it is not defined elsewhere. */
/*
void fatalsys( const char* msg )
{
	perror( msg );
	exit( EXIT_FAILURE );
}
*/

