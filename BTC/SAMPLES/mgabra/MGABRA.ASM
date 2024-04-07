PAGE    55,132
TITLE   ABRACADABRA! MAGIC SOFTWARE TSR/TCR (Terminate Cont Running) LIBRARY

INCLUDE MGMMACS.ASM

DEINT   EQU     0e0h            ;Deinstall Interrupt

STKSAV  EQU     32

INCLUDE MGLANGC.ASM

;------------------------------------------------
; SEGMENT DEFINITIONS
;------------------------------------------------

	EXTRN __psp:WORD

_BSS    SEGMENT WORD PUBLIC 'BSS'
_BSS    ENDS

DGROUP  GROUP   _DATA, _BSS
_DATA   SEGMENT WORD PUBLIC 'DATA'
        ASSUME  DS:DGROUP, SS:DGROUP
_DATA   ENDS

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
        ASSUME  CS:_TEXT

 	EXTRN _tsrexit:FAR
 
;------------------------------------------------
; PUBLICS - Functions you can call
;------------------------------------------------

        PUBLIC  _swpscr
        PUBLIC  _hotkey
        PUBLIC  _timer
        PUBLIC  _swap
        PUBLIC  _tsrset
        PUBLIC  _swping
        PUBLIC  _swapno
	PUBLIC  _swapyes
        PUBLIC  _status
        PUBLIC  _id
        PUBLIC  _putchr
        PUBLIC  _conout
        PUBLIC  _inkey
	PUBLIC  _bykey

;-----------------------------------------------------
; TCB (Task Control Blocks)
; Areas where various info about the processs are kept
;-----------------------------------------------------

dosprg   dw      60 dup (0)             ;TCB for TSR program
tsrprg   dw      60 dup (0)             ;TCB for Interrupted program

dosprgs  db      4000 dup (0)           ;area for saving TSR's screen
tsrprgs  db      4000 dup (0)           ;area for saving interrupted's screen

swptog  dw      0                       ;swapping disabled? 1 = yes
scrswp  dw      0                       ;screen swap mode
timed   dw      0                       ;time triggering on? 1 = yes
by28    dw      0                       ;invoked within int28 call?
keyswap dw      0                       ;was swapping done on key control?
keypop	dw	0			;last swap by key? (retained)
period  dw      0                       ;how many 1/18sec do we run for?
doswap  dw      0                       ;"I want to swap" flag
inproc  dw      0                       ;busy swapping processes flag
hard_bz dw      0                       ;hardware busy flag
flag_28 dw      0                       ;interrupt 28 live
prob28h dw      0                       ;is int28 a problem? 0 = yes
videosz dw      0                       ;size of video screen
videos  dw      0                       ;dynamic base of video seg
videom  dw      0                       ;current video mode
videop  dw      0                       ;current video page
procid  dw      0                       ;this programs id number
stflag  dw      0                       ;polled status
errflag dw      0                       ;critical error number
envadd  dw      0                       ;paragraph of environment
entry   dw      0                       ;IP of TSR upon initialization
indoss  dw      0                       ;dos busy flag segment
indoso  dw      0                       ;dos busy flag offset
init    dw      0                       ;TSR initialized?
savpar  dw      0                       ;paragraphs to save
add6845 dw      0                       ;address of 6845 video controller
color   dw      0b800h                  ;base of color video area
mono    dw      0b000h                  ;base of monochrome video area
hotkey  db      0                       ;hotkey scan code
hotstat db      0                       ;hot key shift key
err0	db	'ABRACADABRA ERROR: $'
err1	db	'Not Enough Memory$'
err2	db	'Illegal Use of DOS Function Below 0CH$'
err3	db	'Insufficient Memory - Language Does Not Return Excess to Dos$'
err4	db	'Synchronization$'
hithere db	'Source Code Is Available Dude! Walt Howard 213-477-4151'
coprght db      'Copyright (c) 1988 by Walter F. Howard Jr.'
vrsn    db	'1.2 For TURBO C ONLY!'

;-------------------------------------------------------
; DOS interrupt values
; Here is where the old interrupt vectors are kept saved
;-------------------------------------------------------

disk_int label  dword                   ;int 13h (disk) vector
old_13h dw      2 dup(?)

