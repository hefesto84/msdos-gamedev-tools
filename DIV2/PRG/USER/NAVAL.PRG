//-----------------------------------------------------------------------------
// AUTOR : NATES FDEZ. RAMON
//         LAREDO (SPAIN) < http://web.jet.es/laredo >
//         rnates@jet.es
//
// A todo aquel que vea el siguiente codigo, antes de nada disculpas,
// por que no esta muy explicado que digamos.
//
// Programado bajo el sistema DIV GAMES STUDIO.
//
// Primer programa realizado con DIV para analizar y aprender el manejo
// de sus funciones.
//
// El programa dispone de un mapeador, muy personalizado. No esta preparado
// aun. Se activa con la tecla (¦)
//
// EL CODIGO NO ESTA DEPURADO, PUEDE QUE HALLA ALGUNA COSA SIN SENTIDO, O
// QUE SEGURO SE PODIA HABER DESARROLLADO DE OTRA FORMA.
//
// NO TENGO TANTO TIEMPO, CUENTAME TUS IDEAS PARA MEJORARLO.
//
// EL PROGRAMA HA SIDO DESARROLLADO EN UN PENTIUN 166 MMX
//-----------------------------------------------------------------------------

PROGRAM NAVAL;


CONST

ALIADO=1;
ENEMIGO=0;
TOPELE=41;
MAXELE=100;       // AUNQUE LA MATRIZ LA HEMOS DISE¥ADO PARA 500
                  // EN LAS COMPROBACIONES LO HAREMOS EN LOS 100 PRIMEROS
                  // YA QUE DE MOMENTO NO ESTOY DISE¥ANDO, MAS DE 100
GLOBAL

SALIDA=0;
MODOPAN=1;
XPANRES=640;
YPANRES=480;
ANGULOGLOBAL=0;
VIENDOMAPA=0;

JUGANDO=0;
PUNELE=1;
CREANDO=0;
NVISOR=0;
file1;           // id del fichero que contiene los graficos
idselec=0;       // guardara el processo al cual estamos dando ordenes.
idobjet=0;       // guardara al enemigo que queremos atacar o el lider a seguir.
VS=8;            // VELOCIDAD SCROLL
FO1; FO2; FO3; FO4;
SO1; SO2; SO3;
SO4; SO5;            // SONIDOS.
FASE=0;
TOPFASE=1;
ESTADOJUEGO=2;
II;              // VARIABLE DE AMBITO GENERAL PARA USAR MOMENTANEAMENTE.
TOENE=0;
TOALI=0;
TODEP=0;
TOBUN=0;
TIPGRAPH[41]=  0,201,251,202,252,203,253,0,0,0,
             300,301,302,  0,  0,  0,  0,0,0,0,
             510,  0,  0,  0,  0,  0,  0,0,0,0,
             550,551,  0,  0,  0,  0,  0,0,0,0;
STRUCT ELEMENTO[500]
       NUMERO;
       TIPO;
       ACTIVO;          // SI LO ESTAMOS USANDO;
       NID;             // ID DEL OBJETO CREADO
       X; Y;            // X e Y
       XDEST; YDEST;    // DESTINO
       ACCION;          // ACCION
       OBJETIVO;
       VELOCIDAD;
       LIDER;
       PARADO;
       NGULODEST;
       ANGLE;
       ANCHO;
       ALTO;

END=500 DUP(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);






LOCAL

VISIBLE=0;
TIPO=0;
TIENEHUELLA=0;   // INDICARA SI HA DEJADO HUELLA EL BARCO
TIENEHUMO=0;     // INDICARA SI EL DEPOSITO ESTA EN LAS ULTIMAS
VELOCIDAD=2;     // indicara la velocidad de cada processo 1 UN PIXEL
XDEST; YDEST;    // destino del processo
XXDEST; YYDEST;  // DESTINO DE CORRECION EN CASO DE CHOQUE
NGULODEST;       // angulo destino
PCOX[4];         // X REAL DE LOS CUATRO PUNTOS DE CONTROL PARA TORRETAS
PCOY[4];         // Y  "
PTOCONTROL;      // CADA TORRETA TENDRA IDENTIFICADO SU PUNTO DE CONTROL
ENERGIA=100;
OBJETIVO=0;      // INDICA A QUIEN DEBE DISPARAR
LIDER=0;         // INDICA A QUIEN DEBE SEGUIR
XANT=0;
YANT=0;
ALCANCE=200;
RECARGO=300;
PRECISION=10;
PARADO=1;
CALIBRE=2;       // EL CALIBRE DEL DISPARO DE CADA TORRETA
CHOCADO=0;       // INDICARA SI HEMOS CHOCADO CON OTRO BARCO
VGIRO=2500;      // VELOCIDAD DE GIRO
ACCION=0;
ACCIONANT=0;
X_RESOL;
Y_RESOL;
BANDO=0;
MUERTO=0;
INCX=1;
INCY=1;
NUMERO=0;
ALTO=0;
ANCHO=0;
MUNICION=1000;
ANGULOANT=0;


BEGIN
   FO1=LOAD_FNT("USER\NAVAL\NUEVOFNT.FNT");
   FO2=LOAD_FNT("USER\NAVAL\MINI.FNT");
   SO1=LOAD_PCM("USER\NAVAL\MOTOR.PCM",0);
   SO2=LOAD_PCM("USER\NAVAL\BOMBA4.PCM",0);
   SO3=LOAD_PCM("USER\NAVAL\AGUA8.PCM",0);
   SO4=LOAD_PCM("USER\NAVAL\FOTO2.PCM",0);
   SO5=LOAD_PCM("USER\NAVAL\BOMBA5.PCM",0);
   file1=LOAD_FPG("USER\NAVAL\naval.fpg");
   SET_MODE(m320x200);
   SET_FPS(28,0);
   LOOP
        SWITCH(ESTADOJUEGO)
         CASE 1:
           JUEGO();
         END                 //*****************COMIENZO JUEGO
         CASE 2:
           FADE_OFF();
           CREDITOS();       //*****************PANTALLA PRINCIPAL
         END
         CASE 3:
           BREAK;            //*****************FIN PROGRAMA
         END
        END
        FRAME;
     END
     LET_ME_ALONE();
     EXIT("rnates@jet.es",0);
END



PROCESS JUEGOENPAUSA();
PRIVATE T;
BEGIN

                      SIGNAL(TYPE JUEGO, S_FREEZE);
                      SIGNAL(TYPE RATON, S_FREEZE);
                      SIGNAL(TYPE BARCO, S_FREEZE_TREE);

                      IF(NOT MODOPAN)
                         FO3=LOAD_FNT("USER\NAVAL\HUECAMED.FNT");
                         T=WRITE(FO3,95,120,0,"PAUSA");
                         ELSE
                         FO3=LOAD_FNT("USER\NAVAL\HUECA.FNT");
                         T=WRITE(FO3,200,220,0,"PAUSA");
                      END
                      WHILE(SCAN_CODE==0) FRAME; END
                      UNLOAD_FNT(FO3);
                      SIGNAL(TYPE JUEGO, S_WAKEUP);
                      SIGNAL(TYPE RATON, S_WAKEUP);
                      SIGNAL(TYPE BARCO, S_WAKEUP_TREE);

                      DELETE_TEXT(T);

END







