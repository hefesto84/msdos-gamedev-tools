
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define GLOBALS
#include "div.h"

// Importante: Para cada funci¢n se debe indicar el retval(int), y hacer
// siempre un getparm() por cada par metro de llamada (el retval() es
// imprescindible incluso si la funci¢n no necesita devolver un valor).

    // Funcion interna....
    void pintalinea (int ,int ,int ,int ,unsigned char );

    int xinicio[10];
    int yinicio[10];
    int tama¤ox[10];
    int tama¤oy[10];
    int colorinicial;
    char colorfinalcuadrado[10];
    int estadocuadrado[10];
    int radio[10];
    int xiniciocirculo[10];
    int yiniciocirculo[10];
    char colorfinalcirculo[10];
    int estadocirculo[10];
    int xiniciolinea[30];
    int yiniciolinea[30];
    int xfinallinea[30];
    int yfinallinea[30];
    char colorfinallinea[30];
    int estadolinea[30];

void poncuadrado() { // Dibuja un cuadrado en pantalla

    int cual;
    // Coge parametros
    cual=getparm();
    if (cual<0) cual=0;
    if (cual>9) cual=9;
    colorinicial=getparm();
    tama¤oy[cual]=getparm();
    tama¤ox[cual]=getparm();
    yinicio[cual]=getparm();
    xinicio[cual]=getparm();
    estadocuadrado[cual]=1;
    // Cuadra valores con los limites de la pantalla
    if (xinicio[cual]<0) xinicio[cual]=0;
    if (yinicio[cual]<0) yinicio[cual]=0;
    if (xinicio[cual]>wide) xinicio[cual]=wide;
    if (yinicio[cual]>height) yinicio[cual]=height;
    if (tama¤ox[cual]<0) tama¤ox[cual]=0;
    if (tama¤oy[cual]<0) tama¤oy[cual]=0;
    if (xinicio[cual]+tama¤ox[cual]>wide) tama¤ox[cual] = wide-xinicio[cual];
    if (yinicio[cual]+tama¤oy[cual]>height) tama¤ox[cual] = height-xinicio[cual];

    // Convierte el parametro int en char
    if (colorinicial<0) colorinicial=0;
    if (colorinicial>255) colorinicial=255;
    colorfinalcuadrado[cual]=colorinicial;
    retval(cual);
}
void quitacuadrado() { // Quita un cuadrado en pantalla
    int cual;
    // Coge parametros
    cual=getparm();
    if (cual<0) cual=0;
    if (cual>9) cual=9;
    estadocuadrado[cual]=0;
    retval(cual);
}
void poncirculo() { // Dibuja un circulo en pantalla
    int cual;
    // Coge parametros
    cual=getparm();
    if (cual<0) cual=0;
    if (cual>9) cual=9;
    // Coge parametros
    colorinicial=getparm();
    radio[cual]=getparm();
    yiniciocirculo[cual]=getparm();
    xiniciocirculo[cual]=getparm();
    estadocirculo[cual]=1;
    if (xiniciocirculo[cual]<0) xiniciocirculo[cual]=0;
    if (xiniciocirculo[cual]>wide) xiniciocirculo[cual]=wide;
    if (yiniciocirculo[cual]<0) yiniciocirculo[cual]=0;
    if (yiniciocirculo[cual]>height) yiniciocirculo[cual]=height;

    // Convierte el parametro int en char
    colorfinalcirculo[cual]=colorinicial;
    retval(cual);
}
void quitacirculo() { // Quita un circulo de pantalla
    int cual;
    // Coge parametros
    cual=getparm();
    if (cual<0) cual=0;
    if (cual>9) cual=9;
    estadocirculo[cual]=0;
    retval(cual);
}

void ponlinea() { // Dibuja una linea en pantalla
    int cual;
    // Coge parametros
    cual=getparm();
    if (cual<0) cual=0;
    if (cual>29) cual=29;
    // Coge parametros
    colorinicial=getparm();
    yfinallinea[cual]=getparm();
    xfinallinea[cual]=getparm();
    yiniciolinea[cual]=getparm();
    xiniciolinea[cual]=getparm();
    estadolinea[cual]=1;
    if (xiniciolinea[cual]<0) xiniciolinea[cual]=0;
    if (xiniciolinea[cual]>wide) xiniciolinea[cual]=wide;
    if (yiniciolinea[cual]<0) yiniciolinea[cual]=0;
    if (yiniciolinea[cual]>height) yiniciolinea[cual]=height;
    if (xfinallinea[cual]<0) xfinallinea[cual]=0;
    if (xfinallinea[cual]>wide) xfinallinea[cual]=wide;
    if (yfinallinea[cual]<0) yfinallinea[cual]=0;
    if (yfinallinea[cual]>height) yfinallinea[cual]=height;

    // Convierte el parametro int en char
    colorfinallinea[cual]=colorinicial;
    retval(cual);
}
void quitalinea() { // Quita una linea de pantalla
    int cual;
    // Coge parametros
    cual=getparm();
    if (cual<0) cual=0;
    if (cual>29) cual=9;
    estadolinea[cual]=0;
    retval(cual);
}