vid_int	 label	dword			;int 10h (video) vector
old_10h	dw	2 dup(?)

timer_int label dword                   ;int 1ch vector
old_timer dw    2 dup(?)

key_int label   dword                   ;int 9h vector
old_key dw      2 dup(?)

back_int label  dword                   ;int 28h vector
old_28h dw      2 dup(?)

clear_int label dword                   ;clear out tsr interrupt
old_clr dw      2 dup(?)

;------------------------------------------------
; INTERRUPT HANDLERS
;------------------------------------------------

;------------------------------------------------
; stime - every timer tick (1/18sec) comes here
;------------------------------------------------

stime   proc    far
        sti

	pushf
	call 	timer_int	     ;pass on vector for others first

        cmp     timed, -1            ;Is time triggering enabled?
        jne     time1                ;no then don't do rest of this block
        cmp     doswap, 1            ;want to swap?
        je      time1                ;yes then skip next section
        dec     period               ;count down
        cmp     period, 0            ;is it zero yet?
        jg      time1                ;if no then skip next
        mov     doswap, 1            ;if yes then set doswap on

time1:

        cmp     swptog, 1            ;is swapping disabled?
        jne     time2                ;no? then continue
        jmp     far ptr time9        ;else exit

time2:

        cmp     doswap, 1            ;is doswap on
        je      time3                ;yes jump into next check
        jmp     far ptr time9        ;else exit

time3:

        cmp     inproc, 0            ;are we already engaged?
        je      time4                ;no then proceed to next check
        jmp     far ptr time9        ;else jump out

time4:

        mov     inproc, 1            ;signal we are engaged
        cmp     hard_bz, 0           ;bios service in progress?
        je      time5                ;no then proceed to next check
        jmp     far ptr time8        ;else exit

time5:

        cmp     prob28h, 0           ;int 28 a problem? (0 = yes)
        je      time6                ;yes, goto check dos busy
        cmp     flag_28, 0           ;else, is int 28 in progress?
        je      time6                ;no, check for dos busy
        cmp     by28, 1              ;are we currently active on a 28 int?
        je      time7                ;good! we can interrupt without
                                     ;checking dos busy if TSR written
                                     ;to not use dos below 0ch
time6:

        push    es                   ;save ES and DI
        push    di
        mov     es,indoss            ;get DOS segment in ES
        mov     di,indoso            ;address of DOS BUSY_FLAG in DI
        cmp     byte ptr es:[di],0   ;DOS service currently active?
        pop     di                   ;clean up
        pop     es

        je      time7                ;not busy enter SWAP routine
        jmp     far ptr time8        ;else, exit

time7:

	jmp	far ptr swpprc

time8:

        mov     inproc, 0            ;no longer in process

time9:

	cmp	by28, 0
	je	time10
	int	28h

time10:

        iret

stime   endp

;-----------------------------------------------------------------
; bakprc - interrupt 28 spooler interrupt almost as good as timer
;-----------------------------------------------------------------

bakprc  proc    far
        sti

	pushf
	call	back_int	     ;pass it on for others first

        mov     flag_28, 1           ;signal that int 28 in progress
        cmp     prob28h, 0           ;Will we be using dos funcs below 0ch?
        jne     bak1                 ;no? good, jump to next check
        mov     flag_28, 0           ;otherwise no go, have to deny
        iret                         ;return to calling process

bak1:

        cmp     swptog, 1            ;is swapping disabled?
        jne     bak2                 ;no? then continue
        jmp     far ptr bak7         ;else exit

bak2:

        cmp     doswap, 1            ;do we have a doswap ????
        je      bak3                 ;yes?? oh good! go to next check
        jmp     far ptr bak7         ;no? oh well, exit

bak3:

        cmp     inproc, 0            ;are we already engaged?
        je      bak4                 ;no? ok, goto next check
        jmp     far ptr bak7         ;otherwise we're outta here

bak4:

        mov     inproc, 1            ;tell them I'm busy
        cmp     hard_bz, 0           ;is bios service active?
        je      bak5                 ;no? ok, go to swap block
        jmp     far ptr bak6         ;otherwise we'll have to wait

bak5:

        mov     by28, 1
        jmp	far ptr swpprc	     ;swap them out

bak6:

        mov     inproc, 0            ;we are not in progress any more!

