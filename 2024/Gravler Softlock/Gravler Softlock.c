/**Gravler Softlock.c
 * by Nathan Pelletier
 * August 10, 2024
 * 
 * Original coding idea:
 * https://youtu.be/M8C8dHQE2Ro
 * 
 * Original code to convert:
 * https://github.com/arhourigan/graveler/blob/main/graveler.py
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Max 1000000000
#define NumTurns 231


int roll(int dice){
    return (rand() % dice) + 1;
}

int main(int argc, char* argv){
	int program_time = time(NULL);
    int max_one_rolls = 0;
    int current_one_rolls = 0;
    int current_roll = 0;
    int rolls;
    srand(time(NULL));

    for(rolls = 0; rolls < Max; rolls++){
        if(current_one_rolls == 177){
            break;
        }
        current_one_rolls = 0;
        for(int j = 0; j < NumTurns; j++){
            current_roll = roll(4);
            if(current_roll == 1){
                current_one_rolls++;
            }
        }
        if(current_one_rolls > max_one_rolls){
            max_one_rolls = current_one_rolls;
        }
    }

	program_time = time(NULL) - program_time;
    printf("Highest Ones Roll: %d \n", max_one_rolls);
    printf("Number of Roll Sessions: %d \n", rolls);
	printf("Program took %d seconds", program_time);
    return 0;
}