void post_process_buffer() { // Dibuja todo en pantalla
        // Reinicia variables
        int contador1;
        int contador2;
        for ( contador2=0 ; contador2<10 ; contador2++ ){        
                if (estadocuadrado[contador2]==1){
                    for ( contador1=0 ; contador1<tama¤ox[contador2] ; contador1++ ){
                             buffer[xinicio[contador2]+contador1+(yinicio[contador2]*wide)]=colorfinalcuadrado[contador2];                    
                             buffer[xinicio[contador2]+contador1+((yinicio[contador2]+tama¤oy[contador2])*wide)]=colorfinalcuadrado[contador2];                     
                    }            
                    for ( contador1=0 ; contador1<tama¤oy[contador2] ; contador1++){
                            buffer[xinicio[contador2]+((yinicio[contador2]+contador1)*wide)]=colorfinalcuadrado[contador2];
                            buffer[xinicio[contador2]+tama¤ox[contador2]+((yinicio[contador2]+contador1)*wide)]=colorfinalcuadrado[contador2];                                         
                    }
                }
        }
        int puntopfinalx;
        int puntopfinaly;
        for ( contador2=0 ; contador2<10 ; contador2++ ){        
                if (estadocirculo[contador2]==1){
                        for (contador1=0;contador1<6*radio[contador2];contador1++){
                                puntopfinalx=xiniciocirculo[contador2]+sin(contador1)*radio[contador2];
                                puntopfinaly=yiniciocirculo[contador2]+cos(contador1)*radio[contador2];
                                if (puntopfinalx>0 && puntopfinalx<wide &&
                                    puntopfinaly>0 && puntopfinaly<height )
                                    buffer[puntopfinalx+((puntopfinaly)*wide)]=colorfinalcirculo[contador2];
                        }
                }
         }
        for ( contador2=0 ; contador2<30 ; contador2++ ){        
                if (estadolinea[contador2]==1)
                        pintalinea (xiniciolinea[contador2],yiniciolinea[contador2],xfinallinea[contador2],yfinallinea[contador2],colorfinallinea[contador2]);
                
         }


}

void __export divlibrary(LIBRARY_PARAMS) {

  COM_export("PONCUADRADO",poncuadrado,6);
  COM_export("QUITACUADRADO",quitacuadrado,1);
  COM_export("PONCIRCULO",poncirculo,5);
  COM_export("QUITACIRCULO",quitacirculo,1);
  COM_export("PONLINEA",ponlinea,6);
  COM_export("QUITALINEA",quitalinea,1);


  // Se indica el nombre que tendr  la funci¢n;
  // en el lenguaje, el puntero a la misma y
  // el n£mero de par metros que tiene.

}

void __export divmain(COMMON_PARAMS) {
  AutoLoad();
  GLOBAL_IMPORT();
  DIV_export("post_process_buffer",post_process_buffer);
}

void pintalinea (int x1,int y1,int x2,int y2,unsigned char elcolor){

int contador;
int inclinacion;
int cuentaincli=0;
int px1,px2,py1,py2;

px1=x1;px2=x2;
py1=y1;py2=y2;


if ((px2-px1)!=0 && (py2-py1)!=0){
if  (abs(y2-y1)<abs(x2-x1) ){
      inclinacion=((abs(py2-py1))*100)/(abs(px2-px1));
      for (contador=0;contador<abs(x1-x2)+1;++contador){
           buffer[px1+(py1*wide)]=elcolor;
	   cuentaincli+=inclinacion;
	   if (cuentaincli>100){cuentaincli-=100;if (y2<y1){--py1;}
						 if (y2>y1){++py1;}}
	   if (x2<x1){--px1;}
	   if (x2>x1){++px1;}}
     }
if (abs(y2-y1)>abs(x2-x1)){
      inclinacion=((abs(px2-px1))*100)/(abs(py2-py1));
      for (contador=0;contador<abs(y1-y2)+1;++contador){
           buffer[px1+(py1*wide)]=elcolor;
	   cuentaincli+=inclinacion;
	   if (cuentaincli>100){cuentaincli-=100;if (x2<x1){--px1;}
						 if (x2>x1){++px1;}}
	   if (y2<y1){--py1;}
	   if (y2>y1){++py1;}}
     }
if (abs(x2-x1)==abs(y2-y1)){
   for (contador=0;contador<abs(y1-y2)+1;++contador){
       buffer[px1+(py1*wide)]=elcolor;
       if (y2>y1){++py1;}
       if (y2<y1){--py1;}
       if (x2>x1){++px1;}
       if (x2<x1){--px1;}}
    }
}
if (x1-x2==0){
   for (contador=0;contador<abs(y1-y2)+1;++contador){
       buffer[px1+(py1*wide)]=elcolor;
       if (y2>y1){++py1;}
       if (y2<y1){--py1;}}
   }
if (y1-y2==0){
   for (contador=0;contador<abs(x1-x2)+1;++contador){
       buffer[px1+(py1*wide)]=elcolor;
       if (x2>x1){++px1;}
       if (x2<x1){--px1;}}
       }


}
