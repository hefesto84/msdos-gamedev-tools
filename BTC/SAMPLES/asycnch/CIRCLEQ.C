#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "circleq.h"

/*
 * Qalloc allocates space for a Queue Control Structure (CircleQ) and
 * a buffer to be contained within.
 * If space is not available for this structure, or it's buffer, NULL
 * is returned, otherwise a pointer to a CirclQ is returned.
*/
CircleQ * Qalloc(int Size)
{
	CircleQ * Qptr;
	if ((Qptr = calloc(sizeof(CircleQ),1)) == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}
	if ((Qptr->Buffer = calloc(Size,1)) == NULL)
	{
		free(Qptr);
		errno = ENOMEM;
		return NULL ;
	}
	Qptr->Head = Qptr->Tail = Qptr->Buffer;
	Qptr->Size = Size;
	return Qptr;
}
/*
 * QinsertChar inserts a character on a queue. The tail pointer is used
 * to indicate where the character shall be placed. Once a character is
 * written, the tail pointer is updated. If the tail pointer overruns the
 * head pointer, the overrun flag is set. Once the overrun flag is set, the
 * head pointer will be updated on all subsequent writes to ensure that
 * the buffer always contains Qptr->Size characters. If Qptr->Tail or
 * Qptr->Head reach the end of the buffer, they are wrapped around to
 * the beginning of the buffer.
*/
void QinsertChar(CircleQ * Qptr, char Ch)
{
	*Qptr->Tail = Ch;
	if ((Qptr->Tail + 1) >= (Qptr->Buffer+Qptr->Size))
		Qptr->Tail = Qptr->Buffer;
	else
		Qptr->Tail ++;

	if (Qptr->Head == Qptr->Tail)
		Qptr->Flag = 1;		/* Queue buffer overflow */

	if (Qptr->Flag)
		Qptr->Head ++ ;
	if (Qptr->Head >= Qptr->Buffer + Qptr->Size)
		Qptr->Head = Qptr->Buffer;
}
/*
 * QpeekChar returns a character from the specified queue without removing
 * it. If the queue is empty -1 is returned.
*/
char QpeekChar(CircleQ * Qptr)
{
	if (Qptr->Head == Qptr->Tail)
	{
		Qptr->Flag = 0;
		return -1;
	}
	return *Qptr->Head;
}
/*
 * QreadChar returns a character from the specified queue. The queue is then
 * updated to reflect the last read. If the queue is empty, -1 is returned.
*/
char QreadChar(CircleQ * Qptr)
{
	char Ch = -1;
	if (Qptr->Head == Qptr->Tail)
	{
		Qptr->Flag = 0;
		return -1;
	}
	Ch = *Qptr->Head;
	Qptr->Head ++ ;
	if (Qptr->Head >= Qptr->Buffer + Qptr->Size)
		Qptr->Head = Qptr->Buffer;
	return Ch;
}
/*
 * Qfree frees a queue control structure (CircleQ) and all buffers
 * associated with it (Qptr->Buffer). This Head and Tail pointers are
 * set to NULL, and Size and Flag (overrun flag) are set to 0.
 * Qptr is then freed, releasing all memory associated with the queue.
 * No subsequent operations may be made on this queue.
*/
void Qfree(CircleQ * Qptr)
{
	if (Qptr != NULL)
	{
		free(Qptr->Buffer);
		Qptr->Buffer = Qptr->Head = Qptr->Tail = NULL;
		Qptr->Size = Qptr->Flag = 0;
		free(Qptr);
	}
}

