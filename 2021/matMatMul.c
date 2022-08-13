/* matMatMul.c
 * by Nathan Pelletier
 * Started Febuary 27 2021
 * Finished March 7 2021
 * 
 * /////////////////WARNING/////////////////////////
 * EXPECTS INT ON COMMAND LINE FOR NUMBER OF THREADS
 * MUST BE RUN IN LINUX
 * /////////////////////////////////////////////////
 * 
 * Preforms Matrix Matrix multiplication
 * Asks user for 3 intigers: m, n and k
 * These are used for [m,n] x [n,k] = [m,k]
 * Asks user if they wish to write in specific data for the matrix
 * If they say no then program fills matrix with random doubles
 * Program times from thread creation till thread deletion
 * 
 * The wrinkle to this program is that the matrix [m,n] 
 * is subdivided between the threads present using pthread library.
 * 
 * Program outputs:
 *   The lines of matrix 1 that each thread was responsible for
 *   The time it took to create threads, complete the work and join threads
 *   The resulting matrix should the user ask for it
 * 
 * Required functions:
 *   allocateMatrix(rows, columns) --> returns double pointer 
 *   destroyMatrix(rows, columns, M)
 *   initializeMatrix(row, columns, M)
 *   timeThreads(params) --> this is the thread function
 *   
 * Test cases:
 * test for thread count of: 1, 2, 4, 8
 * test for square matrixes of size: 1024, 2048, 4096, 8192, 16384
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

///////////
//GLOBALS//
double ** matrix1;
double ** matrix2;
double ** resultMatrix;
pthread_mutex_t lock;
//int isThreadAlive[8] = {0,0,0,0,0,0,0,0};

/////////////
//Functions//
int getThreadCountFromCommandLine(int argc, char * argv[]);
double ** allocateMatrix(int rows, int columns);
int fillMatrices(int m, int n, int k);
int timeThreads(int numberOfThreads, int m, int n, int k);
int printTime(time_t start, time_t end);
char doesUserWantFillMatrix();
int userInitializeMatrix(int rows, int columns, double ** M);
int initializeMatrix(int rows, int columns, double ** M);
char doesUserWantOutput();
int printMatrix(int row, int column, double ** matrixPointer);
int destroyMatrix(int rows, double ** M);
int handleThreads(int numberOfThreads, int m, int n, int k);
int multiplyMatrix(int dimensions[]);


/* Determines number of threads from commandline arguments
 * Asks user how big to make each matrix
 * Reserves space for 3 matrixes
 * Either asks user to fill matrixes or does so with random doubles
 * Creates multiple threads to multiply matrixes and times them
 * Closes threads 
 * Prints out time taken and if asked prints out resulting matrix 
 * Frees space taken by arrays.
 */ 
int main(int argc, char *argv[]){
    char userWantToPrintMatrix;
    int numberOfThreads;
    int m;
    int n;
    int k;

    numberOfThreads = getThreadCountFromCommandLine(argc, argv);
    printf("Please enter the dimensions of the two matricies? \n");
    printf("Remember that the middle two dimensions are the same, so only type it once: ");
    scanf("%d", &m);
    scanf("%d", &n);
    scanf("%d", &k); 
    
    matrix1 = allocateMatrix(m, n);
    matrix2 = allocateMatrix(n, k);
    resultMatrix = allocateMatrix(m, k);

    fillMatrices(m, n, k);
    timeThreads(numberOfThreads, m, n, k);
    
    userWantToPrintMatrix = doesUserWantOutput();
    if(userWantToPrintMatrix == 'y' || userWantToPrintMatrix == 'Y'){
        printMatrix(m, k, resultMatrix);
    }//if user wants matrix printed

    destroyMatrix(m, matrix1);
    destroyMatrix(n, matrix2);
    destroyMatrix(m, resultMatrix);

    return 0;
}//main


/* Checks if multiple command line inputs. 
 * If there are then first argument is returned.
 * Otherwise returns 1 thread. 
 * If user enters non int then atoi() returns 0 and this function will return 1
 */
int getThreadCountFromCommandLine(int argc, char *argv[]){
    int firstArgument;

    if(argc > 1){
        firstArgument = atoi(argv[1]);
        if(firstArgument == 0){
            return 1;
        }//if argument present and non int 
        else{
            return firstArgument;
        }//else argument present and int format
    }
    else{
        return 1;
    }//no argument present
}//getThreadCountFromCommandLine


/* Copied code from vecMatMul.c by Mohammed Qasem
 * ------------------------------------------------------------------
 * Function:    allocateMatrix
 * Purpose:     allocate 2d matrix of size m rows and n columns
 * In args:     title, m, n
 */
double ** allocateMatrix(int rows, int columns){
    double ** ptr = (double **) malloc(sizeof(double *) * rows);

    for(int i = 0; i < rows; i++){
        ptr[i] = (double *) malloc(sizeof(double) * columns);
    }
    return ptr;
}//allocateMatrix


/* Asks user if they want to fill matricies themselves 
 * If so sends program to further functions to allow user to fill matrix1 and matrix2
 * If not then program fills matrix1 and matrix2 with random doubles
 */
