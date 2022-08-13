/* 
 * RockPaperScissors.C
 * by: Nathan Pelletier
 * published: October 3, 2018
 * 
 * Creates a game of rock paper scissors to be played against the computer.
 * Program asks user for their choise of rock, paper or scissors. The 
 * program checks first letter only for the players answer.
 * 
 * TO DO LIST:
 *   Welcome user DONE
 *   Generate a random number from 0 to 2 DONE
 *   Ask user for input DONE
 *   give user feed back 
 *     tell them who won DONE
 *     tell them the final tally DONE
 *     ask user to re-input after error DONE 
 *   convert to lowercase  DONE
 *     (remember to stop game after 10 games) DONE
 *     (remember to ask user if want game to stop early) DONE
 *   
 *  
 * Found Bugs:
 *   logic error led to tautology on a while loop derp...
 *   can only use characters on return statements, not strings
 *   void can not be your return value
 *   srand() is being difficult in combination with time
 *   turns out srand() and rand() can't be in a method, they require a loop...
 *   asked user the opposite of what i wanted to... oops
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*fascinating: 0 stops if statements from working 
 * but any other number doesn't 
 */
#define TRUE 1
#define FALSE 0

///////////
//GLOBALS//
int player_score = 0;
int cpu_score = 0;
int ties = 0;

///////////
//METHODS//
/*
 * main()
 * welcome()
 * ask_user_for_input()
 * to_lower_case(char[])
 * generate_random_num()
 * find_the_winner_for_this_round(char[], int)
 * end_game_early()
 * game_over(int) 
 */
 
 
/* basic introduction and welcoming statements */
int welcome(){
  printf("\n\nROCK PAPER SCISSORS \nDRAW \n");
  return 0;
}//welcome

/* changes only first letter to lower case for later method */
char to_lower_case(char upper_case_word){
  upper_case_word = tolower(upper_case_word);
  return upper_case_word;
}//to_lower_case

/*asks user to re-input a bad input */
char fix_bad_input(){
  char fixed_input[100] = "";
  char fixed_char = 'c';
  printf("Error, I don't recognize your input. Please try again : ");
  scanf("%s", fixed_input);
  fixed_char = fixed_input[0];
  return fixed_char;
}//bad_input

/* asks user if they wish to end game early returns true or false */
int end_game_early(){ //returns 0 or 1  
  char end_answer[100] = "";
  char char_end_answer = 'c';
  printf("Give up? \n (Yes or no): ");

  scanf("%s", end_answer);
  char_end_answer = to_lower_case(end_answer[0]);  

  if(char_end_answer == 'y'){ 
    return TRUE;
  }//if
  else{                  //Decided that a wrong input on this one doesn't
    return FALSE;      //matter as much as user can exit program at any time
  }//else 
	
}//end_game_early


/* asks user for their input and returns a char[] for interpretation */
char ask_user_for_input(){
  char user_input[100] = ""; 
  char user_char = 'd';
  printf("Which do you choose? \n(rock, paper or scissors) :");
  scanf("%s", user_input);
  user_char = user_input[0];
  
  user_char = to_lower_case(user_char);

  while(user_char != 'r' &&
          user_char != 'p' && 
          user_char != 's'){
    user_char = fix_bad_input();
    user_char = to_lower_case(user_char);
  }//while
  	
  return user_char;
}//ask_user_for_input


/* Determines what computer picked and then calculates who 
 * won and increments the variable attached to who won.
 *
 * houses a possible error
 */
int find_the_winner_for_this_round
(char user_choice, int computer_choice){
  printf("Computer chose ");
  switch(computer_choice){
    case 0:
      printf("rock. \n");
      break;
    case 1:
      printf("paper. \n");
      break;
    case 2:
      printf("scissors. \n");
      break;
    default:
      printf("Your not supposed to read this. UH OH.");
      break;
  }//switch table to print cpu output

  if((user_choice == 'r' && computer_choice == 0) || 
          (user_choice == 'p' && computer_choice == 1) || 
          (user_choice == 's' && computer_choice == 2)){
    printf("Tie \n \n");
    ties++;
	return 0;
  }//if tie

  else if((user_choice == 'r' && computer_choice == 2) || 
          (user_choice == 'p' && computer_choice == 0) || 
          (user_choice == 's' && computer_choice == 1)){
    printf("You win this round. \n \n");
    player_score++;
	return 0;
  }//else if player wins
	
  else{
	printf("Better luck next time. \n \n");
    cpu_score++;
	return 0;
  }//else computer wins
  		
}//find_the_winner_for_this_round

/* tells the player the final tally and who one overall */
int game_over(int rounds_played){
  printf("Game over, you played %d rounds. \n", rounds_played);
  printf("You won %d games. \n", player_score);
  printf("You tied %d times. \n", ties);
  printf("You lost %d times. \n", cpu_score);
  return 0;
}//game_over

/*
 *
 */
int main(){
  char user_input;
  int cpu_input = 0;
  int number_of_rounds = 0;
  time_t seconds;
  
	
  welcome();
	
  do{
    user_input = ask_user_for_input();
	srand((unsigned) time(&seconds));
    cpu_input = rand() % 3;
    find_the_winner_for_this_round(user_input, cpu_input);
    number_of_rounds++;
    cpu_input = 0;  //just in case cpu_input is toyed with
  }while(!end_game_early() && number_of_rounds < 10);
	
  game_over(number_of_rounds);
	
  return 0;
}//main