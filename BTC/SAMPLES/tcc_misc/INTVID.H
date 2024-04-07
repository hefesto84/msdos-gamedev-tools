
/* Special union for intvid */

typedef union u_vidreg
   {
   struct
      {
      unsigned ax;
      unsigned bx;
      unsigned cx;
      unsigned dx;
      } x;

   struct
      {
      unsigned char al, ah;
      unsigned char bl, bh;
      unsigned char cl, ch;
      unsigned char dl, dh;
      } h;
   } t_vidreg;

void intvid (t_vidreg *, int);








