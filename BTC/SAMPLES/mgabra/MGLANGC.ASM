;------------------------------------------------
; MGABRA! LINKAGE MACROS FOR TURBO C LANGUAGE
; these define how your language passes parameters
; to assembly language functions. As set now for
; example, the first paramter (PARM1) lies at offset
; 6 into the stack (for turbo and other c's). You must
; define the specifics for your language. Also are the
; general entry and exit routines that must be
; modified for your language. All callable routines
; return 1 int (16 bit) value. These are for TURBO-C.
; If your language requires you to cleanup the
; stack then you have to replace the zeros with
; the proper number of words to pop off
;------------------------------------------------

;------------------------------------------------
; INPUT PARAMTERS - For C
;------------------------------------------------

PARM1	EQU	[BP+14]
PARM2	EQU	[BP+16]
PARM3	EQU	[BP+18]
PARM4	EQU	[BP+20]
PARM5	EQU	[BP+22]
PARM6	EQU	[BP+24]

;------------------------------------------------
; NEGATIVE OFFSET OF PSP FROM CS
;------------------------------------------------

PSPOFFSET	equ	10h

;------------------------------------------------
; RETURN PARAMTERS - For C
;------------------------------------------------

RETHIGH	MACRO	VAL		;to return high word of 32 bit return value
	push	VAL
	pop	dx
	ENDM

RETVAL	MACRO	VAL		;return normal 16 bit value or low word of
	push	VAL		;32 bit value
	pop	ax
	ENDM

;------------------------------------------------
; ENTRY SETUP
;------------------------------------------------

LANGIN	MACRO				;c entry routine
	push	ds
	push	es
	push	di
	push	si
	push	bp
	mov	bp, sp
	ENDM

;------------------------------------------------
; EXIT CLEANUP
;------------------------------------------------

LANGRET MACRO	WORDS			;c exit routine
	pop	bp
	pop 	si
	pop 	di
	pop 	es
	pop 	ds
	ret
	ENDM

;------------------------------------------------
; END OF MGLANGC.ASM
;------------------------------------------------
