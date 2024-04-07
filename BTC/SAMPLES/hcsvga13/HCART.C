/*
 * HCART.C
 *
 * Copyright 1990,1991 Synergrafix Consulting
 *          All Rights Reserved.
 *
 * November 19,1991
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include "hicolor.h"

int main()
{
	int i, j, k;

	if (!hcsetmode(HC_SVGAHI,FALSE)) {
		printf("No HiColor video detected.\n");
		exit(0);
		}

	for (k=0;(k<19)&&!kbhit();k++)

	switch(k) {
		case 1: for (j=0;j<480;j++) {
				for (i=0;i<640;i++) {
					hcputpoint(i,j,i*32+j);
					}
				}
			break;
		case 2:	for (j=0;j<480;j++) {
			   for (i=0;i<640;i++) {
				   hcputpoint(i,j,i*j);
				   }
			   }
			break;
		case 3: for (j=1;j<480;j++) {
			   for (i=1;i<640;i++) {
				   hcputpoint(i,j,i%j);
				   }
			   }
			break;
		case 4: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j,cos(j*i)*100 );
				     }
			     }
			break;
		case 5: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j,(i&j)*(i+j)   );
				     }
			     }
			break;
		case 6: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j,(i&j)*(i-j)   );
				     }
			     }
			break;
		case 7: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j,(i&j)*(i-j)*cos((i+j)/100)  );
				     }
			     }
			break;
		case 8: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j,(i&j)*(i+j)*cos((i+j)/100)  );
				     }
			     }
			break;
		case 9: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j,(i&j)*(i+j)*cos((i+j)/100)-sin((i-j)/100)*i  );
				     }
			     }
			break;
		case 10: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j, (i&j)*(i^j)  );
				     }
			     }
			break;
		case 11: for (j=1;j<480;j++) {
			     for (i=1;i<480;i++) {
				     hcputpoint(i+80,j, (i&j)*(i+j)  );
				     }
			     }
			break;
		case 12: for (j=1;j<480;j++) {
			     for (i=1;i<480;i++) {
				     hcputpoint(i+80,j, (i&j)*(i^j)  );
				     }
			     }
			break;
		case 13: for (j=1;j<480;j++) {
			     for (i=1;i<480;i++) {
				     hcputpoint(i+80,j, (i&j)+(i^j)  );
				     }
			     }
			break;
		case 14: for (j=1;j<480;j++) {
			     for (i=1;i<480;i++) {
				     hcputpoint(i+80,j, (i^j)-(i*j)  );
				     }
			     }
			break;
		case 15: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j, pow((float)(i/600.0),(float)(j/600.0))*300 );
				     }
			     }
			break;
		case 16: for (j=1;j<480;j++) {
			     for (i=1;i<480;i++) {
				     hcputpoint(i+80,j, (i/5)^(j/5) );
				     }
			     }
			break;
		case 17: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j, (i/5)^(j/5) );
				     }
			     }
			break;
		case 18: for (j=1;j<480;j++) {
			     for (i=1;i<640;i++) {
				     hcputpoint(i,j, (i+20)*(j+20) );
				     }
			     }
			break;
		}

	getch();

	hctextmode();

	return 0;
}

