/*learning_c++.cpp
  by Nathan Pelletier
  Started June 10, 2023
  
  This file will be a large collection of my knowlege of c++*/

//header files//
/* libraries of other files*/
#include <iostream>
#include <string>

//Macro code, same a c. Used to replace commonly used code snipits
//can also be used to give code more readbility.
//below is an old example of creating a readable boolean in c
#define TRUE 1;
#define FALSE 0;  
//below is a new example of simplifiying for loop construction
#define count_to(x) for (int i=0; i<=x; i++)

// This next line gives us access to the standard library
// we can also call the object by typing std:: before  
//it is considered bad practice to declair a namespace
//as multiple objects can have the same function calls... ugg
//using namespace std;

//Functions//
/*return_type function_name(parameters){}
    in the case of main adding parameters int argc, char ** argv
    alows us to get inputs from the command line*/
void main(int argc, char **argv){
    /*cin and cout are litterally c input and c output
      both are pipleined through <<*/
    std::string name;
    std::cout << "Please enter your name: ";
    std::cin >> name;
    // concatination can be with + or << in c++
    std::cout << "\n Hello " + name;
}

void varibles(){
    int x=5, y=3;  //2-4 bytes, stores whole numbers
    const float pi = 3.14;  //4 bytes works to 6-7 decimal digits
    std::string work = "Lorim ibsom"; //thank you for exiting, char* alt
    char AH = 'A'; // 1 byte
    char ah = 65; //ASCII for A
}