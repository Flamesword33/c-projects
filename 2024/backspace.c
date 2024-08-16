/**backspace.c
 * by Nathan Pelletier
 * started August 14 2024
 * 
 * Program takes a string from the command line which includes the ^ character. 
 * The ^ character is a substitute for the backspace key (0x0008)
 * Program then removes the character before a backspace key and the backspace key found.
 * 
 * https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/
 * ptr = (int*)malloc(n * sizeof(int));
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define True 1
#define False 0
#define Backspace '^'

/** check_previous_char
 * 
 * Moves backwards through correction_list to find next True
 * value and then sets it to false. 
 * 
 * @param correction_list{int*} A list of bools, should be longer than i
 * @param i{int} An itterator to go backwards through correction_list
 */
void check_previous_char(int* correction_list, int i){
    for(int j = i; j >= 0; j--){
        if(*(correction_list + j) == True){
            *(correction_list + j) = False;
            break;
        }
    }
}//check_previous_char

/** fix_string
 * 
 * Function itterates through original and removes marked characters
 *  
 * @param original{char*} string with characters to correct
 * @param correction_list{int*} a bool list of same size as orginal,
 * 1 - valid character,
 * 0 - invalid character
 * @param new_string{char*} A pointer to a empty string to put the answer in
 */
void fix_string(char* original, int* correction_list){
    char* new_string = (char*)malloc((strlen(original)) * sizeof(char));
    new_string = ""; //clear new_string of garbage data
    int counter = 0;
    for(int i = 0; i < strlen(original); i++){
        if(*(correction_list + i) == True){
            new_string[counter] = *(original + i);
            counter++;
        }
    }
    printf("%s", new_string);
    free(new_string);
}//fix_string

/** backspace
 * 
 * Runs through original_string making a boolean list copy to mark where
 * ^ characters are found, then calls check_previous_char(int*, int).
 * After new bool list is complete it calls and returns fix_string(char*, int*)
 * 
 * @param original_string{char*} Any string containing the ^ character 
 */
void backspace(char* original_string){
    int* correction_list = (int*)malloc(strlen(original_string) * sizeof(int));;
    for(int i = 0; i < strlen(original_string); i++){
        *(correction_list + i) = True;
        if(*(original_string + i) == Backspace){
            correction_list[i] = False;
            check_previous_char(correction_list, i-1);
        }
    }
    fix_string(original_string, correction_list);
    free(correction_list);
}//backspace

/** main
 * Looks for a user string from command line and runs backspace on it.
 * Outputs feedback if string exists or doesn't and outputs finished string.
 */
int main(int argc, char** argv){
    if(argc > 1){
        printf("Inputed string recieved: %s \n", argv[1]);
        backspace(argv[1]);
        return 0;
    }

    else{
        printf("Error, no string entered.");
        return 1;
    }  
}//main

