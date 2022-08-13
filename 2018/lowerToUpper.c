#include <stdio.h>

int main(void) {
    putchar(lower('a'));
	putchar(lower('z'));
	
}//main

lower(a) 
int a; 
{
    if ((a >= 65) && (a >= 90))
        a = a + 32; 
    return a;  
}