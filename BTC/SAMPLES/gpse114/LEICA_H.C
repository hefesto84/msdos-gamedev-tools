/*  3/3/93. `Standard' includes.    */


#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>

#define FALSE 0        /*  use enum here?  */
#define TRUE 1
#define FAIL 0
#define OK 1

typedef char *STRING;
typedef int BOOLEAN;
typedef unsigned char BYTE;

/*-------------------------------------------------------------------*/
void wait()
{
	printf("\nany key to continue ... ");
	if (getch()) ;
}
/*-------------------------------------------------------------------*/
