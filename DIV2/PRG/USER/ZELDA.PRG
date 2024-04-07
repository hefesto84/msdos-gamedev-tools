/////////////////////////////////////////
//
//   POR FAVOR NO DISTRIBUYAS AUN EL CODIGO
//
// YA QUE EL PROGRAMA AUN NO ESTA TERMINADO
//
//  Y PIENSO METERLE COMENTARIOS TIPO L6
//
//                  нннн HASTA PRONTO !!!!
//
/////////////////////////////////////////

PROGRAM ZELDA;
CONST
 LEEME="C:\PROGRAM\DIV\LEEME.TXT";

GLOBAL

    POS_X,POS_Y;
    X_CANGREJO ;Y_CANGREJO;
    X_CANGREJO1;Y_CANGREJO1;
    X_PULPO    ;Y_PULPO;
    X_PULPO1   ;Y_PULPO1;
    X_PULPO2   ;Y_PULPO2;
    X_MALO1    ;Y_MALO1;
    X_MALO2    ;Y_MALO2;
    X_MALO3    ;Y_MALO3;
    X_MALO4    ;Y_MALO4;
    X_MALO5    ;Y_MALO5;
    X_GENERAL  ;Y_GENERAL;

    N_MALOS=10;
    CONT=0;C=1;
    SONO=0;
    ESPERA;
    FICHERO;
    FUENTE;
    FUENTE2;
    T_ESPADA=0;
    ESPADA=0;
    TIEMPO=0;
    SALIR;
    s_BUHO;
    S_ESPADA;
    S_INTRO;
    S_PERRO;
    S_MUERE;
    S_TETOCO;
    S_ENCONTRAR_ESPADA;

BEGIN
    SET_MODE(m320x240);
    S_BUHO=LOAD_PCM("USER\zelda\PAJARO10.PCM",0);
    S_PERRO=LOAD_PCM("USER\zelda\PERRO.PCM",0);
    S_ESPADA=LOAD_PCM("USER\zelda\FLECHA.PCM",0);
    S_MUERE=LOAD_PCM("USER\zelda\PORTAZO.PCM",0);
    S_TETOCO=LOAD_PCM("USER\zelda\QUEJIDO0.PCM",0);
    S_ENCONTRAR_ESPADA=LOAD_PCM("USER\zelda\MUSICA9.PCM",0);

//    S_INTRO=load_pcm("pcm\intro.pcm",0);
//    sound(S_INTRO,100,260);

    load_pal("USER\zelda\zelda.fnt");
    FUENTE=LOAD_FNT("USER\zelda\ZELDA.fnt");
    FUENTE2=LOAD_FNT("USER\zelda\ZELDA2.FNT");
    FICHERO=LOAD_FPG("USER\zelda\ZELDA.FPG");
    clear_screen();
    fade_off();
    fade_on();
    put_screen(fichero,500);
    cont = 0;
    repeat
      if (cont>8)
        if(cont==13)
          cont=0;
        end
        delete_text(all_text);
      else
        write(fuente,62,217,0,"Pulsa espacio para continuar");
      end
      cont++;
      frame;
    until(key(_space));
    STOP_SOUND(1);
    delete_text(all_text);
    fade_off();
    FADE_ON();
    LOAD_PAL("USER\zelda\ZELDA.FPG");
    START_SCROLL(0,FICHERO,1,0,0,0);
    EXCALIBUR();

  // PONER ENEMIGOS //

    CANGREJO (888,708,94);
    CANGREJO1(116,716,94);
    PULPO    (800,710,86);
    PULPO1   (349,704,86);
    PULPO2   (570,725,82);


    PRINCESA(640,225,110);
    PERRO(940,120,66);
    GALLINA(770,70,70);
    VIDA();
    MOVER_ZELDA();

END

PROCESS MOVER_ZELDA();
PRIVATE
//
// 1=ABAJO // 2=ARRIBA // 3=IZQ // 4=DER //
//

 DIRECCION=1;
 DIRECCION_PARADO=1;
 C_ABAJO=2,C_ARRIBA=9,C_IZQ=14,C_DER=20;


