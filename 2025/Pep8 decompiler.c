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

FILE read_file(int argc, char **argv){

}//read_file

FILE interpret_object_code(FILE object_code){

}//interpret_object_code

int output_file(FILE assembly_code){

}//output_file

int main(int argc, char **argv){
    //welp lost the code for reading a c file with a c file... 
    FILE object_code;
    FILE assembly_code;
    object_code = read_file(argc, argv);
    assembly_code = interpret_object_code(object_code);
    output_file(assembly_code);
    return 0;
}//main

