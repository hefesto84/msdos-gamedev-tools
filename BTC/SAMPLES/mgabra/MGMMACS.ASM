;------------------------------------------------
; WHMACS - Assembly Language Macros
;------------------------------------------------

;----------------------------------------------------------
; SAVREGS - Save 8086 Registers To Tcb (task control block)
;----------------------------------------------------------

SAVREGS MACRO   TASK
        mov     [TASK], 0	;0 means process put to sleep
        mov     [TASK+2], ax
        mov     [TASK+4], bx
        mov     [TASK+6], cx
        mov     [TASK+8], dx
        mov     [TASK+10], bp
        mov     [TASK+12], di
        mov     [TASK+14], si
        mov     [TASK+16], es
        mov     [TASK+18], ds
        pop     [TASK+24]       ;ip
        pop     [TASK+26]       ;cs
        pop     [TASK+28]       ;flags
        mov     [TASK+20], ss
        mov     [TASK+22], sp
        ENDM

;----------------------------------------------------
; SAVCUR - Save Cursor Position To Tcb. We use direct
; hardware because some program bypass the bios and
; give an innaccurate cursor position.
;----------------------------------------------------

SAVCUR  MACRO   TASK
;	mov	dx, add6845	;find 6845 controller
;	mov	al, 14		;register number
;	out	dx, al		;do it
;	inc	dx		;set for next register
;	in	al, dx
;	mov	ah, al
;	dec	dx              ;reset dx
;	mov	al, 15		;port number
;	out	dx, al
;	inc	dx
;	in	al, dx
;	and	ax, 07ffh
;	mov	bl, 80
;	div	bl
;	xchg	ah, al
;       mov     [TASK+30], ax	;save cursor location
	mov	bx, 0
	push	bx
	pop	es			;set segment
	mov	bx, 0462h		;page number in low ram
	mov	bh, byte ptr es:[bx]	;get it
	mov	ah, 3
	int	10h
	mov	[TASK+30], dx		;cursor position
	mov	[TASK+52], cx		;cursor size
        ENDM

;------------------------------------------------
; RESCUR - Restore Cursor Address From Tcb
;------------------------------------------------

RESCUR  MACRO   TASK
	LOCAL	LB1, LB2
	mov	ax, cs:scrswp	;get screen swap mode
	and	ax, 1		;is cursor swapping enabled?
	jnz 	LB2		;yes then do it
	mov	ax, cs:scrswp	;check again
	and	ax, 8		;cursor swap on key switch?
	jz	LB1		;if not then exit
	cmp	cs:keyswap, 1	;was it done on a keyswap?
	jne	LB1		;no then exit
LB2:
        mov     dx, [TASK+30]	;cursor row and column to dx
	mov	bh, byte ptr [TASK+51]	;page number
        mov     ah, 2		;set cursor address function number
        int     10h		;call bios
	mov	cx, [TASK+52]	;cursror size
	mov	ah, 1
	int	10h
LB1:
        ENDM

;------------------------------------------------
; SAVDTA - SAVE DTA ADDRESS TO TCB
;------------------------------------------------

SAVDTA  MACRO   TASK
        mov     ah, 2fh		;get dta address function number
        int     21h		;call dos
        mov     [TASK+32], es	;save dta address segment
        mov     [TASK+34], bx	;save dta offset
        ENDM

;------------------------------------------------
; SAV24H - Save Critical Error Address To Tcb
;------------------------------------------------

SAV24h  MACRO   TASK
        SAVINT  24h, [TASK+36], [TASK+38]
        ENDM

;------------------------------------------------
; SAV23H - Save Control Break Handler
;------------------------------------------------

SAV23h	MACRO	TASK
	SAVINT	23h, [TASK+46], [TASK+48]
	ENDM

;------------------------------------------------
; SAVPRC - Save Entire Process
;------------------------------------------------

SAVPRC  MACRO   TASK
        SAVREGS TASK
 	SAVDTA	TASK
 	SAV24H	TASK
	SAV23H	TASK
	SAVCUR	TASK
 	SAVSCR  TASK
        ENDM

;------------------------------------------------
; SAVPAG - Save current video page to TCB
;------------------------------------------------

SAVPAG	MACRO	TASK
	mov	bx, 0
	push	bx
	pop	es			;set segment
	mov	bx, 0462h		;page number in low ram
	mov	al, byte ptr es:[bx]	;get it
	mov	ah, 0
	mov	[TASK+50], ax           ;save it into TCB
	mov	cs:videop, ax		;move it to our global variable
 	call    VIDSEG 			;calculate effective video base
	ENDM

;------------------------------------------------
; SETPAG - set video page
;------------------------------------------------

SETPAG	MACRO	PG
	mov	ax, PG       		;get value page
	mov	ah, 5			;set video page function number
	int	10h			;set it
	ENDM

;------------------------------------------------
; RESPAG - Restore video page
;------------------------------------------------

RESPAG	MACRO	TASK
	mov	ax,  [TASK+50]		;get value of this processes page
	mov	cs:videop, ax           ;save it into our variable
	mov	ah, 5			;set video page function number
	int	10h			;set it
	call 	VIDSEG			;calculate video base
	ENDM

