PROGRAM BALLER;

GLOBAL

FILE1;          // ARCHIVO DE GRAFICOS GENERALES
FILE2;          // GRAFICOS DE LA BOLA PROTA
FONT1;          // FUENTE PARA LAS STAGES
FONT2;          // FUENTE PARA LOS TEXTOS PEICH
GRAVEDAD;

// VARIABLES DEL PROTA---

SJUMP;
VIDAS;
PUNTOS;
MUERTE;

// PARA LOS STAGES------

STAGE;
ST;

// IDENTIFICADORES DE SONIDOS------------

BOTAR;    // BOTA MI PELOTA
PT1;      //COGE ITEM1
PLOF;     // SE CARGA MALO
MORIR;    // LA PELOTA SE VA AL GARETE
MUELL;   // PARA EL IMPULSADOR

STRUCT POS[5]   // ALGO DE AYUDA CON LAS POSICIONES RELATIVAS

X;
Y;
GRAPH; END

BEGIN

SET_MODE (M640X480);
SET_FPS (30,0);
FILE1=LOAD_FPG ("USER\BALLER\BALLER.FPG");
FILE2=LOAD_FPG ("USER\BALLER\BALL.FPG");

FONT1= LOAD_FNT ("USER\BALLER\FONT1.FNT") ;
FONT2= LOAD_FNT ("USER\BALLER\FONT2.FNT") ;

PUT_SCREEN (FILE1,1);
STAGE=0;
PUNTOS=0;
VIDAS=3;

SIGNAL (META(),S_KILL);
SIGNAL (IT1(),S_KILL);
SIGNAL (MALO1(),S_KILL);
SIGNAL (SUELO(),S_KILL);
SIGNAL (PLAT1(),S_KILL);
SIGNAL (PLAT2(),S_KILL);

// SONIDOS TOCANDOSE---------------------

BOTAR= LOAD_PCM ("USER\BALLER\BALL.PCM",0) ;
PT1= LOAD_PCM ("USER\BALLER\ITEM.PCM",0);
PLOF= LOAD_PCM ("USER\BALLER\PLOF.PCM",0);
MORIR= LOAD_PCM ("USER\BALLER\MORIR.PCM",0);
MUELL= LOAD_PCM ("USER\BALLER\MUELLE.PCM",0);



LOOP

IF (TIMER[9]=>300) DELETE_TEXT (ST) ; END

IF (STAGE==0)
    VIDAS=3;
    PUNTOS=0;
    DELETE_TEXT (ALL_TEXT);
    STOP_SCROLL(0); END



IF (STAGE==0 AND KEY(_SPACE))

    STAGE=1;
    CLEAR_SCREEN () ;
    FADE_OFF ();
    START_SCROLL (0,FILE1,2,3,0,5) ;
    FADE_ON ();
    PUNTOS=0;
    VIDAS=3;
    SCROLL[0].X1=100;
    SCROLL[0].Y1=100;
    SCROLL[0].CAMERA=BOLA();
    TIMER[9]=0;
    WRITE (FONT2,0,0,0, "VIDAS:" );
    WRITE (FONT2,0,50,0, "PUNTOS:" );
    WRITE_INT (FONT2,100,0,0, OFFSET VIDAS );
    WRITE_INT (FONT2,150,50,0, OFFSET PUNTOS );
    ST= WRITE (FONT1,215,200,0, "STAGE 1");

    MALO1();
    IT1();
    PLAT1();
    PLAT2();
    META();
    SUELO(); END


IF (KEY(_SPACE) AND STAGE==-1)

    CLEAR_SCREEN();
    STOP_SCROLL(0);
    FADE_OFF ();
    STAGE=0;
    PUT_SCREEN (FILE1,1);
    FADE_ON () ; END


FRAME; END
END
// PROCESO DE LA BOLA PROTAGONISTA, LA TUYA, VAMOS, LA CHACHI----------------

PROCESS BOLA()

PRIVATE

IX;

BEGIN

FILE=FILE2;
GRAPH=2;
Y=400;
X=320;
SIZE=30;
CTYPE=C_SCROLL;
GRAVEDAD=-20;

LOOP

IF (KEY(_T)) WRITE_INT (0,200,0,0, OFFSET IX); END

IF (Y>490)
    SOUND (MORIR,100,256);
    MUERTE=1; END

