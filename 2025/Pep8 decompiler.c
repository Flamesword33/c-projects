/**Pep8 decompiler.c
 * by Nathan Pelletier
 * started June 4 2025
 * 
 * Takes a pepo file full of hex and translates to working assembly
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define True 1
#define False 0

char *interpret_object_code(FILE *object_code){

}//interpret_object_code

int output_file(char *assembly_code){

}//output_file

int main(int argc, char **argv){
    //welp lost the code for reading a c file with a c file... 
    FILE *object_code;
    char *assembly_code;
    printf("%d /n",argc);
    if(argc < 2){
        printf("Please enter a file name to convert./n");
        return 1;
    }
    object_code = fscanf(argv[1], "r");
    assembly_code = interpret_object_code(object_code);
    output_file(assembly_code);
    return 0;
}//main