;------------------------------------------------
; SAVMOD - Save Video Mode To Tcb
;------------------------------------------------

SAVMOD	MACRO	TASK
	mov	bx, 0
	push	bx
	pop	es			;set segment 0
	mov	bx, 0449h		;set offset
	mov	al, es:[bx]		;get video mode
	mov	ah, 0			;clear ah
	mov	[TASK+40], ax		;save it
	mov	cs:videom, ax		;place in global mode variable
	ENDM

;------------------------------------------------
; RESMOD - Restore Video Mode
;------------------------------------------------

RESMOD	MACRO	TASK
	LOCAL	resmod1
	mov	ax, [tsrprg+40]		;compare our video modes
	cmp	ax, [dosprg+40]		;if not different don't switch
	je	resmod1			;because mode swith is unaesthetic
	mov	ax, [TASK+40]		;set mode number from TCB
	mov	cs:videom, ax		;place in global mode variable
	mov	ah, 0			;set video mode function number
	int	10h			;call bios

resmod1:

	ENDM

;------------------------------------------------
; LODTCK Load tasks tick allocation
;------------------------------------------------

LODTCK	MACRO	TASK
	push	[TASK+44]		;get it's ticks
	pop	cs:period		;load them into master timer
	ENDM

;------------------------------------------------
; SAVSCR - SAVE SCREEN CONTENTS TO BUFFER
;------------------------------------------------

SAVSCR	MACRO	TASK
	LOCAL	LB1, LB2, LB3, LB4
	mov	ax, cs:scrswp	;get screen swap parameter
	and	ax, 2		;is save screen bit set?
	jnz	LB4		;yes then swap 'em
				;else ...
	mov	ax, cs:scrswp	;check again for swap by key
	and	ax, 16		;save if swap by key?
	jz	LB1		;no, then exit outta here
				;else ...
	cmp	cs:keyswap, 1	;was swap done by key?
	jne 	LB1		;no, then exit.
LB4:
	SAVMOD	TASK		;save video mode
 	SAVPAG	TASK		;save video page
	push	ds		;duck ds!
	push	cs		;get cs into es
	pop	es
	mov	di, [TASK+42]	;set destination offset
	mov	ds, cs:videos	;set source segment
	mov	si, 0		;set source offset
	mov	cx, cs:videosz	;initialize count
	cld
rep	movsb			;buffer transfer

	pop	ds		;restore ds
LB1:
	ENDM

;--------------------------------------------------
; RESSCR - Restore screen from buffer to video ram
;--------------------------------------------------

RESSCR	MACRO	TASK
	LOCAL	LB1, LB2, LB3, LB4, SNOW1, SNOW2, NOSNOW
	mov	ax, cs:scrswp	;get screen swap mode
	and	ax, 4		;is screen restoration enabled?
	jnz	LB4		;yes? do it
	mov	ax, cs:scrswp	;check again for swap by key
	and	ax, 32		;save if swap by key?
	jz	LB1		;no, then exit outta here
	cmp	cs:keyswap, 1	;was swap done by key?
	jne 	LB1		;no, then exit.
LB4:
 	RESMOD	TASK		;restore video mode
 	RESPAG	TASK		;restore video page
	push	ds		;duck ds!
	push	cs		;get cs into ds
	pop	ds
	mov	si, [TASK+42]	;set source offset, assume cs seg
	mov	es, cs:videos	;set destination segment
	mov	di, 0		;set destination offset
	mov	cx, cs:videosz	;initialize count
	mov	ax, cs:scrswp
	and	ax, 256		;check for snow/no snow option
	jnz	NOSNOW
	cld
	mov	dx, 03dah	;address of video controller
SNOW1:
	in	al, dx
	and	al, 1
	jnz	SNOW1
SNOW2:
	in	al, dx
	and	al, 1
	jz	SNOW2
    	movsb			;buffer transfer
	loop	SNOW1
	pop	ds		;restore ds
	jmp	LB1

NOSNOW:
	cld
rep	movsb
	pop	ds
LB1:
	ENDM

;------------------------------------------------
; RES23H - Restore Control Break Handler
;------------------------------------------------

RES23h	MACRO	TASK
	SETINT	23h, [TASK+46], [TASK+48]
	ENDM

;------------------------------------------------
; RESREGS - Restore 8086 From Tcb
;------------------------------------------------

RESREGS MACRO   TASK
        mov     [TASK], 1	;1 means process awakened
        mov     ax, [TASK+2]
        mov     bx, [TASK+4]
        mov     cx, [TASK+6]
        mov     dx, [TASK+8]
        mov     bp, [TASK+10]
        mov     di, [TASK+12]
        mov     si, [TASK+14]
        mov     es, [TASK+16]
        mov     ds, [TASK+18]
	cli
        mov     ss, [TASK+20]
        mov     sp, [TASK+22]
	sti
        push    [TASK+28]       ;flags
        push    [TASK+26]       ;cs
        push    [TASK+24]       ;ip
        ENDM

