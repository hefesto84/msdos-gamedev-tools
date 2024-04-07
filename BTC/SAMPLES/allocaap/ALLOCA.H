/* alloca for Turbo C
* public domain with the restriction that the following line is always retained:
* ... alloca() written by Alexander Pruss ...
* Options:
** INLINE_ALLOCA -- sets up an inline equivalent to the assembly version.
**                  equivalent in all respects but it is macro.
** FORCE_STACK   -- forces a stack frame for each function.  This will increase
**                  the stack frame for each function that already has a frame
**                  and calls alloca() by 2 bytes.  If it is activated, the
**                  alloca call MUST be of the form:  xxx=yyy alloca(zzz);
**                  where yyy and xxx are anything valid.
*/

#ifndef alloca

#ifndef __TURBOC__
#error Turbo C is required for alloca.h.  Sorry.
#endif

#ifdef INLINE_ALLOCA
void __emit__();

#define MAKE_FAR_PTR(seg,off) \
 (void far *) ( ( (unsigned long)(seg) << 16 ) | ( (unsigned int)(off) ) )

#define MAKE_EVEN(x)   ( ( ((x)+1)>>1 ) <<1 )

#if sizeof(void *)==4
#define _alloca(space) (void *) \
	 ( __emit__(0x07, 0x5B, 0x59, 0x51, 0x53, 0x06), \
    _SP-=MAKE_EVEN((space)), __emit__(0x51,0x53,0x06), MAKE_FAR_PTR(_SS,_SP+8))
#else
#define _alloca(space) (void *) \
         ( __emit__(0x5B, 0x59, 0x51, 0x53), \
    _SP-=MAKE_EVEN((space)), __emit__(0x51,0x53), MAKE_FAR_PTR(_SS,_SP+6))
#endif

/* this is equivalent in operation to the assembly language version.
The emits are: es bx cx
   pop es  (only if large data)
   pop bx
   pop cx
   push cx
   push bx
   push es   (to save the (DS,SI,DI), or (SI,DI) in small data)
  followed by
   push cx
   push bx
   push es   (to make a copy of (DS,SI,DI))
*/
#else

void *_alloca(unsigned length);

#endif  /* INLINE */

#ifdef FORCE_STACK
# if __TURBOC__ > 0x200
  static long __global_long_dummy__;
# define alloca(x) _alloca((x)); { long z; __global_long_dummy__=z; };
#else
# define alloca(x) _alloca((x)); { char z; z=z; };
#endif

#else
#define alloca(x) _alloca((x))
#endif

#endif /* def alloca */
