#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void small_palindrome(char small_str[]){
	int left = 0;
	int right = 1;
	printf("%s is Not a Plaindrome", small_str);
	return;
}//small_palindrome

/* A function to check if a string str is palindrome */
void is_palindrome(char str[]){
	//Start from leftmost and rightmost corners of str
	int left = 0;
	int right = strlen(str) - 1;
	
	if (right == 1){
		small_palindrome(str);
		return;
	}//if

	//Keep compairing characters while they are same
	while (right > 1){
		if (str[left++] != str[right--]){
			printf("%s is Not a Palindrome", str);
			return;
		}//if
	}//while	

	printf("%s is palindrome", str);
}//is_palindrome
	
int main(){
	is_palindrome("abba");
	printf("%\n");
	is_palindrome("racecar");
	printf("%\n");
	is_palindrome("hi");
	printf("%\n");
	is_palindrome("I wish i could...");
	return 0;
}//main