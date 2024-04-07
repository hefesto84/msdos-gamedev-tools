PAGE    55,132
TITLE   ABRACADABRA! MAGIC SOFTWARE TSR/TCR (Terminate Cont Running) LIBRARY

DEINT   EQU     0E0h

INCLUDE MGLANGC.ASM

GETFARPTR MACRO
        mov     ax, PARM1
        mov     farptr[2], ax
        mov     ax, PARM2
        mov     farptr, ax
        ENDM

;------------------------------------------------
; DOC
; certain of these functions poll the tsr for
; data which is returned in the corresponding
; registers and then to the calling program
;
;       ax  	process status
;       cx  	process id number
;       bx, 	environment
;       dx	code segment
;
;------------------------------------------------

;------------------------------------------------
; SEGMENT DEFINITIONS
;------------------------------------------------

_BSS    SEGMENT WORD PUBLIC 'BSS'
_BSS    ENDS

DGROUP  GROUP   _DATA, _BSS
_DATA   SEGMENT WORD PUBLIC 'DATA'
        ASSUME  DS:DGROUP, SS:DGROUP
_DATA   ENDS

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
        ASSUME  CS:_TEXT

	PUBLIC	_tsrs
	PUBLIC	_procid
	PUBLIC	_firstsr
	PUBLIC	_procst
	PUBLIC  _nextsr
	PUBLIC	_un
	PUBLIC	_check

farcall  label  dword
farptr   dw     2 dup(?)

hi	dw      0
lo	dw	0

;------------------------------------------------
; Publics
;------------------------------------------------

;------------------------------------------------
; _TSRS - returns 1 if int E0 is non zero
;------------------------------------------------

_tsrs   proc    far
        LANGIN
        mov     bx, DEINT       ;our interrupt
        mov     cl, 2
        shl     bx, cl          ;times 4
        mov     cx, 0
        push    cx              ;load up es
        pop     es
        mov     ax, es:[bx]     ;get value and return
        LANGRET 0
_tsrs   endp

;------------------------------------------------
; _procid - returns process id number
;------------------------------------------------

_procid proc    far
        LANGIN
	GETFARPTR
        mov     ax, 1           ;execute function 1 (poll process)
        call    farcall
        RETVAL  cx              ;return the value in ax
        LANGRET
_procid endp

;------------------------------------------------------
; _firstsr - returns address of first tsrs clear routine
;------------------------------------------------------

_firstsr proc   far
        LANGIN
        mov     ax, DEINT       ;our interrupt
        mov     cl, 2
        shl     ax, cl          ;times 4
        mov     cx, 0
        push    cx              ;load up es
        pop     es
	mov	bx, ax
        mov     dx, es:[bx]     ;get vectors segment
        mov     ax, es:[bx+2]   ;get vectors offset
        LANGRET 0
_firstsr endp

;------------------------------------------------
; _nextsr(seg, off) of current tsr
;------------------------------------------------

_nextsr proc    far
        LANGIN
	GETFARPTR
        mov     ax, 1           ;execute function 1 (poll process)
        call    farcall
        RETVAL  bx
        RETHIGH dx
        LANGRET 0
_nextsr endp

;------------------------------------------------
; _procst - return process status word
;------------------------------------------------

_procst proc    far
        LANGIN
	GETFARPTR
        mov     ax, 1           ;execute function 1 (poll process)
        call 	farcall         ;returns status in ax
        LANGRET                 ;value ends up in ax which is c's returner
_procst endp

;------------------------------------------------
; _un - deinstall the TSR
;------------------------------------------------

_un     proc    far
        LANGIN
	GETFARPTR
        mov     ax, 0
	call	farcall
	sub	dx, PSPOFFSET
        push    bx
        push    dx
        pop     es
        mov     ah, 49h
        int     21h
        pop     es
        mov     ah, 49h
        int     21h
	cmp	ax, 9
	jg	un1
	mov	ax, 0
un1:
        LANGRET 0
_un     endp

;------------------------------------------------
; _check - is requested tsr loaded?
;------------------------------------------------

_check	proc	far
	push	bp
	mov	bp,sp
	sub	sp,6
	call	far ptr _FIRSTSR
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	mov	hi, dx
	mov	lo, ax

check2:

	mov	ax,word ptr [bp-4]
	or	ax,word ptr [bp-2]
	je	check3
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _PROCID
	pop	cx
	pop	cx
	mov	word ptr [bp-6],ax
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr [bp+6]
	jne	check4
	jmp	short check1

check4:

	mov	lo, 0
	mov	hi, 0
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _NEXTSR
	pop	cx
	pop	cx
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	mov	lo, ax
	mov	hi, dx
	jmp	short check2

check3:

	mov	lo, 0
	mov	hi, 0

check1:

	mov	dx, hi
	mov	ax, lo
	mov	sp,bp
	pop	bp
	ret
_check	endp

_TEXT   ENDS

        END

;------------------------------------------------
; END OF MGUN.ASM
;------------------------------------------------