bak7:

        mov     flag_28, 0           ;int 28 no longer active
	iret

bakprc  endp

;------------------------------------------------
; kbint keyboard checker
;------------------------------------------------

kbint   proc    far
        sti                           ;re-enable interrupts
        push    ax                    ;save registers

        cmp     hotkey, 0             ;tsr key swapping enabled?
        je      kbint2                ;if not then exit

        in      al,60h                ;get scan code from keyboard
        cmp     al, hotkey            ;is it the hotkey?
        jne     kbint2                ;no, then exit

        mov     ah, 2                 ;check shift key status
        int     16h                   ;keyboard bios services
        and     al,15                 ;remove capslock, nlock, slock,insert
        cmp     al, hotstat           ;is the special key pressed?
        jne     kbint2                ;no, then exit

	cmp	inproc, 1
	je	kbint4

        mov     keyswap, 1            ;key was used to initiate swap
        mov     doswap, 1             ;signal we want to swap processes

kbint4:

        call    kbrst                  ;reset keyboard, issue EOI
        pop     ax
        iret                            ;end interrupt routine

kbint2:

        pop     ax
        jmp     key_int                 ;pass control on to others

kbint   endp

;------------------------------------------------
; dchk - bios disk services interrupt
;------------------------------------------------

dchk    proc    far
	sti				;***
        inc     hard_bz                 ;we are in a disk service
        pushf
        call    disk_int                ;into bios we goooooooooo!
	pushf
        dec     hard_bz                 ;ok, were done
	popf
        ret     2                       ;back to calling process
dchk    endp

;------------------------------------------------
; vchk - video services interrupt
;------------------------------------------------

vchk    proc    far
	sti				;***
        inc     hard_bz                 ;we are in a video service
        pushf
        call    vid_int                 ;into bios we goooooooooo!
        dec     hard_bz                 ;ok, were done
        iret                            ;back to calling process
vchk    endp

;------------------------------------------------
; crerr - if critical error come here
;------------------------------------------------

crerr   proc    far
        sti
        iret
crerr   endp

;-----------------------------------------------------------
; Ctrl_Bk - Control Break Handler - naturally we don't
; want a TSR to abort so we disable the abort
;-----------------------------------------------------------

ctrl_bk proc    far
        sti
        iret
ctrl_bk endp

;------------------------------------------------
; PUBLIC FUNCTION DEFINITIONS
;------------------------------------------------

;------------------------------------------------
; id - set this programs id number
;------------------------------------------------

_id     proc    far
        LANGIN
        mov     ax, PARM1
        cmp     ax, 0
        je      id1
        mov     procid, ax
id1:
        mov     ax, procid
        LANGRET
_id     endp

;------------------------------------------------
; status - set removability status
;------------------------------------------------

_status proc    far
        LANGIN
        mov     ax, PARM1            ;ax is integer value
        mov     stflag, ax           ;error message
        LANGRET 0
_status endp

;------------------------------------------------
; CLEAR - Clear task out
; This is a two step process first usually the calling
; program will request information to ensure the TCR is
; in a state where deinstallation is safe. The calling
; process sets it's ax to 0 and issues int DEINT. The
; TSR will return these values in these registers:
; COMMAND AX = 1
; RETURNS:
; ax = remove status value (set with the status function)
; cx = process id (set with ID function)
; bx = segment of next TSR's clear function
; dx = offset of next TSR's clear function
;
; Command 2 is almost identical to call 1 except it is assuming
; the call is made with a plain call and not an interrupt call
;
; COMMAND AX = 2
; RETURNS:
; ax = remove status value (set with the status function)
; cx = process id (set with ID function)
; bx = segment of next TSR's clear function
; dx = offset of next TSR's clear function
; Using the status function from within the TSR the programmer
; can set up a value which will indicate to the calling program
; certain things, foremost being whether or not it is
; safe to uninstal. You the programmer can decide what this
; integer value is. For example a 1 may mean files are open
; so your deinstall routine can check with the user and inform
; him of this and therefor request verification for removal.
; The register paid bx:dx returns the address of the next tsr's
; clear function. You could call it directly and poll THAT one too!
;
; The actual de-installation requires a 1 in the AX register
; This causes the TSR to return all the interrupts it has
; borrowed to their former owners. This effectively puts the
; TSR to sleep. All that the calling program must do now
; is deallocate the memory the TSR was occupying.
;
; COMMAND AX = 0
; RESETS ALL BORROWED INTERRUPTS
; RETURNS:
; ax = remove status value (set with the status function)
; bx = pointer to environment (paragraph absolute)
; cx = process id
; dx = code segment
;------------------------------------------------

