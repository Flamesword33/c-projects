#include <stdio.h>
#define MAX 200000
#define True 1
#define False 0

void fizz_buzz(int i){
    int is_word = False;
    if(i % 3 == 0){
        printf("Fizz");
        is_word = True;
    }
    if(i % 5 == 0){
        printf("Buzz");
        is_word = True;
    }
    if(is_word == False){
        printf("%d", i);
    }
    printf("\n");

}

int main(){
    for(int i=1; i <= MAX; i++){
        fizz_buzz(i);
    }
    return 0;
}