PROCESS CREDITOS()
BEGIN
     ESTADOJUEGO=-1;
     GRAPH=1;

                         IF(NOT MODOPAN)
                            MODOPAN=0;
                            SET_MODE(m320x200);
                            FO3=LOAD_FNT("USER\NAVAL\HUECAMED.FNT");
                            XPANRES=320; YPANRES=200;
                            SIZE=50;
                            X=160; Y=80;
                            WRITE(FO3,20,0,0,"BATALLITAS");
                            WRITE(FO3,20,60,0,"F1 - 640x480");
                            WRITE(FO3,20,90,0,"F2 - EMPEZAR.");
                            WRITE(FO3,20,120,0,"F4 - SALIR.");
                            WRITE(FO2,5,180,0,"RESPIGO GAMES (C). NATES R. Laredo (Spain) 1998");
                            ELSE
                            MODOPAN=1;
                            SET_MODE(m640x480);
                            FO3=LOAD_FNT("USER\NAVAL\HUECA.FNT");
                            XPANRES=640; YPANRES=480;
                            SIZE=100;
                            X=320; Y=240;
                            WRITE(FO3,20,0,0,"BATALLITAS");
                            WRITE(FO3,20,160,0,"F1 - 320x200");
                            WRITE(FO3,20,240,0,"F2 - EMPEZAR.");
                            WRITE(FO3,20,320,0,"F4 - SALIR.");
                            WRITE(FO2,10,460,0,"RESPIGO GAMES (C). NATES R. Laredo (Spain) 1998");
                         END

     ESTADOJUEGO=-1;
     CLEAR_SCREEN();
     FADE_ON();
     LOOP
       CREANDO=0;
       SWITCH(SCAN_CODE)
               CASE _F1: IF(MODOPAN)
                           MODOPAN=0;
                           ELSE
                           MODOPAN=1;
                         END
                         ESTADOJUEGO=2;
                         BREAK;
               END
               CASE _F2: ESTADOJUEGO=1; BREAK; END
               CASE _F4: ESTADOJUEGO=3; BREAK; END
       END
       FRAME;
     END
     DELETE_TEXT(ALL_TEXT);
     UNLOAD_FNT(FO3);

END


//------------------------------------------------------------------------------
PROCESS CARGAFASE()
PRIVATE I; IDR;
BEGIN
         SIGNAL(TYPE BARCO,S_KILL);
         SIGNAL(TYPE TORRETA,S_KILL);
         SIGNAL(TYPE DISPARO,S_KILL);
         SIGNAL(TYPE ISLA,S_KILL);
         SIGNAL(TYPE BUNKER,S_KILL);
         LOAD("USER\NAVAL\NAVALMAP.MAP",&ELEMENTO);
         TOENE=0; TOALI=0;
         FOR(I=0; I<500; I++)
               IF(ELEMENTO[I].ACTIVO)
                 SWITCH(ELEMENTO[I].TIPO)
                    CASE 1: IDR=BARCO(ELEMENTO[I].X,ELEMENTO[I].Y,1,1,3,I); END
                    CASE 2: IDR=BARCO(ELEMENTO[I].X,ELEMENTO[I].Y,1,0,3,I); END
                    CASE 3: IDR=BARCO(ELEMENTO[I].X,ELEMENTO[I].Y,2,1,3,I); END
                    CASE 4: IDR=BARCO(ELEMENTO[I].X,ELEMENTO[I].Y,2,0,3,I); END
                    CASE 5: IDR=BARCO(ELEMENTO[I].X,ELEMENTO[I].Y,3,1,3,I); END
                    CASE 6: IDR=BARCO(ELEMENTO[I].X,ELEMENTO[I].Y,3,0,3,I); END
                    CASE 10:IDR=ISLA(ELEMENTO[I].X,ELEMENTO[I].Y,300,I); END
                    CASE 11:IDR=ISLA(ELEMENTO[I].X,ELEMENTO[I].Y,301,I); END
                    CASE 12:IDR=ISLA(ELEMENTO[I].X,ELEMENTO[I].Y,302,I); END
                    CASE 20:IDR=BUNKER(ELEMENTO[I].X,ELEMENTO[I].Y,I); END
                    CASE 30:IDR=DEPOSITO(ELEMENTO[I].X,ELEMENTO[I].Y,550,I); END
                    CASE 31:IDR=DEPOSITO(ELEMENTO[I].X,ELEMENTO[I].Y,551,I); END

                 END
                 ELEMENTO[I].NID=IDR;
                END
         END
END
//-------------------------------------------------------------------------------
PROCESS CREAELEMENTO(X,Y,PUNELE);
PRIVATE P,I,E=0;
BEGIN
      GRAPH=200;
      P=PUNELE;
      FOR(I=1; I<MAXELE; I++)
       IF(ELEMENTO[I].ACTIVO==0)     // BUSCO ELEMENTO NO USADO
          E=I; BREAK;
       END
      END
      IF(CREANDO)
         X+=SCROLL[0].X0;
         Y+=SCROLL[0].Y0;
      END
      IF(E<>0)
         SWITCH(P)
            CASE 1:  I=BARCO(X,Y,1,1,3,E); END   // BARCO ALIADO DE UNA TORRETA
            CASE 2:  I=BARCO(X,Y,1,0,3,E); END   //         ENEMIGO
            CASE 3:  I=BARCO(X,Y,2,1,3,E); END   // BARCO ALIADO DE DOS TORRETA
            CASE 4:  I=BARCO(X,Y,2,0,3,E); END   //         ENEMIGO
            CASE 5:  I=BARCO(X,Y,3,1,3,E); END   // BARCO ALIADO DE TRES TORRETA
            CASE 6:  I=BARCO(X,Y,3,0,3,E); END   //         ENEMIGO
            CASE 10: I=ISLA(X,Y,TIPGRAPH[PUNELE],E); END
            CASE 11: I=ISLA(X,Y,TIPGRAPH[PUNELE],E); END
            CASE 12: I=ISLA(X,Y,TIPGRAPH[PUNELE],E); END
            CASE 20: I=BUNKER(X,Y,E); END
            CASE 30: I=DEPOSITO(X,Y,TIPGRAPH[PUNELE],E); END
            CASE 31: I=DEPOSITO(X,Y,TIPGRAPH[PUNELE],E); END

         END
         ELEMENTO[E].TIPO=P;
         ELEMENTO[E].X=X;
         ELEMENTO[E].Y=Y;
         ELEMENTO[E].ACTIVO=1;
         ELEMENTO[E].PARADO=1;
         ELEMENTO[E].NID=I;
         ELEMENTO[E].ANGLE=ANGULOGLOBAL;
         ELEMENTO[E].NGULODEST=0;
         ELEMENTO[E].ACCION=3;
         ELEMENTO[E].YDEST=X;
         ELEMENTO[E].YDEST=X;
         I=ANGULOGLOBAL;
         RETURN(ELEMENTO[E].NID);
      END
      RETURN(0);
END

