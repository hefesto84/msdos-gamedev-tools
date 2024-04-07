#include <dos.h>

#define TimerResolution    1193181.667

void cardinal(long l,double *result)
{
	*result = ((l<0)?4294967296.0 + (long)l : (long)l);
}

void elapsedtime(long start, long stop, double *result)
{
	double r;

	cardinal(stop - start, &r);

	*result = (1000.0 * r) / TimerResolution;
}

void initializetimer(void)
{

  outportb(0x043,0x034);
  asm jmp short NullJump1

NullJump1:;

  outportb(0x040,0x000);
  asm jmp short NullJump2

NullJump2:;

  outportb(0x040,0x000);

}

void restoretimer(void)
{
  outportb(0x043,0x036);
  asm jmp short NullJump1

NullJump1:;

  outportb(0x040,0x000);
  asm jmp short NullJump2

NullJump2:;

  outportb(0x040,0x000);

}

long readtimer(void)
{
  asm cli             /* Disable interrupts */
  asm mov  dx,020h     /* Address PIC ocw3   */
  asm mov  al,00Ah     /* Ask to read irr    */
  asm out  dx,al
  asm mov  al,00h     /* Latch timer 0 */
  asm out  043h,al
  asm in   al,dx      /* Read irr      */
  asm mov  di,ax      /* Save it in DI */
  asm in   al,040h     /* Counter --> bx*/
  asm mov  bl,al      /* LSB in BL     */
  asm in   al,040h
  asm mov  bh,al      /* MSB in BH     */
  asm not  bx         /* Need ascending counter */
  asm in   al,021h     /* Read PIC imr  */
  asm mov  si,ax      /* Save it in SI */
  asm mov  al,00FFh    /* Mask all interrupts */
  asm out  021h,al
  asm mov  ax,040h     /* read low word of time */
  asm mov  es,ax      /* from BIOS data area   */
  asm mov  dx,es:[06Ch]
  asm mov  ax,si      /* Restore imr from SI   */
  asm out  021h,al
  asm sti             /* Enable interrupts */
  asm mov  ax,di      /* Retrieve old irr  */
  asm test al,001h     /* Counter hit 0?    */
  asm jz   done       /* Jump if not       */
  asm cmp  bx,0FFh     /* Counter > 0x0FF?    */
  asm ja   done       /* Done if so        */
  asm inc  dx         /* Else count int req. */
done:;
  asm mov ax,bx   /* set function result */
}
