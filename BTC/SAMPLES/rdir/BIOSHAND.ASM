;
; Bios disk write and read software interrupt handler 
;    Sets _biosbusy_fl when in a Bios Disk request.

; Set up the code segment

_TEXT   segment para public 'CODE'
_TEXT   ends
_TEXT   segment
        assume  cs:_TEXT

; Set up the data segment

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group  _DATA
_DATA   segment
        assume ds:_DATA

extrn   _biosbusy_fl:byte
extrn   _old_biosdisk:dword
extrn   DGROUP@:word

_DATA   ends

_biosdisk_handler       proc    far
        public  _biosdisk_handler

        push    ds                      ;save some registers
        push    bx                      
        mov     bx, cs:DGROUP@          ;get DS set up to data group
        mov     ds, bx                  ;through BX
        mov     _biosbusy_fl,1          ;set the flag to one
        pop     bx                      ;restore registers
        pushf                           ;simulate int 13h
        call    [_old_biosdisk]         ;  by calling old interrupt
        mov     _biosbusy_fl,0          ;set flag to zero
        pop     ds
        ret     2                       ;return but leave new flags alone

_biosdisk_handler       endp
_TEXT   ends

        end