//-------------------------------------------------------------------------------
PROCESS MAPEANDO()
PRIVATE I=0; SITUA=0; A; B; XO; YO; G;
IDR[10]=0,0,0,0,0,0,0,0,0,0;
BEGIN
   GRAPH=801;
   // CONGELO EL RATON YA QUE ACTIVO OTRO RATON
   SIGNAL(TYPE RATON,S_KILL);
   SIGNAL(TYPE VERMAPA, S_KILL);
   VIENDOMAPA=0;
   IDR[0]=WRITE(FO2,0,0,0,"SPACE SITUAR F5 ANTERIOR F6 SIGUIENTE F8 LOAD F9 SAVE X SALIR");
   IDR[1]=WRITE(FO2,0,10,0,"ENEMIGOS");
   IDR[2]=WRITE_INT(FO2,100,10,0,&TOENE);
   IDR[3]=WRITE(FO2,200,10,0,"ALIADOS");
   IDR[4]=WRITE_INT(FO2,300,10,0,&TOALI);
   IDR[5]=WRITE_INT(FO2,0,20,0,&PUNELE);
   IDR[6]=WRITE(FO2,300,20,0,"DEPOSITOS");
   IDR[7]=WRITE_INT(FO2,400,20,0,&TODEP);
   IDR[8]=WRITE(FO2,300,30,0,"BUNKER");
   IDR[9]=WRITE_INT(FO2,400,30,0,&TOBUN);

   CREANDO=1;
   LOOP
      IF(KEY(_D))
        LOOP
           I=GET_ID(TYPE DEPOSITO);
           IF(I)
             I.MUERTO=1;
             ELSE
             BREAK;
           END
        END
      END
      IF(KEY(_B))
        LOOP
           I=GET_ID(TYPE BUNKER);
           IF(I)
             I.MUERTO=1;
             ELSE
             BREAK;
           END
        END
      END

      X=MOUSE.X; Y=MOUSE.Y;
      IF(IDSELEC)               // SI TENEMOS SELECCIONADO CON Q Y W CAMB ACCIO
         IF(KEY(_Q))
            IDSELEC.ACCION--;
            IF(IDSELEC.ACCION<0) IDSELEC.ACCION=0; END
         END
         IF(KEY(_W))
            IDSELEC.ACCION++;
         END
         GRAPH=801;
         ELSE
         GRAPH=800;
      END
      ANGLE=ANGULOGLOBAL;
      IF(KEY(_F9))              // QUIERE GRABAR EL MAPA
         WHILE(SCAN_CODE<>0) FRAME; END
         SAVE("DAT\CHETE\NAVALMAP.MAP",&ELEMENTO,SIZEOF(ELEMENTO));
      END
      IF(KEY(_F8))              // QUIERE CARGAR EL MAPA
         WHILE(SCAN_CODE<>0) FRAME; END
         CARGAFASE();
      END
      IF(KEY(_X))
         WHILE(SCAN_CODE<>0) FRAME; END
         CREANDO=0;
         BREAK;
      END
      IF(KEY(_L_SHIFT) AND MOUSE.LEFT)  // PARA CREAR SI PULSA SHIFT Y RATON
         WHILE(SCAN_CODE<>0 OR MOUSE.LEFT)
            GRAPH=TIPGRAPH[PUNELE];
            FRAME;
         END
         GRAPH=801;
         IF(TIPGRAPH[PUNELE]>0)
            CREAELEMENTO(MOUSE.X,MOUSE.Y,PUNELE);
         END
      END
      IF(KEY(_L_SHIFT) AND MOUSE.RIGHT)          // BORRANDO
         WHILE(SCAN_CODE<>0 OR MOUSE.RIGHT)
            FRAME;
         END
         LOOP
            I=COLLISION(TYPE BARCO);
            IF(I) I.MUERTO=1; BREAK; END
            I=COLLISION(TYPE BUNKER);
            IF(I) I.MUERTO=1; BREAK; END
            I=COLLISION(TYPE DEPOSITO);
            IF(I) I.MUERTO=1; BREAK; END
            I=COLLISION(TYPE ISLA);
            IF(I) I.MUERTO=1; BREAK; END
            BREAK;
         END
      END

      IF(SCAN_CODE<>0)
         IF(KEY(_F6) AND PUNELE<TOPELE-1) PUNELE++; END
         IF(KEY(_F5) AND PUNELE>1)        PUNELE--; END
         WHILE(SCAN_CODE<>0) FRAME; END
      END
      IF(KEY(_C_PLUS))
         ANGULOGLOBAL+=5625;
         IF(ANGULOGLOBAL>360000) ANGULOGLOBAL=0; END
      END
      IF(KEY(_C_MINUS))
         ANGULOGLOBAL-=5625;
         IF(ANGULOGLOBAL<0) ANGULOGLOBAL=360000; END
      END
      IF(KEY(_L_SHIFT))
         WHILE(SCAN_CODE<>0) FRAME; END
         IF(TIPGRAPH[PUNELE]>0)
           G=TIPGRAPH[PUNELE];
           ELSE
           G=900;
         END
         VERGRAFICO(G);
      END
      IF(mouse.left) //************************     HA PINCHADO EL RATON
       I=COLLISION(TYPE BUNKER);
       I=COLLISION(TYPE DEPOSITO);
       IF(NOT I)
         I=COLLISION(TYPE ISLA);
       END
       IF(NOT I)
         I=COLLISION(TYPE BARCO);
       END
       IF(I)
         WHILE(MOUSE.LEFT)
           I.X=MOUSE.X+SCROLL[0].X0;
           I.Y=MOUSE.Y+SCROLL[0].Y0;
           FRAME;
         END
       END
       IF(NOT IDSELEC)                           // NO TENIA SELECCION.
         idselec=COLLISION(TYPE barco);      // PINCHO Y SELECCIONA AHORA.
         IF(IDSELEC AND NOT CREANDO) // SI NO ESTA MAPEANDO NO PUEDE ACCEDER A ENEMIGOS
            IF(NOT IDSELEC.BANDO)
              IDSELEC=0;
            END
         END
         IF(IDSELEC)
            IF(NVISOR) NVISOR.MUERTO=1; END  // MIRAMOS SI TENIA VISOR
            NVISOR=VISOR(X,Y,IDSELEC);
         END
         ELSE
         GRAPH=801;                              // SI TENIA SELECCION
         IDOBJET=COLLISION(TYPE barco);
         IF(IDOBJET)
            IF(IDOBJET.BANDO<>IDSELEC.BANDO)              // MIRA SI
               idselec.objetivo=idobjet;            // HA PINCHADO EN UN ENEMIGO
               IDSELEC.ACCION=2;                    // LO PONE EN ACCION ATACAR
               IDSELEC.PARADO=0;
               ELSE
               IF(IDSELEC<>IDOBJET);
                  IDSELEC.LIDER=IDOBJET;
                  IDSELEC.ACCION=4;
               END
            END
            ELSE
            IDOBJET=COLLISION(TYPE BUNKER);
            IF(NOT IDOBJET) IDOBJET=COLLISION(TYPE DEPOSITO); END
            IF(IDOBJET)
              IDSELEC.ACCION=2;
              IDSELEC.OBJETIVO=IDOBJET;
              IDSELEC.PARADO=0;
              ELSE                             // NO HA SELECCIONADO NADA
              IDSELEC.OBJETIVO=0;
              IDSELEC.LIDER=0;
              IDSELEC.XDEST=SCROLL[0].X0+x;
              IDSELEC.YDEST=SCROLL[0].Y0+y;
              IDSELEC.PARADO=0;
              IDSELEC.ACCION=1;
              IDSELEC.VELOCIDAD=2;
            END
         END
       END
      END
      IF(mouse.right)
         idselec=0;
         IF(NVISOR) NVISOR.MUERTO=1; END
      END
      FRAME;
   END
   FOR(I=0; I<10; I++) DELETE_TEXT(IDR[I]); END
   RATON(XPANRES/2,YPANRES/2);
   CREANDO=0;
END
PROCESS VERGRAFICO(GRAPH)               // ES UTILIZADO EN MAPEANDO
BEGIN
    X=50; Y=50;
    FRAME;
END
//------------------------------------------------------------------
PROCESS VERMAPA()
    PRIVATE I,G,T;
