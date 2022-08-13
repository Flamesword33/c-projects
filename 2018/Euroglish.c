/* Euroglish.c
 * by Nathan Pelletier
 * published: October 19 
 *  
 * Translates English to Euroglish
 *   
 *  Year 1
 *   c --> s if followed by e, I or y
 *   else c --> k
 *  Year 2
 *   ph --> f
 *  Year 3 
 *   remove e if at end of word and longer than 3 letters
 *   remove double letters
 *   if end in ed --> d 
 *  Year 4
 *   th --> z
 *   w --> v
 *   remove o if ou
 *   remove a if ea
 *  Year 5
 *   sleep?
 *
 *
 * Additional needs:
 *   Must use a linked list
 *   Must free up memory previously used
 *   Recycle functions wherever possible 
 *   Must handle capitols
 *
 * Bugs and Testing:
 *   Must pass pointers not nodes
 *
 *   fix_your_english --> tested
 *   cut_the_c --> FULLY TESTED
 *   w_is_just_v --> FULLY TESTED
 *   replace_two_letters --> TESTED not confident
 *   remove_doubles --> WORKS, STOP TOUCHING IT
 *   breaking_e --> tested
 *   create_linked_list --> TESTED 
 * (afraid to touch again for fear of breaking)
 *      
 * (last time it broke it took me 3 hours 
 * to fix and im not 100% sure i know how)
 *   
 * 
 * WORKED UNDER ASSUMPTION THAT THE NEXT CHAR WOULD BE KNOWN...
 *  DIDN'T KNOW THAT I WOULD BE READING FILE ONE CHAR AT A TIME... 
 *
 *  SOLUTION 1: CHANGE CODE TO FIT ONE CHAR AT A TIME FORMAT
 *  SOLUTION 2: ONE PASS THROUGH TEXT, TWO PASSES THOUGH LINKED LIST
 *  FIRST TO PUT IN LIST SECOND TO CORRECT THINGS, THIRD TO PRINT
 *    As I am out of time (7 hours overdue) I will be using the second 
 * solution, please note, I would normally rework the 
 * entire project for the increased efficency
 * 
 *
 * IDEA: lower case all chars from user early in and set a bool 
 * on every single node when printing capitalize upon flag
 *
 * IDEA2: Just tell the teacher the program 
 * only gives back lower case words for now...
 *
 * IDEA3: Make separate methods for 
 * uppercase vs lowercase (too much work so no...)
 *
 * CRITICAL ERROR FOUND... forgot i wasn't programming in python... 
 *   its #include <name.h> not #import name.h 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

//creates a node with one char and two pointers called letter_node
typedef struct node{
  char data;
  int is_upper_case;
  struct node *next;
  struct node *last;
} letter_node;

/* fix_your_english(letter_node)
 * loops through a linked list and uses functions to fix mistakes
 */
int fix_your_english(letter_node *english_to_fix){
  letter_node *letter = english_to_fix;
  char checking_on_t;
  int is_not_first_time = FALSE;
  
  while(letter != NULL){
    
    if(is_not_first_time && letter->last->data == letter->data){ 
      //false and error --> false
	  letter = letter->last;
	  remove_doubles(letter->next);
	}//if double is found
	
	switch(letter->data){
	  case 'c':
		cut_the_c(letter);
		break;
	  case 'w':
		w_is_just_v(letter);
		break;
	  case 'p':
	  case 't':
	  case 'o':
		letter = letter->last;
		replace_two_letters(letter->next);
		letter = letter->next;
		break;
	  case 'e':
		letter = letter->last;
	    breaking_e(letter->next);
		letter = letter->next;
	    break;
    }//switch table to find appropriate letter
		
	is_not_first_time = TRUE;
    letter = letter->next;
  }//while
  return 0;
}//fix_your_english
 
/* cut_the_c(letter_node)
 * Takes a linked list and returns the list without c in it
 * c --> s if followed by e, i or y
 * else c --> k
 */
int cut_the_c(letter_node *c){
  letter_node next_letter;
  
  if(c->next == NULL){
    c->data = 'k';
	return 0;
  }//if next letter doesn't exist
  
  next_letter.data = c->next->data;
  next_letter.next = c->next->next;
  next_letter.last = c;
  
  if(next_letter.data == 'e' || 
	next_letter.data == 'i' || 
	next_letter.data == 'y'){
    
    c->data = 's';  
  } //if e i or y are next in the word
  
  else{
    c->data = 'k';  
  }//else c --> k
  
  return 0;
}//cut_the_c


/* w_is_just_v(letter_node)
 * w --> v
 */
int w_is_just_v(letter_node *w){
  w->data = 'v';
  return 0;
} //w_is_just_v


/* replace_two_letters(letter_node) 
 * Takes a linked list and returns list with:
 * ph --> f
 * th --> z
 * ou --> u
 */
