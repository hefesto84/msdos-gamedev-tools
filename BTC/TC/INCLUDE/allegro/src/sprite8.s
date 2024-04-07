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
 *      256 color sprite drawing (written for speed, not readability :-)
 *
 *      By Shawn Hargreaves.
 *
 *      draw_sprite() and draw_trans_sprite() optimised by Erik Sandberg.
 *
 *      See readme.txt for copyright information.
 */


#include "asmdefs.inc"
#include "sprite.inc"

.text



/* void _linear_draw_sprite8(BITMAP *bmp, BITMAP *sprite, int x, y);
 *  Draws a sprite onto a linear bitmap at the specified x, y position, 
 *  using a masked drawing mode where zero pixels are not output.
 */
.globl __linear_draw_sprite8


   /* loop for sprites more than 7 pixels wide */
   /* dest: destination seg. */
   /* name: the start of the label names */
   /* init_edi: code to init edi every line */

   #define LINEAR_SPRITE_LOOP(dest, name, init_edi...)                       \
      movl S_Y, %eax                         /* load line */               ; \
      movl S_W, %edx                         /* convert x loop counter */  ; \
      addl %edx, S_SGAP                      /* increase sprite gap */     ; \
      subl $4, %edx                          /* less iterations */         ; \
      addl %eax, S_H                         /* S_H = end y */             ; \
      addl %edx, %esi                        /* end of src line in esi */  ; \
      movl S_X, %edi                         /* edi = left edge of dest */ ; \
      addl %edx, %edi                        /* add width to left edge */  ; \
      movl %edi, S_X                         /* store right edge */        ; \
      negl %edx                              /* counter/offset negative */ ; \
      movl %edx, S_W                         /* store converted counter */ ; \
									   ; \
   sprite_y_loop_##name:                                                   ; \
      movl S_BMP, %edx                       /* load bitmap pointer */     ; \
      pushl %ebp                                                           ; \
      movl S_W, %ebp                         /* x loop counter */          ; \
      init_edi                                                             ; \
      leal (%esi, %ebp), %eax                /* check unaligned bytes */   ; \
      testb $1, %al                                                        ; \
      jz sprite_##name##_before_waligned                                   ; \
      movb (%esi, %ebp), %bl                 /* write unaligned byte */    ; \
      incl %ebp                                                            ; \
      testb %bl, %bl                                                       ; \
      jz sprite_##name##_before_waligned                                   ; \
      movb %bl, dest -1(%edi, %ebp)                                        ; \
   sprite_##name##_before_waligned:                                        ; \
      incb %al                                                             ; \
      testb $2, %al                                                        ; \
      jz sprite_##name##_before_laligned                                   ; \
      movb (%esi, %ebp), %bl                 /* write unaligned word */    ; \
      addl $2, %ebp                                                        ; \
      testb %bl, %bl                                                       ; \
      jz sprite_##name##_before_lalign_skip_0                              ; \
      movb %bl, dest -2(%edi, %ebp)                                        ; \
   sprite_##name##_before_lalign_skip_0:                                   ; \
      movb -1(%esi, %ebp), %bl                                             ; \
      testb %bl, %bl                                                       ; \
      jz sprite_##name##_before_laligned                                   ; \
      movb %bl, dest -1(%edi, %ebp)                                        ; \
   sprite_##name##_before_laligned:          /* now src is long-aligned */ ; \
									   ; \
      /* copy single long if necessary */                                  ; \
      xorl %eax, %eax                                                      ; \
      subl %ebp, %eax                                                      ; \
      testl $4, %eax                                                       ; \
      jnz sprite_##name##_no_single_long                                   ; \
      movl (%esi, %ebp), %ecx                                              ; \
      subl $4, %ebp                                                        ; \
      jmp sprite_x_loop_##name##_skip_all                                  ; \
   sprite_##name##_no_single_long:                                         ; \
      movl (%esi, %ebp), %eax                                              ; \
      movl 4(%esi, %ebp), %ecx                                             ; \
									   ; \
   sprite_x_loop_##name##_empty:    /* part 1 of unrolling 1 of loop 1 */  ; \
      test %eax, %eax                                                      ; \
      jz sprite_x_loop_##name##_skip_all                                   ; \
      leal -0x01010101(%eax), %ebx                                         ; \
      leal -0x01010101(%ecx), %edx                                         ; \
      xorl %eax, %ebx                                                      ; \
      xorl %ecx, %edx                                                      ; \
      andl $0x80808080, %ebx                                               ; \
      jnz sprite_x_loop_##name##_empty_2                                   ; \
   sprite_x_loop_##name##_full_2:   /* part 2 of unrolling 1 of loop 2 */  ; \
      movl %eax, dest (%edi, %ebp)                                         ; \
      movl 8(%esi, %ebp), %eax                                             ; \
      andl $0x80808080, %edx                                               ; \
      jz sprite_x_loop_##name##2_full                                      ; \
      test %ecx, %ecx                                                      ; \
      jz sprite_x_loop_##name##2_skip_3                                    ; \
   sprite_x_loop_##name##2_empty_2: /* part 2 of unrolling 2 of loop 1 */  ; \
      testb %cl, %cl                                                       ; \
      jz sprite_x_loop_##name##2_skip_0                                    ; \
      movb %cl, dest 4(%edi, %ebp)                                         ; \
   sprite_x_loop_##name##2_skip_0:                                         ; \
      testb %ch, %ch                                                       ; \
      jz sprite_x_loop_##name##2_skip_1                                    ; \
      movb %ch, dest 5(%edi, %ebp)                                         ; \
   sprite_x_loop_##name##2_skip_1:                                         ; \
      shrl $16, %ecx                                                       ; \
      testb %cl, %cl                                                       ; \
      jz sprite_x_loop_##name##2_skip_2                                    ; \
      movb %cl, dest 6(%edi, %ebp)                                         ; \
   sprite_x_loop_##name##2_skip_2:                                         ; \
      testb %ch, %ch                                                       ; \
      jz sprite_x_loop_##name##2_skip_3                                    ; \
      movb %ch, dest 7(%edi, %ebp)                                         ; \
   sprite_x_loop_##name##2_skip_3:  /* part 3 of unrolling 2 of loop 1 */  ; \
      movl 12(%esi, %ebp), %ecx                                            ; \
      movl 8(%esi, %ebp), %eax                                             ; \
      addl $8, %ebp                                                        ; \
      jl sprite_x_loop_##name##_empty                                      ; \
      jmp sprite_x_loop_##name##_done                                      ; \
									   ; \
   sprite_x_loop_##name##_skip_all: /* skip long 1, check long 2 */        ; \
      test %ecx, %ecx                                                      ; \
      jz sprite_x_loop_##name##2_skip_3                                    ; \
      leal -0x01010101(%ecx), %edx                                         ; \
      xorl %ecx, %edx                                                      ; \
      jmp sprite_x_loop_##name##2_empty                                    ; \
									   ; \
   sprite_x_loop_##name##_full:     /* part 1 of unrolling 1 of loop 2 */  ; \
      leal -0x01010101(%eax), %ebx                                         ; \
      leal -0x01010101(%ecx), %edx                                         ; \
      xorl %eax, %ebx                                                      ; \
      xorl %ecx, %edx                                                      ; \
      andl $0x80808080, %ebx                                               ; \
      jz sprite_x_loop_##name##_full_2                                     ; \
      test %eax, %eax                                                      ; \
      jz sprite_x_loop_##name##_skip_all                                   ; \
   sprite_x_loop_##name##_empty_2:  /* part 2 of unrolling 1 of loop 1 */  ; \
      testb %al, %al                                                       ; \
      jz sprite_x_loop_##name##_skip_0                                     ; \
      movb %al, dest (%edi, %ebp)                                          ; \
   sprite_x_loop_##name##_skip_0:                                          ; \
      testb %ah, %ah                                                       ; \
      jz sprite_x_loop_##name##_skip_1                                     ; \
      movb %ah, dest 1(%edi, %ebp)                                         ; \
   sprite_x_loop_##name##_skip_1:                                          ; \
      shrl $16, %eax                                                       ; \
      testb %al, %al                                                       ; \
      jz sprite_x_loop_##name##_skip_2                                     ; \
      movb %al, dest 2(%edi, %ebp)                                         ; \
   sprite_x_loop_##name##_skip_2:                                          ; \
      testb %ah, %ah                                                       ; \
      jz sprite_x_loop_##name##_skip_3                                     ; \
      movb %ah, dest 3(%edi, %ebp)                                         ; \
   sprite_x_loop_##name##_skip_3:   /* part 1 of unrolling 2 of loop 1 */  ; \
      test %ecx, %ecx                                                      ; \
      jz sprite_x_loop_##name##2_skip_3                                    ; \
   sprite_x_loop_##name##2_empty:   /* part 2 of unrolling 2 of loop 1 */  ; \
      andl $0x80808080, %edx                                               ; \
      jnz sprite_x_loop_##name##2_empty_2                                  ; \
      movl 8(%esi, %ebp), %eax                                             ; \
   sprite_x_loop_##name##2_full:    /* unrolling 2 of loop 2 */            ; \
      movl %ecx, dest 4(%edi, %ebp)                                        ; \
      movl 12(%esi, %ebp), %ecx                                            ; \
      addl $8, %ebp                                                        ; \
      jl sprite_x_loop_##name##_full                                       ; \
   sprite_x_loop_##name##_done:                                            ; \
									   ; \
      movl %ebp, %eax                                                      ; \
      testb $1, %al                                                        ; \
      jz sprite_##name##_after_waligned                                    ; \
      movb (%esi, %ebp), %bl                 /* write leftover byte */     ; \
      incl %ebp                                                            ; \
      testb %bl, %bl                                                       ; \
      jz sprite_##name##_after_waligned                                    ; \
      movb %bl, dest -1(%edi, %ebp)                                        ; \
   sprite_##name##_after_waligned:                                         ; \
      incb %al                                                             ; \
      testb $2, %al                                                        ; \
      jz sprite_##name##_after_laligned                                    ; \
      movb (%esi, %ebp), %bl                 /* write leftover bytes */    ; \
      addl $2, %ebp                                                        ; \
      testb %bl, %bl                                                       ; \
      jz sprite_##name##_after_lalign_skip_0                               ; \
      movb %bl, dest -2(%edi, %ebp)                                        ; \
   sprite_##name##_after_lalign_skip_0:                                    ; \
      movb -1(%esi, %ebp), %bl                                             ; \
      testb %bl, %bl                                                       ; \
      jz sprite_##name##_after_laligned                                    ; \
      movb %bl, dest -1(%edi, %ebp)                                        ; \
   sprite_##name##_after_laligned:           /* line is complete */        ; \
									   ; \
      popl %ebp                              /* need stack variables */    ; \
      movl S_Y, %eax                         /* y counter */               ; \
      incl %eax                                                            ; \
      movl S_X, %edi                         /* reload dest x position */  ; \
      movl %eax, S_Y                         /* store counter */           ; \
      addl S_SGAP, %esi                      /* skip sprite bytes */       ; \
      cmpl S_H, %eax                         /* check if complete */       ; \
      jl sprite_y_loop_##name


   /* draws a tiny sprite (width < 8) onto a video or memory bitmap */
   #define LOOP_TINY                                                         \
      movl S_Y, %eax                         /* load line */               ; \
      movl S_W, %edx                         /* convert x loop counter */  ; \
      addl %edx, S_SGAP                      /* increase sprite gap */     ; \
      addl %eax, S_H                         /* S_H = end y */             ; \
      addl %edx, %esi                        /* end of src line in esi */  ; \
      movl S_X, %edi                         /* edi = left edge of dest */ ; \
      addl %edx, %edi                        /* add width to left edge */  ; \
      decl %edi                                                            ; \
      movl %edi, S_X                         /* store right edge */        ; \
      negl %edx                              /* counter/offset negative */ ; \
      movl %edx, S_W                         /* store converted counter */ ; \
   sprite_y_loop_tiny:                                                     ; \
      movl S_BMP, %edx                                                     ; \
      movl S_Y, %eax                         /* load line */               ; \
      WRITE_BANK()                           /* select bank */             ; \
      addl S_X, %eax                         /* add x offset */            ; \
      movl S_W, %ecx                         /* x loop counter */          ; \
      movb (%esi, %ecx), %bl                                               ; \
      jmp sprite_tiny_next_pixel                                           ; \
   sprite_tiny_write_pixel:                                                ; \
      movb %bl, %es:(%eax, %ecx)                                           ; \
      movb (%esi, %ecx), %bl                                               ; \
      jz sprite_tiny_line_complete                                         ; \
   sprite_tiny_next_pixel:                                                 ; \
      cmpb $1, %bl                                                         ; \
      incl %ecx                                                            ; \
      jnc sprite_tiny_write_pixel                                          ; \
      movb (%esi, %ecx), %bl                                               ; \
      jnz sprite_tiny_next_pixel                                           ; \
   sprite_tiny_line_complete:                                              ; \
      movl S_Y, %eax                         /* y counter */               ; \
      addl S_SGAP, %esi                      /* skip sprite bytes */       ; \
      incl %eax                                                            ; \
      movl %eax, S_Y                         /* store counter */           ; \
      cmpl S_H, %eax                         /* check if complete */       ; \
      jl sprite_y_loop_tiny


   /* the actual sprite drawing routine... */
   .align 4