clear   proc    far
        sti

clear1:

        cmp     tsrprg, 1     	     ;Wait until all pending
        je      clear1               ;situations are finished
	cmp	doswap, 1
	je	clear1
	cmp	inproc, 1
	je	clear1
        cmp     ax, 0                ;remove?
        je      clear2               ;if so do it
        mov     ax, stflag   	     ;return status flag value
        mov     cx, procid   	     ;return process id
        mov     bx, old_clr[2]       ;next tsr segment
        mov     dx, old_clr          ;next tsr offset
        ret

clear2:

        mov     doswap, 0                    ;shut off doswap
        mov     inproc, 1                    ;we are busy
	mov	swptog, 1		     ;disable swapping
        SETINT  1ch, old_timer[2], old_timer ;restore borrowed interrupts
        SETINT  28h, old_28h[2], old_28h
        SETINT  9h, old_key[2], old_key
        SETINT  13h, old_13h[2], old_13h
	SETINT	10h, old_10h[2], old_10h
        SETINT  DEINT, old_clr[2], old_clr
 	call	_tsrexit
        push    cs              	     ;return code segment to caller so it can
        pop     dx              	     ;deallocate this space
        mov     ax, stflag   	             ;return status
        mov     bx, envadd   	             ;return environment address
        mov     cx, procid   		     ;return process id
        ret

clear   endp

;------------------------------------------------
; swapno - Enables/Disables swapping
;------------------------------------------------

_swapno proc    far
        LANGIN
        mov     swptog, 1
        LANGRET 0
_swapno endp

;------------------------------------------------
; swapyes - Enables/Disables swapping
;------------------------------------------------

_swapyes proc    far
         LANGIN
         mov     swptog, 0
         LANGRET 0
_swapyes endp

;------------------------------------------------
; timer  - Re-hooks time slice
;------------------------------------------------

_timer   proc   far
         LANGIN
         mov    ax, PARM1              ;are they both zero?
         add    ax, PARM2
         cmp    ax, 0
         je     timer1                 ;if so exit
         mov    timed, -1              ;else enable timer
         mov    ax, PARM1
         add    ax, 2
	 mov	bx, [dosprg+44]
         mov    [dosprg+44], ax        ;store dos programs ticks
         mov    ax, PARM2
         add    ax, 2
	 mov	cx, [tsrprg+44]
         mov    [tsrprg+44], ax        ;store TSR program's ticks
         jmp    timer2

timer1:

         mov    timed, 2

timer2:

         mov    ah, bl
	 mov	al, cl
         LANGRET 0
_timer   endp

;------------------------------------------------
; swap - Swap out this task enables program to
; ask for a swap under it's control
;------------------------------------------------

_swap   proc    far
        LANGIN
        mov     doswap, 1              ;set doswap on

swap1:

        cmp     doswap, 0              ;loop until processes are swapped
        jne     swap1                  ;usually in a wink of an eye
        LANGRET 0
_swap   endp

;------------------------------------------------
; hotkey - Enables/Disables hot key action
;------------------------------------------------

_hotkey proc    far
        LANGIN
        mov     ax, PARM1
        mov     hotkey, al           ;save the key code
        mov     hotstat, ah          ;save the special key code
        LANGRET 0
_hotkey endp

;------------------------------------------------
; scrswp - enables screen saving
;------------------------------------------------

_swpscr proc    far
        LANGIN
        mov     ax, PARM1
        mov     scrswp, ax           ;set swap mode
        LANGRET 0
_swpscr endp

;------------------------------------------------
; swping - are processes multitasking?
;------------------------------------------------

_swping proc    far
        LANGIN
	mov	ax, 0
	cmp	timed, -1
	jne	swp1
	mov	ax, 1
swp1:	
        LANGRET 0
_swping endp

;------------------------------------------------
; bykey - was last swap done with key?
;------------------------------------------------

