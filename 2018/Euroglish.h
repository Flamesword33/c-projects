/* Euroglish.h
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
 */

#include <studio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

typedef struct node letter_node;

/* fix_your_english(letter_node)
 * loops through a linked list and uses functions to fix mistakes
 */
int fix_your_english(letter_node *english_to_fix);

/* cut_the_c(letter_node)
 * Takes a linked list and returns the list without c in it
 * c --> s if followed by e, i or y
 * else c --> k
 */
int cut_the_c(letter_node *c);

/* w_is_just_v(letter_node)
 * w --> v
 */
 int w_is_just_v(letter_node *w);
 
 /* replace_two_letters(letter_node) 
 * Takes a linked list and returns list with:
 * ph --> f
 * th --> z
 * ou --> u
 */
int replace_two_letters(letter_node *to_be_replaced);

/* breaking_e(pointer)
 * ed --> d
 * ea --> e 
 * removes e if at end of word
 */
int breaking_e(letter_node *e);

/* ed_to_d(pointer 
 */
int ed_to_d(letter_node *e);

/* ea_to_e(pointer)
 */
int ea_to_e(letter_node *e);

/*remove_e(int, pointer)
 * checks if word is long enough and if e is at end of word
 * if so then removes e
 */
int remove_e(int counter, letter_node *e);

/* remove_e_null(pointer)
 * removes e if next letter is null 
 */
int remove_e_null(int counter, letter_node *e);

/* remove_doubles(pointer)
 * only use if you know you have doubles 
 * assumes you are on the second letter of the double
 * must assume that the you only have one pointer
 */
int remove_doubles(letter_node *double_letter);

/* create_linked_list(char, pointer)
 * ??? I think it creates a new node and fills it with 
 * data but this function busted my brains
 */
int create_linked_list(char user_input, letter_node *previous_node);

/* print_list(pointer)
 * loops through list structure using pointers 
 * and prints out each list value
 */
int print_list(letter_node *start);

/* main()
 * opens a file, reads the file and creates a list around it
 * closes file 
 * modifies the list
 * prints out the modified list
 */
int main();