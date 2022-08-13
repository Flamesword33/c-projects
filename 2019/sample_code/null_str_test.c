/* stringtest.c - what happens if a string or print function is passed
 *    a null pointer?
 */

#include <stdio.h>
#include <string.h>

main () 
{
  /* Segmentation fault */
  /* 
     printf ( "strlen(0) = %d\n", strlen( 0 )); 
     */

  /* Segmentation fault */
  /* 
     printf ( "About to 'printf(0)'\n" );
     */

  /* Segmentation fault */
  /* 
     printf ( 0 );
     */

  /* Prints '(null)' */
  printf ( "Here is a null string: %s\n", 0 );
}