BEGIN
    GRAPH=970;
    X=XPANRES-200+100;
    Y=100;
    Z=95;
    WHILE(VIENDOMAPA)
        FOR(I=0; I<MAXELE; I++)
            IF(ELEMENTO[I].ACTIVO)
              T=ELEMENTO[I].TIPO;
              G=0;
              SWITCH(T)
                 CASE 1: G=972; END
                 CASE 2: G=971; END
                 CASE 3: G=972; END
                 CASE 4: G=971; END
                 CASE 5: G=972; END
                 CASE 6: G=971; END
              END
              IF(G>0 AND ELEMENTO[I].NID.VISIBLE)
                MOTA(X-100+(ELEMENTO[I].X/10),Y-100+(ELEMENTO[I].Y/10),G);
              END
            END
        END
        IF(MOUSE.LEFT AND COLLISION(TYPE MOUSE))
           WHILE(MOUSE.LEFT) FRAME; END
           IF(IDSELEC)
              IDSELEC.XDEST=(MOUSE.X-(X-100))*10;
              IDSELEC.YDEST=(MOUSE.Y-(Y-100))*10;
              IDSELEC.ACCION=1;
              IDSELEC.OBJETIVO=0;
              ELSE
              SCROLL[0].X0=((MOUSE.X-(X-100))*10)-(XPANRES/2);
              SCROLL[0].Y0=((MOUSE.Y-(Y-100))*10)-(YPANRES/2);
           END
        END
        FRAME;
    END
END
PROCESS MOTA(X,Y,GRAPH)
BEGIN
    Z=4;
    FRAME;
END

//---------------------------------------------------
PROCESS JUEGO()
PRIVATE IT[10]; I;
BEGIN
   ESTADOJUEGO=-1;
   DEFINE_REGION(1,0,0,2000,2000);
   START_SCROLL(0,file1,400,0,1,13);
   RESTORE_TYPE=PARTIAL_RESTORE;
   raton(XPANRES/2,YPANRES/2);
   unload_fpg(file1);
   file1=LOAD_FPG("USER\NAVAL\naval.fpg");
   TODEP=0;
   TOALI=0;
   TOENE=0;
   TOBUN=0;
   SWITCH(FASE)
     CASE 0: CARGAFASE();   // EN PRINCIPIO UNA SOLA FASE
             IT[0]=WRITE(FO2,0,  0,0,"BUENO LO QUE DEBES HACER ES SELECCIONAR");
             IT[1]=WRITE(FO2,0, 20,0,"A CUALQUIERA DE TUS BARCOS Y ORDENARLE ");
             IT[2]=WRITE(FO2,0, 40,0,"QUE SE MUEVA O QUE ATAQUE A OTRO BARCO,");
             IT[3]=WRITE(FO2,0, 60,0,"O A CUALQUIER OBJETIVO TERRESTRE.      ");
             IT[4]=WRITE(FO2,0, 80,0,"EL OBJETIVO DE ESTA MISION ES ACABAR   ");
             IT[5]=WRITE(FO2,0,100,0,"CON TODOS LOS DEPOSITOS DE COMBUSTIBLES");
             IT[6]=WRITE(FO2,0,120,0,"QUE SE ENCUENTRA EN LAS ISLAS DE ESTE  ");
             IT[7]=WRITE(FO2,0,140,0,"SECTOR. ");
             IT[7]=WRITE(FO2,0,180,0,"PULSE UNA TECLA.");
             WHILE(SCAN_CODE==0 AND NOT MOUSE.LEFT) FRAME; END
             FOR(I=0; I<9; I++) DELETE_TEXT(IT[I]); END

     END
   END
   JUGANDO=1;
   // PIE DE PANTALLA
   WRITE(0,0,YPANRES-10,0,"(M) MAPA ON/OFF (P) PAUSA");
   TIMER[0]=0;
   LOOP
      IF(TIMER[0]>3000) // TRANSCURRIO UN MEDIO MINUTO
         TIMER[0]=0;
         INTELIGENCIA(0);
      END
      IF(KEY(_wave))
            WHILE(SCAN_CODE<>0) FRAME; END
            IF(NOT CREANDO)
               CREANDO=1;
               MAPEANDO();
            END
      END
      IF(KEY(_M))
         WHILE(SCAN_CODE==_M) FRAME; END
         IF(VIENDOMAPA) VIENDOMAPA=0;
            ELSE
                        VIENDOMAPA=1;
                        VERMAPA();
         END

      END
      IF(KEY(_P))
         WHILE(SCAN_CODE<>0) FRAME; END
         JUEGOENPAUSA();
      END
      IF(KEY(_ESC)) BREAK; END
      IF(MOUSE.X<10)
          IF( SCROLL[0].X0>0+VS) SCROLL[0].X0-=VS; END
      END
      IF(MOUSE.X>XPANRES-15)
        IF(SCROLL[0].X0<2000-VS) SCROLL[0].X0+=VS; END
      END
      IF(MOUSE.Y<10)
         IF(SCROLL[0].Y0>0+VS)   SCROLL[0].Y0-=VS; END
      END
      IF(MOUSE.Y>YPANRES-15)
        IF(SCROLL[0].Y0<2000-VS) SCROLL[0].Y0+=VS; END
      END
      IF(SALIDA>0) BREAK; END
      FRAME;
   END
   STOP_SCROLL(0);
   SIGNAL(TYPE BARCO,S_KILL);
   SIGNAL(TYPE TORRETA,S_KILL);
   SIGNAL(TYPE BUNKER,S_KILL);
   SIGNAL(TYPE DISPARO,S_KILL);
   SIGNAL(TYPE RATON,S_KILL);
   SIGNAL(TYPE VISOR,S_KILL);
   SIGNAL(TYPE ISLA,S_KILL);
   SIGNAL(TYPE MAPEANDO,S_KILL);
   SIGNAL(TYPE HUMO,S_KILL);
   SWITCH(SALIDA)
     CASE 1:
      WRITE(0,0, 0,0,"MISION TERMINADA.");
      WRITE(0,0,20,0,"SI QUIERES ALGUNA OTRA MISION");
      WRITE(0,0,30,0,"MANDAME UN E-MAIL :");
      WRITE(0,0,40,0,"rnates@jet.es");
      WRITE(0,0,50,0,"ESTA ERA MUY FACILONA, NO ?");
      WHILE(SCAN_CODE==0) FRAME; END
     END
     CASE 2:
      WRITE(FO2,0, 0,0,"FALLASTES........");
      WHILE(SCAN_CODE==0) FRAME; END
     END
   END
   DELETE_TEXT(ALL_TEXT);
   UNLOAD_FNT(FO3);
   ESTADOJUEGO=2;
END