;------------------------------------------------
; RESDTA - Restore Dta Address From Tcb
;------------------------------------------------

RESDTA  MACRO   TASK
        mov     ds, [TASK+32]	;restore dta segment
        mov     dx, [TASK+34]	;restore dta offset
        mov     ah, 1ah		;set dta function number
        int     21h		;call dos
        ENDM

;------------------------------------------------
; RES24H Restore Critical Error Handler From Tcb
;------------------------------------------------

RES24h  MACRO   TASK
        SETINT  24h, [TASK+36], [TASK+38]
        ENDM

;------------------------------------------------
; RESPRC - Restore Entire Process
;------------------------------------------------

RESPRC  MACRO   TASK
 	RESSCR	TASK
	RESCUR	TASK
	RES23H	TASK
  	RES24H	TASK
  	RESDTA	TASK
 	LODTCK	TASK
        RESREGS TASK
        ENDM

;------------------------------------------------
; TSR - Terminate And Stay Resident
;------------------------------------------------

TSR     MACRO   PARA
        mov     dx, PARA	;put paragraphs to save in dx
	mov	ah, 31h		;tsr function number
        int     21h		;call dos
        ENDM

;--------------------------------------------------
; EOI - Restore Interrupts To 8259 - reset hardware
;--------------------------------------------------

EOI     MACRO
        cli			;don't interrupt this!
        push    ax		;get ax out of the way
        mov     al, 20h		;enable interrupts value 20h
        out     20h, al		;send to port 20h
				;it is only a coincidence that the
				;values are the same!
        pop     ax		;restore ax
        sti			;re-enable interrupts
        ENDM

;-------------------------------------------------------
; BEEP - Sound Bell - Utility macro, not used in program
;-------------------------------------------------------

BEEP    MACRO
        mov     ah, 14
        mov     al, 7
        int     10h
        ENDM

;------------------------------------------------
; CHIRP
;------------------------------------------------

CHIRP	MACRO	QUAL
	LOCAL	cp1
	push	cx
	push	ax
	in	ax, 97
	and	ax, 0fch
	mov	cx, QUAL
cp1:
	out	97, ax
	inc	ax
	inc	ax
	out	97, ax
	dec	ax
	dec	ax
	loop	cp1
	pop	ax
	pop	cx
	ENDM

;-------------------------------------------------------------------------
; PUTCHAR - Put A Character To Screen - Utility macro, not used in program
;-------------------------------------------------------------------------

PUTCHR  MACRO   CHR
	push	ax
        mov     ah, 14
        mov     al, CHR
        int     10h
	pop	ax
        ENDM

;------------------------------------------------
; SETINT - Set An Interrupt Vector
;------------------------------------------------

SETINT  MACRO   INTR, SEG, OFF
        push    ds		;get ds out of the way
        push    SEG		;get segment into ds
        pop     ds
        mov     dx, OFF		;get offset into dx
        mov     al, INTR	;put interrupt number into al
        mov     ah, 25h		;set interrupt, function number
        int     21h		;call dos
        pop     ds		;restore ds
        ENDM

;------------------------------------------------
; SAVINT - Save An Interrupt Vector
;------------------------------------------------

SAVINT  MACRO   INTR, SEG, OFF
        mov     al, INTR	;move interrupt number into al
        mov     ah, 35h		;get interrupt function number
        int     21h		;call dos
        mov     SEG, es		;save vector segment
        mov     OFF, bx		;save vector offset
        ENDM

;------------------------------------------------
; SHWCHR - put character to video ram
;------------------------------------------------

SHWCHR  MACRO   CHAR, LOC
        push    es
        push    ax
	push	bx
	mov	ax, 0b800h
	push	ax
	pop	es
        mov     al, CHAR
	mov	ah, 23
        mov     bx, LOC
	mov	es:[bx], ax
	pop	bx
        pop     ax
        pop     es
	ENDM

;------------------------------------------------
; MOVMEM - copy a block of memory
;------------------------------------------------

MOVMEM	MACRO	SSEG, SOFF, DSEG, DOFF, NUM
	push	ds
	push	si
	push	es
	push	di
	push	cx

	push	DOFF
	push	DSEG
	push	SOFF
	push	SSEG

	pop	ds      	;set source segment
	pop	si      	;set source offset
	pop	es      	;set destination segment
	pop	di      	;set destination offset
	cld			;clear direction flag
	mov	cx, NUM		;number of bytes to move
rep	movsb

	pop 	cx
	pop 	di
	pop 	es
	pop 	si
	pop 	ds

	ENDM

;-----------------------------------------------------
; ABORT - takes an error code and $ terminated message
;-----------------------------------------------------

ABORT	MACRO	CODE, MESSAGE
	push	ds
	mov	ax, 0
	call	clear
	push	cs
	pop	ds
	mov	dx, offset err0
	mov	ah, 9
	int	21h
	mov	dx, offset MESSAGE
	mov	ah, 9
	int	21h
	pop	ds
	mov	ah, 4ch
	mov	al, CODE
	int	21h
	ENDM

;------------------------------------------------
; END OF WHMACS
;------------------------------------------------