__linear_draw_sprite8:
   START_SPRITE_DRAW(sprite)

   movl BMP_W(%esi), %eax        /* sprite->w */
   subl S_W, %eax                /* - w */
   movl %eax, S_SGAP             /* store sprite gap */

   movl S_LGAP, %eax
   addl %eax, S_X                /* X += lgap */

   movl S_TGAP, %eax 
   addl %eax, S_Y                /* Y += tgap */

   movl BMP_LINE(%esi, %eax, 4), %esi
   addl S_LGAP, %esi             /* esi = sprite data ptr */

   cmpl $8, S_W
   jl sprite_tiny                /* sprite width < 8, tiny */

   movl S_BMP, %eax              /* check if dest is video memory */
   cmpl $0, BMP_DAT(%eax)
   jz sprite_video

				 /* dest = memory bitmap */
   LINEAR_SPRITE_LOOP( , mem, addl BMP_LINE(%edx, %eax, 4), %edi ; )
   jmp sprite_done

sprite_video:                    /* dest = video memory bitmap */
   LINEAR_SPRITE_LOOP(%es: , video, WRITE_BANK(); addl %eax, %edi ; )
   jmp sprite_done

sprite_tiny:
   LOOP_TINY

   .align 4, 0x90
sprite_done:
   END_SPRITE_DRAW()
   ret                           /* end of _linear_draw_sprite8() */