int replace_two_letters(letter_node *to_be_replaced){
  letter_node *temp;  //need to preserve sturcture
  
  if (to_be_replaced->next == NULL){
		return 0;  
  }//if next letter doesn't exist
  
	if(to_be_replaced->next->next == NULL){
		temp = malloc(sizeof(letter_node));
		temp->data = ' ';
		temp->last = to_be_replaced->next;
		to_be_replaced->next->next = temp;
	}//if temp won't exist
	
	else{
		temp = to_be_replaced->next->next;
	}//else temp can exist
  
  switch(to_be_replaced->data){
    case 'p':
		  if(to_be_replaced->next->data == 'h'){
		    to_be_replaced->next->last = NULL;
		    to_be_replaced->next = temp;
		    temp->last->next = NULL; 
				temp->last = to_be_replaced;
				to_be_replaced->data = 'f';
			}//if ph
			break;
	
	  case 't':
	    if(to_be_replaced->next->data == 'h'){
		    to_be_replaced->next->last = NULL;
		    to_be_replaced->next = temp;
		    temp->last->next = NULL; 
				temp->last = to_be_replaced;
		    to_be_replaced->data = 'z';  
	    }//if th
	    break;
	  
	  case 'o':
	    if(to_be_replaced->next->data == 'u'){
		    to_be_replaced->next->last = NULL;
		    to_be_replaced->next = temp;
		    temp->last->next = NULL; 
				temp->last = to_be_replaced; 
		    to_be_replaced->data = 'u';
	    }//if ou
	    break;

	default:
	  printf("uh oh you arn't supposed to be here... \n");
	  break;
  }//switch table
	
	return 0;
} //replace_two_letters

/* breaking_e(letter_node)
 * ed --> d
 * ea --> e 
 * removes e if at end of word
 */
int breaking_e(letter_node *e){
  letter_node *word_counter;
  int counter = 1;
  word_counter = e;
  counter = 1;
  
  while(word_counter->last != NULL && isalpha(word_counter->last->data)){
	word_counter = word_counter->last;
	counter = counter + 1;
  }//while to count length of word
	
	if(e->next == NULL && counter > 3){
		remove_e_null(counter, e);
	}//if next is null	
	else if(!isalpha(e->next->data) && counter > 3){
		remove_e(counter, e);
	}//else if next is not a letter of the alphabet	
  else if(e->next->data == 'a'){
		ea_to_e(e);
	}//else if next is a	
  else if(e->next->data == 'd' && 
	(e->next->next == NULL || !isalpha(e->next->next->data))){
		ed_to_d(e);
	}//else if next is d	
	
  return 0;
} //breaking_e

/* ed_to_d(letter_node)
 */
int ed_to_d(letter_node *e){
	
	letter_node *last_letter;
	
	last_letter = e->last;
	e = e->next;
		
	e->last->next = NULL;
	e->last = last_letter;
		
	last_letter->next->last = NULL;
	last_letter->next = e;
	return 0;
}//ed_to_d

/* ea_to_e(letter_node)
 */
int ea_to_e(letter_node *e){
	letter_node *next_letter;

	next_letter = e->next->next;
		
	e->next->last = NULL;
	e->next = next_letter;
		
	next_letter->last->next = NULL;
	next_letter->last = e;
		
  return 0;
}//ea_to_e

/*remove_e(int, letter_node)
 * checks if word is long enough and if e is at end of word
 * if so then removes e
 */
int remove_e(int counter, letter_node *e){
	letter_node *last_letter;
	
	last_letter = e->last;
	e = e->next;
		
	last_letter->next->last = NULL;
	last_letter->next = e;
		
	e->last->next = NULL;
	e->last = last_letter;

	return 0;
}//remove_e

/* remove_e_null(int ,letter_node)
 * removes e if next letter is null 
 */
int remove_e_null(int counter, letter_node *e){
	e->data = ' ';	
	return 0;
}//remove_e_null

/* remove_doubles(letter_node)
 * only use if you know you have doubles 
 * assumes you are on the second letter of the double
 * must assume that the you only have one pointer
 */
int remove_doubles(letter_node *double_letter){
	letter_node *temp = double_letter->last;
	double_letter = double_letter->next;
	//cut from the in front of the linked list

	temp->next->last = NULL;
	temp->next = double_letter;

	//cut from behind the linked list
	double_letter->last->next = NULL;
	double_letter->last = temp;

  return 0;
} //remove_doubles

 
/* create_linked_list(char, letter_node)
 * ??? I think it creates a new node and fills it with 
 * data but this function busted my brains
 */
int create_linked_list(char user_input, letter_node *previous_node){
  previous_node->next = malloc(sizeof(letter_node));
  
	if(islower(user_input) || !isalpha(user_input)){
		previous_node->next->data = user_input;
		previous_node->next->last = previous_node;
		previous_node->next->is_upper_case = 0;
		
	}//if lower case or symbol
	
	else{
		previous_node->next->data = tolower(user_input);		
		previous_node->next->last = previous_node;
		previous_node->next->is_upper_case = 1;
		
	}//uppercase

  return 0;
}//create_linked_list

/* print_list(letter_node)
 * loops through list structure using pointers 
 * and prints out each list value
 */
int print_list(letter_node *start){
  letter_node *location = start->next;
  
  while(location != NULL){
	if(location->is_upper_case){
	  location->data = toupper(location->data);	
	}//if data is upper case
	printf("%c", location->data);
	location = location->next;	
  }//while
  printf("\n");
  return 0;

}//printList 

/* main()
 * opens a file, reads the file and creates a list around it
 * closes file 
 * modifies the list
 * prints out the modified list 
 */
int main(){
	char c;
	FILE *file;
	letter_node first;          //allows me to restart start
	letter_node *start = &first;
	
	start->last = NULL;
	start->data = ' ';
	
	file = fopen("C:/Users/Nathan Pelletier/desktop/c/simp.txt", "r");
	
	if (file){
		printf("ENGLISH: \n");

		
		while (fscanf(file, "%c", &c) != EOF){
			printf("%c", c);
			create_linked_list(c, start);
			start = start->next;	
		}//while
		
		fclose(file);
	}//if
	
	fix_your_english(&first);
	
	printf("\n \n EUROGLISH: \n");
	print_list(&first);
	return 0;
}//main