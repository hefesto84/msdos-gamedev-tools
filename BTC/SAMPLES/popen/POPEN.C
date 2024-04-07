/* popen/pclose:
 *
 * simple MS-DOS piping scheme to imitate UNIX pipes
 */

#include <stdio.h>
#include <ctype.h>
#include <alloc.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <process.h>

#include "popen.h"

extern char *getenv( char * );

#ifndef	_NFILE
# define	_NFILE	OPEN_MAX	/* Number of open files */
#endif	_NFILE

#define READIT	1			/* Read pipe */
#define WRITEIT	2			/* Write pipe */

static char *prgname[ _NFILE ];		/* program name if write pipe */
static int pipetype[ _NFILE ];		/* 1=read 2=write */
static char *pipename[ _NFILE ];	/* pipe file name */

/*
 *------------------------------------------------------------------------
 * stoupper: Convert string to uppercase (in place)
 *------------------------------------------------------------------------
 */

static void
stoupper( s )
char *s;
{
   int c;
   for( ; (c = *s) != '\0'; ++s ) {
      if( islower( c ) ) *s = _toupper( c );
   }
}

/*
 *------------------------------------------------------------------------
 * strsave: Copy string into malloc'ed memory and return address
 *------------------------------------------------------------------------
 */
 
static char *
strsave( s )
char *s;
{
   char *sp = malloc( strlen( s ) + 1 );
   if( sp != (char *) NULL ) (void) strcpy( sp, s );
   return( sp );
}

/*
 *------------------------------------------------------------------------
 * strfree: Returm strsave'd string memory
 *------------------------------------------------------------------------
 */

static void
strfree( s )
char *s;
{
   if( s != (char *) NULL ) free( s );
}

/*
 *------------------------------------------------------------------------
 * run: Execute command via SHELL or COMSPEC
 *------------------------------------------------------------------------
 */

static int
run( command )
char *command;
{
   jmp_buf panic;			/* How to recover from errors */
   int lineno;				/* Line number where panic happened */
   char *shell;				/* Command processor */
   char *s = (char *) NULL;		/* Holds the command */
   int s_is_malloced = 0;		/* True if need to free 's' */
   static char *command_com = "COMMAND.COM";
   int status;				/* Return codes */
   char *shellpath;			/* Full command processor path */
   char *bp;				/* Generic string pointer */
   static char dash_c[ 3 ] = { '?', 'c', '\0' };
   if( (lineno = setjmp( panic )) != 0 ) {
      int E = errno;
#ifdef	DEMO
      fprintf( stderr, "RUN panic on line %d: %d\n", lineno, E );
#endif	DEMO
      if( s_is_malloced && (s != (char *) NULL) ) strfree( s );
      errno = E;
      return( -1 );
   }
   if( (s = strsave( command )) == (char *) NULL ) longjmp( panic, __LINE__ );
   /* Determine the command processor */
   if( ((shell = getenv( "SHELL" )) == (char *) NULL) &&
       ((shell = getenv( "COMSPEC" )) == (char *) NULL) ) shell = command_com;
   stoupper( shell );
   shellpath = shell;
   /* Strip off any leading backslash directories */
   shell = strrchr( shellpath, '\\' );
   if( shell != (char *) NULL ) ++shell;
   else                         shell = shellpath;
   /* Strip off any leading slash directories */
   bp = strrchr( shell, '/'  );
   if( bp != (char *) NULL ) shell = ++bp;
   if( strcmp( shell, command_com ) != 0 ) {
      /* MKS Shell needs quoted argument */
      char *bp;
      if( (bp = s = malloc( strlen( command ) + 3 )) == (char *) NULL ) 
	 longjmp( panic, __LINE__ );
      *bp++ = '\'';
      while( (*bp++ = *command++) != '\0' );
      *(bp - 1) = '\'';
      *bp = '\0';
      s_is_malloced = 1;
   } else s = command;
   dash_c[ 0 ] = getswitch();
   /* Run the program */
#ifdef	DEMO
   fprintf( stderr, "Running: (%s) %s %s %s\n", shellpath, shell, dash_c, s );
#endif	DEMO
   status = spawnl( P_WAIT, shellpath, shell, dash_c, s, (char *) NULL );
   if( s_is_malloced ) free( s );
   return( status );
}