.globl __linear_draw_sprite8_end
   .align 4
__linear_draw_sprite8_end:
   ret




/* void _linear_draw_sprite_v_flip8(BITMAP *bmp, BITMAP *sprite, int x, y);
 *  Draws a sprite to a linear bitmap, flipping vertically.
 */
.globl __linear_draw_sprite_v_flip8
   .align 4
__linear_draw_sprite_v_flip8:
   START_SPRITE_DRAW(sprite_v_flip)

   movl BMP_W(%esi), %eax        /* sprite->w */
   addl S_W, %eax                /* + w */
   negl %eax
   movl %eax, S_SGAP             /* store sprite gap */

   movl S_LGAP, %eax
   addl %eax, S_X                /* X += lgap */

   movl S_TGAP, %eax 
   addl %eax, S_Y                /* Y += tgap */

   negl %eax                     /* - tgap */
   addl BMP_H(%esi), %eax        /* + sprite->h */
   decl %eax
   movl BMP_LINE(%esi, %eax, 4), %esi
   addl S_LGAP, %esi             /* esi = sprite data ptr */

   .align 4, 0x90
   SPRITE_LOOP(v_flip) 
   movb (%esi), %bl              /* read pixel */
   testb %bl, %bl                /* test */
   jz sprite_v_flip_skip 
   movb %bl, %es:(%eax)          /* write */
sprite_v_flip_skip: 
   incl %esi 
   incl %eax 
   SPRITE_END_X(v_flip)
   SPRITE_END_Y(v_flip)

sprite_v_flip_done:
   END_SPRITE_DRAW()
   ret                           /* end of _linear_draw_sprite_v_flip8() */




/* void _linear_draw_sprite_h_flip8(BITMAP *bmp, BITMAP *sprite, int x, y);
 *  Draws a sprite to a linear bitmap, flipping horizontally.
 */
.globl __linear_draw_sprite_h_flip8
   .align 4
__linear_draw_sprite_h_flip8:
   START_SPRITE_DRAW(sprite_h_flip)

   movl BMP_W(%esi), %eax        /* sprite->w */
   addl S_W, %eax                /* + w */
   movl %eax, S_SGAP             /* store sprite gap */

   movl S_LGAP, %eax
   addl %eax, S_X                /* X += lgap */

   movl S_TGAP, %eax 
   addl %eax, S_Y                /* Y += tgap */

   movl BMP_W(%esi), %ecx 
   movl BMP_LINE(%esi, %eax, 4), %esi
   addl %ecx, %esi
   subl S_LGAP, %esi 
   decl %esi                     /* esi = sprite data ptr */

   .align 4, 0x90
   SPRITE_LOOP(h_flip) 
   movb (%esi), %bl              /* read pixel */
   testb %bl, %bl                /* test  */
   jz sprite_h_flip_skip 
   movb %bl, %es:(%eax)          /* write */
sprite_h_flip_skip: 
   decl %esi 
   incl %eax 
   SPRITE_END_X(h_flip)
   SPRITE_END_Y(h_flip)

sprite_h_flip_done:
   END_SPRITE_DRAW()
   ret                           /* end of _linear_draw_sprite_h_flip8() */




/* void _linear_draw_sprite_vh_flip8(BITMAP *bmp, BITMAP *sprite, int x, y);
 *  Draws a sprite to a linear bitmap, flipping both vertically and horizontally.
 */
.globl __linear_draw_sprite_vh_flip8 
   .align 4
__linear_draw_sprite_vh_flip8:
   START_SPRITE_DRAW(sprite_vh_flip)

   movl S_W, %eax                /* w */
   subl BMP_W(%esi), %eax        /* - sprite->w */
   movl %eax, S_SGAP             /* store sprite gap */

   movl S_LGAP, %eax
   addl %eax, S_X                /* X += lgap */

   movl S_TGAP, %eax 
   addl %eax, S_Y                /* Y += tgap */

   negl %eax                     /* - tgap */
   addl BMP_H(%esi), %eax        /* + sprite->h */
   decl %eax
   movl BMP_W(%esi), %ecx 
   movl BMP_LINE(%esi, %eax, 4), %esi
   addl %ecx, %esi
   subl S_LGAP, %esi 
   decl %esi                     /* esi = sprite data ptr */

   .align 4, 0x90
   SPRITE_LOOP(vh_flip) 
   movb (%esi), %bl              /* read pixel */
   testb %bl, %bl                /* test  */
   jz sprite_vh_flip_skip 
   movb %bl, %es:(%eax)          /* write */