PROCESS RATON(X,Y)
BEGIN
   graph=800;
   mouse.x=x;
   mouse.y=y;
   Z=2;
   LOOP
      x=mouse.x;
      y=mouse.y;
      IF(mouse.left) //************************     HA PINCHADO EL RATON
       SOUND(SO4,100,512);
       IF(NOT IDSELEC)                           // NO TENIA SELECCION.
         idselec=COLLISION(TYPE barco);      // PINCHO Y SELECCIONA AHORA.
         IF(IDSELEC AND NOT CREANDO) // SI NO ESTA MAPEANDO NO PUEDE ACCEDER A ENEMIGOS
            IF(NOT IDSELEC.BANDO)
              IDSELEC=0;
            END
         END
         IF(IDSELEC)
            IF(NVISOR) NVISOR.MUERTO=1; END  // MIRAMOS SI TENIA VISOR
            NVISOR=VISOR(X,Y,IDSELEC);
         END
         ELSE
         GRAPH=801;                              // SI TENIA SELECCION
         IDOBJET=COLLISION(TYPE barco);
         IF(IDOBJET)
            IF(IDOBJET.BANDO<>IDSELEC.BANDO)              // MIRA SI
               idselec.objetivo=idobjet;            // HA PINCHADO EN UN ENEMIGO
               IDSELEC.ACCION=2;                    // LO PONE EN ACCION ATACAR
               IDSELEC.PARADO=0;
               ELSE
               IF(IDSELEC<>IDOBJET);
                  IDSELEC.LIDER=IDOBJET;
                  IDSELEC.ACCION=4;
               END
            END
            ELSE
            IDOBJET=COLLISION(TYPE BUNKER);
            IF(NOT IDOBJET) IDOBJET=COLLISION(TYPE DEPOSITO); END
            IF(IDOBJET)
              IDSELEC.ACCION=2;
              IDSELEC.OBJETIVO=IDOBJET;
              IDSELEC.PARADO=0;
              ELSE                             // NO HA SELECCIONADO NADA
              IDSELEC.OBJETIVO=0;
              IDSELEC.LIDER=0;
              IDSELEC.XDEST=SCROLL[0].X0+x;
              IDSELEC.YDEST=SCROLL[0].Y0+y;
              IDSELEC.PARADO=0;
              IDSELEC.ACCION=1;
              IDSELEC.VELOCIDAD=2;
            END
         END
       END
      END
      IF(mouse.right)
         idselec=0;
         IF(NVISOR) NVISOR.MUERTO=1; END
      END
      IF(IDSELEC)
         GRAPH=801;
         ELSE
         GRAPH=800;
      END
      FRAME;
   END
END

PROCESS ISLA(X,Y,GRAPH,NUMERO)
    PRIVATE I;
BEGIN
    CTYPE=C_SCROLL;
    Z=13;
    ALTO=GRAPHIC_INFO(FILE1,GRAPH,G_HEIGHT);
    ANCHO=GRAPHIC_INFO(FILE1,GRAPH,G_WIDE);
    ELEMENTO[NUMERO].ANCHO=ANCHO;
    ELEMENTO[NUMERO].ALTO=ALTO;
    LOOP
       ELEMENTO[NUMERO].X=X;
       ELEMENTO[NUMERO].Y=Y;
       IF(MUERTO) BREAK; END
       FRAME;
    END
    ELEMENTO[NUMERO].ACTIVO=0;
    ELEMENTO[NUMERO].X=0;
    ELEMENTO[NUMERO].Y=0;
END

PROCESS DEPOSITO(X,Y,GRAPH,NUMERO)
    PRIVATE I;
BEGIN
    ENERGIA=15;
    CTYPE=C_SCROLL;
    Z=12;
    ALTO=GRAPHIC_INFO(FILE1,GRAPH,G_HEIGHT);
    ANCHO=GRAPHIC_INFO(FILE1,GRAPH,G_WIDE);
    ELEMENTO[NUMERO].ANCHO=ANCHO;
    ELEMENTO[NUMERO].ALTO=ALTO;
    TODEP++;
    LOOP
       ELEMENTO[NUMERO].X=X;
       ELEMENTO[NUMERO].Y=Y;
       IF(MUERTO) BREAK; END
       FRAME;
    END
    IF(NOT CREANDO)
       HUMO();
       TIENEHUMO=1;
       WHILE(TIENEHUMO)
         FRAME;
       END
    END
    ELEMENTO[NUMERO].ACTIVO=0;
    ELEMENTO[NUMERO].X=0;
    ELEMENTO[NUMERO].Y=0;
    TODEP--;
    IF(TODEP<=1) SALIDA=1; END
    //-------------------------- QUITAR A TODOS LOS QUE APUNTEN AQUI
             LOOP                   // QUITO A TODOS ESTE OBJETIVO
               I=GET_ID(TYPE BARCO);
               IF(I==0) BREAK; END
               IF(I.OBJETIVO==ID) I.OBJETIVO=0; I.ACCION=3; END
             END
             LOOP                   // QUITO A TODAS LAS TORRETAS ESTE OBJETIVO
               I=GET_ID(TYPE TORRETA);
               IF(I==0) BREAK; END
               IF(I.OBJETIVO==ID ) I.OBJETIVO=0; I.ACCION=3; END
             END

    EXPLOSION4(X,Y);
END

PROCESS HUMO()
    PRIVATE I=0, T=150;
BEGIN
    CTYPE=C_SCROLL;
    GRAPH=100;
    X=FATHER.X;
    Y=FATHER.Y-16;
    LOOP
      GRAPH=I+100;
      I++;
      IF (I>4) I=0; END
      T--;
      IF(T<=0) BREAK; END
      FRAME(400);
    END
    FATHER.TIENEHUMO=0;
END

PROCESS HUELLA()
BEGIN
        CTYPE=C_SCROLL;
        Z=13;
        GRAPH=110;
        X=FATHER.X;
        Y=FATHER.Y;
        ANGLE=FATHER.ANGLE;
        FRAME;
END



PROCESS BARCO(X,Y,T,BANDERA,ACCION,NUMELE)
PRIVATE
        I; A; B; G; GR, VUELTA=28;  //NUDOS
        EX; EY;
        XO; YO; XD; YD;
        PX[4]; PY[4];
        ADEST,BDEST;
