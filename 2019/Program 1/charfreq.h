/* charfreq.h
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
 

///////////
//Globals//
int letter_count[27];

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
void count_letter(char letter);


/* final_output() --> int
 *
 * prints out the number of times any given letter was called and their percents
 */
void final_output();


/* main() --> int
 *
 * scans a file, gives said character to count_letter(char) 
 * once done scanning said file it closes said file and calls final_output()
 */
int main(int argc, char *argv[]);