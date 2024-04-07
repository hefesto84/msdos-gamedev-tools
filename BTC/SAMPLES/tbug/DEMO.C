/*
                   TURBO C DEBUG DEMO PROGRAM
                   by: Gary L. Mellor    1987
*/
#include <dos.h>
/*                 THESE ARE THE GLOBAL VARIABLES
                   FOR THE DEMO PROGRAM
*/
int gint;
int *gptr;
char gchar;
float gfloat;
char gtemp[80];
/*
                   MAIN DEMO
*/
int main()
{
  int i;

  puts("THIS IS THE TBUG DEMO PROGRAM\n");
  puts("When you are at the debug prompt 'TBUG?' type 'HELP'");
  puts("to get a listing of the tbug commands\n");
  puts("Feel free to use the 'BREAK' command to set a");
  puts("breakpoint at a line number and to execute");
  puts("the main program, and try the other commands\n");
  puts("HAVE FUN!!!\n");

  debug_init("DEMO");

  printf("\n PLEASE ENTER A VALUE FOR global1 ");
  gets(gtemp);
  gptr = &gint;
  gint = atoi(gtemp);
  gchar = gtemp[1];
  gfloat = strlen(gtemp) + 1;
  gint++;
  gfloat =  gfloat / 3;
  for (i = 0; i < 3; i++)
    sub1(gint++);
  puts("MAIN RETURNING");
  return(0);
}
int sub1(x)
int x;
{
  printf("SUB1 PARAMETER = %d\n",x);
  return(x);
}