/* 
 *------------------------------------------------------------------------
 * uniquepipe: returns a unique file name 
 *------------------------------------------------------------------------
 */

static char *
uniquepipe()
{ 
   static char name[ 14 ];
   static short int num = 0;
   (void) sprintf( name, "pipe%05d.tmp", num++ );
   return( name );
}

/*
 *------------------------------------------------------------------------
 * resetpipe: Private routine to cancel a pipe
 *------------------------------------------------------------------------
 */

static void
resetpipe( fd )
int fd;
{
   char *bp;
   if( (fd >= 0) && (fd < _NFILE) ) {
      pipetype[ fd ] = 0;
      if( (bp = pipename[ fd ]) != (char *) NULL ) {
	 (void) unlink( bp );
	 strfree( bp );
	 pipename[ fd ] = (char *) NULL;
      }
      if( (bp = prgname[ fd ]) != (char *) NULL ) {
	 strfree( bp );
	 prgname[ fd ] = (char *) NULL;
      }
   }
}

/* 
 *------------------------------------------------------------------------
 * popen: open a pipe 
 *------------------------------------------------------------------------
 */

FILE *popen( prg, type )
char *prg;			/* The command to be run */
char *type;			/* "w" or "r" */
{ 
   FILE *p = (FILE *) NULL;	/* Where we open the pipe */
   int ostdin;			/* Where our stdin is now */
   int pipefd = -1;		/* fileno( p ) -- for convenience */
   char tmpfile[ BUFSIZ ];	/* Holds name of pipe file */
   char *tmpdir;		/* Points to directory prefix of pipe */
   jmp_buf panic;		/* Where to go if there's an error */
   int lineno;			/* Line number where panic happened */
   /* Find out where we should put temporary files */
   if( (tmpdir = getenv( "TMPDIR" )) == (char *) NULL ) 
      tmpdir = getenv( "TMP" );
   if( tmpdir != (char *) NULL ) {
      /* Use temporary directory if available */
      (void) strcpy( tmpfile, tmpdir );
      (void) strcat( tmpfile, "/" );
   } else *tmpfile = '\0';
   /* Get a unique pipe file name */
   (void) strcat( tmpfile, uniquepipe() );
   if( (lineno = setjmp( panic )) != 0 ) {
      /* An error has occurred, so clean up */
      int E = errno;
#ifdef	DEMO
      fprintf( stderr, "POPEN panic on line %d: %d\n", lineno, E );
#endif	DEMO
      if( p != (FILE *) NULL ) (void) fclose( p );
      resetpipe( pipefd );
      errno = E;
      return( (FILE *) NULL );
   }
   if( strcmp( type, "w" ) == 0 ) {
      /* for write style pipe, pclose handles program execution */
      if( (p = fopen( tmpfile, "w" )) != (FILE *) NULL ) {
	 pipefd = fileno( p );
	 pipetype[ pipefd ] = WRITEIT;
	 pipename[ pipefd ] = strsave( tmpfile );
	 prgname[ pipefd ]  = strsave( prg );
	 if( !pipename[ pipefd ] || !prgname[ pipefd ] ) longjmp( panic, __LINE__ );
      }
   } else if( strcmp( type, "r" ) == 0 ) {
      /* read pipe must create tmp file, set up stdout to point to the temp
      * file, and run the program.  note that if the pipe file cannot be
      * opened, it'll return a condition indicating pipe failure, which is
      * fine.
      */
      if( (p = fopen( tmpfile, "w" )) != (FILE *) NULL ) {
	 int ostdout;
	 pipefd = fileno( p );
	 pipetype[ pipefd ]= READIT;
	 if( (pipename[ pipefd ] = strsave( tmpfile )) == (char *) NULL ) 
	    longjmp( panic, __LINE__ );
	 /* Redirect stdin for the new command */
	 ostdout = dup( fileno( stdout ) );
	 if( dup2( fileno( stdout ), pipefd ) < 0 ) {
	    int E = errno;
	    (void) dup2( fileno( stdout ), ostdout );
	    errno = E;
	    longjmp( panic, __LINE__ );
	 }
	 if( run( prg ) != 0 ) longjmp( panic, __LINE__ );
	 if( dup2( fileno( stdout ), ostdout ) < 0 ) longjmp( panic, __LINE__ );
	 if( fclose( p ) < 0 ) longjmp( panic, __LINE__ );
	 if( (p = fopen( tmpfile, "r" )) == (FILE *) NULL ) longjmp( panic, __LINE__ );
      }
   } else {
      /* screwy call or unsupported type */
      errno = EINVFNC;
      longjmp( panic, __LINE__ );
   }
   return( p );
}

