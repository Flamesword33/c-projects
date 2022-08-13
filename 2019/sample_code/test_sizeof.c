#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
   printf("sizeof( char )  = %d  UCHAR_MAX = %d\n",  sizeof(char), UCHAR_MAX );
   printf("      SCHAR_MIN = %d  SCHAR_MAX = %d\n",  SCHAR_MIN,    SCHAR_MAX );
   printf("      CHAR_MIN  = %d  CHAR_MAX  = %d\n",  CHAR_MIN,     CHAR_MAX );
   printf("sizeof( short ) = %d  USHRT_MAX = %ld\n", sizeof(short),USHRT_MAX );
   printf("      SHRT_MIN  = %d  SHRT_MAX  = %d\n",  SHRT_MIN,     SHRT_MAX );
   printf("sizeof( int )   = %d  UINT_MAX  = %u\n",  sizeof(int),  UINT_MAX );
   printf("      INT_MIN   = %d  INT_MAX   = %d\n",  INT_MIN,      INT_MAX );
   printf("sizeof( long )  = %d  ULONG_MAX = %lu\n", sizeof(long), ULONG_MAX );
   printf("      LONG_MIN  = %ld  LONG_MAX  = %ld\n",LONG_MIN,     LONG_MAX );
   printf("      RAND_MAX  = %ld\n", RAND_MAX );
   printf("sizeof( float ) = %d\n", sizeof( float ));
   printf("sizeof( double )= %d\n", sizeof( double ));
   printf("sizeof( void* )= %d\n", sizeof( void* ));
}