sprite_vh_flip_skip: 
   decl %esi 
   incl %eax 
   SPRITE_END_X(vh_flip)
   SPRITE_END_Y(vh_flip)

sprite_vh_flip_done:
   END_SPRITE_DRAW()
   ret                           /* end of _linear_draw_sprite_vh_flip8() */




/* void _linear_draw_trans_sprite8(BITMAP *bmp, BITMAP *sprite, int x, y);
 *  Draws a translucent sprite onto a linear bitmap.
 */
.globl __linear_draw_trans_sprite8


   /* this loop is used when drawing onto video RAM */
   #define TRANS_LOOP_VIDEO                                                  \
      subl $4, S_W                                                         ; \
      .align 4, 0x90                                                       ; \
   sprite_y_loop_trans_video:                                              ; \
      movl S_BMP, %edx                       /* load bitmap pointer */     ; \
      movl S_Y, %eax                         /* load line */               ; \
      READ_BANK()                            /* select read bank */        ; \
      movl %eax, %ecx                        /* read address in ecx */     ; \
      movl S_Y, %eax                         /* reload line */             ; \
      WRITE_BANK()                           /* select write bank */       ; \
      subl %eax, %ecx                        /* convert ecx to offset */   ; \
      addl S_X, %eax                         /* add x offset */            ; \
      pushl %ebp                                                           ; \
      movl S_W, %ebp                         /* x loop counter */          ; \
      testl %ebp, %ebp                                                     ; \
      jle sprite_x_loop_trans_video_few_pixels                             ; \
      .align 4, 0x90                                                       ; \
   sprite_x_loop_trans_video:                                              ; \
      movl %es:(%eax, %ecx), %edx            /* read four pixels */        ; \
      movb %dl, %bl                          /* lookup pixel 1 */          ; \
      movb (%esi), %bh                                                     ; \
      movb (%edi, %ebx), %dl                                               ; \
      movb %dh, %bl                          /* lookup pixel 2 */          ; \
      movb 1(%esi), %bh                                                    ; \
      movb (%edi, %ebx), %dh                                               ; \
      roll $16, %edx                                                       ; \
      movb %dl, %bl                          /* lookup pixel 3 */          ; \
      movb 2(%esi), %bh                                                    ; \
      movb (%edi, %ebx), %dl                                               ; \
      movb %dh, %bl                          /* lookup pixel 4 */          ; \
      movb 3(%esi), %bh                                                    ; \
      movb (%edi, %ebx), %dh                                               ; \
      roll $16, %edx                                                       ; \
      movl %edx, %es:(%eax)                  /* write four pixels */       ; \
      addl $4, %eax                                                        ; \
      addl $4, %esi                                                        ; \
      subl $4, %ebp                                                        ; \
      jg sprite_x_loop_trans_video                                         ; \
   sprite_x_loop_trans_video_few_pixels:                                   ; \
      addl $4, %ebp                                                        ; \
      .align 4, 0x90                                                       ; \
   sprite_x_loop_trans_video_last_pixels:                                  ; \
      movb %es:(%eax, %ecx), %bl             /* read pixel */              ; \
      movb (%esi), %bh                                                     ; \
      movb (%edi, %ebx), %bl                 /* lookup pixel */            ; \
      movb %bl, %es:(%eax)                   /* write pixel */             ; \
      incl %eax                                                            ; \
      incl %esi                                                            ; \
      decl %ebp                                                            ; \
      jg sprite_x_loop_trans_video_last_pixels                             ; \
      popl %ebp                                                            ; \
      SPRITE_END_Y(trans_video)


   /* this loop is used when drawing onto memory bitmaps */
   #define TRANS_LOOP_MEMORY                                                 \
      movl S_Y, %ecx                         /* load line */               ; \
      movl S_W, %edx                         /* convert x loop counter */  ; \
      addl %edx, S_SGAP                      /* increase sprite gap */     ; \
      addl %ecx, S_H                         /* S_H = end y */             ; \
      movl $0, %ebx                          /* clear ebx */               ; \
      cmpl $2, %edx                          /* if sprite width > 2 */     ; \
      jg m_trans_sprite_not_tiny             /* the sprite isn't tiny */   ; \
      pushl %edx                             /* tiny sprite, unaligned */  ; \
      jmp m_trans_sprite_aligned             /* aligning complete */       ; \
      .align 4, 0x90                                                       ; \
   m_trans_sprite_not_tiny:                                                ; \
      subl S_X, %ebx                         /* unaligned bytes */         ; \
      andl $3, %ebx                          /* at start of line in bl */  ; \
      movb %dl, %bh                          /* unaligned bytes */         ; \
      subb %bl, %bh                          /* at end of line in bh */    ; \
      andb $3, %bh                                                         ; \
      pushl %ebx                             /* store on the stack */      ; \
   m_trans_sprite_aligned:                                                 ; \
      movzbl %bh, %eax                       /* subtract number of */      ; \
      subl %eax, %edx                        /* leftover bytes */          ; \
      subl $4, %edx                          /* one iteration less */      ; \
      addl %edx, %esi                        /* end of src line in esi */  ; \
      movl S_X, %eax                         /* eax = left edge of dest */ ; \
      addl %edx, %eax                        /* add width to left edge */  ; \
      movl %eax, S_X                         /* store right edge */        ; \
      negl %edx                              /* counter/offset negative */ ; \
      movl %edx, S_W                         /* store converted counter */ ; \
      .align 4, 0x90                                                       ; \
   m_sprite_y_loop_trans:                                                  ; \
      movl S_BMP, %edx                       /* load bitmap pointer */     ; \
      pushl %ebp                             /* I need ebp as counter */   ; \
      movl S_W, %ebp                         /* x loop counter */          ; \
      addl BMP_LINE(%edx, %ecx, 4), %eax     /* end of dest line in eax */ ; \
      movl $0, %ecx                          /* clear the high word */     ; \
      movl 4(%esp), %ebx                     /* leftover count in ebx */   ; \
      cmpb $0, %bl                           /* check if long-aligned */   ; \
      je m_trans_sprite_before_laligned      /* no aligning */             ; \
      cmpb $2, %bl                           /* check if word-aligned */   ; \
      je m_trans_sprite_before_waligned      /* copy one word */           ; \
      movb (%esi, %ebp), %ch                 /* load src pixel 0 */        ; \
      movb (%eax, %ebp), %cl                 /* load dest pixel 0 */       ; \
      incl %ebp                              /* increment counter/index */ ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 0 */          ; \
      movb %dl, -1(%eax, %ebp)               /* write the pixel */         ; \
      cmpb $1, %bl                           /* check if long-aligned */   ; \
      je m_trans_sprite_before_laligned      /* no more aligning */        ; \
   m_trans_sprite_before_waligned:                                         ; \
      movb (%esi, %ebp), %ch                 /* load src pixel 0 */        ; \
      movb 1(%eax, %ebp), %bl                /* load dest pixel 1 */       ; \
      movb (%eax, %ebp), %cl                 /* load dest pixel 0 */       ; \
      movb 1(%esi, %ebp), %bh                /* load src pixel 1 */        ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 0 */          ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 1 */          ; \
      movw %dx, (%eax, %ebp)                 /* write two pixels */        ; \
      addl $2, %ebp                          /* increment counter/index */ ; \
   m_trans_sprite_before_laligned:           /* src is long-aligned */     ; \
      testl %ebp, %ebp                       /* check number of longs */   ; \
      jg m_sprite_x_loop_trans_few           /* no aligned long in src */  ; \
      movb 2(%eax, %ebp), %cl                /* load dest pixel 2 */       ; \
      jz m_sprite_x_loop_trans_1long         /* one aligned long in src */ ; \
      movb 3(%esi, %ebp), %bh                /* load src pixel 3 */        ; \
      movb 2(%esi, %ebp), %ch                /* load src pixel 2 */        ; \
      movb 3(%eax, %ebp), %bl                /* load dest pixel 3 */       ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 2 */          ; \
      movb (%eax, %ebp), %cl                 /* load dest pixel 0 */       ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 3 */          ; \
      movb (%esi, %ebp), %ch                 /* load src pixel 0 */        ; \
      shll $16, %edx                                                       ; \
      movb 1(%eax, %ebp), %bl                /* load dest pixel 1 */       ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 0 */          ; \
      movb 1(%esi, %ebp), %bh                /* load src pixel 1 */        ; \
      movb 6(%eax, %ebp), %cl                /* load next loop pixel 2 */  ; \
      addl $4, %ebp                          /* increment counter/index */ ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 1 */          ; \
      jz m_sprite_x_loop_trans_2longs        /* 2 aligned longs in src */  ; \
      .align 4, 0x90                                                       ; \
   sprite_x_loop_trans_m:                                                  ; \
      movb 2(%esi, %ebp), %ch                /* load src pixel 2 */        ; \
      movb 3(%eax, %ebp), %bl                /* load dest pixel 2 */       ; \
      movb 3(%esi, %ebp), %bh                /* load src pixel 3 */        ; \
      movl %edx, -4(%eax, %ebp)              /* write prev. pixels */      ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 2  */         ; \
      movb (%eax, %ebp), %cl                 /* load dest pixel 0 */       ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 3  */         ; \
      movb (%esi, %ebp), %ch                 /* load src pixel 0 */        ; \
      shll $16, %edx                         /* high word done; shift */   ; \
      movb 1(%eax, %ebp), %bl                /* load dest pixel 1 */       ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 0 */          ; \
      movb 1(%esi, %ebp), %bh                /* load src pixel 1 */        ; \
      movb 6(%eax, %ebp), %cl                /* load next loop pixel 2 */  ; \
      addl $4, %ebp                          /* increment counter/index */ ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 1 */          ; \
      jl sprite_x_loop_trans_m                                             ; \
   m_sprite_x_loop_trans_2longs:                                           ; \
      movl %edx, -4(%eax, %ebp)              /* write last pixels */       ; \
   m_sprite_x_loop_trans_1long:                                            ; \
      movb 3(%esi, %ebp), %bh                /* load src pixel 3 */        ; \
      movb 2(%esi, %ebp), %ch                /* load src pixel 2 */        ; \
      movb 3(%eax, %ebp), %bl                /* load dest pixel 3 */       ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 2 */          ; \
      movb (%eax, %ebp), %cl                 /* load dest pixel 0 */       ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 3 */          ; \
      movb (%esi, %ebp), %ch                 /* load src pixel 0 */        ; \
      shll $16, %edx                                                       ; \
      movb 1(%eax, %ebp), %bl                /* load dest pixel 1 */       ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 0 */          ; \
      movb 1(%esi, %ebp), %bh                /* load src pixel 1 */        ; \
      addl $4, %ebp                          /* increment counter/index */ ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 1 */          ; \
      movl %edx, -4(%eax, %ebp)              /* write four pixels */       ; \
   m_sprite_x_loop_trans_few:                                              ; \
      movl 4(%esp), %ebx                     /* leftover count in ebx */   ; \
      cmpb $0, %bh                           /* any leftover bytes? */     ; \
      je m_trans_sprite_line_complete        /* no leftovers */            ; \
      cmpb $1, %bh                           /* any leftover word? */      ; \
      je m_trans_sprite_leftover_byte        /* only copy one byte */      ; \
      movb (%esi, %ebp), %ch                 /* load src pixel 0 */        ; \
      movb 1(%eax, %ebp), %bl                /* load dest pixel 1 */       ; \
      movb (%eax, %ebp), %cl                 /* load dest pixel 0 */       ; \
      movb 1(%esi, %ebp), %bh                /* load src pixel 1 */        ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 0 */          ; \
      movb (%edi, %ebx), %dh                 /* lookup pixel 1 */          ; \
      cmpb $2, 5(%esp)                       /* check if complete */       ; \
      movw %dx, (%eax, %ebp)                 /* write two pixels */        ; \
      leal 2(%ebp), %ebp                     /* increment counter/index */ ; \
      je m_trans_sprite_line_complete        /* don't draw any more */     ; \
   m_trans_sprite_leftover_byte:             /* draw the final pixel */    ; \
      movb (%esi, %ebp), %ch                 /* load src pixel 0 */        ; \
      movb (%eax, %ebp), %cl                 /* load dest pixel 0 */       ; \
      movb (%edi, %ecx), %dl                 /* lookup pixel 0 */          ; \
      movb %dl, (%eax, %ebp)                 /* write the pixel */         ; \
   m_trans_sprite_line_complete:                                           ; \
      popl %ebp                              /* I need stack variables */  ; \
      movl S_Y, %ecx                         /* y counter */               ; \
      incl %ecx                                                            ; \
      movl S_X, %eax                         /* reload dest x position */  ; \
      movl %ecx, S_Y                         /* store counter */           ; \
      addl S_SGAP, %esi                      /* esi points to next line */ ; \
      cmpl S_H, %ecx                         /* check if complete */       ; \
      jc m_sprite_y_loop_trans               /* loop */                    ; \
      popl %ebx                              /* pop unaligned counter */


   /* the actual translucent sprite drawing routine... */
   .align 4
