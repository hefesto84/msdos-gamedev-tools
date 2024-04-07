page 60,132

; Copyright (c) 1987, 1988 by Mark Adler  Pasadena, CA
; This program may be used and freely copied by anyone except that it
; may not be sold by itself or as part of any package without the
; permission of the author.

; RND.ASM by Mark Adler  7 Dec 1987
; updated by Mark Adler  6 Feb 1988  to change randomize() to rndmize()
;                                    to avoid conflict with Turbo C's
;                                    randomize() macro.

; Assemble using Microsoft Macro Assembler 5.0 or later, or Borland
; TASM 1.0 or later.  For TASM, use the /w-pdc option to turn off
; the pass dependent construction warning.  Specify the memory model as
; follows:
;
; C>masm/mx/d__compact__ rnd;
;
; or
;
; C>tasm/mx/w-pdc/d__compact__ rnd
;
; where "compact" can be replaced by tiny, small, medium, large, or
; huge.  Note that two underscores precede "compact", and two
; underscores follow it.  The file 'tc.asi' must be accessible.  It may
; be convenient to add the resulting RND.OBJ file to the C library.
; For example:
;
; C>tasm/mx/w-pdc/d__compact__ rnd;
; C>tlib/e cc +rnd

 title Random Number Generators for Turbo C

 include tc.asi

comment #

by Mark Adler --- algorithms shamelessly copied from Knuth's "Art
                  of Computer Programming", vol 2, 2nd ed.

This module provides two different random number generators, both
augmented by shuffling.  The two methods are linear conguential and
additive generation.  Both run at approximately the same speed.  The
reason for having two is to allow switching between them to check the
sensitivity of the simulation to the random number generation method.
The routines are:

void setseed(s)  - Set the seed to the 32 bit value 's' and initialize
long s;            the tables for the additive generator and shuffling.
                   Also sets the method to linear congruential.

long seed()      - Return the current linear congruential seed.

long ticks()     - Return the current tick count (IBM PC compatibles).

long rndmize()   - Same as setseed(ticks()) and returns the seed used.

void rndpick(f)  - Select the random number generator method to be used.
int f;             f=0 picks linear congruential, f=1 picks additive.

long lrnd()      - Return a 32 bit random number.

int rnd(n)       - Return a random number between 0 and n, inclusive.
int n;             (n is a 16 bit integer greater than 0.)

double drnd()    - Return a random floating point number in [0..1].

void shuffle(a, n) - Shuffle the (16 bit) integer array 'a' with 'n'
int a[], n;          entries.  (Unrelated to the shuffling used in the
                     random number generation.)

The expected use is to call either rndmize() or setseed() to set the
seed and initialize the tables, then call rndpick() to select the
generation method, and then any combination of lrnd()'s, rnd()'s,
drnd()'s, and shuffle()'s.  rndpick() should not be used in the middle
of the simulation to try to get a "more random" sequence.  The same
method should be used throughout, but the method can be switched between
runs and the results compared to see if they have any dependence on the
random number generators themselves.

The drnd() routine assumes the presence of an 80X87, but does not check
for it.

These routines should be easily customizable for use with other
compilers.

#

 .8087


@header


@dseg
 even
rmul dd 1664525         ;Good multiplier (Knuth Vol. 2, 2nd Ed,
                        ;                 pg. 102, line 26).
scale dw -31            ;Scale to make fraction out of long.
@endd


@bseg
 even
        ; data for linear conguential method with shuffling.
rseed dw ?,?            ;32 bit random number seed.
rvals dw 128 dup(?)     ;Room for 64 old seeds.
        ; (note: the rvals and radd tables must be adjacent for setup.)
        ; data for additive generator method.
radd dw 110 dup(?)      ;Room for 55 integers for additive sequence.
raj dw ?                ;j and k for additive generator.
rak dw ?
        ; flag for method choice.
rtyp db ?

@endb


@cseg