_bykey	proc	far
	LANGIN
	mov	ax, keypop
	LANGRET 
_bykey	endp

;------------------------------------------------
; inkey - Wait for and return next Keystroke
;------------------------------------------------

_inkey  proc    far
        LANGIN
        mov     ah, 0
        int     16h
        LANGRET 0
_inkey  endp

;------------------------------------------------
; conout - Output a String to the Console
;------------------------------------------------

_conout proc    far
        LANGIN
        mov     dx, PARM1       ;get segment of string
        mov     es, PARM2       ;get offset of string
conout1:
        mov     bx, dx
        mov     al, es:[bx]     ;get next char
        cmp     al, 0           ;is it null?
        je      con2
        mov     ah, 14          ;prepare bios service
        int     10h             ;call it
        inc     dx              ;prepare for next character
        jmp     conout1
con2:
        LANGRET 0
_conout endp

;--------------------------------------------------------------
; putchr - put character to video page
; putchr(int row, int col, int page, int att, char c)
;--------------------------------------------------------------

_putchr proc    far
        LANGIN
        mov     bx, PARM2       ;position cursor
        mov     cx, PARM1
        mov     dl, bl
        mov     dh, cl
        mov     bx, PARM3       ;set video page
        mov     bh, bl
        mov     ah, 2
        int     10h             ;position cursor
        mov     ax, PARM5       ;write character
        mov     bx, PARM4       ;set attribute into bl
        mov     dx, PARM3	;page number
        mov     bh, dl          ;page number
        mov     cx, 1           ;character count
        mov     ah,9            ;service number
        int     10h             ;fire

        LANGRET 0
_putchr endp

;------------------------------------------------
; TSRSET(hotkey, timer, paragraphs to save, swap screens, int28)
;
; the HOTKEY is passed as a word value, high byte is
; special key scan code status and low byte is scan key status
;
;    special key (high byte)
;
;    0 = normal
;    1 = right shift
;    2 = left shift
;    4 = control
;    8 = alt
;   16 = scroll lock
;   32 = num lock
;   64 = caps lock
;  128 = insert
;
; PARAGRAPHS TO SAVE is the size of memory to reserve for
; the program when it terminates
;
; the TIMER parameters determines how many timer ticks each process
; will get. The first parameter is how many ticks the dos program
; will get and the second is how many ticks the TSR gets
; This can be used to have a program continue to run when it is
; swapped out.
;
; The next parameter is the number of paragraphs to reserve
; in memory for the TSR. If the value is zero the TSR uses
; the position of the stack pointer + 32. This uses up a lot of
; memory in a small model program but you can set the exe file
; to have a lower stack with exemod
;
; The next parameter controls the manner in which the TSR will
; manage screen when it is swapped. Each bit in this word has
; a special function.
;
;    1 = swap cursor whenever programs swap
;    2 = save screen whenever programs swap
;    4 = restore screen whenever programs swap
;    8 = swap cursor only if TSR swap by key
;   16 = save screen only if TSR swap by key
;   32 = restore screen only if TSR swap by key
;   64 = turn cursor off on swap
;  128 = turn cursor on on swap
;  256 = don't take any precautions to avoid "snow" on screen
;
; The reason for the "invoked by key" parameters is so you can have
; your TSR running in background and if the user wants it full blown
; into foreground only then will the screens swap. You wouldn't want
; the screen swapping every 1/2 second if you were running the TSR
; on the timer ticks. You TSR can poll the funtion SWPING and if its
; true (-1) then don't have it write to the screen. If it's true then
; that means your TSR has the full screen so it's ok to write to it.
; Even better is to have your TCR (Terminate Continue Running) write
; all it's output to a video page other than 1. Then when you swap it
; in all the video output it has created is live on that page.
;
; The INT28 is an option. If your TSR program uses dos services
; under 0ch you will not be able to invoke it under certain
; circumstances. That is, whenever dos is using those services.
; They are all services that deal with screen IO and keyboard
; input. For greater versatility you should write your own
; screen and keyboard I/O and never use language statements like
; PRINT and INPUT (basic) but if you must, set this parameter to
; 0. You will not be able to invoke the TSR from the COMMAND.COM
; prompt. If your program does not use dos services less than
; 0ch you can set this flag to 1 which will allow you to invoke
; it in a greater number of circumstances.
;
;------------------------------------------------