BEGIN
        ctype=c_scroll;
        Z=12;
        NUMERO=NUMELE;
        BANDO=BANDERA;
        IF(BANDO==0) GRAPH=250; ELSE GRAPH=200; END
        X_RESOL=X*1000;
        Y_RESOL=Y*1000;
        ENERGIA=100;
        XANT=X;
        YANT=Y;
        XDEST=X;
        YDEST=Y;
        ANGULOANT=ANGLE;
        OBJETIVO=0;
        VELOCIDAD  =ELEMENTO[NUMERO].VELOCIDAD;
        XDEST      =ELEMENTO[NUMERO].XDEST;
        YDEST      =ELEMENTO[NUMERO].YDEST;
        PARADO     =ELEMENTO[NUMERO].PARADO;
        NGULODEST  =ELEMENTO[NUMERO].NGULODEST;
        ANGLE      =ELEMENTO[NUMERO].ANGLE;
        TIPO       =ELEMENTO[NUMERO].TIPO;
        GRAPH=200+T;  // ESTO SE HACE PARA COGER LOS PUNTOS DE CONTROL
        FOR(I=0; I<T; I++)
           GET_REAL_POINT(I+1,&A,&B);
           TORRETA(A,B,I);
        END
        IF(BANDO)
           GRAPH=200+T;
           TOALI++;
           VISIBLE=1;
           ELSE
           GRAPH=250+T; // A PARTIR DEL 250 ENEMIGOS
           TOENE++;
           VISIBLE=0;
           ENERGIA=50;  // LA MITAD DE ENERGIA PARA LOS ENEMIGOS
        END
        GR=GRAPH;
        ALTO=GRAPHIC_INFO(FILE1,GRAPH,G_HEIGHT);
        ANCHO=GRAPHIC_INFO(FILE1,GRAPH,G_WIDE);
        LOOP
          IF(MUERTO)
             ELEMENTO[NUMERO].ACTIVO=0;
             ELEMENTO[NUMERO].X=0;
             ELEMENTO[NUMERO].Y=0;
             ELEMENTO[NUMERO].TIPO=0;
             ELEMENTO[NUMERO].PARADO=1;
             ELEMENTO[NUMERO].NID=0;
             ELEMENTO[NUMERO].ANGLE=0;
             ELEMENTO[NUMERO].NGULODEST=0;
             ELEMENTO[NUMERO].ACCION=0;
             ELEMENTO[NUMERO].YDEST=0;
             ELEMENTO[NUMERO].YDEST=0;
             EXPLOSION4(X,Y);
             LOOP                   // QUITO A TODOS ESTE OBJETIVO
               I=GET_ID(TYPE BARCO);
               IF(I==0) BREAK; END
               IF(I.OBJETIVO==ID) I.OBJETIVO=0; I.ACCION=3; END
             END
             LOOP
               I=GET_ID(TYPE TORRETA);  // MATAMOS LAS TORRETAS DEL MUERTO
               IF(I==0) BREAK; END
               IF(I.FATHER==ID) SIGNAL(I,S_KILL); END
             END
             LOOP                   // QUITO A TODAS LAS TORRETAS ESTE OBJETIVO
               I=GET_ID(TYPE TORRETA);
               IF(I==0) BREAK; END
               IF(I.OBJETIVO==ID ) I.OBJETIVO=0; I.ACCION=3; END
             END
             LOOP                   // QUITO A TODOS ESTE OBJETIVO
               I=GET_ID(TYPE BUNKER);
               IF(I==0) BREAK; END
               IF(I.OBJETIVO==ID) I.OBJETIVO=0; END
             END
             IF(BANDO)
                TOALI--;
                IF(TOALI<=0) SALIDA=2; END
                ELSE
                TOENE--;
                INTELIGENCIA(2);  // LLAMA A INTELIGENCIA PARA CREAR OTRO ENEMIGO
             END
             BREAK;
          END
          IF(VISIBLE)
            GRAPH=GR;
            FOR(I=0; I<T; I++)            // ACTUALIZO EN LA TABLA LOS PUNTOS DE
               GET_REAL_POINT(I+1,&A,&B); // CONTROL ESTOS SERAN CONSULTADOS POR
               PCOX[I]=A; PCOY[I]=B;      // LAS TORRETAS.
            END
            ELSE
            GRAPH=0;
            IF(CREANDO)                   // ESTO CUANDO MAPEAMOS, LO PODEMOS
               GRAPH=GR;                  // QUITAR PARA GANAR TIEMPO
            END
          END
//
//-----------------------------------------------------------------------------
// NUEVA PARTE PARA COMPROBAR LO DEL CHOQUE
IF(NOT PARADO)
          // PONEMOS RASTRO
          IF(VISIBLE) HUELLA(); END
          // FUERA DE COORDENADAS
          IF(X<0 OR X>2000 OR Y<0 OR Y>2000)
              ACCION=1;
              XDEST=500+RAND(0,1000);
              YDEST=500+RAND(0,1000);
          END
          VUELTA-=(ENERGIA/10);
          IF(VUELTA<=0);
             ANGLE=NEAR_ANGLE(ANGLE,NGULODEST,VGIRO);
             INCX=get_distx(ANGLE,2000);
             INCY=get_disty(ANGLE,2000);
             I=0;
             VUELTA=28-VELOCIDAD;
             X_RESOL+=INCX;
             Y_RESOL+=INCY;
             //---------------------------------- MIRAMOS SI TOCA
             G=0;
             FOR(I=0; I<MAXELE; I++)
                IF(I<>NUMERO AND ELEMENTO[I].ACTIVO)
                  A=(ELEMENTO[I].ANCHO/2);
                  B=(ELEMENTO[I].ALTO/2);
                  EX=ELEMENTO[I].X;
                  EY=ELEMENTO[I].Y;
                  G=0;
                  XO=(X_RESOL+(INCX*25) )/1000;
                  YO=(Y_RESOL+(INCY*25) )/1000;
                  IF( XO>EX-A AND XO<EX+A AND YO>EY-B AND YO<EY+B)
                    G=1;
                    BREAK;
                    ELSE
                    G=0;
                  END
                END
             END
             IF(G)                     // AQUI ESTARIA TODO EL TOMATE PARA
               X_RESOL=XANT*1000;      // ESQUIVAR, PERO RECONOZCO QUE NO
               Y_RESOL=YANT*1000;      // ME HE COMIDO MUCHO EL COCO,
               ANGLE=ANGULOANT;        // NO SON MUY INTELIGENTES QUE DIGAMOS
               ANGLE=NEAR_ANGLE(ANGLE,NGULODEST,VGIRO);
               ACCION=5;    // CORRIGIENDO RUMBO
               ADEST=X+RAND(-200,200);
               BDEST=Y+RAND(-200,200);
               NGULODEST=FGET_ANGLE(X,Y,ADEST,BDEST);
               ANGLE=NEAR_ANGLE(ANGLE,NGULODEST,VGIRO);
               ELSE                  //--------------------- CONTRARIO
               SWITCH(ACCION)
                     // SIN ACCION
                     CASE 0: PARADO=1;
                     END
                     // MOVIENDOSE
                     CASE 1:
                           PARADO=0;
                           NGULODEST=FGET_ANGLE(X,Y,XDEST,YDEST);
                           IF (abs(xdest-x)<ALCANCE/4 AND abs(ydest-y)<ALCANCE/4)
                              PARADO=1;
                              ACCION=3;
                           END
                     END
                     CASE 2:
                           PARADO=0;
                           NGULODEST=GET_ANGLE(OBJETIVO);
                           XDEST=OBJETIVO.X;
                           YDEST=OBJETIVO.Y;
                           IF (GET_DIST(OBJETIVO)<ALCANCE)
                              PARADO=1;
                           END
                     END
                     CASE 3:
                           PARADO=1;
                           //
                     END
                     CASE 4:
                           PARADO=0;
                           NGULODEST=GET_ANGLE(LIDER);
                           XDEST=LIDER.X;
                           YDEST=LIDER.Y;
                           IF (GET_DIST(LIDER)<ALCANCE)
                              PARADO=1;
                              IF(COLLISION(TYPE BARCO))
                                 PARADO=0;
                                 XDEST+=RAND(-100,100)*1;
                                 YDEST+=RAND(-100,100)*1;
                              END
                              ELSE
                              PARADO=0;
                           END
                     END
                     CASE 5: // CORRIGIENDO RUMBO

                           PARADO=0;
                           NGULODEST=FGET_ANGLE(X,Y,ADEST,BDEST);
                           IF (abs(adest-x)<ALCANCE/8 AND abs(bdest-y)<ALCANCE/8)
                              ACCION=1;
                           END

                     END
               END
             END
             X=X_RESOL/1000;
             Y=Y_RESOL/1000;
             XANT=X;
             YANT=Y;
             ANGULOANT=ANGLE;

          END
END

//----------------------------------------------------------------------------
          ELEMENTO[NUMERO].X         =X;
          ELEMENTO[NUMERO].Y         =Y;
          ELEMENTO[NUMERO].XDEST     =XDEST;
          ELEMENTO[NUMERO].YDEST     =YDEST;
          ELEMENTO[NUMERO].VELOCIDAD =VELOCIDAD;
          ELEMENTO[NUMERO].OBJETIVO  =OBJETIVO;
          ELEMENTO[NUMERO].LIDER     =LIDER;
          ELEMENTO[NUMERO].ACCION    =ACCION;
          ELEMENTO[NUMERO].PARADO    =PARADO;
          ELEMENTO[NUMERO].NGULODEST =NGULODEST;
          ELEMENTO[NUMERO].ANGLE     =ANGLE;
          ELEMENTO[NUMERO].ANCHO=ANCHO;
          ELEMENTO[NUMERO].ALTO=ALTO;
          FRAME;
        END