int fillMatrices(int m, int n, int k){
    char userFillMatrix;

    userFillMatrix = doesUserWantFillMatrix();

    if(userFillMatrix == 'y' || userFillMatrix == 'Y'){
        userInitializeMatrix(m, n, matrix1);
        userInitializeMatrix(n, k, matrix2);
    }//if user wants to fill matrixes themselves

    else{
        initializeMatrix(m, n, matrix1);
        initializeMatrix(n, k, matrix2);
    }//if user wants random stuff in matrixes

    return 0;
}//fillMatrices


int timeThreads(int numberOfThreads, int m, int n, int k){
    time_t startTime;
    time_t endTime;

    startTime = time(NULL);
    handleThreads(numberOfThreads, m, n, k);
    endTime = time(NULL);

    printTime(startTime, endTime);
    return 0;
}//timeThreads


int printTime(time_t start, time_t end){
    time_t totalTime = end - start;
    printf("Elapsed: %ld\n", totalTime);
    return 0;
}//printTime


/* prompts user for feedback
 * Asks for Y or y to mean yes
 * Any other character is assumed to mean no
 */
char doesUserWantFillMatrix(){
    char userInput;
    
    printf("Would you like to enter the matrix values yourself?  (Y/N): ");
    scanf(" %c", &userInput);
    return userInput;
}//doesUserWantFillMatrix


int userInitializeMatrix(int rows, int columns, double ** M){
    double tempDouble = 0.0;

    printf("Please enter the matrix value for the following:\n");
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("[%d][%d]: ", i, j);
            scanf("%lf", &tempDouble); 
            M[i][j] = tempDouble;
        }//fill columns loop
    }//fill rows loop
    return 0;
}//userInitializeMatrix


/* Starts a random seed on current time clock time
 * Fills matrix with random ints.
 * I know this should be random doubles but to do so is quite a process
 */
int initializeMatrix(int rows, int columns, double ** M){
    srand(time(NULL));

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            M[i][j] = rand();
        }//fill columns loop
    }//fill rows loop
    return 0;
}//initializeMatrix


/* prompts user for feedback
 * Looks for Y or y
 * Any other character is asumed to mean no
 */
char doesUserWantOutput(){
    char userInput;
    
    printf("Do you want the resulting matrix printed?  (Y/N): ");
    scanf(" %c", &userInput);
    return userInput;
}//doesUserWantFillMatrix


/* Prints out a double pointer matrix
 */
int printMatrix(int row, int column, double ** matrixPointer){
    double temp;
    for(int x = 0; x < row; x++){
        for(int y = 0; y < column; y++){
            temp = matrixPointer[x][y];
            printf("%fl ", temp);
        }
        printf("\n");
    }
    return 0;
}//printMatrix


/* Idea taken from https://stackoverflow.com/questions/11015360/free-a-double-pointer
 * ---------------------------------------
 * Frees each row of a double pointer and then frees the ponter itself
 */
int destroyMatrix(int rows, double ** M){
    for(int i = 0; i < rows; i++){
        free(M[i]);
    }
    free(M);
    return 0;
}//destroyMatrix


int handleThreads(int numberOfThreads, int m, int n, int k){
    pthread_t threadId[numberOfThreads];
    //pthread_mutex_init(&lock, NULL);
    int dimensions[5] = {m,n,k,numberOfThreads}; //allows passing of parameters into thread

    for(int i = 0; i < numberOfThreads; i++){
        dimensions[4] = i;
        //isThreadAlive[i] = 1;
        printf("Thread %d of %d - start: %d\n", i, numberOfThreads, i);
        pthread_create(&threadId[i], NULL, multiplyMatrix, dimensions);
        //multiplyMatrix(dimensions);
    }

    for(int j = 0; j < numberOfThreads; j++){
       printf("- end: %d\n", j);
       pthread_join(threadId[j], NULL); 
      // while(isThreadAlive[j] == 1)
        //   sleep(1);
    }
    //pthread_mutex_destroy(&lock);
    return 0;
    
}//handleThreads


//*(*(matrixPointer + x) + y))
int multiplyMatrix(int dimensions[]){
    double temp = 0.0;
    //pthread_t id = pthread_self();
    pthread_t id = dimensions[4] + 1;
    int numberOfThreads = dimensions[3];
    int min = (id - 1) * dimensions[0]/numberOfThreads;
    int max = id * dimensions[0]/numberOfThreads;
    
    for(int m = min; m < max; m++){ //divide m based on id
        for(int k = 0; k < dimensions[2]; k++){
            for(int n = 0; n < dimensions[1]; n++){
                temp = matrix1[m][n] * matrix2[n][k] + temp;
            }//the shared size itterator

            //THIS LINE REQUIRES WRITE LOCK//
            //pthread_mutex_lock(&lock);
            resultMatrix[m][k] = temp;
            //pthread_mutex_unlock(&lock);
            /////////////////////////////////
            temp = 0.0;
        }//matrix2 column itterator
    }//matrix1 row itterator
    //isThreadAlive[dimensions[4]] = 0;
    return 0;
}//multiplyMatrix
