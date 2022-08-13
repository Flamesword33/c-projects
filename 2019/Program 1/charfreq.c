/* charfreq.c
 * 
 * by Nathan Pelletier
 * AUCSC 380 
 * Published: February 24 2019
 *
 *
 * This program counts the number of times any letter comes up in
 * a given file and prints out the number of occurrences along with 
 * their associated percents.
 */
 
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

///////////
//Globals//
int letter_count[27] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
//each number in letter count is used for one of the 26 letters of the alphabet
//the 27th spot is reserved for oddities like space and 1234567890!@#$%^&*()_+=-;:'"

///////////
//Methods//
//
// count_letter(char)
// final_output() --> int
// main() --> int


/* count_letter(char) 
 *
 * increments a counter based on which letter is passed in
 */
void count_letter(char letter){
	
  if(letter < 'a'){ //this turns all letters lower case
	letter = letter + 32;
  }//if uppercase
	
  //printf("%c", letter); //just for testing for now
	
  switch(letter){
	case 'a':
	  letter_count[0]++;
	  break;
	case 'b' :
	  letter_count[1]++;
	  break;
	case 'c' :
	  letter_count[2]++;
	  break;
	case 'd' :
	  letter_count[3]++;
	  break;
	case 'e' :
	  letter_count[4]++;
	  break;
	case 'f' :
	  letter_count[5]++;
	  break;
	case 'g' :
	  letter_count[6]++;
	  break;
	case 'h' :
	  letter_count[7]++;
	  break;
	case 'i' :
	  letter_count[8]++;
	  break;
	case 'j' :
	  letter_count[9]++;
	  break;
	case 'k' :
	  letter_count[10]++;
	  break;
	case 'l' :
	  letter_count[11] = letter_count[11] + 1;
	  break;
	case 'm' :
	  letter_count[12] = letter_count[12] + 1;
	  break;
	case 'n' :
	  letter_count[13] = letter_count[13] + 1;
	  break;
	case 'o' :
	  letter_count[14] = letter_count[14] + 1;
	  break;
	case 'p' :
	  letter_count[15] = letter_count[15] + 1;
	  break;
	case 'q' :
	  letter_count[16] = letter_count[16] + 1;
	  break;
	case 'r' :
	  letter_count[17] = letter_count[17] + 1;
	  break;
	case 's' :
	  letter_count[18] = letter_count[18] + 1;
	  break;
	case 't' :
	  letter_count[19] = letter_count[19] + 1;
	  break;
	case 'u' :
	  letter_count[20] = letter_count[20] + 1;
	  break;
	case 'v' :
	  letter_count[21] = letter_count[21] + 1;
	  break;
	case 'w' :
	  letter_count[22] = letter_count[22] + 1;
	  break;
	case 'x' :
	  letter_count[23] = letter_count[23] + 1;
	  break;
	case 'y' :
	  letter_count[24] = letter_count[24] + 1;
	  break;
	case 'z' :
	  letter_count[25] = letter_count[25] + 1;
	  break;
	default: 
	  letter_count[26] = letter_count[26] + 1;
  }//switch
}//count_letter

/* final_output() --> int
 *
 * prints out the number of times any given letter was called and their percents
 */
void final_output(){
  double percent_array[26];
  int counter = 0;
  double total = 0;
  
  while(counter < 27){
	total = letter_count[counter] + total;
	counter++;
  }//while
  counter = 0;
  while(counter < 26){
	percent_array[counter] = (letter_count[counter] / total) * 100;
	counter++;
  }//while 
  
  printf("A: %6d ( %2.1f%%)\n", letter_count[0], percent_array[0]);
  printf("B: %6d ( %2.1f%%)\n", letter_count[1], percent_array[1]);
  printf("C: %6d ( %2.1f%%)\n", letter_count[2], percent_array[2]);
  printf("D: %6d ( %2.1f%%)\n", letter_count[3], percent_array[3]);
  printf("E: %6d ( %2.1f%%)\n", letter_count[4], percent_array[4]);
  printf("F: %6d ( %2.1f%%)\n", letter_count[5], percent_array[5]);
  printf("G: %6d ( %2.1f%%)\n", letter_count[6], percent_array[6]);
  printf("H: %6d ( %2.1f%%)\n", letter_count[7], percent_array[7]);
  printf("I: %6d ( %2.1f%%)\n", letter_count[8], percent_array[8]);
  printf("J: %6d ( %2.1f%%)\n", letter_count[9], percent_array[9]);
  printf("K: %6d ( %2.1f%%)\n", letter_count[10], percent_array[10]);
  printf("L: %6d ( %2.1f%%)\n", letter_count[11], percent_array[11]);
  printf("M: %6d ( %2.1f%%)\n", letter_count[12], percent_array[12]);
  printf("N: %6d ( %2.1f%%)\n", letter_count[13], percent_array[13]);
  printf("O: %6d ( %2.1f%%)\n", letter_count[14], percent_array[14]);
  printf("P: %6d ( %2.1f%%)\n", letter_count[15], percent_array[15]);
  printf("Q: %6d ( %2.1f%%)\n", letter_count[16], percent_array[16]);
  printf("R: %6d ( %2.1f%%)\n", letter_count[17], percent_array[17]);
  printf("S: %6d ( %2.1f%%)\n", letter_count[18], percent_array[18]);
  printf("T: %6d ( %2.1f%%)\n", letter_count[19], percent_array[19]);
  printf("U: %6d ( %2.1f%%)\n", letter_count[20], percent_array[20]);
  printf("V: %6d ( %2.1f%%)\n", letter_count[21], percent_array[21]);
  printf("W: %6d ( %2.1f%%)\n", letter_count[22], percent_array[22]);
  printf("X: %6d ( %2.1f%%)\n", letter_count[23], percent_array[23]);
  printf("Y: %6d ( %2.1f%%)\n", letter_count[24], percent_array[24]);
  printf("Z: %6d ( %2.1f%%)\n", letter_count[25], percent_array[25]);
  printf("other symbols used: %d \n", letter_count[26]);
  printf("total characters:   %.0f \n", total);
}//final_output

/* main() --> int
 *
 * scans a file, gives said character to count_letter(char) 
 * once done scanning said file it closes said file and calls final_output()
 */
int main(int argc, char *argv[]) {
  char c;
  FILE *file;
  
  if(argc == 1){
	printf("Failed to read file, please manually input your story:(press ctrl + d to end) \n");
	while (scanf("%c", &c) != EOF){
	  count_letter(c);
	}//while user inputs string
	
	final_output();
	return(EXIT_SUCCESS);
  }//if there is only this file
  
  for (int i = 1; i < argc; i++){
	file = fopen(argv[i], "r");
	if (file){
	  while (fscanf(file, "%c", &c) != EOF){
		//printf("%c", c);
		count_letter(c);
	  }//while
	  
	  fclose(file);
	}//if
  }//for
	
  final_output();
	
  return EXIT_SUCCESS;
}//main