__linear_draw_trans_sprite8:
   START_SPRITE_DRAW(trans_sprite)

   movl BMP_W(%esi), %eax        /* sprite->w */
   subl S_W, %eax                /* - w */
   movl %eax, S_SGAP             /* store sprite gap */

   movl S_LGAP, %eax
   addl %eax, S_X                /* X += lgap */

   movl S_TGAP, %eax 
   addl %eax, S_Y                /* Y += tgap */

   movl BMP_LINE(%esi, %eax, 4), %esi
   addl S_LGAP, %esi             /* esi = sprite data ptr */

   xorl %ebx, %ebx
   movl _color_map, %edi         /* edi = color mapping table */

   movl S_BMP, %eax
   cmpl $0, BMP_DAT(%eax)        /* check if it's video memory */
   jz trans_sprite_video
   TRANS_LOOP_MEMORY
   jmp trans_sprite_done

trans_sprite_video:
   TRANS_LOOP_VIDEO

   .align 4, 0x90
trans_sprite_done:
   END_SPRITE_DRAW()
   ret                           /* end of _linear_draw_trans_sprite8() */




/* void _linear_draw_lit_sprite8(BITMAP *bmp, BITMAP *sprite, int x, y, color);
 *  Draws a lit sprite onto a linear bitmap.
 */
