/*
 *  TCOD:   This quick-and-dirty utility will create a .COD file which 
 *    contains a mixed assembly and Turbo-C source code listing. The
 *    result is similar to that obtained by using the "-Fc" option flag
 *    with Microsoft C v3.0 or v4.0.  This utility has been tested with
 *    Turbo-C v1.00.
 *
 *    To compile this utility:
 *        "tcc -G -O -Z -w tcod.c"
 *        
 *    To obtain a listing of foo.c:
 *        "tcc -S <all other flags> foo.c"
 *        "tcod foo"
 *        
 *    To build an implicit rule for use with MAKE, place the  following
 *    lines into BUILTINS.MAK
 *    .c.cod:
 *       tcc -S $<
 *       tcod $*
 *       del $*.ASM
 *
 *    Beware:  If an included file contains code, we will find "; Line"
 *       comments in the ASM file pointing to the line number of the 
 *       included file.  Unfortunately, we will not have any way to
 *       determine that the code originated from a different source file.
 *       We can do nothing about this.  Let the buyer beware!  I think the
 *       rule here is... don't put code in include files!!
 *
 *    Placed in the public domain 5/19/87 by Lenox Brassell.
 *    [CompuServe PPN: 76224,75].
 *
 *    Modified by Dean McCrory to strip the extension upon startup.
 *    Also changed function headers to match the old format since the new
 *    format is not supported by most compilers.
 */          


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char self[] = "TCOD";  /* Now this code is "self-aware". */
                              /* It knows its own name. */

#define PATHNAMELEN  64
typedef char PATHNAME [PATHNAMELEN];

/* Input and output streams */
static FILE *asm_stream;   /* Input stream for ?.ASM */
static FILE *c_stream;     /* Input stream for ?.C */
static FILE *merge_stream; /* Output stream for ?.COD */


/* Input/output buffer for a singe line of the ASM/COD file */
/* We use a leading tab to make the COD output prettier */
#define ASM_LINEMAX  256
static char tab_asm_line[ASM_LINEMAX+2] = "\t";
#define asm_line &tab_asm_line[1]

/*  This tag indicates that we should look for the given source code line. */
static char c_tag[] = "; Line ";
#define LINE_LEN  (sizeof(c_tag) - 1)  /* Don't compare the '\0' */
#define C_LINE_COMMENT(s)  ((*(s)==';') && strncmp((s),c_tag,LINE_LEN)==0)


/* Function prototypes */
static FILE *fopen_by_extension (char *, char *, char *);
static char *seek_c_line (int);


void main (argc, argv)
   int argc;
   char *argv[];
{
   char *rootname;
   char *c_line;
   int line_number;

   if (argc != 2)
      {
      fprintf (stderr, "%s: Usage is \"%s FILENAME\".\n", self,self);
      exit (1);
      }

   rootname = strchr (argv[1], '.');   /* search for a period */
   if (rootname != NULL)
      *rootname = '\0';                /* cut string off at period */

   rootname = argv[1];
   asm_stream = fopen_by_extension (rootname, ".ASM", "r");
   c_stream = fopen_by_extension (rootname, ".C", "r");
   merge_stream = fopen_by_extension (rootname, ".COD", "w");

   /* For each line of ASM source: */
   while (fgets (asm_line,ASM_LINEMAX,asm_stream) != NULL)
      {
      if (C_LINE_COMMENT(asm_line))
         {
         line_number = atoi (asm_line+LINE_LEN);
         c_line = seek_c_line (line_number);
         if (c_line != NULL)
            fputs (c_line, merge_stream);
         else /* Humph!  We can't find the source line. */
            fputs (asm_line, merge_stream);
         }
      else
         fputs (tab_asm_line, merge_stream);
      }

   exit (0);
}

static FILE *fopen_by_extension (rootname, extension, mode)
   char *rootname;
   char *extension;
   char *mode;
{
   PATHNAME pathname;
   FILE *stream;
    
   stream = fopen (strcat (strcpy (pathname, rootname), extension), mode);
   if (stream == NULL)
      {
      fprintf (stderr, "%s: Cannot open \"%s\"\n", self, pathname);
      perror (pathname);
      exit (1);
      }

   return (stream);
}


#define C_LINE_MAX   256
static char *seek_c_line (line_number)
   int line_number;
{
   static char c_line [C_LINE_MAX+2] = ";";
   static int current_line = 1;

   if (line_number < current_line)
      {
      rewind (c_stream);   
      current_line = 1;
      }

   while (fgets (c_line+1, C_LINE_MAX, c_stream) != NULL)
      {
      if (current_line++ == line_number)
         return (c_line);
      }

   return (NULL);
}
      
