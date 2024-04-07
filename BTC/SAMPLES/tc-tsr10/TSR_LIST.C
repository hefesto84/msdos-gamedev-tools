/*--------------------------------------------------------------------------*
 | Author:                                                                  |
 | Sherif El-Kassas        .       :.                                       |
 | EB dept           \_____o__/ __________                                  |
 | Eindhoven U of Tec       .. /                                            |
 | The Netherlands            /             Email: elkassas@eb.ele.tue.nl   |
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 | The author shall not be liable to the user for any direct, indirect      |
 | or consequential loss arising from the use of, or inability to use,      |
 | any program or file howsoever caused.  No warranty is given that the     |
 | programs will work under all circumstances.                              |
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 | TSR_LIST.C                                                               |
 |   TSR_LIST prints a list of memory resident programs.                    |
 |   TSR_LIST is supplied as is with no warranty, expressed or implied.     |
 |   It is not copyrighted, either.                                         |
 |   Compile with: TCC tsr_list.c                                           |
 *--------------------------------------------------------------------------*/


#include <stdio.h>
#include <dos.h>

/*
 |
 |  Memory control block (arena header) structure:
 |
 |            00  +---------------------------------+
 |                | ID byte 'M' or 'Z'              |
 |            01  +---------------------------------+
 |                | block owner (PSP of owner)      |
 |            03  +---------------------------------+
 |                | memory block size in paragraphs |
 |            05  +---------------------------------+
 |                | reserved                        |
 |            0F  +---------------------------------+
 |
 |
 */

/*--------------------------------------------------------------------------*
 |                         MACRO DEFINITIONS                                |
 *--------------------------------------------------------------------------*/

#define FAR_WORD(seg, ofs)         (* (unsigned far *) MK_FP(seg, ofs) )

#define FAR_CHAR(seg, ofs)         (* (char far *) MK_FP(seg, ofs) )

#define IS_ARENA_HEADER(seg)       ( FAR_CHAR(seg, 0) == 'M' )

#define IS_PSP_ARENA_HEADER(seg)   ( FAR_WORD(seg, 1) == ((seg)+1) )

#define IS_FIRST_ARENA_HEADER(seg) ( IS_ARENA_HEADER( seg )  &&  \
                                     IS_PSP_ARENA_HEADER( seg ) )

#define IS_LAST_ARENA_HEADER(seg)  ( FAR_CHAR(seg, 0) == 'Z')

#define ARENA_SIZE(seg)            ( FAR_WORD(seg, 3) )

#define NEXT_ARENA_HEADER(seg)     ( ARENA_SIZE(seg) + (seg) + 1 )


/*--------------------------------------------------------------------------*
 | print_program_name                                                       |
 |   get program name from environment space (if possible) and print it     |
 *--------------------------------------------------------------------------*/

void print_program_name(register unsigned psp){
  char     far      *env_ptr;      /* far pointer to environment space      */
  register unsigned env_owner;     /* actual owner's psp                    */

  env_owner = FAR_WORD( FAR_WORD(psp, 0x2c) - 1, 1 ); /* get actual owner's */
                                                    /* psp from environment */
  if ( (env_owner == psp) && (_osmajor >= 3) ){
    env_ptr = MK_FP( FAR_WORD(psp, 0x2c), 0 ); /* point to environment      */
    while(* (int far *) env_ptr++) ;          /* scan to end of environment */
    env_ptr += 3;                    /* jump over 'number of strings' field */
    printf("%Fs", env_ptr);              /* print far string (program name) */
  }/* if */
  else printf("(unknown program name)");
}/* print_program_name */


/*--------------------------------------------------------------------------*
 | print_used_ints                                                          |
 |   print interrupts that point to the current psp block                   |
 *--------------------------------------------------------------------------*/

void print_used_ints(register unsigned psp){
  void huge *begin, huge *end; /* huge pointers to block beginning and end  */
  void huge * huge * intr_list = 0L; /* pointer to interrupt table          */
  register  int i;

  begin = MK_FP( psp, 0 );                      /* point to block beginning */
  end   = MK_FP( NEXT_ARENA_HEADER( psp-1 ), 0 ); /* point to block end     */

  printf(" ( ");
  for (i=0; i <= 0xFF; i++){                      /* if an interrupt vector */
    if ( *intr_list > begin && *intr_list < end ) /* points to an address   */
      printf("%X ", i);                           /* between begin and end  */
    intr_list++;                                  /* print the interrupt No.*/
  }/* for */
  printf(" )\n");
}/* print_used_ints */


/*--------------------------------------------------------------------------*
 | main                                                                     |
 |   find first arena header, then print loaded program names (if any)      |
 *--------------------------------------------------------------------------*/

main(){
  register unsigned seg;
  extern   void     _restorezero(void);

  _restorezero(); /* restore interrupt vectors 0, 4, 5, and 6           */


  /* scan memory until a memory control block (arena header) is found   */
  for ( seg = 0; ! IS_FIRST_ARENA_HEADER( seg ); seg++ );

  seg = NEXT_ARENA_HEADER(seg); /* skip the first block (command.com !) */

  /* scan memory until the last memory block is found                   */
  while ( ! IS_LAST_ARENA_HEADER( seg ) ){
    if ( IS_PSP_ARENA_HEADER( seg ) && (_psp != seg+1) ){  /* if seg is */
      print_program_name(seg+1);/* a PSP block and it's not the current */
      print_used_ints(seg+1);   /* program (TSR_LIST.EXE) then print    */
                                /* program name and used interrupts.    */
    }/* if */
    seg = NEXT_ARENA_HEADER( seg );/* point to next memory control block*/
  }/* while */
}/* main */

