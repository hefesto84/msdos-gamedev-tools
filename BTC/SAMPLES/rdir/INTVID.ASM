
; Special intvid () procedure, had to use this because int86 would not
; work for within an ISR.

_TEXT   segment para public 'CODE'
        assume  cs:_TEXT
_TEXT   ends
_TEXT   segment

; Register structure definition

vid_regs struc
        r_ax    dw ?
        r_bx    dw ?
        r_cx    dw ?
        r_dx    dw ?
vid_regs ends

_intvid proc    near
        public  _intvid

        push    bp                      ;set up stack frame
        mov     bp,sp
        push    si                      ;save si

        mov     si,word ptr [bp+4]      ;load pointer to vidregs

        mov     ax,[si].r_ax            ;copy in all of the registers
        mov     bx,[si].r_bx
        mov     cx,[si].r_cx
        mov     dx,[si].r_dx

        int     10h                     ;call video BIOS

        cmp     word ptr [bp+6],0       ;see if need to be stored
        je      exit                    ;if not don't store

        mov     [si].r_ax,ax            ;put the registers away
        mov     [si].r_bx,bx
        mov     [si].r_cx,cx
        mov     [si].r_dx,dx

exit:
        pop     si                      ;restore si
        pop     bp                      ;restore bp
        ret                             ;return to the caller

_intvid endp

_TEXT   ends

         end

         






