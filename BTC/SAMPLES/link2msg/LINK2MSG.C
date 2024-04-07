/***************************************************************\
**                                                             **
**  link2msg - Tlink output filter to the IDE message window.  **
**                                                             **
**  Copyright (c) 1993 by Dale Nurden                          **
**                                                             **
**  Modified from Borland International's Tasm2msg             **
**                                                             **
\***************************************************************/


/*
   This filter accepts input through the standard input stream, converts
   it and outputs it to the standard output stream.  The streams are linked
   through pipes, such that the input stream is the output from Tlink being
   invoked, and the output stream is connected to the message window of the
   IDE, ie.

	      tlink fname | link2msg | IDE message window

   Compile using the LARGE memory model.
*/

#include <dir.h>
#include <dos.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <alloc.h>
#include <io.h>
#include <ctype.h>
#include "filter.h"

#define TRUE (1 == 1)
#define FALSE !(TRUE)

/* Tlink text for non-conversion */
char HeaderMessage[]  = "Turbo Link";

char     CurFile[MAXPATH];       /* Current file in message window */
unsigned BufSize,                /* Size of internal working buffer */
         CurBufLen;              /* Buffer space in use */
char     *InBuffer,              /* Input buffer */
         *OutBuffer,             /* Output buffer */
         *CurInPtr,              /* current character in input buffer */
         *CurOutPtr,             /* current character in output */
         *LinePtr;               /* pointer to the current character in
                                    the input buffer */
char     Line[133];              /* static buffer to store line most recently
                                    input */
long int InOff;                  /* position in actual input stream */
char     EndMark;                /* Used to output end of data to message
                                    window */

/*************************************************************************
Function  : NextChar
Parameters: none
Returns   : next character in input buffer or 0 on end of file

Input from the standard input stream is buffered in a global buffer InBuffer
which is allocated in function main.  The function will return
the next character in the buffer, reading from the input stream when the
buffer becomes empty.
**************************************************************************/
char NextChar(void)
{
  if (CurInPtr < InBuffer+CurBufLen)  /* if buffer is not empty */
  {
    return *(CurInPtr++);             /* return next character */
  }
  else
  {
    CurInPtr = InBuffer;              /* reset pointer to front of buffer */
    lseek(0,InOff,0);                 /* seek to the next section for read */
    InOff += BufSize;                 /* increment pointer to next block */
    if ((CurBufLen = read(0,InBuffer,BufSize)) !=0)
      return NextChar();              /* recursive call merely returns
                                         first character in buffer after read */
    return 0;                         /* return 0 on end of file */
  }
}

/*************************************************************************
Function  : flushOut
Parameter : Size   The number of characters to be written out
Returns   : nothing

Strings to be sent to the message window are buffered in a buffer called
OutBuffer.  A call to this function will write out Size bytes from OutBuffer
to the standard output stream and resets the output buffer pointer to the
beginning of the buffer.  The output buffer is considered empty after a call
to this function.
*************************************************************************/
void flushOut(unsigned Size)
{
   if (Size != 0)                /* don't flush an empty buffer */
   {
      CurOutPtr = OutBuffer;     /* reset pointer to beginning of buffer */
      lseek(1,0,2);              /* seek output stream to end */
      write(1,OutBuffer,Size);   /* write out Size bytes */
   }
}

/************************************************************************
Function  : Put
Parameters: S    pointer to bytes being put into output buffer
            Len  number of bytes to be put in output buffer
Returns   : nothing

Put places bytes into OutBuffer so they may be later flushed out into
the standard output stream.
************************************************************************/
void Put(char *S,int Len)
{
   int i;

   for (i = 0; i < Len; i++)
   {
      *CurOutPtr++ = S[i];                   /* place byte in buffer */
      if (CurOutPtr >= OutBuffer+BufSize)    /* if buffer overflows */
         flushOut(BufSize);                  /* flush the buffer */
   }
}


/*************************************************************************
Function  : ProcessLine
Parameters: Line  a pointer to the current line of characters to process
Returns   : 1 if line is a Tlink line
            0 if line is not
*************************************************************************/
int ProcessLine(char *Line)
{
   char     Type;
   unsigned i;

   /* don't try to process a NULL line */
   if (Line[0] == 0)
      return 0;

   if (strnicmp(Line,HeaderMessage,strlen(HeaderMessage)))
   {
     /* IDE expects the first message to
	   be preceded by a filename.  Since
        we don't have one, fake it by
	   sending a NULL file before the
	   message.
	*/
 	Type = MsgNewFile;                  /* indicate by sending type out to message window */
	*CurFile = '\0';
	Put(&Type,1);
	Put(CurFile,1);                     /* along with null filename */

	Type = MsgNewLine;            /* Fake line # etc.                   */
	i    = 0;
	Put(&Type,1);
	Put((char *)&i,2);
	Put((char *)&i,2);
	while (Line[0] == ' ' && Line[0] != 0)
	  memmove(Line,&Line[1],strlen(Line));
	Put(Line,strlen(Line)+1);
   }
  return 1;
}


/***********************************************************************
Function  : main

Returns   : zero for successful execution
            3    if an error is encountered

The main routine allocates buffers for the input and output buffer.
Characters are then read from the input buffer building the line buffer
that will be sent to the filter processor.  Lines are read and filtered
until the end of input is reached.
***********************************************************************/
int main( void )
{
   char c;
   unsigned long core;

   setmode(1,O_BINARY);               /* set output stream to binary mode */
   core = farcoreleft();
   if (core > 64000U)
      BufSize = 64000U;
   else BufSize = (unsigned)core;     /* get available memory */
                                      /* stay under 64K */
   if ((CurInPtr = malloc(BufSize)) == NULL) /* allocate buffer space */
      exit(3);
   InBuffer = CurInPtr;               /* input buffer is first half of space */
   BufSize = BufSize/2;               /* output buffer is 2nd half */
   OutBuffer = InBuffer + BufSize;
   CurOutPtr = OutBuffer;             /* set buffer pointers */
   LinePtr = Line;
   CurBufLen = 0;
   Put(PipeId,PipeIdLen);             /* send ID string to message window */
   while ((c = NextChar()) != 0)      /* read characters */
   {
      if ((c == 13) || (c == 10))     /* build until line end */
      {
         *LinePtr = 0;
         ProcessLine(Line);           /* filter the line */
         LinePtr = Line;
      }
      /* characters are added to buffer up to 132 characters max */
      else if ((FP_OFF(LinePtr) - FP_OFF(&Line)) < 132)
      {
         *LinePtr = c;                /* add to line buffer */
         LinePtr++;
      }
   }
   *LinePtr = 0;
   ProcessLine(Line);                 /* filter last line */
   EndMark = MsgEoFile;
   Put(&EndMark,1);                   /* indicate end of input to
                                         the message window */
   flushOut((unsigned)(CurOutPtr-OutBuffer));     /* flush the buffer */
   return 0;                          /* return OK */
}

