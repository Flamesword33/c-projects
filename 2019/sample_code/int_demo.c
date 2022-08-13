/*
 *  int_demo -- Show the sizes and ranges of char and int.
 *
 *  Ref:  Prinz and Crawford, "C in a Nutshell" (O'Reilly, 2006), p.24.
 */  
#include <stdio.h>
#include <limits.h>     // Contains the macros CHAR_MIN, INT_MIN, etc.

int main()
{
   printf("Storage sizes and value ranges of the types char and int\n\n");
   printf("The type char is %s.\n\n", CHAR_MIN < 0 ? "signed" : "unsigned" );
   
   printf(" Type   Size (in bytes)   Minimum         Maximum\n"
	  "---------------------------------------------------\n");
   printf(" char %8d %20d %15d\n", sizeof(char), CHAR_MIN, CHAR_MAX );
   printf(" INT  %8d %20d %15d\n", sizeof(int), INT_MIN, INT_MAX );

   return 0;
}