shfval macro
 ;;
 ;; shfval takes the 32 bit random number in DX:AX and uses part of it
 ;; to pick a previous random value from a set of 64, swaps the new one
 ;; with the old one and returns the old one in DX:AX.  This should make
 ;; a random sequence "randomer" (Knuth Vol.2, 2nd Ed., pg 32, algorithm
 ;; B.)  This macro uses the registers AX, BX, and DX.
 ;;
  mov BL,DH
  and BX,0FCh                   ;;Pick off high 6 bits of DX:AX.
  xchg AX,dgroup:rvals[BX]      ;;Exchange new seed with old one.
  xchg DX,dgroup:rvals[BX+2]
 ;;
endm


lincon macro
 ;;
 ;; lincon uses the linear congruential method to generate a 32 bit
 ;; random number.  The number is left in DX:AX and updates rseed.
 ;; This macro assumes the direction flag is cleared and that ES and DS
 ;; are equal.  It uses the registers AX, BX, CX, DX, SI, and DI.
 ;;
        ;; multiply seed by multiplier modulo 2^32.
  mov SI,offset dgroup:rmul     ;;Point SI to multiplier.
  mov DI,offset dgroup:rseed    ;;Point DI to seed.
  lodsw                 ;;Get low word of multiplier.
  mov BX,AX             ;;Save that.
  mul word ptr [DI+2]   ;;Multiply by high word of seed.
  mov CX,AX             ;;Save low word of that for high word of result.
  lodsw                 ;;Get high word of multiplier.
  mul word ptr [DI]     ;;Multiply by low word of seed.
  add CX,AX             ;;Add low word into high word of result.
  xchg AX,BX            ;;Get low word of multiplier.
  mul word ptr [DI]     ;;Multiply by low word of seed.
  add DX,CX             ;;Add other high word components.
        ;; add constant modulo 2^32.
  add AX,1              ;;Constant = 1 (relatively prime to
  adc DX,0              ;; everything).
        ;; store seed back.
  stosw                 ;;Store low word.
  mov [DI],DX           ;;Store high word.
 ;;
endm


addgen macro
 local jcyc, kcyc, done
 ;;
 ;; addgen uses the additive generator of Mitchell and Moore (Knuth
 ;; Vol.2, 2nd Ed., pg. 27.) to generate a 32 but random number.  The
 ;; number is left in DX:AX.  This macro uses the registers AX, BX, DX,
 ;; SI, and DI.
 ;;
        ;; get random 32 bit number in DX:AX.
  mov SI,dgroup:raj     ;;Get j.
  mov DI,dgroup:rak     ;;Get k.
  mov BX,offset dgroup:radd     ;;Point to circular list of 55 integers.
  mov AX,[BX+SI]        ;; r = (radd[k] += radd[j]);
  add AX,[BX+DI]
  mov [BX+DI],AX
  inc BX
  inc BX
  mov DX,[BX+SI]
  adc DX,[BX+DI]
  mov [BX+DI],DX
  sub SI,4
  jb jcyc               ;; j = j ? j - 1 : 54;
  sub DI,4
  jb kcyc               ;; k = k ? k - 1 : 54;
  mov dgroup:raj,SI     ;;Update j and k.
  mov dgroup:rak,DI
  jmp short done
        ;; j cycles up to top.
 jcyc:
   sub DI,4             ;;If j bad, then k is OK.
   mov dgroup:raj,54*4  ;;Cycle j.
   mov dgroup:rak,DI    ;;Set k.
   jmp short done
        ;; k cycles up to top.
 kcyc:
   mov dgroup:raj,SI    ;;If k bad, then j is OK.
   mov dgroup:rak,54*4  ;;Cycle k.
        ;; now everything is updated properly.
 done:
 ;;
endm


choice macro
 local lin,shf
 ;;
 ;; Use either the linear congruential or the additive generator
 ;; depending on the value of rtyp.  Follow with a shuffle.
 ;;
  cmp dgroup:rtyp,0     ;;See if zero.
  je lin                ;;If so, use linear congruential.
   addgen               ;;Else, use additive generator.
   jmp short shf
 lin:
   lincon               ;;Use linear congruential.
 shf:
  shfval                ;;Shuffle with old values.
 ;;
endm


