/* convert.c -- convert a Fahrenheit temperature to Celsius  */

#include <stdio.h>

main()
{
	float fahr, celsius;

	/*  Get a Fahrenheit value from the user.  */
	printf("Enter a Fahrenheit temperature: ");
	scanf("%f", &fahr );

	/*  Convert it to Celsius and print the result.  */
	celsius = ( fahr - 32 ) * 5 / 9;
	printf("%g Fahrenheit is %g Celsius\n", fahr, celsius );
}
