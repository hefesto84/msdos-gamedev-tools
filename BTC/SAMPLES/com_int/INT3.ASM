;----------------------------------------------------------------------------
;    INT3.ASM		August 23,1987
;
; Data input/output via RS-232/2 (COM 2)
;      * initialization
;      * ISR
;----------------------------------------------------------------------------
int3_TEXT	SEGMENT byte public 'CODE'
int3_TEXT	ENDS
_DATA		SEGMENT byte public 'DATA'
_DATA		ENDS
_BSS		SEGMENT byte public 'BSS'
_BSS		ENDS
;
DGROUP		GROUP	_DATA,_BSS

            ASSUME cs:int3_TEXT, ds:DGROUP, es:DGROUP
;
;----------------------------------------------------------------------------
;
_DATA		SEGMENT byte public 'DATA'
old_int3_CS	dw	0		;old interrupt 4 CS
old_int3_IP	dw	0		;old interrupt 4 IP
_DATA		ENDS
;
;----------------------------------------------------------------------------
;	PROGRAM EXTERNALS
;
	extrn	_com2_input_queue:byte
	extrn	_com2_in_queue_ptr:word

	extrn	_com2_rs232_error:word
	extrn	_com2_port_status:word
	extrn 	_com2_interrupt_status:word

	extrn	_com2_out_queue_ptr:word
	extrn	_com2_out_queue_out:word
	extrn	_com2_output_queue:byte
;
;----------------------------------------------------------------------------
;	PROGRAM EQUATES
;
BASE_ADR	EQU	02F8h		;COM 2 port base address
BUFSIZ		EQU	2048		;size of data queues
;
; Don't enable the xmit interrupt unless there is something to xmit
;
;        1 ='s enable interrupt
;        0 ='s disable interrupt
;
;   Bits  7 to 4      ------    unused
;              3      ------    change in modem status register
;              2      ------    data reception error
;              1      ------    transmit register empty
;              0      ------    data received
;
INT_CODE	EQU	1101B		;receive, error and modem change
					;interrupts are enabled here

RTS_ON		EQU	0bh		;interrupts on, RTS on, DTR on
RTS_OFF		EQU	09h		;interrupts on, RTS off, DTR on
;
;----------------------------------------------------------------------------
;    Initialize the 8250 UART depending on parameter given as argument
;	and enable communication interrupts via INT 4 (primary com. port)
;
;   BIT    7   6   5          4    3         2                  1     0
;        { baud rate }      { parity } { stop bits }         { word length }
;    110 = 0   0   0   NONE = 0    0    1 =  0       7 bits =   1      0
;    150 = 0   0   1    or    1    0    2 =  1       8 bits =   1      1
;    300 = 0   1   0    ODD = 0    1
;    600 = 0   1   1   EVEN = 1    1
;   1200 = 1   0   0
;   2400 = 1   0   1
;   4800 = 1   1   0
;   9600 = 1   1   1
;
;----------------------------------------------------------------------------
;
int3_TEXT	SEGMENT byte public 'CODE'
;
	PUBLIC  _com2_set_interrupt
