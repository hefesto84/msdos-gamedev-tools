#include	<graphics.h>
#include	<stdio.h>
#include	<bios.h>

static int port=0;		/* port to print too	*/

static int PrintChar (int out)

{ int a;
  a=biosprint (0,out,port);
  return (a!=16);
}

void	setport	(int portnum)

{ port=portnum;
}

/*	hardcopynorm dumps a screen to the printer from	*/
/* 	any graphics mode using the current viewport	*/
/* 	Valid values for gmode:				*/
/*	-4 to -1 for Epson and IBM Graphic Printers	*/
/*	 0 to 7  for Epson Printers			*/

int	hardcopynorm	(int gmode)

{ int			X,Y,YOfs;
  int			BitData,MaxBits;
  unsigned int		Height,Width;
  struct viewporttype 	Vport;

  getviewsettings (&Vport);
  Height=Vport.bottom-Vport.top;
  Width=(Vport.right+1)-Vport.left;
  if (PrintChar(27)!=0) return(1);
  if (PrintChar('3')!=0) return(1);
  if (PrintChar(24)!=0) return(1);
  Y=0;
  while (Y<Height)
  { if (PrintChar(27)!=0) return(1);
    switch (gmode)
    { case -1 : if (PrintChar('K')!=0) return(1);break;
      case -2 : if (PrintChar('L')!=0) return(1);break;
      case -3 : if (PrintChar('Y')!=0) return(1);break;
      case -4 : if (PrintChar('Z')!=0) return(1);break;
      case 0  :
      case 1  :
      case 2  :
      case 3  :
      case 4  :
      case 5  :
      case 6  :
      case 7  : if (PrintChar('*')!=0) return(1);
		if (PrintChar(gmode)!=0) return(1);break;
      default : return (1);
    };
    if (PrintChar (Width&0xff)!=0) return(1);
    if (PrintChar (Width>>8)!=0) return (1);
    for (X=0;X<Width;X++)
    { BitData=0;
      if ((Y+7)<=Height) MaxBits=7;
	else MaxBits=Height-Y;
      for(YOfs=0;YOfs<=MaxBits;YOfs++)
      { BitData=BitData<<1;
	if (getpixel(X,YOfs+Y)>0) BitData++;
      };
      if (PrintChar(BitData)!=0) return (1);
    };
    if (PrintChar('\r')!=0) return (1);
    if (PrintChar('\n')!=0) return (1);
    Y+=8;
  };
  return (0);
}
/*	hardcopyland dumps a screen to the printer from	*/
/* 	any graphics mode using the current viewport	*/
/*	in a landscape format				*/
/* 	Valid values for gmode:				*/
/*	-4 to -1 for Epson and IBM Graphic Printers	*/
/*	 0 to 7  for Epson Printers			*/

int	hardcopyland	(int gmode)

{ int			X,Y,YOfs;
  int			BitData,MaxBits;
  unsigned int		Height,Width;
  struct viewporttype 	Vport;

  getviewsettings (&Vport);
  Width=Vport.bottom-Vport.top;
  Height=(Vport.right+1)-Vport.left;
  if (PrintChar(27)!=0) return(1);
  if (PrintChar('3')!=0) return(1);
  if (PrintChar(24)!=0) return(1);
  Y=0;
  while (Y<Height)
  { if (PrintChar(27)!=0) return(1);
    switch (gmode)
    { case -1 : if (PrintChar('K')!=0) return(1);break;
      case -2 : if (PrintChar('L')!=0) return(1);break;
      case -3 : if (PrintChar('Y')!=0) return(1);break;
      case -4 : if (PrintChar('Z')!=0) return(1);break;
      case 0  :
      case 1  :
      case 2  :
      case 3  :
      case 4  :
      case 5  :
      case 6  :
      case 7  : if (PrintChar('*')!=0) return(1);
		if (PrintChar(gmode)!=0) return(1);break;
      default : return (1);
    };
    if (PrintChar (Width&0xff)!=0) return(1);
    if (PrintChar (Width>>8)!=0) return (1);
    for (X=Width-1;X>=0;X--)
    { BitData=0;
      if ((Y+7)<=Height) MaxBits=7;
	else MaxBits=Height-Y;
      for(YOfs=0;YOfs<=MaxBits;YOfs++)
      { BitData=BitData<<1;
	if (getpixel(YOfs+Y,X)>0) BitData++;
      };
      if (PrintChar(BitData)!=0) return (1);
    };
    if (PrintChar('\r')!=0) return (1);
    if (PrintChar('\n')!=0) return (1);
    Y+=8;
  };
  return (0);
}