IF (VIDAS==0)

    STAGE=-1;
    DELETE_TEXT (ALL_TEXT);
    VIDAS=3;
    CLEAR_SCREEN();
    PUT_SCREEN (FILE1,14);
    STOP_SCROLL(0); END


POS[0].X=X;
POS[0].Y=Y;

IF (MUERTE==0)

X=X + IX;
Y=Y + GRAVEDAD;
POS[4].X=IX;
GRAVEDAD=GRAVEDAD + 1;

IF (COLLISION(TYPE SUELO) AND Y<480)
    SOUND (BOTAR,128,256) ;
    GRAVEDAD=-15 ; END

IF (COLLISION(TYPE PLAT1) AND GRAVEDAD>0)
    IF (STAGE<3)
    SOUND (BOTAR,128,256);
    GRAVEDAD=-15;
    ELSE
    SOUND (BOTAR,128,256);
    GRAVEDAD=-5;END
    END


IF (COLLISION(TYPE PLAT2) AND GRAVEDAD>0)
    SOUND (BOTAR,128,256);
    GRAVEDAD=-10; END

IF (COLLISION(TYPE PLAT_CUEVA2))
    IF (POS[4].X>0)
    IX=-15;
    ELSE
    IX=15; END
    END


IF (COLLISION(TYPE MUELLE) AND GRAVEDAD>0)
    IF (STAGE<3)
    SOUND (MUELL,128,256);
    IX=50;
    GRAVEDAD=-25;
    ELSE
    SOUND (MUELL,128,256);
    GRAVEDAD=-20; END
    END


IF (IX>0) IX=IX - 1; END
IF (IX<0) IX=IX + 1; END

IF (KEY(_RIGHT) AND IX<8) IX+=2; END
IF (X>340 AND KEY(_LEFT)  AND IX>-8) IX-=2; END

IF (GRAPH==11) GRAPH=1;
    ELSE
    GRAPH++; END
END

IF (MUERTE==1)
    ANGLE-=10000;
    SIZE+=3;
    Y=Y - 20; END


IF (Y<-100)

    MUERTE=0;
    VIDAS-=1;
    FADE_OFF();
    X=320;
    Y=300;
    SIZE=30;
    ANGLE=0;
    FADE_ON(); END


IF (COLLISION(TYPE META) AND (STAGE==1))

    STAGE=2;
    CLEAR_SCREEN () ;
    FADE_OFF ();
    STOP_SCROLL (0);
    START_SCROLL (0,FILE1,2,3,0,5) ;
    FADE_ON ();
    SCROLL[0].X1=100;
    SCROLL[0].Y1=100;
    SCROLL[0].CAMERA=BOLA();
    TIMER[9]=0;
    ST= WRITE (FONT1,215,200,0, "STAGE 2");

    MALO1();
    IT1();
    PLAT1();
    PLAT2();
    META();
    MUELLE();
    SUELO(); END

IF ((POS[0].X>3100 AND Y>470 AND (STAGE==2)) OR (KEY(_N)))

    STAGE=3;
    CLEAR_SCREEN () ;
    FADE_OFF ();
    STOP_SCROLL (0);
    START_SCROLL (0,FILE1,12,0,0,5) ;
    FADE_ON ();
    SCROLL[0].X1=100;
    SCROLL[0].Y1=100;
    SCROLL[0].CAMERA=BOLA();
    TIMER[9]=0;
    ST= WRITE (FONT1,215,200,0, "STAGE 3");

    MALO1();
    IT1();
    PLAT1();
    PLAT2();
    META();
    PLAT_CUEVA2();
    MUELLE();
    FINAL();
    SUELO(); END



FRAME; END
END



// LAS PLATAFORMAS--------

PROCESS SUELO()

BEGIN

FILE=FILE1;
GRAPH=4;
Y=-900;
CTYPE=C_SCROLL;

IF (STAGE==1)

GRAPH=4;
CLONE
Y=490;


CLONE
X=200;

CLONE
X=400;

CLONE
X=600;

CLONE
X=800;

CLONE
X=1100;

CLONE
X=1300;

CLONE
X=1500;

CLONE
X=2400;

CLONE
X=2600;

CLONE
X=2800;


CLONE
X=3600;

CLONE
X=3800;

END
END
END
END
END
END
END
END
END
END
END
END
END
END