_tsrset proc    far
        LANGIN
        cmp     init, 1             ;is it already installed
        jne     tsrset1             ;no, then install it
        LANGRET 0

tsrset1:

;------------------------------------------------
; basic setup routine
;------------------------------------------------

;------------------------------------------------
; save environment for use in de-installing
;------------------------------------------------

        push    cs                      ;get environment
        pop     bx                      ;get code segment
        sub     bx, 10h                 ;get psp
        push    bx                      ;push it again
        pop     es                      ;get it into es
        mov     bx, 2Ch                 ;get offset
        mov     ax, es:[bx]             ;get envadd
        mov     envadd, ax           	;save it

;------------------------------------------------
; get paragraphs to save paramter
;------------------------------------------------

        mov     ax, PARM4               ;get value of paragraphs to save
        mov     savpar, ax

;------------------------------------------------
; some miscellaneous things
;------------------------------------------------

	mov	swptog, 1	     ;disable swapping
        mov     doswap, 0	     ;0 out swap request
        mov     keyswap, 0           ;simulate first swap by key
        mov     inproc, 1            ;we are busy

;------------------------------------------------
; save int28 compatibility parameter
;------------------------------------------------

        mov     ax, PARM6            ;get INT28 parameter
        mov     prob28h, ax          ;save it

;------------------------------------------------
; save screenswap mode parameter
;------------------------------------------------

        mov     ax, 7                ;set scrswp to saveall
        mov     scrswp, ax           ;save it

;------------------------------------------------
; save hotkey values
;------------------------------------------------

        mov     ax, PARM1               ;check for hotkey operation
        cmp     ax, 0                   ;no hot key?
        je      tsrset2                 ;check for timed execution
        mov     hotkey, al              ;save the key code
        mov     hotstat, ah             ;save the special key code

;------------------------------------------------
; save timer tick/multitask values
;------------------------------------------------

tsrset2:

        mov     timed, 0                ;initialize value
        mov     ax, PARM2               ;get timer parameters
        add     ax, PARM3               ;see if they are both zero
        cmp     ax, 0                   ;do we want timer operation?
        je      tsrset3                 ;no?, exit this section

        mov     timed, -1               ;turn timer on
        mov     ax, PARM2
        add     ax, 2
        mov     [dosprg+44], ax         ;store dos program's ticks

        mov     ax, PARM3
        add     ax, 2
        mov     [tsrprg+44], ax         ;store TSR programs ticks

        mov     period, ax              ;set first period
        jmp     tsrset4

tsrset3:

        mov     timed, 2

tsrset4:

;------------------------------------------------
; get address of dos busy flag
;------------------------------------------------

        call    getb                    ;get address of the dos flag

;------------------------------------------------
; initialize pointers to screen save buffers
;------------------------------------------------

        mov     [dosprg+42], offset dosprgs ;set screen buffer addresses
        mov     [tsrprg+42], offset tsrprgs

;------------------------------------------------
; get base port address of 6845 video controller
;------------------------------------------------

        mov     ax, 40h                 ;get base port address of
        mov     es, ax                  ;6845 video contoller chip
        mov     di, 63h                 ;this is stored in the dos
        mov     dx, es:[di]             ;low memory area
        mov     add6845, dx

;------------------------------------------------
; set control break and critical error addresses
;------------------------------------------------

        SETINT  23h, cs, <offset ctrl_bk> ;set control break vector
        SETINT  24h, cs, <offset crerr>   ;set critical error handler

;--------------------------------------------------------
; save our TSR's state at this point for later invocation
;--------------------------------------------------------

        pushf                           ;prepare to snapshot TSR
        push    cs                      ;push CS
        mov     entry, offset here
        push    entry                   ;push program counter

        SAVPRC  tsrprg                  ;click! save it at this moment

here:

        cmp     init, 0                 ;are we initialized?
        je      tsrset5                 ;no, then don't do next

        LANGRET 0                       ;return

tsrset5:

;------------------------------------------------
; set up interrupt structure
;------------------------------------------------

        call    insint                  ;steal interrupts