END


//---------------------------------------------------

//---------------------------------------------------
PROCESS TORRETA(X,Y,P)
PRIVATE
   A; I=0, GR=0;
BEGIN
   CTYPE=C_SCROLL;
   Z=11;
   PTOCONTROL=P;
   GRAPH=500;
   ALCANCE=100;
   RECARGO=RAND(200,300);
   ENERGIA=100;
   PRECISION=20;
   CALIBRE=FATHER.CALIBRE;
   BANDO=FATHER.BANDO;
   GR=GRAPH;
   LOOP
      IF(FATHER.VISIBLE)
         GRAPH=GR;
         X=FATHER.PCOX[PTOCONTROL]; //SE SITUA EN EL PUNTO DE CONTROL
         Y=FATHER.PCOY[PTOCONTROL];
         ELSE
         GRAPH=0;
      END
      ALCANCE=100+FATHER.ENERGIA;
      IF(ENERGIA<=0) BREAK; END
      IF(FATHER.OBJETIVO) OBJETIVO=FATHER.OBJETIVO; END
      SWITCH(FATHER.ACCION)
         CASE 0: ANGLE=FATHER.ANGLE;
         END
         CASE 1: ANGLE=FATHER.ANGLE;
         END
         CASE 2: OBJETIVO=FATHER.OBJETIVO;
         END
         CASE 3: IF(NOT OBJETIVO)   // LAS TORRETAS SIEMPRE EN ALERTA
                    // MIRAR SI HAY ALGUNO CERCA.
                    LOOP
                       I=GET_ID(TYPE BARCO);
                       IF(NOT I); BREAK;  END
                       IF (I.BANDO<>BANDO)
                          IF(ABS(X-I.X)<ALCANCE*2 AND ABS(Y-I.Y<ALCANCE*2) )
                            OBJETIVO=I;
                            I.VISIBLE=1;
                            FATHER.VISIBLE=1;
                            BREAK;
                          END
                       END
                    END
                 END
         END
      END
      IF(OBJETIVO)
        RECARGO--;
        ANGLE=GET_ANGLE(OBJETIVO);
        A=GET_DIST(OBJETIVO);
        IF(A<ALCANCE*2) // DISPARAMOS SI ESTA RECARGADA
          IF(RECARGO<0)
             IF(FATHER.MUNICION>0)
                RECARGO=300-FATHER.ENERGIA;
                DISPARO(X,Y,ANGLE);
                FATHER.MUNICION--;
             END
          END
          ELSE
          IF(FATHER.ACCION==2) FATHER.PARADO=0; END
          IF(OBJETIVO<>FATHER.OBJETIVO)
             OBJETIVO=0;  // ERA UN OBJETIVO ELEJIDO POR LA TORRETA.
          END
        END
        ELSE
        ANGLE=FATHER.ANGLE;
      END
      IF(FATHER.MUERTO) BREAK; END
      FRAME;
   END

END

//---------------------------------------------------
PROCESS BUNKER(X,Y,T)
PRIVATE
   A; I=0;
BEGIN
   CTYPE=C_SCROLL;
   Z=12;
   GRAPH=510;
   ALTO=GRAPHIC_INFO(FILE1,GRAPH,G_HEIGHT);
   ANCHO=GRAPHIC_INFO(FILE1,GRAPH,G_WIDE);
   ELEMENTO[NUMERO].ANCHO=ANCHO;
   ELEMENTO[NUMERO].ALTO=ALTO;
   ANGLE=ELEMENTO[NUMERO].ANGLE;
   Z=11;
   NUMERO=T;
   ALCANCE=100;
   RECARGO=RAND(200,300);
   ENERGIA=10;
   PRECISION=20;
   CALIBRE=2;
   TOBUN++;
   LOOP
      ELEMENTO[NUMERO].X=X;
      ELEMENTO[NUMERO].Y=Y;
      IF(ENERGIA<=0 OR MUERTO)
         EXPLOSION(X,Y);
         ELEMENTO[NUMERO].ACTIVO=0;
         ELEMENTO[NUMERO].X=0;
         ELEMENTO[NUMERO].Y=0;
         ELEMENTO[NUMERO].TIPO=0;
         ELEMENTO[NUMERO].NID=0;
         ELEMENTO[NUMERO].ANGLE=0;
         BREAK;
      END
      IF(NOT OBJETIVO)   // LAS TORRETAS SIEMPRE EN ALERTA
         LOOP
           I=GET_ID(TYPE BARCO);
           IF(NOT I); BREAK;  END
           IF (I.BANDO==ALIADO)
              IF( ABS(X-I.X)<ALCANCE*2 AND ABS(Y-I.Y<ALCANCE*2))
                OBJETIVO=I;
                BREAK;
              END
            END
         END
      END

      IF(OBJETIVO)
        RECARGO--;
        ANGLE=GET_ANGLE(OBJETIVO);
        A=GET_DIST(OBJETIVO);
        IF(A<ALCANCE*2) // DISPARAMOS SI ESTA RECARGADA
          IF(RECARGO<0)
             RECARGO=150-ENERGIA;
             DISPARO(X,Y,ANGLE);
          END
          ELSE
          OBJETIVO=0;
        END
      END
      ELEMENTO[NUMERO].OBJETIVO  =OBJETIVO;
      ELEMENTO[NUMERO].ANGLE     =ANGLE;
      FRAME;
   END
   LOOP                   // QUITO A TODOS ESTE OBJETIVO
    I=GET_ID(TYPE BARCO);
    IF(I==0) BREAK; END
    IF(I.OBJETIVO==ID) I.OBJETIVO=0; I.ACCION=3; END
   END
   LOOP                   // QUITO A TODOS ESTE OBJETIVO
    I=GET_ID(TYPE TORRETA);
    IF(I==0) BREAK; END
    IF(I.OBJETIVO==ID) I.OBJETIVO=0; I.ACCION=3; END
   END

   TOBUN--;
END

//---------------------------------------------------
PROCESS DISPARO(X,Y,ANGLE)
   PRIVATE IDTOCO,I,A,B;