.globl __linear_draw_lit_sprite8

   #define COLOR     ARG5

   .align 4
__linear_draw_lit_sprite8:
   START_SPRITE_DRAW(lit_sprite)

   movl BMP_W(%esi), %eax        /* sprite->w */
   subl S_W, %eax                /* - w */
   movl %eax, S_SGAP             /* store sprite gap */

   movl S_LGAP, %eax
   addl %eax, S_X                /* X += lgap */

   movl S_TGAP, %eax 
   addl %eax, S_Y                /* Y += tgap */

   movl BMP_LINE(%esi, %eax, 4), %esi
   addl S_LGAP, %esi             /* esi = sprite data ptr */

   xorl %ebx, %ebx
   movb COLOR, %bh               /* store color in high byte */
   movl _color_map, %edi         /* edi = color mapping table */

   .align 4, 0x90
   SPRITE_LOOP(lit_sprite) 
   movb (%esi), %bl              /* read pixel into low byte */
   orb %bl, %bl
   jz lit_sprite_skip
   movb (%edi, %ebx), %bl        /* color table lookup */
   movb %bl, %es:(%eax)          /* write pixel */
lit_sprite_skip:
   incl %esi
   incl %eax
   SPRITE_END_X(lit_sprite)
   SPRITE_END_Y(lit_sprite)

lit_sprite_done:
   END_SPRITE_DRAW()
   ret                           /* end of _linear_draw_lit_sprite8() */




/* void __linear_draw_character8(BITMAP *bmp, BITMAP *sprite, int x, y, color);
 *  For proportional font output onto a linear bitmap: uses the sprite as 
 *  a mask, replacing all set pixels with the specified color.
 */
.globl __linear_draw_character8 

   #undef COLOR
   #define COLOR  ARG5

   .align 4
__linear_draw_character8:
   START_SPRITE_DRAW(draw_char)

   movl BMP_W(%esi), %eax        /* sprite->w */
   subl S_W, %eax                /* - w */
   movl %eax, S_SGAP             /* store sprite gap */

   movl S_LGAP, %eax
   addl %eax, S_X                /* X += lgap */

   movl S_TGAP, %eax 
   addl %eax, S_Y                /* Y += tgap */

   movl BMP_LINE(%esi, %eax, 4), %esi
   addl S_LGAP, %esi             /* esi = sprite data ptr */

   movb COLOR, %bl               /* bl = text color */
   movb __textmode, %bh          /* bh = background color */
   cmpl $0, __textmode
   jl draw_masked_char

   /* opaque (text_mode >= 0) character output */
   .align 4, 0x90
   SPRITE_LOOP(draw_opaque_char) 
   cmpb $0, (%esi)               /* test pixel */
   jz draw_opaque_background
   movb %bl, %es:(%eax)          /* write pixel */
   jmp draw_opaque_done
draw_opaque_background: 
   movb %bh, %es:(%eax)          /* write background */
draw_opaque_done:
   incl %esi 
   incl %eax 
   SPRITE_END_X(draw_opaque_char)
   SPRITE_END_Y(draw_opaque_char)
   jmp draw_char_done

   /* masked (text_mode -1) character output */
   .align 4, 0x90
draw_masked_char:
   SPRITE_LOOP(draw_masked_char) 
   cmpb $0, (%esi)               /* test pixel */
   jz draw_masked_skip
   movb %bl, %es:(%eax)          /* write pixel */
draw_masked_skip:
   incl %esi 
   incl %eax 
   SPRITE_END_X(draw_masked_char)
   SPRITE_END_Y(draw_masked_char)

draw_char_done:
   END_SPRITE_DRAW()
   ret                           /* end of _linear_draw_character8() */




/* void _linear_textout_fixed8(BITMAP *bmp, void *font, int height,
 *                            char *str, int x, y, color);
 *  Fast text output routine for fixed size fonts onto linear bitmaps.
 */
.globl __linear_textout_fixed8 

   .align 4
__linear_textout_fixed8:
   pushl %ebp
   movl %esp, %ebp
   subl $28, %esp

   pushl %edi
   pushl %esi
   pushl %ebx
   pushw %es

   /* initialises the inner drawing loop */
   #define START_X_LOOP()                                                    \
      movl T_COLOR, %eax                                                   ; \
      movl __textmode, %ebx

   /* cleans up after the inner drawing loop */
   #define END_X_LOOP()

   /* offsets an address by a number of pixels */
   #define GET_ADDR(a, b)                                                    \
      addl b, a

   /* writes ax to the destination */
   #define PUTA()                                                            \
      movb %al, %es:(%edi)

   /* writes bx to the destination */
   #define PUTB()                                                            \
      movb %bl, %es:(%edi)

   /* increments the destination */
   #define NEXTDEST()                                                        \
      incl %edi

   DRAW_TEXT()

   #undef START_X_LOOP
   #undef END_X_LOOP
   #undef GET_ADDR
   #undef PUTA
   #undef PUTB
   #undef NEXTDEST

   popw %es
   popl %ebx
   popl %esi
   popl %edi
   movl %ebp, %esp
   popl %ebp
   ret                           /* end of _textout_fixed8() */




