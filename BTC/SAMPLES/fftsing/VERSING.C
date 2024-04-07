/*
 *	versing.c   Verifies sing.c  
 *
 *	Generates a square pulse of width width and total length of
 *   2*half_order. Computes the discrete Fourier transform and compares
 *   it to the theoretical transform.
 *   Obtains the inverse transform and compares it to the original pulse.
 */

				/* Version 2.0  April 1992 */

/**************************************************************************

	Javier Soley, Ph. D,   FJSOLEY @UCRVM2.BITNET
	Escuela de F¡sica y Centro de Investigaciones Geof¡sicas
	Universidad de Costa Rica

***************************************************************************/


/* Includes */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <alloc.h>
#include <string.h>
#include <dos.h>
#include <time.h>

/* Defines */

#define	TWO_PI	((double)2.0 * M_PI)

/* Globals */

unsigned long  half_order,  mem_avail, max_size, width;
double huge *hreal, huge *himag;                 
 /* use to reference long series without
						  wrap around */

/* Prototypes and forward declarations */


double amp_rect_pulse (unsigned long, unsigned long, unsigned long);
double pha_rect_pulse (unsigned long, unsigned long, unsigned long);



/* The program */

void  main(void)
{
	double far  *real;
	double far  *imag;   /* returned by memory allocation functions */
	unsigned long counter, j,  r_index, i_index;
	long start;
	double tmag, tpha;  /* magnitud and phase of theoretical FFT */
	time_t start_time, finish;

	mem_avail = farcoreleft();
	printf("%lu bytes free in far heap\n", mem_avail);
	max_size =  (mem_avail-2) / sizeof(double) ;
	printf( "Maximum length of data is %lu \n", max_size);
	printf( "Maximum half-length is %lu  \n", max_size /2);

	printf("Half-length of rectangular pulse ?  ");
	cscanf("%lu", &half_order);
	if ( 2*half_order  > max_size) {
		printf("\nLength of series exceeds far heap\n");
		exit(2);
	}

	printf("\nWidth of rectangular pulse ?  "); cscanf("%lu", &width);
	if ( width  >= 2*half_order) {
		printf("\nWidth of pulse exceeds length of pulse\n");
		exit(3);
	}

	
	if ((real =  farcalloc(half_order +1, sizeof(double))) == NULL) {
		printf("\nAllocation error with real part\n");
		exit(4);
	}

	if ((imag =  farcalloc(half_order + 1, sizeof(double))) == NULL) {
		printf("\nAllocation error with imaginary part\n");
		exit(5);
	}
	hreal = real; himag = imag;  
	printf("\nThe real e imaginary parts start at %Fp and %Fp\n",
		hreal, himag);

/* Generate the rectangular pulse */

	r_index = 0; i_index =0;  counter =0;
	while ( counter < width ) {
		hreal[r_index] = (double) 1.0;
		if (counter == width -1) break;
		r_index++; counter ++;
		himag[i_index] = (double) 1.0;
		i_index++; counter++;
	}
	r_index++;
	while( r_index <= half_order ) {
		hreal[r_index] = (double) 0; r_index++;
	}
	i_index++;
	while( i_index <= half_order ) {
		hreal[i_index] = (double) 0; i_index++;
	}

		/* Now call Singleton's FFT procedures */
	start_time = time(NULL);
	sing(hreal, himag, half_order, half_order, half_order, -1);
	realtr(hreal, himag, half_order, -1);
	finish = time(NULL);
	printf("\nIt took aproximately %f  seconds\n",
	 difftime(finish, start_time)); 
				/* Whistle when done */
	sound(440); delay(4000); nosound();

	do {
		printf("\n\nNegative number to exit");
		printf("\nVerify results starting where ? [0..%lu ] ",
			  half_order-19);
		cscanf("%lu", &start); 
		if( start <0 ) break;
		clrscr();
		printf("          Real theory     Real calculated    Imag. theory     Imag.calculated\n");

			for ( j=start; j < start + 20; j++) {
				tmag = amp_rect_pulse(j, width, 2*half_order);
				tpha = pha_rect_pulse(j, width, 2*half_order);
				printf( "\n%5lu  %16.9e  %16.9e  %16.9e  %16.9e", j,
				tmag*cos(tpha), hreal[j]/(4*half_order),
				tmag*sin(tpha), himag[j]/(4*half_order));
			}
	   } while (1);

		/* Now call the inverse transform */
	printf("\nCalculating the inverse transform\n");
	start_time = time(NULL);
	realtr(hreal, himag, half_order, 1);
	sing(hreal, himag, half_order, half_order, half_order, 1);
	
	finish = time(NULL);
	printf("\nIt took aproximately %f  seconds\n",
	 difftime(finish, start_time)); 
				/* Whistle when done */
	sound(440); delay(4000); nosound();

	do {
		printf("\n\nNegative number to exit");
		printf("\nVerify results starting where ? [0..%lu ] ",
			  half_order-20);
		cscanf("%lu", &start); 
		if( start <0 ) break;
		clrscr();
		printf("            Even index       Odd index \n");

			for ( j=start; j < start + 20; j++) {
				printf( "\n%5lu  %16.9e  %16.9e ", j,
				hreal[j]/(4*half_order),himag[j]/(4*half_order));
			}
	   } while (1);	   
					
}




/* Calculates the amplitude spectra of a rectangular pulse */

double amp_rect_pulse (i, w, n)
unsigned long i, w, n;  /* frequency index, width of pulse, order of fft)*/
{	double angle, amp;
	angle = 	M_PI*i/n;
	if ( i==0 || i==n) amp = (double) w / (double)n;
	else	 {
	amp = sin(w*angle)/sin(angle);
	amp /= n;
	}
	return amp;
}
/* Calculates the phase spectra of a rectangular pulse */

double pha_rect_pulse (i, w, n)
unsigned long i, w, n;  /* frequency index, width of pulse, order of fft)*/
{    double phase;
	phase = (double)M_PI*(double)(w-1)* (double)i/(double)n;
	return -phase;
}
	

/* EOF */
