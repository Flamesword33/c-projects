#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0


void fizz(int is_fizz){
  if(is_fizz == 0){
    printf("fizz");
  }//if fizz
}//fizz

void buzz(int is_buzz){
  if(is_buzz == 0){
    printf("buzz");
  }//if buzz
}//buzz

void not_fizz_buzz(int current_number, int is_fizz, int is_buzz){

  if(is_fizz != 0 && is_buzz != 0){
    printf("%d", current_number);
  }//if not fizz or buzz
}//not_fizz_buzz

void play_fizz_buzz(){
  int itterator = 0;
  int is_fizz = 0;
  int is_buzz = 0;
  while(itterator < 100){
    itterator = itterator + 1;
    is_fizz = itterator % 3;
	is_buzz = itterator % 5;
    fizz(is_fizz);
	buzz(is_buzz);
	not_fizz_buzz(itterator, is_fizz, is_buzz);
    
    printf("\n");	
  }//while
}//play_fizz_buzz

int main(){
  play_fizz_buzz();
  return 0;
}//main