	NAME	c0romx
	PAGE	60,96
	TITLE	C0ROMX - Example ROM Startup Code
;[]------------------------------------------------------------[]
;|                                                              |
;|	C0ROMX.ASM -- Example ROM Startup Code			|
;|								|
;|	Bob Haas	CompuServe 70357,3530			|
;|								|
;[]------------------------------------------------------------[]

;	Segment and Group declarations

_TEXT	SEGMENT BYTE PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT PARA PUBLIC 'DATA'
_DATA	ENDS
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS
_BSSEND SEGMENT BYTE PUBLIC 'BSSEND'
_BSSEND ENDS

DGROUP	GROUP	_BSS, _BSSEND
CGROUP  GROUP   _DATA,_TEXT

; The following values will depend upon the amount and location of RAM
; This example assumes 32K of RAM starting at 0000:0
; The "startds" value puts the _BSS segment at 0:100 (allowing for
;  up to 256 bytes of interrupt vectors).
;  _BSS is 28K less 256 bytes

startds	equ	010h

; These values set up the last 4K of the 32K RAM as a stack

startss	equ	0700h
startsp	equ	0FFEh		; sets up 4K stack

	ASSUME	CS:CGROUP, DS:DGROUP

_TEXT	SEGMENT
	extrn	_main:near
        public	STARTX

STARTX	PROC	NEAR
	jmp	realstart
	db	'Here is a good place to put a copyright statement'

realstart:
        mov	ax,startds
        mov	ds,ax

	mov	ax,startss
	mov	ss,ax
	mov	sp,startsp

; Uncomment the following lines if you need to have
; uninitialized static variables set to 0 as in "ordinary" C

;	xor	ax, ax
;	push	ds
;	pop	es
;	mov	di, offset DGROUP: bdata@
;	mov	cx, offset DGROUP: edata@
;	sub	cx, di
;	rep	stosb

	jmp	_main   ; will not return

STARTX          ENDP

_TEXT	ENDS

_BSS	SEGMENT
bdata@	label	byte
_BSS	ENDS

; The _BSSEND segment is defined only so that we can get "edata@"
;  as the end of _BSS for the above initialization code
; No other module defines a _BSSEND segment, so "edata@" is
;  guaranteed to be the last byte of _BSS

_BSSEND SEGMENT
edata@	label	byte
_BSSEND ENDS
	public	bdata@	; make these show on the map
	public	edata@	; they don't need to be public otherwise
	END

