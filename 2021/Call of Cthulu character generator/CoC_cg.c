/* Call of Cthulu NPC generator
 * by Nathan Pelletier
 * started August 27 2021
 */
 
/* Uses srand with mod 6 to simulate a d6 roll
 * Rolls up character using 7th edition rules
 * Exports character to new_character.txt
 */
 
/* CURRENT ERRORS:
 * creates a lot of blank files 
 * Each file is called ./new_character(RANDOM BINARY).txt"
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//stats
#define STR 0
#define CON 1
#define DEX 2
#define APP 3
#define POW 4
#define SIZ 5
#define INT 6
#define EDU 7
#define LUCK 8

//derived_stats index
#define DMG_BONUS 0
#define HP 1
#define MP 2
#define MOVE_RATE 3
#define SANITY 4

int* generate_stats();
int* derive_stats(int* stats, int age);
int derive_dmg_bonus(int build);
int derive_move_rate(int strength, int dexterity, int size, int age);
int* generate_skills();
int* generate_unique_random_numbers(int number_needed, int largest_number);
int output_character_sheet(int age, int* stats, int* derived_stats, int* skills);
void free_pointers(int* stats, int* derived_stats, int* skills);

// TRY CHANGING TO ENUM
//stats index



int main(int argc, char* argv[]){
	srand(time(NULL));   // Initialization, should only be called once.
	
	int age = (rand() % 74) + 15; 
	//printf("Line 56: age: %d\n", age);
	
	int* stats = generate_stats();
	//for(int i = 0; i < 9; i++)
		//printf("Line 60: stat %d: %d\n", i, *(stats + i));
	
	int* derived_stats = derive_stats(stats, age);
	//for(int j = 0; j < 5; j++)
		//printf("Line 64: derived stat %d: %d\n", j, *(derived_stats + j));
	
	int* skills = generate_skills();
	//printf("Line 67\n");
	//for(int k = 0; k < 12; k++)
		//printf("skill %d: %d\n", k, *(skills + k));
	output_character_sheet(age, stats, derived_stats, skills);
	
	free_pointers(stats, derived_stats, skills);
	
	return 0;
}//main


/* generate_stats() --> int*
 *
 * rolls 3d6*5 for STR, CON, DEX, APP, and POW
 * rolls (2d6+6)*5 for SIZ, INT, EDU and LUCK
 * Returns correct output
 */
int* generate_stats(){
	int* stats = malloc(9 * sizeof(int));
	int i;
	
	for(i = 0; i < 5; i++){
		*(stats + i) = ((rand() % 6) + (rand() % 6) + (rand() % 6) + 3) * 5; //3d6*5
		//printf("Line 90: %d: %d \n", i, stats[i]);
	}
	
	for(i = 5; i < 9; i++){
		*(stats + i) = ((rand() % 6) + (rand() % 6) + 8) * 5; //(2d6+6)*5 
		//printf("Line 95: %d: %d \n", i, stats[i]);
	}
	
	return stats;
	
}//generate_stats


int* derive_stats(int* stats, int age){
	int* derived_stats = malloc(5 * sizeof(int));
	int dmg_bonus = derive_dmg_bonus(stats[STR]+stats[SIZ]); 
	int hp = (*(stats + CON) + *(stats + SIZ))/ 10;
	int magic_points = *(stats + POW)/5;
	int move_rate = derive_move_rate(*(stats + STR), *(stats + DEX), *(stats + SIZ), age);
	int sanity = (rand() % (*(stats + POW))) + 1;
	
	*(derived_stats + DMG_BONUS) = dmg_bonus;
	*(derived_stats + HP) = hp;
	*(derived_stats + MP) = magic_points;
	*(derived_stats + MOVE_RATE) = move_rate;
	*(derived_stats + SANITY) = sanity;
	
	return derived_stats;
	
}//derive_stats


int derive_dmg_bonus(int build){ 
	if(build < 65)
		return -2;
	
	else if(build < 85)
		return -1;
	
	else if(build < 125)
		return 0;
	
	else if(build < 165)
		return 1;
	
	else
		return 2;
}//derive_dmg_bonus


int derive_move_rate(int strength, int dexterity, int size, int age){
	int temp_age = age/10; 		//a small hack, I require every ten years from 40-80
	int move_rate;
	if(dexterity < size && strength < size)
		move_rate = 7;
	else if(dexterity > size && strength > size)
		move_rate = 9;
	else
		move_rate = 8;
	
	switch(temp_age){
		case 4:
			return move_rate - 1;
		case 5:
			return move_rate - 2;
		case 6:
			return move_rate - 3;
		case 7:
			return move_rate - 4;
		case 8:
			return move_rate - 5;
		default:
			return move_rate;
	}
}//derive_move_rate


/* generate_skills() --> int*
 * 
 * randomly picks 8 main traits (one of which is CR)
 * These stats will be assigned 80, 70, 60, 60, 50, 50, 50, 40
 * then picks 4 sub stats
 * each will later be assigned 20
 *
 * returns a list of selected skills in order of importance from high to low
 */