BEGIN
   ctype=c_scroll;
   Z=10;
   GRAPH=150;
   ALCANCE=GET_DIST(FATHER.OBJETIVO)+RAND(-FATHER.PRECISION,FATHER.PRECISION);
   X_RESOL=X*1000;
   Y_RESOL=Y*1000;
   ANGLE+=RAND(-FATHER.PRECISION,FATHER.PRECISION);
   IF(X>SCROLL[0].X0 AND SCROLL[0].X0<SCROLL[0].X0+XPANRES
      AND Y>SCROLL[0].Y0 AND SCROLL[0].Y0<SCROLL[0].Y0+YPANRES)
      SOUND(SO5,256,512);
      ELSE
      SOUND(SO5,128,512);
   END
   LOOP
      ALCANCE-=4;
      IF(ALCANCE<=0)
         IDTOCO=COLLISION(TYPE ISLA);
         IF(NOT IDTOCO)
            ALAGUA(X,Y); // CAE EN EL AGUA
            ELSE
                                 // CAE EN TIERRA PASAMOS EL ID DE LA ISLA
                                 // HUELLA EN SU GRAFICO CON MAP_PUT
            EXPLOSION(X,Y);
            I=RAND(725,727);
            A=X- (IDTOCO.X-(IDTOCO.ANCHO/2));  // ZONA DEL GRAFICO A MANCHAR
            B=Y- (IDTOCO.Y-(IDTOCO.ALTO /2));
            MAP_PUT(FILE1,IDTOCO.GRAPH,I,A,B);
         END
         BREAK;
      END
      IDTOCO=0;
      IF(ALCANCE<20) IDTOCO=COLLISION(TYPE BARCO); END
      IF(IDTOCO)
        IDTOCO.ENERGIA-=FATHER.CALIBRE;
        EXPLOSION(X,Y);
        IF(IDTOCO.ENERGIA<=0)     // MUERE UN ENEMIGO
           IDTOCO.MUERTO=1;
        END
        IDTOCO=COLLISION(TYPE TORRETA);   // MIRO SI TOCA ALGUNA TORRETA
        IF(IDTOCO)
           IDTOCO.ENERGIA-=(FATHER.CALIBRE*2);
           EXPLOSION(X,Y);
           IF(IDTOCO.ENERGIA<=0)     // MUERE UN ENEMIGO
              IDTOCO.MUERTO=1;
           END
        END
        BREAK;
        ELSE                // NO TOCO EN NINGUN BARCO MIRA EN FIJOS
        IF(ALCANCE<20)
           IDTOCO=COLLISION(TYPE BUNKER);
           IF(IDTOCO)
              IDTOCO.ENERGIA-=(FATHER.CALIBRE*2);
              EXPLOSION(X,Y);
              IF(IDTOCO.ENERGIA<=0)     // MUERE UN BUNKER
                 IDTOCO.MUERTO=1;
              END
              BREAK;
              ELSE
              //------------------------------ MIRA SI TOCA EN DEPOSITO
              IDTOCO=COLLISION(TYPE DEPOSITO);
              IF(IDTOCO)
                 IDTOCO.ENERGIA-=(FATHER.CALIBRE*2);
                 EXPLOSION5(X,Y);
                 IF(IDTOCO.ENERGIA<=0)     // MUERE UN DEPOSITO
                    IDTOCO.MUERTO=1;
                 END
                 BREAK;
              END
           END
        END
      END
      X_RESOL+=GET_DISTX(ANGLE,4000);
      Y_RESOL+=GET_DISTY(ANGLE,4000);
      X=X_RESOL/1000;
      Y=Y_RESOL/1000;
      FRAME;
   END
END


PROCESS EXPLOSION(X,Y)
BEGIN
  CTYPE=C_SCROLL;
  IF(X>SCROLL[0].X0 AND SCROLL[0].X0<SCROLL[0].X0+XPANRES
     AND Y>SCROLL[0].Y0 AND SCROLL[0].Y0<SCROLL[0].Y0+YPANRES)
     SOUND(SO2,100,512);
  END
  FOR(GRAPH=4; GRAPH<=7; GRAPH++)
      FRAME;
  END
END

PROCESS EXPLOSION4(X,Y)
BEGIN
  CTYPE=C_SCROLL;
  SOUND(SO2,150,512);
  FOR(GRAPH=24; GRAPH<=29; GRAPH++)
      FRAME;
  END
END

PROCESS EXPLOSION5(X,Y)
BEGIN
  CTYPE=C_SCROLL;
  SOUND(SO2,150,512);
  FOR(GRAPH=951; GRAPH<=959; GRAPH++)
      FRAME;
  END
END

PROCESS ALAGUA(X,Y)
BEGIN
  CTYPE=C_SCROLL;
  IF(X>SCROLL[0].X0 AND SCROLL[0].X0<SCROLL[0].X0+XPANRES
     AND Y>SCROLL[0].Y0 AND SCROLL[0].Y0<SCROLL[0].Y0+YPANRES)
     SOUND(SO3,100,512);
  END
  FOR(GRAPH=14; GRAPH<=17; GRAPH++)
      FRAME;
  END
END

//------------------------------------------------------------------------------
PROCESS VISOR(X,Y,PADRE)
     PRIVATE P; I1; I2; I3; I4; I5; I6; A; B;
BEGIN
     CTYPE=C_SCROLL;
     GRAPH=810;
     MUERTO=0;
     P=PADRE;
     I1=WRITE(FO2,0,0,0,"ENERGIA");
     I2=WRITE_INT(FO2,0,0,0,&IDSELEC.ENERGIA);
     I3=WRITE(FO2,0,0,0,"MUNICION");
     I4=WRITE_INT(FO2,0,0,0,&IDSELEC.MUNICION);
     I5=WRITE(FO2,0,0,0,"ACCION");
     I6=WRITE_INT(FO2,0,0,0,&IDSELEC.ACCION);
     LOOP
        SOUND(SO1,100,512);
        X=PADRE.X; Y=PADRE.Y;
        A=(X-SCROLL[0].X0)-50;
        B=(Y-SCROLL[0].Y0)-50;
        MOVE_TEXT(I1,A,B);
        MOVE_TEXT(I2,A+75,B);
        MOVE_TEXT(I3,A,B+10);
        MOVE_TEXT(I4,A+75,B+10);
        MOVE_TEXT(I5,A,B+20);
        MOVE_TEXT(I6,A+75,B+20);
        IF(MUERTO OR FATHER.MUERTO) BREAK; END
        FRAME;
     END
     DELETE_TEXT(I1);
     DELETE_TEXT(I2);
     DELETE_TEXT(I3);
     DELETE_TEXT(I4);
     DELETE_TEXT(I5); DELETE_TEXT(I6);
     STOP_SOUND(SO1);
END
//---------------------------------------------------

PROCESS INTELIGENCIA(MODO)
PRIVATE O,I,MENOR=1000,IDR=0,ESTE=0;
BEGIN
    IF(CREANDO) MODO=-1; END
    SWITCH(MODO)
      CASE 0:
           LOOP
              I=GET_ID(TYPE BARCO);        //COJO EL QUE MENOS ENERGIA TENGA
              IF(I==0) BREAK; END          //SI HAY ALGUNO QUE ESTE ATACANDO
              IF(I.BANDO)                  //PONGO COMO OBJETIVO A ESE
                IF(I.OBJETIVO) IDR=I; BREAK; END
                IF(I.ENERGIA<MENOR) MENOR=I.ENERGIA; IDR=I; END
                ELSE
                IF(I.ACCION==3)
                  ESTE=I;                  //COGO CUALQUIER BARCO ENEMIGO
                END                        //PARA DARLE EL OBJETIVO
              END
           END
           IF(IDR AND ESTE)
             ESTE.ACCION=2;
             ESTE.OBJETIVO=IDR;
             ESTE.PARADO=0;
             ESTE.VISIBLE=1;
           END
      END
      CASE 2: // CREA UN ENEMIGO EN LA ESQUINA INFERIOR IZQUIERDA
           ESTE=CREAELEMENTO(RAND(1800,1950),RAND(100,250),FATHER.TIPO);
           IF(ESTE)
              IF(FATHER.OBJETIVO)
                 ESTE.OBJETIVO=FATHER.OBJETIVO;
                 ESTE.ACCION=2;
                 ESTE.VISIBLE=1;
                 ELSE
                 ESTE.ACCION=3;
                 LOOP
                  I=GET_ID(TYPE BARCO);        //COJO EL QUE MENOS ENERGIA TENGA
                  IF(I==0) BREAK; END          //SI HAY ALGUNO QUE ESTE ATACANDO
                  IF(I.BANDO)                  //PONGO COMO OBJETIVO A ESE
                     ESTE.OBJETIVO=I;
                     ESTE.ACCION=2;
                     ESTE.VISIBLE=1;
                     ESTE.PARADO=0;
                     BREAK;
                  END
                 END
              END
           END

      END
    END
END