/* close a pipe */

int
pclose( p )
FILE *p;
{
   int pipefd = -1;		/* Fildes where pipe is opened */
   int ostdout;			/* Where our stdout points now */
   int ostdin;			/* Where our stdin points now */
   jmp_buf panic;		/* Context to return to if error */
   int lineno;			/* Line number where panic happened */
   if( (lineno = setjmp( panic )) != 0 ) {
      /* An error has occurred, so clean up and return */
      int E = errno;
#ifdef	DEMO
      fprintf( stderr, "POPEN panic on line %d: %d\n", lineno, E );
#endif	DEMO
      if( p != (FILE *) NULL ) (void) fclose( p );
      resetpipe( pipefd );
      errno = E;
      return( -1 );
   }
   pipefd = fileno( p );
   if( fclose( p ) < 0 ) longjmp( panic, __LINE__ );
   switch( pipetype[ pipefd ] ) {
      case WRITEIT:
	 /* open the temp file again as read, redirect stdin from that
	  * file, run the program, then clean up.
	  */
      if( (p = fopen( pipename[ pipefd ],"r" )) == (FILE *) NULL ) 
	 longjmp( panic, __LINE__ );
      ostdin = dup( fileno( stdin ));
      if( dup2( fileno( stdin ), fileno( p ) ) < 0 ) longjmp( panic, __LINE__ );
      if( run( prgname[ pipefd ] ) != 0 ) longjmp( panic, __LINE__ );
      if( dup2( fileno( stdin ), ostdin ) < 0 ) longjmp( panic, __LINE__ );
      if( fclose( p ) < 0 ) longjmp( panic, __LINE__ );
      resetpipe( pipefd );
      break;
   case READIT:
      /* close the temp file and remove it */
      resetpipe( pipefd );
      break;
   default:
      errno = EINVFNC;
      longjmp( panic, __LINE__ );
      /*NOTREACHED*/
   }
   return( 0 );
}

#ifdef	DEMO
int
main( argc, argv )
int argc;
char **argv;
{
   FILE *pipe;
   char buf[ BUFSIZ ];
   int n;
   *buf = '\0';
   for( n = 1; n < argc; ++n ) {
      (void) strcat( buf, argv[ n ] );
      (void) strcat( buf, " " );
   }
   if( (pipe = popen( buf, "r" )) != (FILE *) NULL ) {
      while( fgets( buf, sizeof( buf ), pipe ) != (char *) NULL ) 
	 (void) fputs( buf, stdout );
      if( pclose( pipe ) != 0 ) fprintf( stderr, "error closing pipe!\n" );
   } else fprintf( stderr, "it didn't work!\n" );
   exit( 0 );
   /*NOTREACHED*/
}
#endif	DEMO