int* generate_skills(){
	int* skill_indexes;
	//printf("Line 180\n");

	skill_indexes = generate_unique_random_numbers(12, 46);
	return skill_indexes;
	
}//generate_skills


/* generate_unique_random_numbers(int, int) --> int*
 *
 * Asks for how many numbers to generate and what range they should be
 * returns a list of unique random numbers
 * All numbers generated are positive between 0 and range set
 *
 * WARNINGS: (1) will loop infinately if largest_number < number_needed
 *			 (2) becomes horribly ineficient if largest_number is close to number_needed
 *
 */
int* generate_unique_random_numbers(int number_needed, int largest_number){
	int temp;
	//int list_of_numbers[number_needed]; //unknown error so had to hard code :|
	int list_of_numbers[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int* list_to_return = malloc(12 * sizeof(int));
	int h;
	
	//printf("line 207 Check for uniqueness\n");
	for(h = 0; h < number_needed; h++){
		do{
			temp = rand() % largest_number;
		}while(temp == list_of_numbers[0] || 
			temp == list_of_numbers[1] ||
			temp == list_of_numbers[2] ||
			temp == list_of_numbers[3] ||
			temp == list_of_numbers[4] ||
			temp == list_of_numbers[5] ||
			temp == list_of_numbers[6] ||
			temp == list_of_numbers[7] ||
			temp == list_of_numbers[8] ||
			temp == list_of_numbers[9] ||
			temp == list_of_numbers[10] ||
			temp == list_of_numbers[11]);
		list_of_numbers[h] = temp;
		*(list_to_return + h) = list_of_numbers[h];
		//printf("Line 225 %d: %d\n", h, list_of_numbers[h]);

	}//for
									
	return list_to_return;
	
}//generate_unique_random_numbers

/*
 *
 * 80, 70, 60, 60, 50, 50, 50, 40, 20, 20, 20, 20
 */
int output_character_sheet(int age, int* stats, int* derived_stats, int* skills){
	int temp;
	FILE* file = fopen("./new_character.txt", "w");
	char* list_of_skills[47] = {"Accounting", "Anthropology", "Appraise", "Archaeology", "Art/Craft",
		"Charm", "Climb", "Cthulhu Mythos", "Disguise", "Dodge", 
		"Drive Auto", "Elec Repair", "Fast Talk", "Fighting(Brawl)", "Fighting(???)", 
		"Firearms(Handgun)", "Firearms(Rifle/Shotgun)", "Firearms(???)", "First aid", "History", 
		"Intimidate", "Jump", "Language(Other)", "Language(own)", "Law", 
		"Library Use", "Listen", "Locksmith", "Mech Repair", "Medicine", 
		"Natural World", "Navigate", "Occult", "Op. Hv. Machine", "Persuade", 
		"Pilot", "Psychology", "Psychoanalysis", "Ride", "Science", 
		"Sleight of Hand", "Spot Hidden", "Stealth", "Survival", "Swim", 
		"Throw", "Track"};
	int skill_index[12] = {*(skills + 0), *(skills + 1), *(skills + 2), *(skills + 3), *(skills + 4),
		*(skills + 5), *(skills + 6), *(skills + 7), *(skills + 8), 
		*(skills + 9), *(skills + 10), *(skills + 11)};
	
	temp = fprintf(file, "HP: %d", *(derived_stats + HP));
	//printf("Line 255 %d \n", temp);
	fprintf(file, "  MP: %d", *(derived_stats + MP));
	fprintf(file, "  sanity: %d", *(derived_stats + SANITY));
	fprintf(file, "  age: %d \n", age);
	
	fprintf(file, "STR  DEX  INT  CON  APP  POW  SIZ  EDU  Move Rate\n");
	fprintf(file, "%d   %d   %d   %d   %d   %d   %d   %d   %d \n\n",
		*(stats + STR), *(stats + DEX), *(stats + INT), *(stats + CON), 
		*(stats + APP), *(stats + POW), *(stats + SIZ), *(stats + EDU),
		*(derived_stats + MOVE_RATE));
		
	fprintf(file, "Skills: \n  %s:80 \n  %s:70 \n  %s:60 \n  %s:60 \n  %s:50 \n  %s:50 \n  %s:50",
		list_of_skills[skill_index[0]], list_of_skills[skill_index[1]], list_of_skills[skill_index[2]], list_of_skills[skill_index[3]], 
		list_of_skills[skill_index[4]], list_of_skills[skill_index[5]], list_of_skills[skill_index[6]]);
	temp = fprintf(file, "\n  %s:40 \n  %s:20 \n  %s:20 \n  %s:20 \n  %s:20",
		list_of_skills[skill_index[7]], list_of_skills[skill_index[8]], list_of_skills[skill_index[9]], list_of_skills[skill_index[10]], 
		list_of_skills[skill_index[11]]);
	//printf("Line 272 temp:%d \n", temp);
	fclose(file);
	
	return 0;
}//output_character_sheet


void free_pointers(int* stats, int* derived_stats, int* skills){
	//printf("Line 280\n");
	realloc(stats, 9 * sizeof(int));
	realloc(derived_stats, 5 * sizeof(int));
	realloc(skills, 12 * sizeof(int));
}//free_pointers
