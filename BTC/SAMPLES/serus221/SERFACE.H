
   /********************************************************************/
   /*                                                                  */
   /*  Header file for Turbo C applications                            */
   /*  using the interface routines in  serface.c                      */
   /*                                                                  */
   /*  by Norman J. Goldstein                                          */
   /*                                                                  */
   /********************************************************************/

/*---------------------------------------------------------------------
These are the prototypes for the interface functions in  serface.c ---*/

unsigned S_Open(char * devname , unsigned port , unsigned irq ,
                char far * inbptr , unsigned inbord);

unsigned S_SetParms( unsigned baud, unsigned data, unsigned stop,
                     unsigned parity );

unsigned S_Close(void);

unsigned S_SendChar( char c );

int S_RecvChar(void);

unsigned S_SetMode( char mode );
/*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
  These are the error codes used by  SERIOUS  version 2.21 . -------------*/

#define     ERR_BadCommand      1
#define     ERR_Active          2
#define     ERR_BadData         3
#define     ERR_BadStop         4
#define     ERR_BadParity       5
#define     ERR_BadBaud         6
#define     ERR_NotActive       7
#define     ERR_BadOrder        8
#define     ERR_UARTsleeps      9
#define     ERR_BadIrq         10

#define     E_NoEntryAddress   0xFE
/*--------------------------------------------------------------------------*/