BEGIN

    CTYPE=C_SCROLL;
    SCROLL.CAMERA=ID;
    GRAPH=2;
    X=972;
    Y=115;
    Z=-5;
    ESPERA=1;
    REPEAT
     ESCRIBE("USA CONTROL PARA","REALIZAR ACCIONES");
     ESPERA++;
     FRAME;
    UNTIL(ESPERA==3);
    LOOP

     IF(KEY(_ESC))
      CREDITOS();
     END
     IF(KEY(_DOWN))
       Y+=3;
       C_ABAJO++;
       IF(C_ABAJO==8);
        C_ABAJO=2;
       ELSE
        GRAPH=C_ABAJO;
       END
       DIRECCION_PARADO=1;

     END
     IF(KEY(_UP))
       Y-=3;
       C_ARRIBA++;
       IF(C_ARRIBA==14)
        C_ARRIBA=9;
       ELSE
        GRAPH=C_ARRIBA;
       END
       DIRECCION_PARADO=2;
     END
     IF(KEY(_LEFT))
       X-=3;
       C_IZQ++;
       IF(C_IZQ==19)
        C_IZQ=14;
       ELSE
        GRAPH=C_IZQ;
       END
       DIRECCION_PARADO=3;
     END
     IF(KEY(_RIGHT))
       X+=3;
       C_DER++;
       IF(C_DER==26)
        C_DER=20;
       ELSE
        GRAPH=C_DER;
       END
       DIRECCION_PARADO=4;
     END
     IF(KEY(_SPACE) AND (ESPADA==1))
      SWITCH(DIRECCION_PARADO)
       CASE 1: SOUND(S_ESPADA,200,300);
               FROM GRAPH=46 TO 49;
                IF(COLLISION(TYPE CANGREJO))
                 X_GENERAL=X_CANGREJO;
                 Y_GENERAL=Y_CANGREJO;
                 MUERE();
                 SIGNAL(TYPE CANGREJO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE CANGREJO1))
                 X_GENERAL=X_CANGREJO1;
                 Y_GENERAL=Y_CANGREJO1;
                 MUERE();
                 SIGNAL(TYPE CANGREJO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO))
                 X_GENERAL=X_PULPO;
                 Y_GENERAL=Y_PULPO;
                 MUERE();
                 SIGNAL(TYPE PULPO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO1))
                 X_GENERAL=X_PULPO1;
                 Y_GENERAL=Y_PULPO1;
                 MUERE();
                 SIGNAL(TYPE PULPO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO2))
                 X_GENERAL=X_PULPO2;
                 Y_GENERAL=Y_PULPO2;
                 MUERE();
                 SIGNAL(TYPE PULPO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO1))
                 X_GENERAL=X_MALO1;
                 Y_GENERAL=Y_MALO1;
                 MUERE();
                 SIGNAL(TYPE MALO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO2))
                 X_GENERAL=X_MALO2;
                 Y_GENERAL=Y_MALO2;
                 MUERE();
                 SIGNAL(TYPE MALO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO3))
                 X_GENERAL=X_MALO3;
                 Y_GENERAL=Y_MALO3;
                 MUERE();
                 SIGNAL(TYPE MALO3,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO4))
                 X_GENERAL=X_MALO4;
                 Y_GENERAL=Y_MALO4;
                 MUERE();
                 SIGNAL(TYPE MALO4,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO5))
                 X_GENERAL=X_MALO5;
                 Y_GENERAL=Y_MALO5;
                 MUERE();
                 SIGNAL(TYPE MALO5,S_KILL);
                 N_MALOS--;
                END

                FRAME;
               END
               GRAPH=C_ABAJO;
       END
       CASE 2: SOUND(S_ESPADA,200,300);
               FROM GRAPH=50 TO 53;
                IF(COLLISION(TYPE CANGREJO))
                 X_GENERAL=X_CANGREJO;
                 Y_GENERAL=Y_CANGREJO;
                 MUERE();
                 SIGNAL(TYPE CANGREJO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE CANGREJO1))
                 X_GENERAL=X_CANGREJO1;
                 Y_GENERAL=Y_CANGREJO1;
                 MUERE();
                 SIGNAL(TYPE CANGREJO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO))
                 X_GENERAL=X_PULPO;
                 Y_GENERAL=Y_PULPO;
                 MUERE();
                 SIGNAL(TYPE PULPO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO1))
                 X_GENERAL=X_PULPO1;
                 Y_GENERAL=Y_PULPO1;
                 MUERE();
                 SIGNAL(TYPE PULPO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO2))
                 X_GENERAL=X_PULPO2;
                 Y_GENERAL=Y_PULPO2;
                 MUERE();
                 SIGNAL(TYPE PULPO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO1))
                 X_GENERAL=X_MALO1;
                 Y_GENERAL=Y_MALO1;
                 MUERE();
                 SIGNAL(TYPE MALO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO2))
                 X_GENERAL=X_MALO2;
                 Y_GENERAL=Y_MALO2;
                 MUERE();
                 SIGNAL(TYPE MALO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO3))
                 X_GENERAL=X_MALO3;
                 Y_GENERAL=Y_MALO3;
                 MUERE();
                 SIGNAL(TYPE MALO3,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO4))
                 X_GENERAL=X_MALO4;
                 Y_GENERAL=Y_MALO4;
                 MUERE();
                 SIGNAL(TYPE MALO4,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO5))
                 X_GENERAL=X_MALO5;
                 Y_GENERAL=Y_MALO5;
                 MUERE();
                 SIGNAL(TYPE MALO5,S_KILL);
                 N_MALOS--;
                END

                FRAME;
               END
               GRAPH=C_ARRIBA;
       END
       CASE 3: SOUND(S_ESPADA,200,300);
               FROM GRAPH=54 TO 57;
                IF(COLLISION(TYPE CANGREJO))
                 X_GENERAL=X_CANGREJO;
                 Y_GENERAL=Y_CANGREJO;
                 MUERE();
                 SIGNAL(TYPE CANGREJO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE CANGREJO1))
                 X_GENERAL=X_CANGREJO1;
                 Y_GENERAL=Y_CANGREJO1;
                 MUERE();
                 SIGNAL(TYPE CANGREJO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO))
                 X_GENERAL=X_PULPO;
                 Y_GENERAL=Y_PULPO;
                 MUERE();
                 SIGNAL(TYPE PULPO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO1))
                 X_GENERAL=X_PULPO1;
                 Y_GENERAL=Y_PULPO1;
                 MUERE();
                 SIGNAL(TYPE PULPO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO2))
                 X_GENERAL=X_PULPO2;
                 Y_GENERAL=Y_PULPO2;
                 MUERE();
                 SIGNAL(TYPE PULPO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO1))
                 X_GENERAL=X_MALO1;
                 Y_GENERAL=Y_MALO1;
                 MUERE();
                 SIGNAL(TYPE MALO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO2))
                 X_GENERAL=X_MALO2;
                 Y_GENERAL=Y_MALO2;
                 MUERE();
                 SIGNAL(TYPE MALO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO3))
                 X_GENERAL=X_MALO3;
                 Y_GENERAL=Y_MALO3;
                 MUERE();
                 SIGNAL(TYPE MALO3,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO4))
                 X_GENERAL=X_MALO4;
                 Y_GENERAL=Y_MALO4;
                 MUERE();
                 SIGNAL(TYPE MALO4,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO5))
                 X_GENERAL=X_MALO5;
                 Y_GENERAL=Y_MALO5;
                 MUERE();
                 SIGNAL(TYPE MALO5,S_KILL);
                 N_MALOS--;
                END

                FRAME;
               END
               GRAPH=C_IZQ;
       END
       CASE 4: SOUND(S_ESPADA,200,300);
               FROM GRAPH=42 TO 45;
                IF(COLLISION(TYPE CANGREJO))
                 X_GENERAL=X_CANGREJO;
                 Y_GENERAL=Y_CANGREJO;
                 MUERE();
                 SIGNAL(TYPE CANGREJO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE CANGREJO1))
                 X_GENERAL=X_CANGREJO1;
                 Y_GENERAL=Y_CANGREJO1;
                 MUERE();
                 SIGNAL(TYPE CANGREJO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO))
                 X_GENERAL=X_PULPO;
                 Y_GENERAL=Y_PULPO;
                 MUERE();
                 SIGNAL(TYPE PULPO,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO1))
                 X_GENERAL=X_PULPO1;
                 Y_GENERAL=Y_PULPO1;
                 MUERE();
                 SIGNAL(TYPE PULPO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE PULPO2))
                 X_GENERAL=X_PULPO2;
                 Y_GENERAL=Y_PULPO2;
                 MUERE();
                 SIGNAL(TYPE PULPO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO1))
                 X_GENERAL=X_MALO1;
                 Y_GENERAL=Y_MALO1;
                 MUERE();
                 SIGNAL(TYPE MALO1,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO2))
                 X_GENERAL=X_MALO2;
                 Y_GENERAL=Y_MALO2;
                 MUERE();
                 SIGNAL(TYPE MALO2,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO3))
                 X_GENERAL=X_MALO3;
                 Y_GENERAL=Y_MALO3;
                 MUERE();
                 SIGNAL(TYPE MALO3,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO4))
                 X_GENERAL=X_MALO4;
                 Y_GENERAL=Y_MALO4;
                 MUERE();
                 SIGNAL(TYPE MALO4,S_KILL);
                 N_MALOS--;
                END
                IF(COLLISION(TYPE MALO5))
                 X_GENERAL=X_MALO5;
                 Y_GENERAL=Y_MALO5;
                 MUERE();
                 SIGNAL(TYPE MALO5,S_KILL);
                 N_MALOS--;
                END

                FRAME;
               END
               GRAPH=C_DER;
       END
      END
     END

     IF(KEY(_CONTROL))

