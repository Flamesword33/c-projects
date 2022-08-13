#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>  //sleep()

#define BUFF_SIZE 5				/* total number of slots */
#define NUMBER_OF_PRODUCERS 3		
#define NUMBER_OF_CONSUMERS 3		
#define NUMBER_OF_ITEMS 4		/* number of items to produce/consume */

typedef struct {
    int buf[BUFF_SIZE];   	/* shared var */
    int in;         	    /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;        	  	/* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full;    	  		/* keep track of the number of full spots */
    sem_t empty;            /* keep track of the number of empty spots */
    pthread_mutex_t mutexC;	/* enforce mutual exclusion to shared data */
    pthread_mutex_t mutexP;
} sbuf_t;

sbuf_t shared;


/* Producer(int) --> int
 *
 * 
 * returns 0 if successful
 */
void* Producer(void *index){
	
	int item;

    pthread_detach(pthread_self()); //unsure why we do this

    for (item=0; item < NUMBER_OF_ITEMS; item++) {

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared.empty);
        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutexP);
        shared.buf[shared.in] = item;
        shared.in = (shared.in + 1) % BUFF_SIZE;
        printf("[P%d] Producing %d ...\n", *((int *)index), item); 
		fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutexP);
        /* Increment the number of full slots */
        sem_post(&shared.full);

        /* Interleave  producer and consumer execution */
        if (item % 2 == 1) sleep(1);
    }//for

}//Producer


/* Consumer(int) --> int
 * 
 * Thread driven function that preforms FIFO on a list of numbers made by producer
 * Waits if list is empty
 * Ends after consuming a number of items defined on line 9
 */
void* Consumer(void *index){
    	
	int item;

    pthread_detach(pthread_self()); //unsure why we do this

    for (int i=0; i < NUMBER_OF_ITEMS; i++) {

        /* Prepare to read item from buf */

        /* If there are no full slots, wait */
        sem_wait(&shared.full);
        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutexC);
        item = shared.buf[shared.out];
        shared.out = (shared.out + 1) % BUFF_SIZE;
        printf("------> [C%d] consumed %d\n", *((int *)index), item); 
		fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutexC);
        /* Increment the number of empty slots */
        sem_post(&shared.empty);

        /* Interleave  producer and consumer execution */
        if (item % 2 == 0) sleep(1);
    }//for

}//Consumer


/* main() --> int
 *  
 * Creates Producer and Consumer threads
 * The number of producers and consumers it creates are defined on line 7 and 8
 * Initializes two semiphores, one to stop producers from entering a full queue (empty)
 * another to stop consumers from grabbing a non-existant element (full)
 * Initializes a mutex for critical code in producers and consumers
 * returns 0 on successful exit
 */
int main(){
    pthread_t producerId; 
	pthread_t consumerId;
    int index;
    shared.out = 0;
    shared.in = 0;

    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);

    /* Insert code here to initialize mutex*/
    pthread_mutex_init(&shared.mutexP, NULL);
    pthread_mutex_init(&shared.mutexC, NULL);

    for (index = 0; index < NUMBER_OF_PRODUCERS; index++){  
       /* Create a new producer */
       pthread_create(&producerId, NULL, Producer, (void*) &index);
    }

    /* Insert code here to create NUMBER_OF_CONSUMERS consumers */
    for(index = 0; index < NUMBER_OF_CONSUMERS; index++){
        pthread_create(&consumerId, NULL, Consumer, (void*) &index);
    }

    pthread_exit(NULL); //incredibly handy way to free all memory used by threads, semphores and mutexes
}//main
