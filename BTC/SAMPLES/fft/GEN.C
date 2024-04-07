/*
 *	gen.c
 *
 *	C Version 1.0 by Steve Sampson, Public Domain
 *
 *	This program is used to generate time domain sinewave data
 *	for fft.c.  If you want an opening target - negate the test frequency
 *
 *	Usage: gen samples output
 */

#include <stdio.h>
#include <alloc.h>
#include <math.h>

#define	PI2	((double)2.0 * M_PI)

main(argc, argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	double	sample, freq, time, *real, *imag;
	int	loop, samples;

	if (argc != 3)  {
		printf("Usage: gen samples output_file\n");
		printf("Where samples is a power of 2\n");
		exit(-1);
	}

	if ((fp = fopen(argv[2], "wb")) == (FILE *)NULL)  {
		printf("Unable to create write file\n");
		exit(-1);
	}

	samples = abs(atoi(argv[1]));

	real = (double *)malloc(samples * sizeof(double));
	imag = (double *)malloc(samples * sizeof(double));

	printf("Input The Test Frequency (Hz) ? ");
	scanf("%lf", &freq);
	printf("Input The Sampling Frequency (Hz) ? ");
	scanf("%lf", &sample);
	sample = (double)1.0 / sample;

	time = (double)0.0;
	for (loop = 0; loop < samples; loop++)  {
		real[loop] =  sin(PI2 * freq * time);
		imag[loop] = -cos(PI2 * freq * time);
		time += sample;
	}

	fwrite(real, sizeof(double), samples, fp);
	fwrite(imag, sizeof(double), samples, fp);

	fclose(fp);
	putchar('\n');
}

/* EOF */