//////////  REFERENCIA DE COLORES  \\\\\\\\\\
//                                         \\
//  245   OBSTACULOS, QUEDARSE PARADO      \\
//  225   CARTEL "ENTRADA A LA CIUDAD".    \\
//  209   CARTEL "CAMINO A LA PLAYA".      \\
//  193   CARTEL "BOSQUE ENCANTADO".       \\
//  177   HABLAR CON EL HOMBRE.            \\
//  161   SONIDO DEL PERRO.                \\
//  145   HABLAR CON LA PRINCESA.          \\
//  129   SONIDO DEL BUHO.                 \\
//                                         \\
////////////////////////\\\\\\\\\\\\\\\\\\\\\

      IF(MAP_GET_PIXEL(FICHERO,300,X,Y)==225);
       ESCRIBE("ENTRADA A LA CIUDAD.","");
      END

      IF(MAP_GET_PIXEL(FICHERO,300,x,y)==209);
       ESCRIBE("CAMINO DE LA PLAYA.","");
      END

      IF(MAP_GET_PIXEL(FICHERO,300,x,y)==193);
       ESCRIBE("BOSQUE ENCANTADO.","");
      END

      IF(MAP_GET_PIXEL(FICHERO,300,x,y)==177);
       ESCRIBE("HIJO, HABLA CON LA ","PRINCESA EN LA PLAZA...");
       REPEAT
        FRAME;
       UNTIL(SCAN_CODE<>0);
       ESCRIBE("Y RECUPERA LA ESPADA ","EN EL BOSQUE ENCANTADO");
      END

      IF(MAP_GET_PIXEL(FICHERO,300,x,y)==161);
        SOUND(S_PERRO,100,200);
      END

      IF(MAP_GET_PIXEL(FICHERO,300,x,y)==145);
       ESCRIBE("ZELDA LIBERANOS DE LOS","MALVADOS ORCS");
      END

     END

     IF (COLLISION(TYPE EXCALIBUR))
       ESCRIBE("PULSA ESPACE PARA","USAR LA ESPADA");
       SOUND(S_ENCONTRAR_ESPADA,250,200);
       ESPADA=1;
       GRAPH=37;
       T_ESPADA=1;
       MALO1(320,375,189);
       MALO2(95,140,177);
       MALO3(82,390,177);
       MALO4(67,623,183);
       MALO5(139,360,195);

       SIGNAL(TYPE EXCALIBUR,S_KILL);
     END


     IF (COLLISION(TYPE CANGREJO) OR COLLISION(TYPE PULPO)
       OR COLLISION (TYPE CANGREJO1) OR COLLISION(TYPE PULPO1)
        OR COLLISION (TYPE PULPO2) OR COLLISION(TYPE MALO1)
         OR COLLISION (TYPE MALO2) OR COLLISION(TYPE MALO3)
          OR COLLISION (TYPE MALO4) OR COLLISION(TYPE MALO5))
       SOUND(S_TETOCO,100,200);
       X+=GET_DISTX(-180000,25);
       Y+=GET_DISTY(-180000,25);
       c++;
       VIDA();
     END

     IF(MAP_GET_PIXEL(FICHERO,300,X,Y)==245)
       X=POS_X;
       Y=POS_Y;
     END

     IF(MAP_GET_PIXEL(FICHERO,300,X,Y)==129)
      IF(SONO==0)
       SOUND(S_BUHO,100,200);
       SONO=1;
      END
     END
     POS_X=X;
     POS_Y=Y;
     IF(N_MALOS==0);
      CREDITOS();
     END
     FRAME;

    END