/* void _linear_draw_rle_sprite8(BITMAP *bmp, RLE_SPRITE *sprite, int x, int y)
 *  Draws an RLE sprite onto a linear bitmap at the specified position.
 */
.globl __linear_draw_rle_sprite8

   .align 4
__linear_draw_rle_sprite8:

   /* bank switch routine */
   #define INIT_RLE_LINE()                                                   \
      movl R_Y, %eax                                                       ; \
      WRITE_BANK()                                                         ; \
      movl %eax, %edi                                                      ; \
      addl R_X, %edi


   /* copy a clipped pixel run */
   #define SLOW_RLE_RUN(n)                                                   \
      rep ; movsb


   /* no special initialisation required */
   #define INIT_FAST_RLE_LOOP()


   /* copy a run of solid pixels */
   #define FAST_RLE_RUN()                                                    \
      shrl $1, %ecx                                                        ; \
      jnc rle_noclip_no_byte                                               ; \
      movsb                      /* copy odd byte? */                      ; \
   rle_noclip_no_byte:                                                     ; \
      jz rle_noclip_x_loop                                                 ; \
      shrl $1, %ecx                                                        ; \
      jnc rle_noclip_no_word                                               ; \
      movsw                      /* copy odd word? */                      ; \
   rle_noclip_no_word:                                                     ; \
      jz rle_noclip_x_loop                                                 ; \
      rep ; movsl                /* 32 bit string copy */


   /* tests an RLE command byte */
   #define TEST_RLE_COMMAND(done, skip)                                      \
      testb %al, %al                                                       ; \
      jz done                                                              ; \
      js skip


   /* adds the offset in %eax onto the destination address */
   #define ADD_EAX_EDI()                                                     \
      addl %eax, %edi


   /* zero extend %al into %eax */
   #define RLE_ZEX_EAX()                                                     \
      movzbl %al, %eax


   /* zero extend %al into %ecx */
   #define RLE_ZEX_ECX()                                                     \
      movzbl %al, %ecx 


   /* sign extend %al into %eax */
   #define RLE_SEX_EAX()                                                     \
      movsbl %al, %eax


   /* do it! */
   DO_RLE(rle, 1, b, %al, $0)
   ret

   #undef INIT_RLE_LINE
   #undef SLOW_RLE_RUN
   #undef INIT_FAST_RLE_LOOP
   #undef FAST_RLE_RUN




/* void _linear_draw_trans_rle_sprite8(BITMAP *bmp, RLE_SPRITE *sprite, 
 *                                   int x, int y)
 *  Draws a translucent RLE sprite onto a linear bitmap.
 */
.globl __linear_draw_trans_rle_sprite8

   .align 4
__linear_draw_trans_rle_sprite8:

   /* bank switch routine */
   #define INIT_RLE_LINE()                                                   \
      movl R_BMP, %edx                                                     ; \
      movl R_Y, %eax                                                       ; \
      READ_BANK()                /* select read bank */                    ; \
      movl %eax, R_TMP                                                     ; \
      movl R_Y, %eax                                                       ; \
      WRITE_BANK()               /* select write bank */                   ; \
      movl %eax, %edi                                                      ; \
      movl R_TMP, %edx           /* calculate read/write diff */           ; \
      subl %edi, %edx                                                      ; \
      addl R_X, %edi


   /* copy a clipped pixel run */
   #define SLOW_RLE_RUN(n)                                                   \
      pushl %ebx                                                           ; \
      movl _color_map, %ebx                                                ; \
      xorl %eax, %eax                                                      ; \
									   ; \
   trans_rle_clipped_run_loop##n:                                          ; \
      movb (%esi), %ah           /* read sprite pixel */                   ; \
      movb %es:(%edi, %edx), %al /* read destination pixel */              ; \
      movb (%ebx, %eax), %al     /* blend */                               ; \
      movb %al, %es:(%edi)       /* write the pixel */                     ; \
      incl %esi                                                            ; \
      incl %edi                                                            ; \
      decl %ecx                                                            ; \
      jg trans_rle_clipped_run_loop##n                                     ; \
									   ; \
      popl %ebx


   /* initialise the drawing loop */
   #define INIT_FAST_RLE_LOOP()                                              \
      movl _color_map, %ebx


   /* copy a run of solid pixels */
   #define FAST_RLE_RUN()                                                    \
      xorl %eax, %eax                                                      ; \
									   ; \
      shrl $1, %ecx                                                        ; \
      jnc trans_rle_run_no_byte                                            ; \
									   ; \
      movb (%esi), %ah           /* read sprite pixel */                   ; \
      movb %es:(%edi, %edx), %al /* read destination pixel */              ; \
      movb (%ebx, %eax), %al     /* blend */                               ; \
      movb %al, %es:(%edi)       /* write the pixel */                     ; \
      incl %esi                                                            ; \
      incl %edi                                                            ; \
									   ; \
   trans_rle_run_no_byte:                                                  ; \
      orl %ecx, %ecx                                                       ; \
      jz trans_rle_run_done                                                ; \
									   ; \
      shrl $1, %ecx                                                        ; \
      jnc trans_rle_run_no_word                                            ; \
									   ; \
      pushl %ecx                                                           ; \
      xorl %ecx, %ecx                                                      ; \
      movw (%esi), %ax           /* read two sprite pixels */              ; \
      movw %ax, R_TMP                                                      ; \
      movw %es:(%edi, %edx), %ax /* read two destination pixels */         ; \
      movb %al, %cl                                                        ; \
      movb R_TMP, %ch                                                      ; \
      movb (%ebx, %ecx), %al     /* blend pixel 1 */                       ; \
      movb %ah, %cl                                                        ; \
      movb 1+R_TMP, %ch                                                    ; \
      movb (%ebx, %ecx), %ah     /* blend pixel 2 */                       ; \
      movw %ax, %es:(%edi)       /* write two pixels */                    ; \
      addl $2, %esi                                                        ; \
      addl $2, %edi                                                        ; \
      popl %ecx                                                            ; \
									   ; \
   trans_rle_run_no_word:                                                  ; \
      orl %ecx, %ecx                                                       ; \
      jz trans_rle_run_done                                                ; \
									   ; \
      movl %ecx, R_TMP2                                                    ; \
      xorl %ecx, %ecx                                                      ; \
									   ; \
   trans_rle_run_loop:                                                     ; \
      movl (%esi), %eax          /* read four sprite pixels */             ; \
      movl %eax, R_TMP                                                     ; \
      movl %es:(%edi, %edx), %eax   /* read four destination pixels */     ; \
      movb %al, %cl                                                        ; \
      movb R_TMP, %ch                                                      ; \
      movb (%ebx, %ecx), %al     /* blend pixel 1 */                       ; \
      movb %ah, %cl                                                        ; \
      movb 1+R_TMP, %ch                                                    ; \
      movb (%ebx, %ecx), %ah     /* blend pixel 2 */                       ; \
      roll $16, %eax                                                       ; \
      movb %al, %cl                                                        ; \
      movb 2+R_TMP, %ch                                                    ; \
      movb (%ebx, %ecx), %al     /* blend pixel 3 */                       ; \
      movb %ah, %cl                                                        ; \
      movb 3+R_TMP, %ch                                                    ; \
      movb (%ebx, %ecx), %ah     /* blend pixel 4 */                       ; \
      roll $16, %eax                                                       ; \
      movl %eax, %es:(%edi)      /* write four pixels */                   ; \
      addl $4, %esi                                                        ; \
      addl $4, %edi                                                        ; \
      decl R_TMP2                                                          ; \
      jg trans_rle_run_loop                                                ; \
									   ; \
   trans_rle_run_done:


   /* do it! */
   DO_RLE(rle_trans, 1, b, %al, $0)
   ret 

   #undef INIT_RLE_LINE
   #undef SLOW_RLE_RUN
   #undef INIT_FAST_RLE_LOOP
   #undef FAST_RLE_RUN