;------------------------------------------------
; reset video page if we changed it
;------------------------------------------------

        SETPAG  0
        mov     ax, PARM5               ;restore proper swap screen param
        mov     scrswp, ax

;------------------------------------------------
; determine TSR memory default stack or set it?
; depends on passed parameter. If 0, then default
;------------------------------------------------

        mov     ax, savpar              ;get paragraphs to save
        cmp     ax, -1                  ;is it -1?
        je      tsrset7                 ;OK, allocate by stack
	cmp	ax, 0 			;is it 0 ?
	je 	tsrset8			;Allocate by 48h
	jmp	tsrset6			;else truncate at absolute

;------------------------------------------------
; set TSR top memory to by stack location (TURBOC)
;------------------------------------------------

tsrset7:

        mov     ax, sp                  ;where is stack pointer?
        mov     cl, 4                   ;divide it by 16
        shr     ax, cl
        push    ss                      ;get ss into dx
        pop     dx
        add     ax, dx                  ;add them up to absolute stack loc
        push    cs                      ;where is our code segment?
        pop     dx
        sub     ax, dx                  ;get the total memory from cs to ss:sp
        add     ax, STKSAV              ;add 32 for safety margin
        mov     init, 1                 ;we are initialized!
        mov     inproc, 0               ;no longer in process
	mov	swptog, 0
        TSR     ax                      ;terminate and stay resident

;------------------------------------------------
; Set TSR allocation by Memory Remaining
; Here we ask DOS for more memory. What we get we
; assume is the next paragraph above our program's
; needs so we truncate here
;------------------------------------------------

tsrset8:

	mov	bx, 2000h		;request 2 paragraphs
	mov	ah, 48h			;
	int	21h                     ;result is in ax
	mov	savpar, ax		;save it
	cmp	ax, 1000h
	jg	noprob

	ABORT 	1, err3

noprob:					;return our borrowed memory

	push	ax
	pop	es
	mov	ah, 49h
	int	21h		

noprob1:

	mov	ax, savpar		;get top paragraph
	sub	ax, __psp		;caculate difference
        mov     init, 1                 ;we are initialized!
        mov     inproc, 0               ;no longer in process
	mov	swptog, 0
        TSR     ax                      ;terminate and stay resident

