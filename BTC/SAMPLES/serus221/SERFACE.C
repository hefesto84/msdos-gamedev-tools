   /********************************************************************/
   /*                                                                  */
   /*  Interface routines for  SERIOUS version 2.21                    */
   /*  by Norman J. Goldstein                                          */
   /*                                                                  */
   /********************************************************************/

   /*********************************************************************
   This file contains  Turbo C  functions to interface with the SERIOUS
   device driver, version  2.21 ; it should be compiled separately, using
   the command line version of Turbo C, as it contains inline code.
   Link it with the application. The file  serdemo.c  contains a model
   program illustrating the use of  SERIOUS  with the routines in
   this file.

   This file also illustrates how to write an assembler interface
   for  SERIOUS , and how to get hold of the entry address of  SERIOUS .
   **********************************************************************/

#pragma inline         /* Tell TC about inline code. */
#include <stdio.h>
#include <dos.h>       /* _doserrno */
#include "serface.h"   /* Prototypes for the routines in this source file. */

#include <io.h>        /* For open , ioctl . */
#include <fcntl.h>     /* For open flags. */

typedef void ( far * PROC)(void);
static PROC EntryAddr;  /* To hold the entry address of  SERIOUS */


/*- - - - - - - - - SetEntryAddress - - - - - - - - - - - - - - - - -
 * The purpose of this routine is to get the entry address of  SERIOUS
 * and store it in the variable  EntryAddr .
 * It is called only when  SERIOUS  is opened by the application.
 *
 * The return values for  SetEntryAddress
 * are  0        -- success,
 *      non-zero -- low-order byte  = E_NoEntryAddress, and
 *                  high-order byte = _doserrno
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
static unsigned SetEntryAddr(char * devname)
{
 int ret,           /* For return values. */
     SeriousHandle; /* A DOS file handle for the driver. */

 if( (SeriousHandle = open(devname,O_RDONLY)) < 0 )
 return _doserrno<<8 | E_NoEntryAddress;

 /* This performs an ioctl read from the driver.
    The driver fills in  EntryAddr  with the appropriate address.
    The code, 4 , tells the driver which information is being requested.
  */
 ret = ioctl(SeriousHandle , 2 , &EntryAddr , 4);
 close( SeriousHandle );

 /* In case of error, return the error code. */
 if( ret == -1 ) return  _doserrno<<8 | E_NoEntryAddress;

  return 0;
}/*SetEntryAddr*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/**********************************************************************
   The remaining functions are direct calls into the SERIOUS device
   driver.  Detailed descriptions are in the documentation, which includes,
   as well, how to interpret the error codes.

   The one execption is the first function  S_Open , which begins with
   a call to  SetEntryAddr .  Also, for  S_Open , the return error code is
   adjusted in the case where the port is specified as  0 , and the driver
   is reported as  Active .  In this case, the return value is changed
   to  0 .  When the port is specified as  0 , the driver is expected to
   be open.

   The "entry" conditions are required by the device driver; setting
   them up is the responsibility of each function.
*** ***********************************************************************/