_com2_set_interrupt	PROC	FAR
	push	bp		;save the registers used
	mov	bp,sp		;make a copy for variable access

	push	ax		;save all registers used
	push	bx
	push	dx
	push	ds
	push	es

	mov	ah,35h		;get vector function
	mov	al,0bh		;want INT 3 CS and IP vector
	int	21h		;execute DOS function call
	mov	old_int3_CS,es	;ES contains the CS for INT 3
	mov	old_int3_IP,bx	;BX contains the IP for INT 3

	mov	dx,offset INT3	;point DS:DX to COM routine
	mov	ax,seg    INT3	;segment
	mov	ds,ax		;point DS
	mov	al,0bh		;INT 3 vector
	mov	ah,25h		;function to change vector
	int	21h		;execute DOS function call

	mov	ax,[bp+6]	;grab baud rate and parms
	mov	dx,1		;0 = COM 1    1 = COM 2
	int	14h		;initialize RS232 port to given parameters
				;(warning: COM interrupts are left disabled)

	mov	dx,BASE_ADR+4	;modem control register (2fc)
	mov	al,RTS_ON 	;INT's on(bit3), RTS on(bit1), DTR on(bit0)
	out	dx,al

	mov	dx,BASE_ADR+1	;interrupt enable register (2f9)
	mov	al,INT_CODE	;which interrupts to enable
	out	dx,al		;enable interrupts on 8250

	mov	dx,BASE_ADR	;point at recieve buffer register(2f8)
	in	al,dx		;read any trash in buffer

	mov	dx,BASE_ADR+5	;line status register(2fd)
	in	al,dx		;read any trash in status register

	mov	dx,BASE_ADR+6	;modem status register (2fe)
	in	al,dx		;read any trash in status register

	in	al,21h		;find out which interrupts are currently on
	and	al,0f7h		;enable bit 3 (INT 3 ='s SECONDARY COM. PORT)
	out	21h,al		;rewrite new interrupt flags

	pop	es		;restore registers used
	pop	ds
	pop	dx
	pop	bx
	pop	ax
	pop	bp
	ret
_com2_set_interrupt	ENDP
;
;----------------------------------------------------------------------------
;
;  This routine restores the interrupt vector which was modified by the
;	_set_interrupt routine so don't call this routine without 1st
;	 calling the other routine.
;
	PUBLIC  _com2_restore_interrupt
_com2_restore_interrupt	PROC	FAR
	push	ax		;save all registers used
	push	dx
	push	ds

	mov	dx,old_int3_IP	;point DS:DX to COM routine
	mov	ax,old_int3_CS	;segment
	mov	ds,ax		;point DS
	mov	al,0bh		;INT 3 vector
	mov	ah,25h		;function to change vector
	int	21h		;execute DOS function call

	in	al,21h		;read current interrupt status
	or	al,08h		;disable bit 3 (INT 3)
	out	21h,al		;rewrite new interrupt status

	pop	ds		;restore all registers used
	pop	dx
	pop	ax
	ret
_com2_restore_interrupt	ENDP
;
;----------------------------------------------------------------------------
; This is the interrupt routine that is executed with every
;   INT 3.
;
;  Options currenty implemented are:
;	1. receive interrupt
;	2. data reception error interrupt
;	3. transmit holding register empty interrupt
;	4. change in modem status register
;
;	Receive interrupt places characters in _input_queue at _in_queue_ptr
;		bytes into the queue.
;
;	Data reception error places the error int _rs232_error.
;
;	Transmit interrupt xmits characters in _output_queue at _out_queue_ptr
;		bytes into the queue.
;
;	Modem status register change stores its status in port_status
;
;----------------------------------------------------------------------------
;
	public	INT3

INT3	PROC	FAR
	push	ds		;always save anything you use in an INT routine
	push	di
	push	dx
	push	cx
	push	ax

	mov	ax, SEG _DATA		;make sure your pointing to the correct
	mov	ds,ax			; segment otherwise it's lockup city.
;----------------------------------------------------------------------------
pending_interrupt:
	mov	dx,BASE_ADR+2		;interrupt ident. register (2fa)
	in	al,dx			;read whose requesting service

	mov	ah,00h			;zero high byte
	mov	_com2_interrupt_status,ax;store status

	cmp	al,00h			;modem interrupt?
	je	status_int		;YES! jump

	cmp	al,02h			;is it a transmit buf empty interrupt?
	je	transmit_int		;YES! jump

	cmp	al,04h			;is it a recieve interrupt?
	je	receive_int		;YES! jump

	cmp	al,06h			;is it reception error interrupt?
	je	reception_error		;YES! jump

	jmp	leave_interrupt		;no more left.... so leave
;
;----------------------------------------------------------------------------
;	Modem status register change
;
status_int:
	mov	dx,BASE_ADR+6		;modem status register (2fe)
	in	al,dx

	mov	ah,00h			;zero high byte
	mov	_com2_port_status,ax	;store status
	jmp	leave_interrupt		;go see if any interrupts pending
;
;----------------------------------------------------------------------------
; Receive buffer routine
;
receive_int:
	mov	dx,BASE_ADR		;COM 1 recieve buffer (2f8)
	in	al,dx			;read the character in

	mov	dl,al			;save character in another register for now

	mov	di,[_com2_in_queue_ptr]	;grab input pointer

	mov	_com2_input_queue[di],dl;and place the char in the queue

	cmp	di,BUFSIZ		;check for queue wrap
	jge	reset_queue		;reset if past end or equal

	inc	word ptr [_com2_in_queue_ptr] ;bump input pointer
	jmp	leave_interrupt
reset_queue:
	mov	word ptr [_com2_in_queue_ptr],0 ;reset pointer to beginning
;----------------------------------------------------------------------------
leave_interrupt:
	mov	dx,BASE_ADR+2		;interrupt ident. register (2fa)
	in	al,dx			;read whose request service

	and	al,01h			;is there a pending interrupt?
	jz	pending_interrupt	;YES! jump
ack_interrupt:
	mov	al,20h			;EOI code
	out	20h,al			;acknowledge END OF INTERRUPT

	pop	ax			;put em back so the machine doesn't
	pop	cx			; DIE!
	pop	dx
	pop	di
	pop	ds
	iret				;interrupt return
;
;----------------------------------------------------------------------------
; Reception error routine
;
reception_error:
	mov	dx,BASE_ADR+5		;line status register (2fd)
	in	al,dx			; read which error occured

	mov	ah,00h			;zero high byte
	mov	_com2_rs232_error,ax	;save error flags for someone else

	jmp	leave_interrupt		;go look for pending interrupts
;
;----------------------------------------------------------------------------
; Transmit buffer empty routine
;
transmit_int:

	mov	ax,[_com2_out_queue_ptr];grab input pointer
	mov	di,[_com2_out_queue_out];grab output pointer

	cmp	ax,di			;are the pointers equal?
	jz	disable_xmit_int	;YES! then disable interrupts

	mov	dx,BASE_ADR+6		;point at modem status reg. (2fe)
	mov	cx,200			;give it 200 trys for CTS signal

wait_for_CTS:
	in	al,dx			;read port status
	and	al,10h			;test CTS line
	loopnz	wait_for_CTS		;


	mov	al,_com2_output_queue[di];grab next char to output
	mov	dx,BASE_ADR		;output buffer port (2f8)
	out	dx,al			;send character out

	cmp	di,BUFSIZ		;check for queue wrap
	jge	out_queue_reset		;reset if past end or equal

	inc	word ptr [_com2_out_queue_out] ;bump output pointer
	jmp	leave_interrupt		;and check for pending interrupts
out_queue_reset:
	mov	word ptr [_com2_out_queue_out],0 ;reset pointer to beginning
	jmp	leave_interrupt		;and check for pending interrupts

disable_xmit_int:
	mov	dx,BASE_ADR+1		;interrupt enable register (2f9)
	in	al,dx			;read current interrupt status
	and	al,11111101B		;turn off xmit interrupts (fdh)
	out	dx,al			;disable xmit interrupts on 8250
	jmp	leave_interrupt		; and check for pending interrupts

INT3	ENDP
;----------------------------------------------------------------------------
int3_TEXT	ENDS
	END