;--------------------------------------------------------------------
; adjust program size - move stack down (ax contains # paras to save)
;--------------------------------------------------------------------

tsrset6:

        push    ss                      ;get stack segment as base
        pop     ax                      ;
        add     ax, savpar              ;add save for new top of stack
        mov     cl, 4                   ;
        shl     ax, cl                  ;x 16 for offset value
        mov     bx, 128                 ;how many stack para to save
        mov     cx, [tsrprg+22]         ;get stack pointer we want to save

        MOVMEM  ss, cx, ss, ax, bx      ;relocate the stack to ax
        mov     [tsrprg+22], ax         ;save new stack pointer!

        mov     ax, savpar              ;restore save paragraphs to ax
        add     ax, STKSAV              ;add safety margin
        mov     init, 1
        mov     inproc, 0
	mov	swptog, 0
        TSR     ax                      ;terminate and use passed parameter
                                        ;for memory save value
_tsrset endp

;------------------------------------------------
; INTERNAL SUPPORT FUNCTIONS
;------------------------------------------------

;------------------------------------------------
; swapit - Swap processes
;------------------------------------------------

swapit	proc	near

swpprc:

	mov	keypop, 0	      ;default keypop state
        cmp     timed, 2              ;timer always off?
	je	ts0

	cmp	keyswap, 1	      ;was swap request by key?
	jne	ts0		      ;no, then go ahead and swap
	mov	keypop, 1	      ;preserve this one

	not	timed		      ;toggle timer
	cmp	[tsrprg], 1	      ;tsr sleeping?
	jne	ts0   		      ;wake it up!

	cmp	timed, -1	      ;timer off?
	je	ts0 		      ;swap processes
	call	ksw		      ;swap screens only
	mov	keyswap, 0
	mov	inproc, 0
	mov	doswap, 0          ;doswap satisfied, turn it off now
	EOI
	iret

ts0:

        cmp     [tsrprg], 0	;task1 sleeping?
        je      ts1		;yes, then wake it up
        jmp     far ptr ts2	;else put it to sleep and awaken task2

ts1:

        SAVPRC  dosprg
        RESPRC  tsrprg
	mov	keyswap, 0	;reset "key did it"
	mov	inproc, 0	;we are out of swap process now
	mov	doswap, 0       ;doswap satisfied, turn it off now
        EOI
	iret

ts2:

        SAVPRC  tsrprg
        RESPRC  dosprg
	mov	flag_28, 0
	mov	by28, 0
	mov	keyswap, 0	;reset "key did it"
	mov	inproc, 0	;we are out of swap process now
	mov	doswap, 0       ;doswap satisfied, turn it off now
        EOI
	iret

swapit	endp

;------------------------------------------------
; ksw - Swap screens only
;------------------------------------------------

ksw     proc    near
	push	bp
	push	ax
	push	bx
	push	cx
	push	dx
	push	si
	push	di
	push	ds
	push 	es
	pushf

        SAVCUR  dosprg                ;save dos programs's cursor
        SAVSCR  dosprg                ;save dos program's screen
        RESSCR  tsrprg                ;restore tsr's screen
        RESCUR  tsrprg                ;restore tsr's cursor

	popf
	pop 	es
	pop	ds
	pop	di
	pop	si
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	pop	bp

        ret
ksw     endp

;-------------------------------------------------------
; insint - this replaces the necessary interrupt vectors
; Here is where all the interrupts are grabbed
;-------------------------------------------------------

insint  proc    near

        SAVINT  9h, old_key[2], old_key
        SETINT  9h, cs, <offset kbint>

        SAVINT  13h, old_13h[2], old_13h
        SETINT  13h, cs, <offset dchk>

	SAVINT	10h, old_10h[2], old_10h
	SETINT	10h, cs, <offset vchk>

        SAVINT  DEINT, old_clr[2], old_clr
        SETINT  DEINT, cs, <offset clear>
        SAVINT  1ch, old_timer[2], old_timer
        SETINT  1ch, cs, <offset stime>
        SAVINT  28h, old_28h[2], old_28h
        SETINT  28h, cs, <offset bakprc>

        ret

insint  endp

;------------------------------------------------
; getb - gets the address of the dos busy flag
;------------------------------------------------

getb    proc    near
        mov     ah, 34h                 ;hey Uncle Woody! lets get the
                                        ;dos busy flag address!
        int     21h
        mov     indoso,bx               ;store it in our code segment
        mov     indoss,es
        ret
getb    endp

;------------------------------------------------
; vidseg - calculates base of current video mode
;------------------------------------------------

vidseg  proc    near

        cmp     videom, 7            ;mono mode, setup appropriately
        je      vidseg1
        cmp     videom, 1            ;40 wide screen, setup appropriately
        jle     vidseg2
                                     ;otherwise assume modes 2 or 3
        mov     ax, videop           ;get page number
        mov     dx, 100h
        mul     dx                   ;multiply by page size
        add     ax, color            ;add color base
        mov     videos, ax           ;save effective video offset
        mov     videosz, 4000        ;save page size
        jmp     vidseg3

vidseg1:

        mov     ax, videop           ;get page number
        mov     dx, 100h
        mul     dx                   ;multiply by page size
        add     ax, mono             ;add mono base
        mov     videos, ax           ;save effective video offset
        mov     videosz, 4000        ;save page size
        jmp     vidseg3

vidseg2:

        mov     ax, videop           ;get page number
        mov     dx, 80h
        mul     dx                   ;multiply by page size
        add     ax, color            ;add color base
        mov     videos, ax           ;save effective video offset
        mov     videosz, 2000        ;save page size

vidseg3:

        ret
vidseg  endp

;------------------------------------------------------------------------------
;kbrst resets the keyboard and issues an EOI to the 8259 PIC.
;------------------------------------------------------------------------------

kbrst   proc    near
	cli
        in      al,61h                  ;get current control value
        mov     ah,al                   ;save it in AH
        or      al,80h                  ;set the high bit
        out     61h,al                  ;send it to the control port
        mov     al,ah                   ;recover original value
        out     61h,al                  ;send it out
        EOI                             ;hardware end of interrupt
        ret
kbrst   endp

_TEXT   ENDS

        END

;------------------------------------------------
; end of tsrlib
;------------------------------------------------
