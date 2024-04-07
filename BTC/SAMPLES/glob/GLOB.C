/* glob  - glob matching filenames for turbo C */
/* path names must use a / instead of msdos \ */
/* compile with tcc glob.c regexp.obj */
/* $header: glob.c ver 2 Matt Cohen size 2315 time 4/22/88 20:18:54$ */

/*
*
* Name: glob - match files against regular expression
*
* Synopsis: glob pathname
*
* Description: glob uses regexp(3) to match files against the pathname
*		   which may be a regular expression as described in ed(1)
*		   and egrep(1). If a path is not given, the current 
*		   directory is used. The pathname must use '/' instead
*		   of the MSDOS '\' because the \ is interpreted as
*		   part of the regular expression.
*
* Example:	   The following shows all executable files in the 
*		   current dir:	
*			glob "*\.(EXE|BAT|COM)"
*
* Bugs:        Case is insignifigant due to MSDOS . 
*
*/	


#include <stdio.h>
#include <ctype.h>
#define DIRCH '/'
#define SLASH '\\'
#define MODE 0x1f
#include <dir.h>
static char *h="$header: glob.c ver 2 Matt Cohen size 2315 time 4/22/88 20:18:54$";
main(argc,argv)
int argc; char *argv[];
{ 
	struct ffblk ff;
	char dirname[BUFSIZ], *getcwd();
   char pat[BUFSIZ];
	char *a,*b,*c,*regcomp(),*regexec(), *strrchr();
	int i,done;
	c=argv[1]; uppercase(c); strcpy(dirname,c);
   if (*c=='*') /* fix up to .* */
   sprintf(pat,"^.%s$",c);
	else
	sprintf(pat,"^%s$",c);
	if ((b=strrchr(dirname,DIRCH))==NULL) /* no slash - current dir */
   sprintf(dirname,"%s%c",getcwd(NULL,100),DIRCH);
	/* is a slash.. seperate out the slash part and pattern */
	else  { 
			if (*(b+1)=='*') /* fix * into .* */
			sprintf(pat,"^.%s$",b+1);
         else sprintf(pat,"^%s$",b+1);
         *(b+1)='\0'; /* null it out */
			}	
#ifdef DEBUG
			printf("pat is %s\n",pat);
#endif
	if ((a=regcomp(pat))==0) { printf("could not compile\n");
	exit(0);
	}
   for(i=0;dirname[i];i++) if (dirname[i]==SLASH) dirname[i]=DIRCH;
   strcat(dirname,"*.*");
 #ifdef DEBUG
	printf("dirname is %s\n",dirname);
#endif

	done=findfirst(dirname, &ff,MODE);
	
	if (done) { printf("no files\n") ; exit(0);}
	do {
      b=ff.ff_name;
		if (regexec(a,b)) printf("%s\n",b);
		done=findnext(&ff);
		} while (!done);

}

/* convert a string to upper case */
uppercase(s)
char *s;
{
	int i;
	for (i=0; i < strlen(s); i++)
	if(islower(s[i]))
	s[i]=toupper(s[i]);
}	