zerton macro n
 local ismax
 ;;
 ;; zerton takes a random 32 bit number in DX:AX and makes a random
 ;; number in the range 0..n (inclusive).  n may be anything allowed as
 ;; the source of 'mov CX,'.  The result is left in AX and n+1 is left
 ;; in CX.  This macro uses the registers AX, BX, CX, and DX.
 ;;
        ;; get random number in [0..n] (as per Knuth's recommendation).
  ifdifi <n>,<cx>
   mov CX,n             ;;Get n.
  endif
  inc CX                ;;Use n+1 to multiply "fraction" in [0..1).
  jz ismax              ;;If n is 65535, then return high part of seed.
   mov BX,DX            ;;Save high part of seed.
   mul CX               ;;Multiply low part of fraction by n+1.
   xchg AX,BX           ;;Get high part of fraction.
   mov BX,DX            ;;Save high part of product.
   mul CX               ;;Multiply high part of fraction by n+1.
   add AX,BX            ;;Add component from low part of fraction.
   adc DX,0
 ismax:
  xchg AX,DX            ;;Truncate to integer.
 ;;
endm


@proc shuffle
 a equ [BP+@a]
 n equ [BP+@a+@d]
 ;
 ; void shuffle(a, n)
 ; int a[], n;
 ;
 ; Shuffle n items (algorithm from Knuth).  n must be greater than zero
 ; and 'int a[n]' must fit in the segment.
 ;
  @enter
  push SI
  push DI
        ; set up to use string instructions (for lincon).
  mov AX,DS             ;Get DS.
  mov ES,AX             ;Set ES to same as DS.
  cld                   ;Autoincrement.
        ; do shuffle.
  mov CX,n              ; get n, j = n - 1.
 shflp:
   loop swp             ;Do n-1 swaps.
   jmp shfend
 swp:
   push CX              ;Save j.
   choice               ;Make 32 bit random number.
   pop CX               ;Restore j.
   zerton CX            ;k = random number in 0..j.
   dec CX               ;Restore j.
   cmp AX,CX            ;See if swap needed (k != j).
   je shno              ;If not, skip swap.
    mov SI,AX           ;Put offsets in SI, DI.
    shl SI,1
    mov DI,CX
    shl DI,1
    if __LDATA__
     lds BX,a           ;Point to a[].
    else
     mov BX,a           ;Point to a[].
    endif
    mov AX,[BX+SI]      ;Get a[k].
    xchg AX,[BX+DI]     ;Exchange with a[j].
    mov [BX+SI],AX      ;Set new a[k].
    if __LDATA__
     mov AX,ES          ;Restore DS.
     mov DS,AX
    endif
  shno:
   jmp shflp
        ; done.
 shfend:
  pop DI
  pop SI
  @leave
 ;
@endp shuffle


@proc lrnd
 ;
 ; long lrnd()
 ;
 ; Return random 32 bit number using linear congruential method.
 ;
  @enter
  push SI
  push DI
        ; set up to use string instructions (for lincon).
  mov AX,DS             ;Set ES to same as DS.
  mov ES,AX
  cld                   ;Autoincrement.
        ; get a new random number in DX:AX.
  choice                ;Make 32 bit random number.
        ; return seed.
  pop DI
  pop SI
  @leave
 ;
@endp lrnd


@proc rnd
 n equ [BP+@a]
 ;
 ; int rnd(n)
 ; unsigned n;
 ;
 ; Return random number in [0..n] using linear congruential method.
 ;
  @enter
  push SI
  push DI
        ; set up to use string instructions (for lincon).
  mov AX,DS             ;Set ES to same as DS.
  mov ES,AX
  cld                   ;Autoincrement.
        ; get a new random number in DX:AX.
  choice                ;Make 32 bit random number.
        ; convert to random number in 0..n.
  zerton n
        ; return it.
  pop DI
  pop SI
  @leave
 ;
@endp rnd


@proc drnd
 ;
 ; double drnd()
 ;
 ; Return random number in [0..1].  This routine assumes a numeric
 ; coprocessor is present, but does not check for it.  The fraction
 ; generated has 31 random bits plus the possibility of 1.  The values
 ; of 0 and 1 have half the probability of any other fraction, as it
 ; should be.  The floating point value is left on the numeric processor
 ; stack.  Two levels of stack are used.
 ;
  @enter
  push SI
  push DI
        ; set up to use string instructions (for lincon).
  mov AX,DS             ;Set ES to same as DS.
  mov ES,AX
  cld                   ;Autoincrement.
        ; do NP operation concurrently with random number generation.
  fild dgroup:scale     ;Get scale to make fraction.
        ; get a new random number in DX:AX.
  choice                ;Make 32 bit random number.
        ; make seed into floating point, leave on NP stack.
  push DX               ;Put in memory.
  push AX
  fild dword ptr [BP-8] ;Load seed into stack.
  fabs                  ;Take absolute value.
  fscale                ;Scale to fraction.
  fstp st(1)            ;Pull scale out of stack.
  add SP,4              ;Trash 32 bit value.
        ; done.
  pop DI
  pop SI
  @leave
 ;
@endp drnd


@proc seed
 ;
 ; long seed()
 ;
 ; Return current seed.
 ;
  @enter
  mov AX,dgroup:rseed   ;Get low word.
  mov DX,dgroup:rseed+2 ;Get high word.
  @leave
 ;
@endp seed


@proc ticks
 ;
 ; long ticks()
 ;
 ; Get tick count (assumes IBM PC like).
 ;
  @enter
  mov AH,0              ;Read ticks.
  int 1Ah               ;Time of day interrupt.
  mov AX,DX             ;Get low word in AX.
  mov DX,CX             ;Get high word in CX.
  @leave
 ;
@endp ticks


setup proc near
 ;
 ; Setup radd, raj, and rak for addgen and rvals for shfval.  See Knuth
 ; for why 23 and 54 are the desired values for j and k.
 ; Note: one of the requirements is that not all of the integers in
 ; radd[] can be even.  This is satisfied since lincon returns
 ; alternating even and odd numbers.
 ;
  push SI
  push DI
        ; set up to use string instructions (for lincon).
  mov AX,DS             ;Set ES to same as DS.
  mov ES,AX
  cld                   ;Autoincrement.
        ; fill rvals[] and radd[] tables.
  mov DI,offset dgroup:rvals
  mov CX,64+55          ;Set rvals[0..63] and radd[0..54] (longs).
 fill:
   push DI              ;Save registers.
   push CX
   lincon               ;Get 32 bit random number in DX:AX.
   pop CX               ;Restore registers.
   pop DI
   stosw                ;Store number.
   xchg AX,DX
   stosw
   loop fill
        ; set j and k.
  mov dgroup:raj,23*4   ; j = 23;
  mov dgroup:rak,54*4   ; k = 54;
        ; set rtyp to use linear congruential.
  mov dgroup:rtyp,0
        ; done.
  pop DI
  pop SI
  ret
 ;
setup endp


@proc rndmize
 ;
 ; long rndmize()
 ;
 ; Randomize seed and set up for addgen.  This sets the seed to the
 ; current tick count, generates random numbers for addgen and shfval,
 ; sets j and k for addgen, and returns the tick count used.  The
 ; returned tick count could be saved and used as the argument for
 ; setseed() to exactly repeat the pseudo-random sequence.
 ;
  @enter
  call _ticks
  mov dgroup:rseed,AX   ;Set low word.
  mov dgroup:rseed+2,DX ;Set high word.
  push DX               ;Save seed.
  push AX
  call setup            ;Setup tables.
  pop AX                ;Return seed.
  pop DX
  @leave
 ;
@endp rndmize


@proc setseed
 s equ [BP+@a]
 ;
 ; void setseed(s)
 ; long s;
 ;
 ; Set seed to argument.
 ;
  @enter
  mov AX,s              ;Get low word.
  mov dgroup:rseed,AX   ;Set low word.
  mov AX,s+2            ;Get high word.
  mov dgroup:rseed+2,AX ;Set high word.
  call setup            ;Set up tables.
  @leave
 ;
@endp setseed


@proc rndpick
 f equ [BP+@a]
 ;
 ; void rndpick(f)
 ; int f;
 ;
 ; Set the method to be used for random number generation.  f=0 selects
 ; linear conguential (the default after rndmize() or setseed() called)
 ; and f=1 selects additive generator.
 ;
  @enter
  mov AL,f
  mov dgroup:rtyp,AL
  @leave
 ;
@endp rndpick


@endc

 end