/* void _linear_draw_lit_rle_sprite8(BITMAP *bmp, RLE_SPRITE *sprite, 
 *                                  int x, int y, int color)
 *  Draws a tinted RLE sprite onto a linear bitmap.
 */
.globl __linear_draw_lit_rle_sprite8

   .align 4
__linear_draw_lit_rle_sprite8:

   /* bank switch routine */
   #define INIT_RLE_LINE()                                                   \
      movl R_BMP, %edx                                                     ; \
      movl R_Y, %eax                                                       ; \
      WRITE_BANK()                                                         ; \
      movl %eax, %edi                                                      ; \
      addl R_X, %edi                                                       ; \
      movl _color_map, %edx


   /* copy a clipped pixel run */
   #define SLOW_RLE_RUN(n)                                                   \
      xorl %eax, %eax                                                      ; \
      movb R_COLOR, %ah          /* store color in high byte */            ; \
									   ; \
   lit_rle_clipped_run_loop##n:                                            ; \
      movb (%esi), %al           /* read a pixel */                        ; \
      movb (%edx, %eax), %al     /* lookup in color table */               ; \
      movb %al, %es:(%edi)       /* write the pixel */                     ; \
      incl %esi                                                            ; \
      incl %edi                                                            ; \
      decl %ecx                                                            ; \
      jg lit_rle_clipped_run_loop##n


   /* initialise the drawing loop */
   #define INIT_FAST_RLE_LOOP()                                              \
      xorl %ebx, %ebx                                                      ; \
      movb R_COLOR, %bh          /* store color in high byte */


   /* copy a run of solid pixels */
   #define FAST_RLE_RUN()                                                    \
      shrl $1, %ecx                                                        ; \
      jnc lit_rle_run_no_byte                                              ; \
									   ; \
      movb (%esi), %bl           /* read pixel into low byte */            ; \
      movb (%edx, %ebx), %bl     /* lookup in lighting table */            ; \
      movb %bl, %es:(%edi)       /* write the pixel */                     ; \
      incl %esi                                                            ; \
      incl %edi                                                            ; \
									   ; \
   lit_rle_run_no_byte:                                                    ; \
      orl %ecx, %ecx                                                       ; \
      jz lit_rle_run_done                                                  ; \
									   ; \
      shrl $1, %ecx                                                        ; \
      jnc lit_rle_run_no_word                                              ; \
									   ; \
      movw (%esi), %ax           /* read two pixels */                     ; \
      movb %al, %bl                                                        ; \
      movb (%edx, %ebx), %al     /* lookup pixel 1 */                      ; \
      movb %ah, %bl                                                        ; \
      movb (%edx, %ebx), %ah     /* lookup pixel 2 */                      ; \
      movw %ax, %es:(%edi)       /* write two pixels */                    ; \
      addl $2, %esi                                                        ; \
      addl $2, %edi                                                        ; \
									   ; \
   lit_rle_run_no_word:                                                    ; \
      orl %ecx, %ecx                                                       ; \
      jz lit_rle_run_done                                                  ; \
									   ; \
   lit_rle_run_loop:                                                       ; \
      movl (%esi), %eax          /* read four pixels */                    ; \
      movb %al, %bl                                                        ; \
      movb (%edx, %ebx), %al     /* lookup pixel 1 */                      ; \
      movb %ah, %bl                                                        ; \
      movb (%edx, %ebx), %ah     /* lookup pixel 2 */                      ; \
      roll $16, %eax                                                       ; \
      movb %al, %bl                                                        ; \
      movb (%edx, %ebx), %al     /* lookup pixel 3 */                      ; \
      movb %ah, %bl                                                        ; \
      movb (%edx, %ebx), %ah     /* lookup pixel 4 */                      ; \
      roll $16, %eax                                                       ; \
      movl %eax, %es:(%edi)      /* write four pixels */                   ; \
      addl $4, %esi                                                        ; \
      addl $4, %edi                                                        ; \
      decl %ecx                                                            ; \
      jg lit_rle_run_loop                                                  ; \
									   ; \
   lit_rle_run_done:


   /* do it! */
   DO_RLE(rle_lit, 1, b, %al, $0)
   ret 

   #undef INIT_RLE_LINE
   #undef SLOW_RLE_RUN
   #undef INIT_FAST_RLE_LOOP
   #undef FAST_RLE_RUN




