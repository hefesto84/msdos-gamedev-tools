/*	While compiling, Dont forget to include the *.lib
	In the command line compiler,
	type tcc -ms demo.c imcs.lib
	since this program is written in small model!	*/


/*	Programmed by Leung Ka Wai	*/

/*	If you have any comments or opinions,
	dont hesitate to e-mail to me
	or give message to me in BBS

	e-mail : kwleung2@se.cuhk.hk	*/


#include <graphics.h>
#include <alloc.h>
#include <bios.h>
#define time 299999   /* This is the elapsed time between displaying
					and erasing an image */

extern void* imc(char*);	/* This is the declaration of the showimc function */
					/* It can be 'extern void far* imc(char*)' for large
						model  */

main()
{    int gd=VGA, gm=VGAMED;
	void* wong[3],*lung[4],*panda;
	int wong_stand=200,lung_stand=200;
	unsigned long x;

	/* palette for Mircosoft's paintbrush */
	struct palettetype pal = { {MAXCOLORS},
					  { EGA_BLACK,
					    EGA_RED,
					    EGA_GREEN,
					    EGA_BROWN,
					    EGA_BLUE,
					    EGA_MAGENTA,
					    EGA_CYAN,
					    EGA_DARKGRAY,
					    EGA_LIGHTGRAY,
					    EGA_LIGHTRED,
					    EGA_LIGHTGREEN,
					    EGA_YELLOW,
					    EGA_LIGHTBLUE,
					    EGA_LIGHTMAGENTA,
					    EGA_LIGHTCYAN,
					    EGA_WHITE
					   }
	};

	/* The use of showimc function */
	/* Inside the blackets are the filename */
	wong[0]=imc("wong1.imc");
	wong[1]=imc("wong3.imc");
	wong[2]=imc("wong2.imc");
	lung[0]=imc("lung1.imc");
	lung[1]=imc("lung2.imc");
	lung[2]=imc("lung3.imc");
	lung[3]=imc("lung4.imc");
	panda=imc("panda.imc");

	initgraph(&gd,&gm,"");
	setallpalette(&pal);
	putimage(220,50,panda,COPY_PUT);
	while(bioskey(1)==0)
	{    putimage(100,wong_stand,wong[1],COPY_PUT);
		putimage(450,lung_stand,lung[0],COPY_PUT);
		for(x=0;x<time;x++);
		putimage(100,wong_stand,wong[1],XOR_PUT);
		putimage(450,lung_stand,lung[0],XOR_PUT);

		putimage(100,wong_stand-50,wong[0],COPY_PUT);
		putimage(450,lung_stand,lung[3],COPY_PUT);
		for(x=0;x<time;x++);
		putimage(100,wong_stand-50,wong[0],XOR_PUT);
		putimage(450,lung_stand,lung[3],XOR_PUT);

		putimage(100,wong_stand-70,wong[0],COPY_PUT);
		putimage(450,lung_stand,lung[1],COPY_PUT);
		for(x=0;x<time;x++);
		putimage(100,wong_stand-70,wong[0],XOR_PUT);
		putimage(450,lung_stand,lung[1],XOR_PUT);

		putimage(100,wong_stand-40,wong[2],COPY_PUT);
		putimage(465,lung_stand-20,lung[2],COPY_PUT);
		for(x=0;x<time;x++);
		putimage(100,wong_stand-40,wong[2],XOR_PUT);
		putimage(465,lung_stand-20,lung[2],XOR_PUT);

		putimage(100,wong_stand-10,wong[2],COPY_PUT);
		putimage(450,lung_stand,lung[0],COPY_PUT);
		for(x=0;x<time;x++);
		putimage(100,wong_stand-10,wong[2],XOR_PUT);
		putimage(450,lung_stand,lung[0],XOR_PUT);

		putimage(100,wong_stand,wong[1],COPY_PUT);
		putimage(450,lung_stand,lung[0],COPY_PUT);
		for(x=0;x<time;x++);
		putimage(100,wong_stand,wong[1],XOR_PUT);
		putimage(450,lung_stand,lung[0],XOR_PUT);
	}
	closegraph();
	free(wong[0]);
	free(wong[1]);
	free(wong[2]);
	free(lung[0]);
	free(lung[1]);
	free(lung[2]);
	free(lung[3]);
}