END

PROCESS EXCALIBUR();
BEGIN
    CTYPE=C_SCROLL;
    GRAPH=58;
    X=275;
    Y=155;
    LOOP
     IF(COLLISION(TYPE MOVER_ZELDA))
      BREAK;
     ELSE
      FRAME;
     END
    END
END

PROCESS MALO1(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(X==320);
      DIRECCION=1;
      GRAPH=189;
     END
     IF(X==405);
      DIRECCION=0;
      GRAPH=195;
     END
     IF(DIRECCION==1)
      X++;
      IF(GRAPH==194)
       GRAPH=189;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      X--;
      IF(GRAPH==200);
       GRAPH=195;
      ELSE
       GRAPH++;
      END
     END
     X_MALO1=X;
     Y_MALO1=Y;
     FRAME;
    END
END

PROCESS MALO2(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(Y==140);
      DIRECCION=1;
      GRAPH=177;
     END
     IF(Y==210);
      DIRECCION=0;
      GRAPH=183;
     END
     IF(DIRECCION==1)
      Y++;
      IF(GRAPH==182)
       GRAPH=177;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      Y--;
      IF(GRAPH==188);
       GRAPH=183;
      ELSE
       GRAPH++;
      END
     END
     X_MALO2=X;
     Y_MALO2=Y;
     FRAME;
    END
END

PROCESS MALO3(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(Y==390);
      DIRECCION=1;
      GRAPH=177;
     END
     IF(Y==470);
      DIRECCION=0;
      GRAPH=183;
     END
     IF(DIRECCION==1)
      Y++;
      IF(GRAPH==182)
       GRAPH=177;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      Y--;
      IF(GRAPH==188);
       GRAPH=183;
      ELSE
       GRAPH++;
      END
     END
     X_MALO3=X;
     Y_MALO3=Y;
     FRAME;
    END
END

PROCESS MALO4(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(Y==623);
      DIRECCION=1;
      GRAPH=183;
     END
     IF(Y==530);
      DIRECCION=0;
      GRAPH=177;
     END
     IF(DIRECCION==1)
      Y--;
      IF(GRAPH==188)
       GRAPH=183;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      Y++;
      IF(GRAPH==182);
       GRAPH=177;
      ELSE
       GRAPH++;
      END
     END
     X_MALO4=X;
     Y_MALO4=Y;
     FRAME;
    END
END

PROCESS MALO5(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(X==139);
      DIRECCION=1;
      GRAPH=189;
     END
     IF(X==235);
      DIRECCION=0;
      GRAPH=195;
     END
     IF(DIRECCION==1)
      X++;
      IF(GRAPH==194)
       GRAPH=189;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      X--;
      IF(GRAPH==200);
       GRAPH=195;
      ELSE
       GRAPH++;
      END
     END
     X_MALO5=X;
     Y_MALO5=Y;
     FRAME;
    END
END


PROCESS PULPO(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(Y==705);
      DIRECCION=1;
      GRAPH=86;
     END
     IF(Y==755);
      DIRECCION=0;
      GRAPH=90;
     END
     IF(DIRECCION==1)
      Y++;
      IF(GRAPH==89)
       GRAPH=86;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      Y--;
      IF(GRAPH==93);
       GRAPH=90;
      ELSE
       GRAPH++;
      END
     END
     X_PULPO=X;
     Y_PULPO=Y;
     FRAME;
    END
END

PROCESS PULPO1(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(Y==704);
      DIRECCION=1;
      GRAPH=86;
     END
     IF(Y==760);
      DIRECCION=0;
      GRAPH=90;
     END
     IF(DIRECCION==1)
      Y++;
      IF(GRAPH==89)
       GRAPH=86;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      Y--;
      IF(GRAPH==93);
       GRAPH=90;
      ELSE
       GRAPH++;
      END
     END
     X_PULPO1=X;
     Y_PULPO1=Y;
     FRAME;
    END
END

PROCESS PULPO2(X,Y,GRAPH);
PRIVATE
PARRIBA;
DIRECCION=1;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(X==570);
      DIRECCION=1;
      GRAPH=82;
     END
     IF(X==635);
      DIRECCION=0;
      GRAPH=78;
     END
     IF(DIRECCION==1)
      X++;
      IF(GRAPH==85)
       GRAPH=82;
      ELSE
       GRAPH++;
      END
     END
     IF(DIRECCION==0)
      X--;
      IF(GRAPH==81);
       GRAPH=78;
      ELSE
       GRAPH++;
      END
     END
     X_PULPO2=X;
     Y_PULPO2=Y;
     FRAME;
    END
END


PROCESS CANGREJO(X,Y,GRAPH);
PRIVATE
PATRAS;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(GRAPH<>98);
      IF (X==970)
       PATRAS=1;
      END
      IF (X==888)
       PATRAS=0;
      END
      IF(PATRAS)
       X--;
      ELSE
       X++;
      END
      FRAME;
      X_CANGREJO=X;
      Y_CANGREJO=Y;
     ELSE
      GRAPH=94;
     END
     GRAPH++;
    END
END

PROCESS CANGREJO1(X,Y,GRAPH);
PRIVATE
PATRAS;
BEGIN
    CTYPE=C_SCROLL;
    LOOP
     IF(GRAPH<>98);
      IF (X==116)
       PATRAS=0;
      END
      IF (X==175)
       PATRAS=1;
      END
      IF(PATRAS)
       X--;
      ELSE
       X++;
      END
      FRAME;
      X_CANGREJO1=X;
      Y_CANGREJO1=Y;
     ELSE
      GRAPH=94;
     END
     GRAPH++;
    END
END


PROCESS ESCRIBE(TEXTO,TEXTO2);
BEGIN
    GRAPH=350;
    X=160;
    Y=120;
    z=-3;
    WRITE(FUENTE2,75,168,0,TEXTO);
    WRITE(FUENTE2,75,195,0,TEXTO2);
    IF(T_ESPADA==1)
     FROM ESPERA=1 TO 3;
      FRAME;
     END
    END
    REPEAT
      SIGNAL(TYPE MOVER_ZELDA,S_FREEZE);
      FRAME;
    UNTIL(SCAN_CODE<>0);
    SIGNAL(TYPE MOVER_ZELDA,S_WAKEUP);
    GRAPH=0;
    DELETE_TEXT(ALL_TEXT);
END

PROCESS VIDA();
PRIVATE;

BEGIN
    GRAPH=170;
    SIZE=175;
    X=290;
    Y=10;
    Z=-15;
    LOOP
      SWITCH(C)
       CASE 1:
        GRAPH=170;
       END
       CASE 2:
        GRAPH=171;
       END
       CASE 3:
        GRAPH=172;
       END
       CASE 4:
        GRAPH=173;
       END
       CASE 5:
        GRAPH=174;
       END
       CASE 6;
        GRAPH=175;
       END
       CASE 7;
        GRAPH=176;
       END
       CASE 8;
        CREDITOS();
       END
      END
     FRAME;
    END
END

PROCESS MUERE();
BEGIN
    SOUND(S_MUERE,100,250);
    CTYPE=C_SCROLL;
    X=X_GENERAL;
    Y=Y_GENERAL;
    FROM GRAPH=150 TO 162;
      FRAME;
    END
END

PROCESS PERRO(X,Y,GRAPH);
BEGIN
 CTYPE=C_SCROLL;
 LOOP;
  IF(GRAPH==69)
   GRAPH=66;
  ELSE
   GRAPH++;
  END
  FRAME;
 END
END

PROCESS GALLINA(X,Y,GRAPH);
PRIVATE
 DIRECCION;
BEGIN
 CTYPE=C_SCROLL;
 LOOP
  IF(X==770)
   DIRECCION=1;
   GRAPH=70;
  END
  IF(X==806);
   DIRECCION=0;
   GRAPH=74;
  END
  IF(DIRECCION==1)
   X++;
   IF (GRAPH==73);
    GRAPH=70;
   ELSE
    GRAPH++;
   END
  END
  IF(DIRECCION==0);
   X--;
   IF(GRAPH==77);
    GRAPH=74;
   ELSE
    GRAPH++;
   END
  END
  FRAME;
 END
END

PROCESS PRINCESA(X,Y,GRAPH);
BEGIN
 CTYPE=C_SCROLL;
 LOOP
  IF(GRAPH==115);
   GRAPH=110;
  ELSE
   GRAPH++;
  END
  FRAME;
 END
END

PROCESS CREDITOS();
BEGIN

 GRAPH=375;
 X=160;
 Y=460;
 FADE_OFF();
 FADE_ON();
 LOOP
    LET_ME_ALONE();
    Y--;
    IF(Y==-460 OR KEY(_ESC))
     FADE_OFF();
     exit("PARA MAS INFORMACION MIRATE EL LEEME.TXT",0);
    END
  FRAME;
 END
END