IF (STAGE==2)

CLONE
X=200;
Y=490;

CLONE
X=400;
Y=490;


CLONE
X=1000;
Y=490;


CLONE
X=2100;
Y=490;

CLONE
X=2600;

CLONE
X=2800;

CLONE
X=3000;


END
END
END
END
END
END
END
END

IF (STAGE==3)

GRAPH=13;

CLONE
X=300;
Y=490;

CLONE
X=500;

END
END
END

LOOP

FRAME; END
END

PROCESS PLAT1()

BEGIN

FILE=FILE1;
GRAPH=5;
X=-600;
Y=-400;
CTYPE=C_SCROLL;

IF (STAGE==1)

GRAPH=5;

CLONE

Y=300;
X=600;

CLONE

X=600;
Y=400;

CLONE

X=1800;
Y=400;

CLONE

X=1900;
Y=300;

CLONE
X=2000;
Y=200;

CLONE
X=2000;
Y=400;

CLONE
X=2100;
Y=300;

CLONE
X=2200;
Y=400;

END
END
END
END
END
END
END
END
END

IF (STAGE==3)
GRAPH=15;

CLONE
X=645;
Y=360;

CLONE
X=745;
Y=360;

CLONE
X=845;
Y=360;


CLONE
X=645;
Y=260;

CLONE
X=745;
Y=260;

CLONE
X=845;
Y=260;

CLONE
X=945;
Y=260;

CLONE
X=945;
Y=460;

CLONE
X=1045;
Y=460;

CLONE
X=1035;
Y=160;

CLONE
X=1135;
Y=260;

CLONE
X=1135;
Y=160;

CLONE
X=1440;
Y=363;

CLONE
X=1440;
Y=460;

CLONE
X=1540;
Y=460;

CLONE
X=1640;
Y=355;

CLONE
X=1640;
Y=255;

CLONE
X=1640;
Y=460;

CLONE
X=1740;
Y=355;

CLONE
X=1740;
Y=255;

CLONE
X=1740;
Y=460;

CLONE
X=1840;
Y=355;

CLONE
X=1840;
Y=255;

CLONE
X=1840;
Y=460;

END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
LOOP

FRAME; END
END


PROCESS PLAT2()   //PLATAFORMA MOVIL

PRIVATE

DIR;

BEGIN

FILE=FILE1;
GRAPH=8;
CTYPE=C_SCROLL;
DIR=2;
X=-2800;
Y=-320;


IF (STAGE==1)

CLONE
X=2800;
Y=320;


CLONE
X=3000;
Y=400;

CLONE
X=3200;
Y=320;

CLONE
X=3400;
Y=400;

END
END
END
END
END

IF (STAGE==2)

CLONE

X=700;
Y=350;

CLONE

X=2300;
Y=360;

END
END
END


LOOP

IF (DIR==1)
    Y=Y-2; END

IF (DIR==2)
    Y=Y+2; END

IF (Y<280) DIR=2; END
IF (Y>400) DIR=1; END


FRAME; END
END


PROCESS PLAT_CUEVA2()

BEGIN

FILE=FILE1;
GRAPH=16;
CTYPE=C_SCROLL;
X=-1000;
Y=-1000;

IF (STAGE==3)

CLONE
X=600;
Y=415;

CLONE
X=890;
Y=415;

CLONE
X=990;
Y=315;

CLONE
X=990;
Y=215;

CLONE
X=1090;
Y=415;

CLONE
X=1090;
Y=315;

CLONE
X=1390;
Y=320;

CLONE
X=1390;
Y=230;

CLONE
X=1390;
Y=145;

CLONE
X=1490;
Y=320;

CLONE
X=1490;
Y=245;

CLONE
X=1490;
Y=145;

CLONE
X=1885;
Y=200;

CLONE
X=1875;
Y=405;

CLONE
X=1875;
Y=410;

END
END
END
END
END
END
END
END
END
END
END
END
END
END
END
END

LOOP



FRAME;END
END


// PROCESOS DE ITEMS----------------

PROCESS IT1()

BEGIN

FILE=FILE1;
GRAPH=6;
CTYPE=C_SCROLL;
X=-600;
Y=-250;

IF (STAGE==1)

CLONE
X=600;
Y=250;

CLONE
X=600;
Y=350;

CLONE
X=1350;
Y=450;

