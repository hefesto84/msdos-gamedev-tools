/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      16 bit bitmap blitting (written for speed, not readability :-)
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 */


#include "asmdefs.inc"
#include "blit.inc"

#ifdef ALLEGRO_COLOR16

.text



/* void _linear_clear_to_color16(BITMAP *bitmap, int color);
 *  Fills a linear bitmap with the specified color.
 */
.globl __linear_clear_to_color16 
   .align 4
__linear_clear_to_color16:
   pushl %ebp
   movl %esp, %ebp
   pushl %ebx
   pushl %esi
   pushl %edi
   pushw %es 

   movl ARG1, %edx               /* edx = bmp */
   movl BMP_CT(%edx), %ebx       /* line to start at */

   movw BMP_SEG(%edx), %es       /* select segment */

   movl BMP_CR(%edx), %esi       /* width to clear */
   subl BMP_CL(%edx), %esi
   cld

   .align 4, 0x90
clear_loop:
   movl %ebx, %eax
   WRITE_BANK()                  /* select bank */
   movl BMP_CL(%edx), %edi 
   leal (%eax, %edi, 2), %edi    /* get line address  */

   movw ARG2, %ax                /* duplicate color twice */
   shll $16, %eax
   movw ARG2, %ax 

   movl %esi, %ecx               /* width to clear */
   shrl $1, %ecx                 /* halve for 32 bit clear */
   jnc clear_no_word
   stosw                         /* clear an odd word */

clear_no_word:
   jz clear_no_long 

   rep ; stosl                   /* clear the line */

clear_no_long:
   incl %ebx
   cmpl %ebx, BMP_CB(%edx)
   jg clear_loop                 /* and loop */

   popw %es
   popl %edi
   popl %esi
   popl %ebx
   movl %ebp, %esp
   popl %ebp
   ret                           /* end of _linear_clear_to_color16() */




/* void _linear_blit16(BITMAP *source, BITMAP *dest, int source_x, source_y, 
 *                                     int dest_x, dest_y, int width, height);
 *  Normal forwards blitting routine for linear bitmaps.
 */
.globl __linear_blit16 
   .align 4
__linear_blit16:
   pushl %ebp
   movl %esp, %ebp
   pushw %es 
   pushl %edi
   pushl %esi
   pushl %ebx

   movl B_DEST, %edx
   movw BMP_SEG(%edx), %es       /* load destination segment */
   movw %ds, %bx                 /* save data segment selector */
   cld                           /* for forward copy */

   shrl $1, B_WIDTH              /* halve counter for long copies */
   jz blit_only_one_word
   jnc blit_even_words

   .align 4, 0x90
   BLIT_LOOP(longs_and_word, 2,  /* long at a time, plus leftover word */
      rep ; movsl
      movsw
   )
   jmp blit_done

   .align 4, 0x90
blit_even_words: 
   BLIT_LOOP(even_words, 2,      /* copy a long at a time */
      rep ; movsl
   )
   jmp blit_done

   .align 4, 0x90
blit_only_one_word: 
   BLIT_LOOP(only_one_word, 2,   /* copy just the one word */
      movsw
   )

   .align 4, 0x90
blit_done:
   popl %ebx
   popl %esi
   popl %edi
   popw %es
   movl %ebp, %esp
   popl %ebp
   ret                           /* end of _linear_blit16() */




/* void _linear_blit_backward16(BITMAP *source, BITMAP *dest, int source_x, 
 *                      int source_y, int dest_x, dest_y, int width, height);
 *  Reverse blitting routine, for overlapping linear bitmaps.
 */
.globl __linear_blit_backward16
   .align 4
__linear_blit_backward16:
   pushl %ebp
   movl %esp, %ebp
   pushw %es 
   pushl %edi
   pushl %esi
   pushl %ebx

   movl B_HEIGHT, %eax           /* y values go from high to low */
   decl %eax
   addl %eax, B_SOURCE_Y
   addl %eax, B_DEST_Y

   movl B_WIDTH, %eax            /* x values go from high to low */
   decl %eax
   addl %eax, B_SOURCE_X
   addl %eax, B_DEST_X

   movl B_DEST, %edx
   movw BMP_SEG(%edx), %es       /* load destination segment */
   movw %ds, %bx                 /* save data segment selector */

   .align 4
blit_backwards_loop:
   movl B_DEST, %edx             /* destination bitmap */
   movl B_DEST_Y, %eax           /* line number */
   WRITE_BANK()                  /* select bank */
   movl B_DEST_X, %edi           /* x offset */
   leal (%eax, %edi, 2), %edi

   movl B_SOURCE, %edx           /* source bitmap */
   movl B_SOURCE_Y, %eax         /* line number */
   READ_BANK()                   /* select bank */
   movl B_SOURCE_X, %esi         /* x offset */
   leal (%eax, %esi, 2), %esi

   movl B_WIDTH, %ecx            /* x loop counter */
   movw BMP_SEG(%edx), %ds       /* load data segment */
   std                           /* backwards */
   rep ; movsw                   /* copy the line */

   movw %bx, %ds                 /* restore data segment */
   decl B_SOURCE_Y
   decl B_DEST_Y
   decl B_HEIGHT
   jg blit_backwards_loop        /* and loop */

   cld                           /* finished */

   popl %ebx
   popl %esi
   popl %edi
   popw %es
   movl %ebp, %esp
   popl %ebp
   ret                           /* end of _linear_blit_backward16() */



.globl __linear_blit16_end
   .align 4
__linear_blit16_end:
   ret




/* void _linear_masked_blit16(BITMAP *source, *dest, int source_x, source_y, 
 *                            int dest_x, dest_y, int width, height);
 *  Masked (skipping zero pixels) blitting routine for linear bitmaps.
 */
.globl __linear_masked_blit16
   .align 4
__linear_masked_blit16:
   pushl %ebp
   movl %esp, %ebp
   pushw %es 
   pushl %edi
   pushl %esi
   pushl %ebx

   movl B_DEST, %edx
   movw BMP_SEG(%edx), %es 
   movw %ds, %bx 
   cld 

   .align 4, 0x90
   BLIT_LOOP(masked, 2,

      movl BMP_VTABLE(%edx), %edx
      movl VTABLE_MASK_COLOR(%edx), %edx

      .align 4, 0x90
   masked_blit_x_loop:
      movw (%esi), %ax           /* read a byte */
      addl $2, %esi

      cmpw %ax, %dx              /* test it */
      je masked_blit_skip

      movw %ax, %es:(%edi)       /* write the pixel */
      addl $2, %edi
      decl %ecx
      jg masked_blit_x_loop
      jmp masked_blit_x_loop_done

      .align 4, 0x90
   masked_blit_skip:
      addl $2, %edi              /* skip zero pixels */
      decl %ecx
      jg masked_blit_x_loop

   masked_blit_x_loop_done:
   )

   popl %ebx
   popl %esi
   popl %edi
   popw %es
   movl %ebp, %esp
   popl %ebp
   ret                           /* end of _linear_masked_blit16() */




#endif      /* ifdef ALLEGRO_COLOR16 */

