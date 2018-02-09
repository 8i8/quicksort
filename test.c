#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* from: The Practice of Programming (pp: 32-34)
 *   by: Brian W. Kernighan and Rob Pike
 */
/* swap: interchange v[i] and v[j] */
void swap( int v[], int i, int j )
{
  int tmp;
  tmp = v[i];
  v[i] = v[j];
  v[j] = tmp;
}
/* quicksort: sort v[0]..v[n-1] into increasing
 * order
 */
void quicksort( int v[], int n )
{
  int i = 0, last = 0;
  if ( n <= 1 )              /* nothing to do */
    return;
  swap( v, 0, n/2 );  /* move pivot elem
                                to v[0] */
  for ( i = 1; i < n; i++ )  /* partition */
    if ( v[i] < v[0] )
      swap( v, ++last, i );
  swap( v, 0, last );        /* restore pivot */
  quicksort( v, last );      /* sort smaller
                                values */
  quicksort( v+last+1, n-last-1 );  /* sort larger
                                       values */
}
void print_array( const int array[], int elems )
{
  int i;
  printf("{ ");
  for ( i = 0; i < elems; i++ )
    printf( "%d ", array[i] );
  printf("}\n");
}
#define NUM 9
int main( void )
{
  int arr[NUM] = { 6, 12, 4, 18, 3,
                  27, 16, 15, 19 };
  /* commented out to aid in predictability
   * srand( (unsigned int) time(NULL) ); */
  print_array(arr, NUM);
  quicksort(arr, NUM);
  print_array(arr, NUM);
  return EXIT_SUCCESS;
}