CLONE
X=1375;
Y=425;

CLONE
X=1400;
Y=400;

CLONE
X=2000;
Y=350;

CLONE
X=3100;
Y=300;

CLONE
X=3300;
Y=300;

END
END
END
END
END
END
END
END
END

IF (STAGE==2)

CLONE

X=1700;
Y=160;

CLONE

X=1800;
Y=150;

CLONE

X=1900;
Y=140;

END
END
END
END

IF (STAGE==3)

CLONE
X=1650;
Y=310;

CLONE
X=1750;
Y=310;

CLONE
X=1850;
Y=310;

END
END
END
END

LOOP

IF (COLLISION(TYPE BOLA) AND MUERTE==0)

    SOUND (PT1,128,256);
    PUNTOS=PUNTOS + 500;
    BREAK; END


FRAME; END
END




// PROCESOS DE LOS MALOS----------

PROCESS MALO1()

PRIVATE

GRAV1;

BEGIN


FILE=FILE1;
GRAPH=7;
CTYPE=C_SCROLL;
X=-800;
Y=-400;
GRAV1=-20;

IF (STAGE==1)

CLONE
X=800;
Y=400;


CLONE
X=1050;
Y=450;

CLONE
X=1100;
Y=425;

CLONE
X=1150;
Y=375;

CLONE
X=2400;
Y=300;

CLONE
X=2450;
Y=400;

CLONE
X=2500;
Y=300;

END
END
END
END
END
END
END
END

IF (STAGE==2)

CLONE
X=600;
Y=325;

CLONE
X=700;
Y=350;

CLONE
X=800;
Y=375;

CLONE
X=2800;
Y=300;

CLONE
X=2825;
Y=325;

CLONE
X=2850;
Y=350;

CLONE
X=2875;
Y=375;

CLONE
X=2900;
Y=400;

CLONE
X=2925;
Y=425;

CLONE
X=2950;
Y=400;

CLONE
X=2975;
Y=375;

CLONE
X=3000;
Y=350;

CLONE
X=3025;
Y=325;

CLONE
X=3050;
Y=300;

END
END
END
END
END
END
END
END
END
END
END
END
END
END
END

IF (STAGE==3)

CLONE
X=1235;
Y=470;

CLONE
X=1285;
Y=430;

CLONE
X=1335;
Y=470;

END
END
END
END


LOOP

Y=Y + GRAV1;
GRAV1=GRAV1 + 1 ;

IF (Y>460) GRAV1=-20; END

IF (COLLISION(TYPE BOLA) AND MUERTE==0)
    IF (Y>POS[0].Y)
    GRAVEDAD=-15;
    SOUND (PLOF,128,256);
    BREAK;
    ELSE
    SOUND (MORIR,100,256);
    MUERTE=1; END
    END


FRAME; END
END


// PROCESO PARA LA META

PROCESS META()

BEGIN

FILE=FILE1;
GRAPH=9;
X=-3800;
Y=-375;
CTYPE=C_SCROLL;

IF (STAGE==1)

CLONE
GRAPH=9;
X=3800;
Y=375; END
END

IF (STAGE==2)

CLONE

GRAPH=11;
X=3200;
Y=400; END

END

LOOP


FRAME; END
END


// OTROS OBJETILLOS CHACHIS

PROCESS MUELLE()

BEGIN

FILE=FILE1;
GRAPH=10;
CTYPE=C_SCROLL;
X=-1000;

IF (STAGE==2)

CLONE

GRAPH=10;
X=1090;
Y=455;
SIZE=50;
Z=-100:
END
END

IF (STAGE==3)

CLONE

GRAPH=17;
X=1045;
Y=445;
SIZE=50;
Z=-100;

CLONE

X=1550;
Y=445;

END
END
END

LOOP
FRAME; END
END

PROCESS FINAL() // EL PROCESO DEL GLOBO DE "THE END"

BEGIN

FILE=FILE1;
GRAPH=18;
CTYPE=C_SCROLL;

IF (STAGE==3)

CLONE
X=1900;
Y=315;

END
END

LOOP

IF (COLLISION(TYPE BOLA))

    STAGE=-1;
    DELETE_TEXT (ALL_TEXT);
    CLEAR_SCREEN();
    PUT_SCREEN (FILE1,19);
    STOP_SCROLL(0); END


FRAME; END
END
