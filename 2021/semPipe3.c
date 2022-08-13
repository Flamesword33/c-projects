/* semPipe3.c
 * by Nathan Pelletier
 * First attempt started: March 23, 2021
 * Second attempt started: March 26, 2021
 * Thrird attempt started: March 28, 2021
 * finished: March 28, 2021
 *
 * Prints all numbers from 1 to 1000
 * Removes numbers that are divisable by 2 and 3 from being printed.
 * Calls 3 processes to achieve this. 
 *  First process generates the numbers and passes them through the B0 buffer
 *  Second process determines if the number in B0 is divisible by 2 and 
 *      passes it to buffer B1 if it isn't
 *  Third process determines if number in B1 is divisible by 3 and
 *      if it isn't then process 3 prints said number
 * Sending 0 into a buffer stops process execution
 * 
 * Potensial problems:
 *  Buffers are only 1 int large and as such process 1 is going to be waiting the longest
 *  mmap() is very finiky to work with 
 *  Process 2 and 3 can run into an infinite loop as they are stopped by process 1
 *  Process 2 and 3 can be starved by process 1 
 *  requires 2 mutexes for b0 and b1
 *  
 * First attempt ran but hung on execution with no feedback
 * Second attempt was a proof of concept with pipes instead, 
 *      children exited early causing program to only reach ~700
 * 
 */

#include <stdio.h>
#include <stdlib.h>     //for exit()
#include <semaphore.h>  //for sem_t
#include <sys/mman.h>   //for mmap()
#include <sys/wait.h>   //for pidwait()
#include <unistd.h>     //for sleep()

#define SIZE (2 * sizeof(int))
#define STOP 0

/////////////
//Functions//
void fork_processes(int* critical_values);
void generate_numbers(int* b0, sem_t lock0);
void remove_multiples_of_two(int* b0, int* b1, sem_t lock0, sem_t lock1);
void remove_multiples_of_three(int* b1, sem_t lock1);


/* main(char*, int)
 * 
 * creates shared memory pointer to an array
 * reclaims pointers memory and exits 
 * Returns 1 to report error
 * 
 * @param argv 
 * @param argc 
 * @return int 
 */
int main(char* argv, int argc){
    int* critical_values;
    
    critical_values = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, 
                        MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (critical_values == (caddr_t) -1){ 
        perror ("error in mmap while allocating shared memory\n");
        exit (1);
    }//exit if error setting up mmap

    fork_processes(critical_values);

    munmap(critical_values);
    exit(0);
}//main


/* fork_processes(int*)
 * 
 * Creates 2 forked children
 * Forces parent processes to wait for next child process
 * creates 2 locks and initializes 2 pointer int buffers from shared memory
 * 
 * @param critical_values 
 */
void fork_processes(int* critical_values){
    int *b0 = &critical_values[0];
    int *b1 = &critical_values[1];
    sem_t lock0;
    sem_t lock1;
    pid_t process01;
    pid_t process12;

    seminit(&lock0, 1, 1);
    seminit(&lock1, 1, 1);

    process01 = fork();

    if(process01 < 0){
        perror ("Error creating fork\n");
        exit(1);
    }//if fork failed

    else if (process01 > 0){
        generate_numbers(b0, lock0);
        waitpid(process01, NULL, 0);
    }//else if parent

    else{
        process12 = fork();

        if(process12 > 0){
            remove_multiples_of_two(b0, b1, lock0, lock1);
            waitpid(process12, NULL, 0);
        }//second child

        else{
            remove_multiples_of_three(b1, lock1);
        }//else third child
    }//else
    
    sem_destroy(&lock0);
	sem_destroy(&lock1);
}//fork_processes


/* generate_numbers(int*, sem_t)
 * 
 * simple 1 to 1000 for loop inserts numbers into shared buffer b0
 * sleeps for 1 second after writing to allow child to see change
 * after completing loop, function sends stop command into buffer to signal 
 *  termination
 * 
 * @param b0 
 * @param lock0 
 */
void generate_numbers(int* b0, sem_t lock0){
    for(int i = 1; i < 1000; i++){
        sem_wait(lock0);
        *b0 = i;
        sem_post(lock0);
        sleep(1);               //writers sleep to allow readers to catch up
    }//for
    sem_wait(lock0);
    *b0 = STOP;
    sem_post(lock0);
}//generate_numbers


/* remove_multiples_of_two(int*, int*, sem_t, sem_t)
 *  
 * Reads numbers incomming from b0
 *  if odd and a new number then function sends number to b1
 *  function sleeps after sending to allow child to read b1
 * Function loops until it recieves stop command through the buffer
 *  upon recieving stop command, function passes it along to b1
 * 
 * @param b0 
 * @param b1 
 * @param lock0 
 * @param lock1 
 */
void remove_multiples_of_two(int* b0, int* b1, sem_t lock0, sem_t lock1){
    int value = -1;
    int last_loop = -1;

    while (value != STOP){
        sem_wait(lock0);
        value = *b0;
        sem_post(lock0);

        if((value % 2) > 0 && value != last_loop){
            sem_wait(lock1);
            *b1 = value;
            sem_post(lock1);
            last_loop = value;
            sleep(1);           //writers sleep to allow readers to catch up
        }//if not divisible by 2 and different than before

    }//while

    sem_wait(lock1);
    *b1 = STOP;
    sem_post(lock1);
}//remove_multiples_of_two


/* remove_multiples_of_three(int*, sem_t)
 * 
 * recieves input from b1 buffer
 * if b1 isn't divisible by 3 and its a new number
 *  then function prints out the number
 * function ends on receiving stop command
 * 
 * @param b1 
 * @param lock1 
 */
void remove_multiples_of_three(int* b1, sem_t lock1){
    int value = -1;
    int last_loop = -1;

    while (value != STOP){
        sem_wait(lock1);
        value = *b1;
        sem_post(lock1);

        if((value % 3) > 0 && value != last_loop){
            printf("%d ", value);
        }//if not divisible by 2 and different than before

    }//while
}//remove_multiples_of_two