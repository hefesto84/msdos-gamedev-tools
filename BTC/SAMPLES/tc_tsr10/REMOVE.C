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
 | REMOVE.C                                                                 |
 |   Removes a memory resident utility, and restores interrupt vectors.     |
 |   Remove is supplied as is with no warranty, expressed or implied.       |
 |   It is not copyrighted, either.                                         |
 |   Usage: REMOVE <program name> <interrupt file>                          |
 |   Compile with: TCC remove.c                                             |
 *--------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <dos.h>
#ifdef ANSI_LIB
#define far
#define huge
#define near
#endif

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


typedef unsigned int word;                    /* define 'word' data type    */

#define cli()     __cli__()
#define sti()     __sti__()
#define INTR(x)   __int__(x)

#define ERROR_MESSAGE  "Invalid number of parameters\n\n"  \
                       "Usage: REMOVE <program name>.<ext>" \
                       " <interrupt file name>\n"

/*--------------------------------------------------------------------------*
 | halt                                                                     |
 |   Terminate program, and return rc. Use with care, halt dose not do any  |
 |   cleaning up. (interrupt vectors are not restored, buffers not flushed, |
 |   ... etc)                                                               |
 *--------------------------------------------------------------------------*/

void halt(int rc){
  _AL = rc; _AH = 0x4C;
  INTR(0x21);
}/* halt */

/*--------------------------------------------------------------------------*
 | reset_timer                                                              |
 |   Reset timer to normal speed                                            |
 *--------------------------------------------------------------------------*/

void reset_timer(void){
  cli();
  outportb(0x43, 0x36);
  outportb(0x40, 0);
  outportb(0x40, 0);
  sti();
}/* reset_timer */

/*--------------------------------------------------------------------------*
 | reset_display                                                            |
 *--------------------------------------------------------------------------*/
void reset_display(void){
  _AX = 0x0F00; INTR(0x10); /* get video mode */
  _AH = 0; INTR(0x10); /* set video mode ! */
}/* reset_display */


/*--------------------------------------------------------------------------*
 | first_mcb                                                                |
 |   returns the segment address of the first (interesting) memory control  |
 |   block. (it skips command.com ??)                                       |
 *--------------------------------------------------------------------------*/

word first_mcb(void){
  register word seg;

  for ( seg = 0; ! IS_FIRST_ARENA_HEADER( seg ); seg++ ) ;
  seg = NEXT_ARENA_HEADER(seg); /* skip the first block (command.com ?) */
  return(seg);
}/* first_mcb */


/*--------------------------------------------------------------------------*
 | get_program_name                                                         |
 |   Search environment space and, if successful return a pointer to the    |
 |   program name (no path), otherwise return a NULL pointer.               |
 |   The program name is stored in a static area that is overwritten each   |
 |   call !                                                                 |
 *--------------------------------------------------------------------------*/

char *get_program_name(word psp){
  static   char buffer[80];        /* static buffer for returned string     */
  char     far  *env_ptr;          /* far pointer to environment space      */
  register word env_owner;         /* actual owner's psp                    */
  register char *p=buffer+79;      /* pointer to end of buffer              */

  env_owner = FAR_WORD(FAR_WORD(psp, 0x2c) - 1, 1); /* get actual owner's   */
                                                    /* psp from environment */
  if ( (env_owner == psp) && (_osmajor >= 3) ){
    env_ptr = MK_FP( FAR_WORD(psp, 0x2c), 0 ); /* point to environment      */
    while(* (int far *) env_ptr++) ;          /* scan to end of environment */
    env_ptr += 3; /* jump over 2nd byte of integer zero and 2 bytes number  */
                  /* of strings field                                       */

    while (*env_ptr) env_ptr++;                   /* seek to end of string  */

    while (*env_ptr != '\\' && p > buffer) /* copy name (only) to buffer    */
      *p-- = *env_ptr--;

    return(++p);                         /* return pointer to program name  */
  }
  else return(NULL);                /* return NULL if name can't be found   */
}/* get_program_name */