/*- - - - - - - - - -  Function 0: S_Open - - - - - - - - - - - - - - - - -
 *
 * Purpose: Set the static variable  EntryAddr  to the entry address
 *          of  SERIOUS .  Inform the driver of the user's input buffer,
 *          and set up the hardware interrupt routines.
 *
 * Input: inbptr = the far address of the input buffer.
 *        inbord = the order of the input buffer.
 *        port = base port number of the serial chip, and
 *        irq = interrupt request number of the chip.
 *
 * On entry,  es:di -> 1st byte of input buffer,
 *            cl = order of the buffer,
 *            ax = port number, and
 *            ch = interrupt request number.
 *
 * NOTES:
 * -- If  port  is specified as 0, the driver is assumed to be active; this
 *    function will, then, only obtain the entry address of the driver.
 * -- If  inbord  is specified as 0, the driver uses its own buffer, which
 *    was created at installation via a parameter on the command line
 *    in config.sys .  The user need not supply a buffer in this case.
 *
 * Returns an error code:
 *              0 : Success.
 *        non-zero: If the low order byte equals  E_NoEntryAddress, then
 *                  the return value is the return value of  SetEntryAddr .
 *                  Otherwise, the return value is the error code of  SERIOUS .
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
unsigned S_Open(char * devname , unsigned port , unsigned irq ,
                char far * inbptr , unsigned inbord)
{
 unsigned u;

 /* Sets the static variable  EntryAddr  to the entry address of  SERIOUS . */
 if( (u = SetEntryAddr(devname)) != 0 ) return u;

 /* Now set up the registers for the  open  call into  SERIOUS . */
 asm mov bx , 0
 asm les di , inbptr
 asm mov cl,  byte ptr inbord
 asm mov ch,  byte ptr irq
 asm mov ax , port
 asm call dword ptr EntryAddr

 asm mov u , bx    /* Save the error code in  u . */

 /* Return an appropriate error code if the driver is assumed "open" . */
 if( (port == 0) && (u == ERR_Active) ) return 0;

 return u;
}/* S_Open */


/*- - - - - - - - Function 1: S_SetParms - - - - - - - - - - - - - - -
 *
 * Purpose: Set the transmission parameters.
 *
 * Input: baud = rate of transmission, in bits/second.
 *        data = the number of data bits in each character.
 *        stop = the number of stop bits for each character.
 *        parity is the code to specify the type of parity generated.
 *
 * On entry, ax = baud
 *           ch = stop , cl = data
 *           dx = parity
 *
 * Returns an error code.
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
unsigned S_SetParms( unsigned baud, unsigned data, unsigned stop,
                     unsigned parity )
{
 asm mov bx , 1
 asm mov ax , baud
 asm mov cl , byte ptr data
 asm mov ch , byte ptr stop
 asm mov dx , parity
 asm call dword ptr EntryAddr
 return _BX;
}/* S_SetParms */


/*- - - - - - - - - - - -  Function 2: S_Close - - - - - - - - - - - -
 *
 * Purpose: Disable the UART, and restore the hardware interrupt vectors
 *          to their values before the previous call to  S_Open .  It is
 *          important to close  SERIOUS  before deallocating the input buffer.
 *
 * Returns an error.
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
unsigned S_Close(void)
{
 asm mov bx , 2
 asm call dword ptr EntryAddr
 return _BX;
}/* S_Close */


/*- - - - - - - - - - - Function 3: S_SendChar - - - - - - - - - - - - - -
 *
 * Purpose: To send a character to the remote machine.
 *
 * Input: c = the character to be sent.
 *
 * On entry, al = c .
 *
 * Returns an error.
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
unsigned S_SendChar( char c )
{
 asm mov bx , 3
 asm mov al , c
 asm call dword ptr EntryAddr
 return _BX;
}/* S_Close */


/*- - - - - - - - - - -  Function 4: S_RecvChar - - - - - - - - - - - - - -
 *
 * Purpose: To receive a character from the remote machine.
 *
 * Returns the character received, if there is one.
 *         If there is none, the return value is  -1 .
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
int S_RecvChar(void)
{
 asm mov bx , 4
 asm call dword ptr EntryAddr
 /*
    Note: The return value is already in the  AX  register, having been placed
          there by  SERIOUS .  The error code (in BX) should always be  0 .
  */
}/* S_RecvChar */


/*- - - - - - - - - - -  Function 5: S_SetMode - - - - - - - - - - - -
 *
 * Purpose: To select which interrupts to activate on the UART.
 *
 * Input: al = interrupt mask
 *        A bit is set if the interrupt is to be active.
 *        bit 0 -- Received data.
 *            1 -- Transmitter holding register empty.
 *            2 -- Line error.
 *            3 -- Modem status change.
 *
 * Returns  0 .
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
unsigned S_SetMode( char mode )
{
 asm mov bx , 5
 asm mov al , mode
 asm call dword ptr EntryAddr
 return _BX;
}

/*********************** EOF serface.c ***********************************/
