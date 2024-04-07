// BGI.HPP
//
// Code to allow C++ encapsulation of graphics system
//
// Michael Chen 7/24/92

#ifndef __BGI_HPP
#define __BGI_HPP

#include <graphics.h>

// Define BGI class

class BGI
{
public:
  int driver;
  int mode;
  BGI(int d=DETECT,int m=DETECT);	// defined later
  ~BGI(void);				// defined later
};


inline BGI::BGI(int d, int m)
{
  char *cp;

  driver = d;
  mode = m;
  cp = getenv("BGIDIR");
  initgraph(&driver,&mode, (cp ? cp : "") );
}

inline BGI::~BGI(void)
{
  closegraph();
}


#endif
