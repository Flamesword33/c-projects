/* 
 * RockPaperScissors.h
 * by: Nathan Pelletier
 * published: October 3, 2018
 * 
 * Creates a game of rock paper scissors to be played against the computer.
 * Program asks user for their choise of rock, paper or scissors. The 
 * program checks first letter only for the players answer. 
 *
 */
 
/////////////
//LIBRARIES//
/* stdio.h
 * stdlib.h
 * string.h
 */
 
///////////
//GLOBALS//
/* #define TRUE 1
 * #define FALSE 0
 * int player_score;
 * int cpu_score;
 * int ties;
 */

///////////
//METHODS//
/*
 * main()
 * welcome()
 * ask_user_for_input()
 * to_lower_case(char)
 * generate_random_num()
 * find_the_winner_for_this_round(char, int)
 * end_game_early()
 * game_over(int) 
 */
  
/* main()
 * 
 * Welcomes user to a game of rock paper scissors. 
 * Asks the user for input.
 * Generates a random number as the computers choise.
 * Calculates the winner of the current round.
 * Counts how many rounds have passed.
 * Asks the player if they want to continue and checks
 *  if ten rounds have passed.
 * Finally main informs the player of who won over all 
 *  and gives the player a closing statement.
 */
int main();

/* welcome()
 * 
 * Greets the user to the game.
 */
int welcome();

/* ask_user_for_input() --> char
 * 
 * Prompts the user to enter a string. If the user enters a wrong
 *  input the program will infinity ask for a correct input.
 * Returns the first letter of the persons entry in lower case.
 */
char ask_user_for_input();

/* to_lower_case(char) --> char
 * 
 * Takes an upper case (or lower case) letter and converts 
 * it to lower case.
 * Returns the lower case letter.
 */
char to_lower_case(char upper_case_word);


/* find_the_winner_for_this_round(char, int)
 * 
 * Reveals the computers choise.
 * Compairs computers choise to users and determines a winner.
 */
int find_the_winner_for_this_round(char user_choise, int computer_choise);

/* end_game_early() --> Boolean
 *
 * Asks the user whether they are done playing or not.
 * Returns TRUE defined as 1 and FALSE defined as 0.
 * TRUE means player is done. FALSE means player wishes to continue.
 * Bad inputs from the user are interpreted as FALSE.
 */
int end_game_early();

/* game_over()
 * 
 * Announces the end of the game. 
 * Tells user how many wins they had, the computer had
 * and how many ties were made. It also states how many rounds were played in total.
 */
int game_over(int rounds_played);