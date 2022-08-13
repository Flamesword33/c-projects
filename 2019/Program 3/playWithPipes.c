/* playWithPipes.c
 * AUCSC 380
 * by Nathan Pelletier
 * Published: March 2_ 2019
 *
 * This program:
 * 	creates 3 child processes
 *	simulates a paging system
 *		deals with victim frames and empty frames
 *		only 12 frames
 *		each child has four frames
 *		looks for requested page (scanner allowed)
 *		
 *	communicates to children through several pipes
 *	polls child processes in circular order
 *	translates pages for children if they request 
 *		it when the parent checks said child
 *	keeps track of which page is in each frame
 *	returns frame number to child
 *	counts the number of page faults from each child
 *	takes three files containing memory reference strings
 *  Implemented with First in first out and Least recently used
 *		For bonus points, should be selectable from command line with
 *		-f for FIFO, -l for LRU
 *	Starts by printing which (FIFO or LRU) is used
 *	Upon child termination:
 *		print filename, # of page references, # of page faults, 
 *			fault/reference, numbers of pages in memory (in order)
 *	Upon all children terminating:
 *		print total page references, total page faults, average fault rate
 *		assume unsigned short will be recieved in file
 *	
 *
 *  Each child:
 *		simulates a different user process
 *		has one read pipe and one write pipe
 *		reads memory reference strings
 *		requests page translation
 *		exits with EXIT_FAILURE if file is unreadable
 *		prints a message upon termination after done reading
 */

enum{READ = 0, WRITE = 1};


struct Pager{
	int page_number = -1;
	int used_resently = FALSE;
}page;
 
//remember to pipe before forking
//int pipe1[2];
//int pipe2[2];
//pipe(pipe1);
//pipe(pipe2);
//fork(); 
// parent should close pipe1[WRITE], pipe2[READ]
// child should close pipe1[READ], pipe2[WRITE]
// when child finishes then child will close pipes
void doing_things_fifo(int argc, char* argv[]){
	int counter;
	for(counter = 2; counter < argc; counter++){
		start_child(argv[counter]);
		
	}//for to start children
}//doing_things_fifo

void doing_things_lru(int argc, char* argv[]){
	
}//doing_things_lru
 
int main(int argc, char* argv[]){
	if(argv[1] == "-f"){
		doing_things_fifo(argc, argv);
	}//if FIFO
	if(argv[1] == "-l"){
		doing_things_lru(argc, argv);
	}//if LRU
	
	return EXIT_SUCCESS;
}//main