/*--------------------------------------------------------------------------*
 | get_program_psp_seg                                                      |
 |   Searches for a TSR called 'prog_name', if successful returns the PSP   |
 |   segment address of the program (not the MCB address), otherwise it     |
 |   returns 0.                                                             |
 *--------------------------------------------------------------------------*/
word get_program_psp_seg(char *prog_name, register word seg){
  register char *name;                      /* pointer to retrieved names ! */

  while ( ! IS_LAST_ARENA_HEADER( seg ) ){
    if ( IS_PSP_ARENA_HEADER( seg ) && (_psp != (seg+1)) ){
      name = get_program_name(seg+1);
      if ( (*prog_name=='*') ||   /* Eindhoven 17/5/89 */
         (strcmpi(name, prog_name) == 0) /* compare strings ignoring case   */
         )
        return(seg+1);            /* if a match is found return PSP address */
    }
    seg = NEXT_ARENA_HEADER( seg );
  }
  return(0);                                 /* return 0 if TSR not found   */
}/* get_program_psp_seg */

/*--------------------------------------------------------------------------*
 | get_last_program_psp_seg                                                 |
 |   Return the PSP segment address of the LAST TSR in memory, if non exist |
 |   it returns 0.                                                          |
 |   (Eindhoven 23-8-1990 )                                                 |
 *--------------------------------------------------------------------------*/
word get_last_program_psp_seg(register word seg){
  register word last_seg=0;
  while ( ! IS_LAST_ARENA_HEADER( seg ) ){
    if ( IS_PSP_ARENA_HEADER(seg) && _psp != seg+1 )
      last_seg = seg+1;
    seg = NEXT_ARENA_HEADER( seg );
  }
  return( last_seg );
}/*get_last_program_psp_seg*/

/*--------------------------------------------------------------------------*
 | free_mem                                                                 |
 |   Scan memory, starting at 'seg', and free any blocks that are owned by  |
 |   'prog_seg'                                                             |
 *--------------------------------------------------------------------------*/

void free_mem(register word prog_seg, register word seg){
  while ( ! IS_LAST_ARENA_HEADER( seg ) ){
    if (FAR_WORD(seg, 1) == prog_seg)
      freemem(seg+1);
    seg = NEXT_ARENA_HEADER( seg );
  }
}/* free_mem */


/*--------------------------------------------------------------------------*
 | set_interrupts                                                           |
 |   Reads an interrupt table from 'intr_file_name', and copies it to the   |
 |   system interrupt table.                                                |
 *--------------------------------------------------------------------------*/

void set_interrupts(char *intr_file_name){
  static  void interrupt (* intr_buffer[0x100])(); /* buffer to hold ints.  */
  FILE          *file;
  register word i;

  if ((file = fopen(intr_file_name, "rb")) != NULL){
    fread(intr_buffer, sizeof(void interrupt (*)()), 0x100, file);
    fclose(file);

    for(i=0; i<0x100; i++)
      setvect(i, intr_buffer[i]);
  }
}/* set_interrupts */


/*--------------------------------------------------------------------------*
 | main                                                                     |
 |   Locates program 'av[1]', copies interrupt vectors from 'av[2]' (file), |
 |   and frees all memory blocks that belong to 'av[1]'                     |
 *--------------------------------------------------------------------------*/

main(int ac, char **av){
  register word seg;
  register word prog_psp_seg;

  if (ac > 2){
    seg = first_mcb();                                /* locate first MCB   */

    if (*av[1]=='*')  /*Eindhoven 23-8-1990*/
         prog_psp_seg = get_last_program_psp_seg(seg);
    else prog_psp_seg = get_program_psp_seg(av[1], seg);

    if (prog_psp_seg != 0){                /*if program is loaded, then     */
      reset_timer();                       /* reset timer to normal speed   */
      set_interrupts(av[2]);               /* restore interrupt vectors     */
      reset_display();                     /* reset display characteristics */
      free_mem(prog_psp_seg, seg);         /* free memory                   */
      halt(0);             /* exit program without restoring the interrupts */
                           /* saved by TC's start up code !                 */
    }
    else printf("Program %s not found\n", av[1]);
  }
  else printf(ERROR_MESSAGE);      /* Invalid number of parameters          */

}/* main */
