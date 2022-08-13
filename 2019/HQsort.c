/* HQsort.c
 * by Nathan Pelletier
 * Started: November 18, 2019
 * Finished: December 1, 2019
 *
 * This is a serial version of Hybrid quick sort 
 *   Hybrid quick sort:
 *	preforms several steps recursively 
 *          1. pick a pivot 
 *          2. correctly place the pivot in the array by finding
 *              elements bigger and
 *		smaller than it, exchanging the position of the
 *              first element that was
 *		bigger than the pivot with a smaller element 
 *              found after it
 *          3. after reaching the pivot swap it with the position of
 *		the first number known to be bigger than the pivot.
 *	Quicksort is then traditionally called on the 
 *          left and right portions of the list
 *
 * 	However in the case of hybrid quicksort there are several 
 *      optimizations
 *          1. only the smaller chunck found is recursivly called to
 *              reduce the 
 * 		amount of memory consumed by recursion 
 *          2. the larger chunk is caught in a while loop 
 *          3. if either the smaller recusive chunk or the 
 *              larger looping chunk
 *              are 10 or less elements then a different sorting 
 *              algorithm is called
 * 
 * Please note this algorithm can be any algorithm
 *  which has been proven to work
 *  on a smaller list consistently.
 * Initially I chose to use insertion sort as it was 
 * recomended by the example
 * I initially followed on https://www.techiedelight.com/hybrid-quicksort/
 * I then moved to bubble sort as I found a parallel version which used a 
 * extra value to track each elements rank and then placed each element with
 * a while{for{}} loop which was also easy to make parallel.
 * Unfortunatly I began to question the number
 * of threads needed to place a parallel
 * algorithm on the tail end of what was essentially a recursive 
 * bionary tree
 * in parallel. As such I desided to drop the idea in favour 
 * of insertion sort.
 */
 

#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
    #include <omp.h>
#endif


#define MAX_LENGTH 10000
#define BEGIN_INSERTION_SORT 10

int array[MAX_LENGTH];

/* insertion_sort(int, int)
 *
 * takes a given global array called array and
 * with an upper and lower bound the code below preforms 
 * insertion sort on it
 * 
 * Ex) 2 5 1 7 3 2
 * step 1: sorts 2 5
 * step 2: sorts 2 5 1 --> 1 2 5
 * ...
 */
int insertion_sort(int low, int high){
    // Start from second element (element at index 0 
    // is already sorted)
    for (int k = low + 1; k <= high; k++){
        int value = array[k];
	int index = k;

	// Find the index j within the sorted subset array[0..k-1]
	// where element array[k] belongs
	while (index > low && array[index - 1] > value) {
            array[index] = array[index - 1];
            index--;
	}//while
        // Note that subarray array[index...k-1] is shifted to
        // the right by one position i.e. array[index+1...k]
		
	array[k] = value;
    }//for
    return 0;
}//insertion_sort


int swap(int index1, int index2){
    int temp = array[index1];

    array[index1] = array[index2];
    array[index2] = temp;

    return 0;
}//swap


/* partition(int, int) --> int
 *
 * function takes a global list to be sorted and puts a single element in its
 * correct position while putting elements
 * greater than the element right of it
 * and placing all other elements left of it. The pivot selected is the last 
 * element in the list given by the index high.
 * 
 * function returns the place to put the pivot
 * 
 * requires function:
 *   swap(index1, index2);
 */
int partition (int low, int high){

    int pivot = array[high];
    int pivot_index = low;	
	
    // each time we finds an element less than or equal to pivot, pivot index
    // is incremented and that element is placed before the pivot. 
    for (int j = low; j < high; j++){
	if (array[j] <= pivot){
            swap(j, pivot_index);
            pivot_index++;
	}//if
    }//for

    swap(pivot_index, pivot);
	
    return pivot_index;
}//partition


/* hybrid_quick_sort(int, int)
 *
 * preforms quick sort until the sublist is 10 elements or less
 * upon breaking it into a small enough chunk it preforms 
 * insertion sort
 * 
 * during this tasks are assigned to the threads present
 * on each subsequent call to hybrid quick sort
 * 
 * requires methods:
 *   insertion_sort(int, int)
 * 	 partition(int, int)
 */
int hybrid_quick_sort(int low, int high){

    int pivot;

    while (low < high) {
	// do insertion sort if 10 or smaller
	if(high - low <= BEGIN_INSERTION_SORT){
            insertion_sort(low, high);
            break;
	}//if
	else {
            pivot = partition(low, high);
			
            // tail call optimizations - recur on smaller sub-array
            if (pivot - low < high - pivot) {
		#pragma omp task
                hybrid_quick_sort(low, pivot - 1);
		low = pivot + 1;
            }//if

            else {
		#pragma omp task
		hybrid_quick_sort(pivot + 1, high);
		high = pivot - 1;
            }//else
	}//else 
    }//while
    return 0;
}//hybrid_quick_sort


double get_time(){
    double time;
    # ifdef _OPENMP	
    time = omp_get_wtime();
    # else
    time = clock();
    # endif

    return time;
}//get_time


int fill_array(){

    srand(time(NULL));

    // generate random input
    for (int i = 0; i < MAX_LENGTH; i++){
	array[i] = rand() % MAX_LENGTH;
    }//for

    return 0;
}//fill_array


int main(int argc, char *argv[]){
    char *temp;
    double start_time = 0.0;
    double end_time = 0.0;
    double total_time;
    int number_of_threads;
	
    number_of_threads = strtol(argv[1], &temp, 10);

    fill_array();
	

    start_time = get_time();

    #pragma omp parallel num_threads(number_of_threads) shared(array)
    #pragma omp single nowait
    {
	hybrid_quick_sort(0, MAX_LENGTH - 1);
    }//parallel section
    end_time = get_time();

    total_time = end_time - start_time;
	
    printf("%f\n",total_time);

    //print_results(total_time);

    return 0;
}//main