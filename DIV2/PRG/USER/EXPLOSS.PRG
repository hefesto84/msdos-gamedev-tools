COMPILER_OPTIONS _EXTENDED_CONDITIONs;
/****************************************************************/
/*                             EXPLOSS                          */
/* CODIGO: Ismael Fern ndez Bustos                              */
/* E-MAIL: ismafb@hotmail.com                                   */
/* COMENTARIO: El juego consiste en el manejo de un coche que va*/
/* moviendose por la pantalla y va eliminando unos bloques ver- */
/* des para conseguir borrarla por completo                     */
/****************************************************************/

PROGRAM EXPLOSS_V_SHAREWARE;

CONST

    MAX_BOMBAS=20; //Numero maximo de bombas que se pueden tener

GLOBAL

    /*****************************************************************/
    /*                       VARIABLES  GENERALES                    */
    /*****************************************************************/

    DADA; //Flag para comprobar si se ha dado la vida cada 3000 puntos
    CONTINUADO; //Flag para ver si se ha continuado
    PROXIMA_VIDA; //Variable para controlar a que puntuacion se da la proxima vida
    CONT1,CONT2; //Dos contadores de uso general
    NIVEL; //Nivel de dificultad
    TECLADO; //Flag para ver si se usa teclado
    PORCENTAJE; //Porcentaje de pantalla borrado
    CUANTAS_VIDAS; //Numero de vidas
    CUANTAS_BOMBAS; //Numero de bombas
    PUNTOS; //Puntuacion
    PANTALLA; //Pantalla en la que nos encontramos
    DIRECCION; //Direccion que tiene el coche (diagonales,arriba,...)
    ID_COLISION1; //Identifica con que obstaculo colisiona el disparo
    ID_COLISION2; //Identifica con que bola colisiona el disparo
    ID_ARCHIVO1; //Identifica el archivo cargado
    ID_ARCHIVO2; //Identifica el archivo cargado
    ID_ARCHIVO3; //Identifica el archivo cargado
    ID_COCHE; //Identificador del proceso coche que este activo
    ID_BOLA1; //Identificadores de la bola con la que choca la proteccion
    ID_BOLA2;
    ID_BOLA3;
    EXISTE; //Tabla para ver si existe un proceso que da un poder en pantalla
    DISPARANDO=0; //Comprueba si est  pulsada la tecla de disparo
    PUEDE_DISPARAR=1; //Comprueba si se puede disparar (no esta en cuadro marron)
    PONIENDO_BOMBA=0; //Comprueba si est  pulsada la tecla de bomba
    PASADA; //Flag para comprobar si se sale del bucle por que se ha pasado de pantalla
    MUERTO=0; //Flag para comprobar si se sale del bucle por que le han matado una vida
    ACABAR=0; //Flag para comprobar si se pulsa escape durante el juego
    ID_TEXTO[6]; //Tabla que contiene los textos del final del juego
    INCR_Y; //Varible que servir  para el movimiento de los textos al final del juego

    /*****************************************************************/
    /*                        VARIABLES DE SONIDOS                   */
    /*****************************************************************/

    SONIDO_EXTRA;
    SONIDO_DISPARO;
    SONIDO_MUERTO;
    SONIDO_BOLA;
    SONIDO_COGE_FONDO;
    SONIDO_OBSTACULO;
    SONIDO_PASADA;
    SONIDO_COGE_BOMBAS;
    SONIDO_EXPLOSIONA_BOMBA;

/***********************************************************************/
/*                                                                     */
/*                           PROGRAMA PRINCIPAL                        */
/*                                                                     */
/***********************************************************************/


BEGIN

    //Cargamos los sonidos

    SONIDO_EXTRA=LOAD_PCM("USER\EXPLOSS\EXPLOSS1.PCM",0);
    SONIDO_DISPARO=LOAD_PCM("USER\EXPLOSS\EXPLOSS2.PCM",0);
    SONIDO_MUERTO=LOAD_PCM("USER\EXPLOSS\EXPLOSS3.PCM",0);
    SONIDO_BOLA=LOAD_PCM("USER\EXPLOSS\EXPLOSS4.PCM",0);
    SONIDO_COGE_FONDO=LOAD_PCM("USER\EXPLOSS\EXPLOSS5.PCM",0);
    SONIDO_OBSTACULO=LOAD_PCM("USER\EXPLOSS\EXPLOSS6.PCM",0);
    SONIDO_PASADA=LOAD_PCM("USER\EXPLOSS\EXPLOSS7.PCM",0);
    SONIDO_COGE_BOMBAS=LOAD_PCM("USER\EXPLOSS\EXPLOSS8.PCM",0);
    SONIDO_EXPLOSIONA_BOMBA=LOAD_PCM("USER\EXPLOSS\EXPLOSS9.PCM",0);

    // Iniciamos el n£mero de pantallas por segundo y el modo 640X480

    SET_FPS (200,0);
    SET_MODE (M640X480);

    //Cargamos los ficheros de graficos y tipos de letras

    ID_ARCHIVO1=LOAD_FPG ("USER\EXPLOSS\EXPLOSS'.FPG");
    ID_ARCHIVO3=LOAD_FPG ("USER\EXPLOSS\EXPLOSS7.FPG");
    LOAD_FNT ("USER\EXPLOSS\EXPLOSS1.FNT");
    LOAD_FNT ("USER\EXPLOSS\EXPLOSS2.FNT");
    LOAD_FNT ("USER\EXPLOSS\EXPLOSS3.FNT");
    LOAD_FNT ("USER\EXPLOSS\EXPLOSS4.FNT");

    NIVEL=2; //Iniciamos el nivel de dificultad a 2 (nivel medio)
    TECLADO=1; //Al principio el control es por teclado

    //Bucle general (mientras no se salga del juego)

    LOOP
        // Iniciamos varibles que se necesitar n para el juego

        LOAD_PAL ("USER\EXPLOSS\EXPLOSS2.PAL");
        ID_ARCHIVO2=LOAD_FPG ("USER\EXPLOSS\EXPLOSS2.FPG");
        PROXIMA_VIDA=3000; //La primera vida se dar  a 3000 puntos (1¦)
        MUERTO=0; //Al principio no le han matado
        PANTALLA=1; //Iniciamos en la pantalla 1
        CUANTAS_VIDAS=3; //Tenemos 3 vidas al empezar
        CUANTAS_BOMBAS=4; //Tenemos 4 bombas al empezar
        ACABAR=0; //Al principio de cada partida no se ha pulsado escape
        CONTINUADO=0; //Al principio no se ha continuado
        PASADA=1; //Ponemos este flag a 1 para que pueda iniciar la 1¦ pantalla
        PORCENTAJE=0; //No hemos borrado nada de la pantalla
        PUNTOS=0; //Antes de empezar no tenemos ning£n punto
        PUT_SCREEN (ID_ARCHIVO3,1); //Ponemos el gr fico de presentacion
        WRITE (1,319,140,4,"SHAREWARE");
        WRITE (3,319,350,3,"1. Jugar"); //Escribimos las opciones
        WRITE (3,319,375,3,"2. Dificultad");
        WRITE (3,319,400,3,"3. Controles");
        WRITE (3,319,425,3,"4. Creditos");
        WRITE (3,319,450,3,"5. Salir al DOS");
        LOOP
            //En funcion de la tecla pulsada hacemos una cosa u otra

            //Empezar a jugar

            IF (KEY(_1)) DELETE_TEXT (ALL_TEXT);BREAK;END

            //Elegir dificultad

            IF (KEY(_2))
                FADE_OFF();
                DELETE_TEXT (ALL_TEXT);
                LOAD_PAL("USER\EXPLOSS\EXPLOSS1.PAL");
                CLEAR_SCREEN();
                FADE_ON();
                WRITE (1,319,150,4,"NIVEL");
                WRITE (3,319,300,4,"1. Facil");
                WRITE (3,319,325,4,"2. Medio");
                WRITE (3,319,350,4,"3. Dificil");
                LOOP
                    FRAME;
                    IF (KEY(_1)) NIVEL=1; BREAK; END
                    IF (KEY(_2)) NIVEL=2; BREAK; END
                    IF (KEY(_3)) NIVEL=3; BREAK; END
                END
                FOR (CONT1=0;CONT1<=10;CONT1++) FRAME; END
                FADE_OFF();
                DELETE_TEXT(ALL_TEXT);
                LOAD_PAL("USER\EXPLOSS\EXPLOSS2.PAL");
                PUT_SCREEN (ID_ARCHIVO3,1);
                WRITE (1,319,140,4,"SHAREWARE");
                WRITE (3,319,350,3,"1. Jugar");
                WRITE (3,319,375,3,"2. Dificultad");
                WRITE (3,319,400,3,"3. Controles");
                WRITE (3,319,425,3,"4. Creditos");
                WRITE (3,319,450,3,"5. Salir al DOS");
                FADE_ON();
            END

            //Elegir control

            IF (KEY(_3))
                FADE_OFF();
                DELETE_TEXT (ALL_TEXT);
                LOAD_PAL("USER\EXPLOSS\EXPLOSS1.PAL");
                CLEAR_SCREEN();
                FADE_ON();
                WRITE (1,319,150,4,"CONTROLES");
                WRITE (3,319,300,4,"1. Teclado");
                WRITE (3,319,325,4,"2. Joystick");
                LOOP
                    FRAME;
                    IF (KEY(_1)) TECLADO=1; BREAK; END
                    IF (KEY(_2)) TECLADO=0; BREAK; END
                END
                FOR (CONT1=0;CONT1<=10;CONT1++) FRAME; END
                FADE_OFF();
                DELETE_TEXT(ALL_TEXT);
                LOAD_PAL("USER\EXPLOSS\EXPLOSS2.PAL");
                PUT_SCREEN (ID_ARCHIVO3,1);
                WRITE (1,319,140,4,"SHAREWARE");
                WRITE (3,319,350,3,"1. Jugar");
                WRITE (3,319,375,3,"2. Dificultad");
                WRITE (3,319,400,3,"3. Controles");
                WRITE (3,319,425,3,"4. Creditos");
                WRITE (3,319,450,3,"5. Salir al DOS");
                FADE_ON();
            END

            //Pantalla de cr‚ditos

            IF (KEY(_4))
                FADE_OFF();
                DELETE_TEXT (ALL_TEXT);
                LOAD_PAL ("USER\EXPLOSS\EXPLOSS1.PAL");
                CLEAR_SCREEN();
                PUT_SCREEN (ID_ARCHIVO1,2);
                FADE_ON();
                LOOP
                    FRAME;
                    IF (KEY(_SPACE)) BREAK; END
                END
                FADE_OFF();
                CLEAR_SCREEN();
                LOAD_PAL("USER\EXPLOSS\EXPLOSS2.PAL");
                PUT_SCREEN (ID_ARCHIVO3,1);
                WRITE (1,319,140,4,"SHAREWARE");
                WRITE (3,319,350,3,"1. Jugar");
                WRITE (3,319,375,3,"2. Dificultad");
                WRITE (3,319,400,3,"3. Controles");
                WRITE (3,319,425,3,"4. Creditos");
                WRITE (3,319,450,3,"5. Salir al DOS");
                FADE_ON();
            END

            //Salir del juego

            IF (KEY(_5)) DELETE_TEXT (ALL_TEXT);EXIT ("Instrucciones para adquirer la version completa en REGISTRO.TXT",0); END
            FRAME;
        END

        //Si llegamos a este punto es porque se eligio la opcion de jugar

        LOAD_PAL ("USER\EXPLOSS\EXPLOSS1.PAL");
        FADE_OFF();
        CLEAR_SCREEN(); //Borramos la pantalla para quitar el gr fico de presentacion
        FADE_ON();

        //Bucle mientras dura la partida

        REPEAT

            //Compobamos si se ha pasado de pantalla para iniciar una nueva
            //como pusimos el flag a 1, se inicia la primera pantalla pero al
            //final de esta condicion se pone a cero para que no inicie m s
            //pantallas

            IF (PASADA)
                FADE_ON();
                EXISTE=0;
                INICIA_BORDE(); //Iniciamos el borde de bloques grises
                INICIA_FONDO(PANTALLA); //Iniciamos el fondo de la pantalla en que nos encontremos
                PORCENTAJE=0; //Ponemos aqui esta variable a 0 para cuando se inicie una nueva pantalla
                WRITE (2,319,196,4,"READY");
                FOR (CONT1=0;CONT1<=25;CONT1++) FRAME; END //Retardo
                DELETE_TEXT(ALL_TEXT);
                WRITE (1,310,18,4,"PANTALLA ");
                WRITE_INT (1,463,18,3,&PANTALLA);
                VIDAS();
                WRITE (1,60,410,4,"X");
                WRITE_INT (1,95,410,3,&CUANTAS_VIDAS);
                BOMBAS();
                WRITE (1,60,450,4,"X");
                WRITE_INT (1,95,450,3,&CUANTAS_BOMBAS);
                WRITE (3,639,410,5,"(80%)");
                WRITE (1,639,450,5,"%");
                WRITE_INT(1,600,450,5,&PORCENTAJE);
                WRITE (3,320,410,4,"PUNTUACION");
                WRITE_INT(1,320,450,4,&PUNTOS);
                ID_COCHE=COCHE();
                //Cada vez que se inicia una pantalla se crea al menos una bola grande
                INICIO_BOLA_GRANDE(RAND(108,532),RAND(108,287));
                PASADA=0;
            END

            //Comprobamos si se sale del bucle de abajo porque le han matado
            //si es as¡, no iniciamos el fondo, sino que lo dejabamos tal y como
            //estaba antes de que le mataran (no se vuelve a empezar la pantalla
            //desde el principio)

            IF (MUERTO)
                FADE_OFF();

                //Destruimos todos los procesos bola (grande, mediana,...)
                //que existian y todos los disparos

                EXISTE=0;
                SIGNAL (TYPE INICIO_BOLA_GRANDE,S_KILL);
                SIGNAL (TYPE BOLA_GRANDE,S_KILL);
                SIGNAL (TYPE BOLA_MEDIANA,S_KILL);
                SIGNAL (TYPE BOLA_PEQUE¥A,S_KILL);
                SIGNAL (TYPE DISPARO,S_KILL);
                FADE_ON();
                WRITE (2,319,196,4,"READY");
                FOR (CONT1=0;CONT1<=25;CONT1++) FRAME; END
                DELETE_TEXT(ALL_TEXT);
                SIGNAL (TYPE VIDAS,S_KILL);
                SIGNAL (TYPE BOMBAS,S_KILL);
                WRITE (1,310,18,4,"PANTALLA ");
                WRITE_INT (1,463,18,3,&PANTALLA);
                VIDAS();
                WRITE (1,60,410,4,"X");
                WRITE_INT (1,95,410,3,&CUANTAS_VIDAS);
                BOMBAS();
                WRITE (1,60,450,4,"X");
                WRITE_INT (1,95,450,3,&CUANTAS_BOMBAS);
                WRITE (3,639,410,5,"(80%)");
                WRITE (1,639,450,5,"%");
                WRITE_INT(1,600,450,5,&PORCENTAJE);
                WRITE (3,320,410,4,"PUNTUACION");
                WRITE_INT(1,320,450,4,&PUNTOS);
                ID_COCHE=COCHE();
                INICIO_BOLA_GRANDE(RAND(108,532),RAND(108,287));
                MUERTO=0; //Ponemos el flag a 0 para que no considere que nos han vuelto a matar
            END

            //Bucle por pantallas o por vidas muertas

            LOOP
                //Damos una vida extra cada 3000 puntos, como es posible
                //coger dos o tres fondos a la vez damos un margen para que
                //de la vida fijo (si los puntos son de 3000 a 3010...)

                IF (PUNTOS>=PROXIMA_VIDA AND PUNTOS<=(PROXIMA_VIDA+10))
                    IF (NOT DADA)
                        SOUND(SONIDO_EXTRA,75,256);
                        PROXIMA_VIDA+=3000;
                        CUANTAS_VIDAS++;
                        DADA=1;
                    END
                ELSE
                    DADA=0;
                END

                //Generamos una bola grande en funcion de la pantalla que
                //nos encontremos, a mayor pantalla mayor probabilidad de que
                //se cree una bola grande (m s dificultad)

                IF ((RAND(1,4000/(PANTALLA*NIVEL))<=2))
                    INICIO_BOLA_GRANDE(RAND(108,532),RAND(108,287));
                END

                //Generamos un proceso que si es cogido por el coche nos da
                //cuatro bombas m s en funcion de la pantalla en que estemos
                //a mayor pantalla mayor probabilidad de que genere esto

                IF ((RAND(1,10000/PANTALLA)<=2) AND (NOT(EXISTE)))
                    COGER_BOMBAS(RAND(108,532),RAND(108,287));
                END

                //Comprobamos si se pasa de pantalla o si le matan

                IF (PASADA OR MUERTO)
                    BREAK;
                END
                IF (KEY(_ESC))
                    ACABAR=1;
                    BREAK;
                END
                FRAME;
            END

            //Si se verifica la condicion siguiente sale a la pantalla
            //de presentacion (si se verifica es porque se pulso ESCAPE)

            IF (ACABAR)
                LET_ME_ALONE();
                DELETE_TEXT (ALL_TEXT);
                UNLOAD_FPG (ID_ARCHIVO2);
                BREAK;
            END

            //Cuando llegue aqui se ha pasado una pantalla
            //Paralizamos los procesos del coche, los disparos y las bolas

            IF (PASADA)
                SIGNAL (TYPE COCHE,S_FREEZE);
                SIGNAL (TYPE DISPARO,S_FREEZE);
                SIGNAL (TYPE INICIO_BOLA_GRANDE,S_FREEZE);
                SIGNAL (TYPE BOLA_GRANDE,S_FREEZE);
                SIGNAL (TYPE BOLA_MEDIANA,S_FREEZE);
                SIGNAL (TYPE BOLA_PEQUE¥A,S_FREEZE);
                SOUND (SONIDO_PASADA,50,256);
                WRITE (2,319,196,4,"CLEAR");
                FOR (CONT1=0;CONT1<=50;CONT1++) FRAME; END
                DELETE_TEXT(ALL_TEXT);
                LET_ME_ALONE(); //Destruimos todos los procesos
                FADE_OFF();

                PUT (ID_ARCHIVO2,PANTALLA,319,239); //Ponemos la foto correspondiente a la pantalla
                                                    //hasta que se pulse la barra espacidora o el
                                                    //boton 1 del joystick segun el control
                                                    //elegido
                FADE_ON();
                LOOP
                    FRAME;
                    IF (TECLADO)
                        IF (KEY(_SPACE))
                            BREAK;
                        END
                    ELSE
                        IF (JOY.BUTTON1)
                            BREAK;
                        END
                    END
                END
                FADE_OFF();
                CLEAR_SCREEN(); //Quitamos la foto que pusimos
                FADE_ON();
            END

            //Comprobamos si la pantalla que se ha pasado es la £ltima del
            //juego, en cuyo caso ponemos la parte del final del juego

            IF ((PANTALLA==10) AND (PASADA))
                FADE_OFF();
                DELETE_TEXT(ALL_TEXT);
                LET_ME_ALONE();
                LOAD_PAL ("USER\EXPLOSS\EXPLOSS2.PAL");
                PUT_SCREEN (ID_ARCHIVO3,1);
                FADE_ON();

                //Escribimos los texto abajo de la pantalla

                ID_TEXTO[0]=WRITE (2,319,550,4,"THE END");
                ID_TEXTO[1]=WRITE (1,319,500,4,"Programacion");
                ID_TEXTO[2]=WRITE (3,319,500,4,"Ismael Fernandez Bustos");
                ID_TEXTO[3]=WRITE (1,319,500,4,"Idea Original");
                ID_TEXTO[4]=WRITE (3,319,500,4,"Ismael Fernandez Bustos");
                ID_TEXTO[5]=WRITE (3,319,500,4,"Gracias especiales a Ana Luis Campos");
                ID_TEXTO[6]=WRITE (3,319,500,4,"Para adquirir version completa: REGISTRO.TXT");

                //Vamos moviendo texto a texto hasta que lleguen al centro

                X=319;
                INCR_Y=550;
                REPEAT
                    MOVE_TEXT(ID_TEXTO[0],X,INCR_Y-=5);
                    FRAME;
                UNTIL (INCR_Y<=239)
                FOR (CONT1=0;CONT1<=100;CONT1++) FRAME; END
                DELETE_TEXT (ID_TEXTO[0]);
                INCR_Y=500;
                REPEAT
                    MOVE_TEXT(ID_TEXTO[1],X,INCR_Y-=5);
                    FRAME;
                UNTIL (INCR_Y<=239)
                INCR_Y=500;
                REPEAT
                    MOVE_TEXT(ID_TEXTO[2],X,INCR_Y-=5);
                    FRAME;
                UNTIL (INCR_Y<=280)
                FOR (CONT1=0;CONT1<=100;CONT1++) FRAME; END
                DELETE_TEXT (ID_TEXTO[1]);
                DELETE_TEXT (ID_TEXTO[2]);
                INCR_Y=500;
                REPEAT
                    MOVE_TEXT(ID_TEXTO[3],X,INCR_Y-=5);
                    FRAME;
                UNTIL (INCR_Y<=239)
                INCR_Y=500;
                REPEAT
                    MOVE_TEXT(ID_TEXTO[4],X,INCR_Y-=5);
                    FRAME;
                UNTIL (INCR_Y<=280)
                FOR (CONT1=0;CONT1<=100;CONT1++) FRAME; END
                DELETE_TEXT (ID_TEXTO[3]);
                DELETE_TEXT (ID_TEXTO[4]);
                INCR_Y=500;
                REPEAT
                    MOVE_TEXT(ID_TEXTO[5],X,INCR_Y-=5);
                    FRAME;
                UNTIL (INCR_Y<=239)
                FOR (CONT1=0;CONT1<=100;CONT1++) FRAME; END
                DELETE_TEXT (ID_TEXTO[5]);
                INCR_Y=500;
                REPEAT
                    MOVE_TEXT(ID_TEXTO[6],X,INCR_Y-=5);
                    FRAME;
                UNTIL (INCR_Y<=239)
                FOR (CONT1=0;CONT1<=100;CONT1++) FRAME; END
                DELETE_TEXT (ALL_TEXT);
                FADE_OFF();
                FADE_ON();

                //Descargamos de memoria el archivo de fotos que estaba
                //cargado para luego volver a cargarlo (nueva partida)

                UNLOAD_FPG (ID_ARCHIVO2);

                //Como es el final del juego hacemos BREAK para que no
                //inicie una nueva pantalla, sino que muestre la pantalla
                //de presentacion de nuevo

                BREAK;
            END

            //Si no era la £ltima pantalla llega a esta parte y comprueba si
            //se ha pasado de pantalla (puede llegar a este punto del programa
            //habiendole matado), si se la ha pasado y no era la £ltima incrementa
            //el n£mero de pantalla

            IF (PASADA) PANTALLA++; END

            //Comprobamos si se ha continuado alguna vez, si no se ha hecho
            //se da la opcion de continuar (m ximo una vez por partida);

            IF ((CUANTAS_VIDAS==0) AND (NOT(CONTINUADO)))
                SIGNAL (TYPE BOLA_GRANDE,S_FREEZE);
                SIGNAL (TYPE BOLA_MEDIANA,S_FREEZE);
                SIGNAL (TYPE BOLA_PEQUE¥A,S_FREEZE);
                WRITE (1,319,220,4,"CONTINUAR (S/N)");
                WRITE_INT (1,319,260,4,&CONT1);
                FOR (CONT1=9;CONT1>=0;CONT1--)
                    FOR (CONT2=0;CONT2<=10;CONT2++)
                        FRAME;
                        IF (KEY(_S))
                            CUANTAS_VIDAS=3;
                            CUANTAS_BOMBAS=4;
                            MUERTO=1;
                            CONTINUADO=1; //Ya se ha continuado una vez
                            BREAK;
                        END
                        IF (KEY(_N)) CUANTAS_VIDAS=-1;BREAK;END
                    END
                    IF (CUANTAS_VIDAS>0) BREAK; END;
                    IF (CUANTAS_VIDAS<0) CUANTAS_VIDAS=0; BREAK; END;
                END
                SIGNAL (TYPE BOMBA,S_KILL);
                SIGNAL (TYPE VIDAS,S_KILL);
                SIGNAL (TYPE BOMBAS,S_KILL);
                DELETE_TEXT(ALL_TEXT);
            END

        //Realizara este bucle REPEAT ... UNTIL hasta que las vidas sean 0
        //o se haya pasado el final del juego (ya sale de este bucle auto-
        //maticamente)

        UNTIL (CUANTAS_VIDAS==0)

        //Si llega a este punto porque no le quedan m s vidas, volvemos a la
        //pantalla de presentacion (podr¡a ser porque se ha pasado el fin del
        //juego

        IF (CUANTAS_VIDAS==0)
            SIGNAL (TYPE BOLA_GRANDE,S_FREEZE);
            SIGNAL (TYPE BOLA_MEDIANA,S_FREEZE);
            SIGNAL (TYPE BOLA_PEQUE¥A,S_FREEZE);
            SIGNAL (TYPE DISPARO,S_FREEZE);
            WRITE (2,319,146,4,"GAME");
            WRITE (2,319,236,4,"OVER");
            FOR (CONT1=0;CONT1<=50;CONT1++); FRAME; END
            FADE_OFF();
            DELETE_TEXT (ALL_TEXT);
            LET_ME_ALONE();
            FADE_ON();

            //Descargamos de memoria el archivo de fotos que estaba cargado
            //para luego volver a cargarlo (nueva partida)

            UNLOAD_FPG (ID_ARCHIVO2);

        END
    END
END

/***********************************************************************/
/*                                                                     */
/*                       FIN DEL PROGRAMA PRINCIPAL                    */
/*                                                                     */
/***********************************************************************/

/*********************************************************************/
/*                       PROCESO COGER_BOMBAS (X,Y)                  */
/* Este proceso se usa para visualizar el gr fico que si es cogido   */
/* por el coche da 4 bombas mas                                      */
/*********************************************************************/

PROCESS COGER_BOMBAS (X,Y)
PRIVATE
    CON_1;
BEGIN
    EXISTE=1;
    GRAPH=91;
    Z=-1;
    FOR (CON_1=0;CON_1<=100;CON_1++)
        FRAME;

        // Comprobamos si es cogido por el coche con la funcion COLLISION

        IF (COLLISION(TYPE COCHE))
            SOUND (SONIDO_COGE_BOMBAS,150,256);
            CUANTAS_BOMBAS+=4;
            IF (CUANTAS_BOMBAS>MAX_BOMBAS) CUANTAS_BOMBAS=MAX_BOMBAS; END
            BREAK;
        END
    END
    EXISTE=0;
END

/*********************************************************************/
/*                         PROCESO VIDAS()                           */
/* Este proceso se usa para visualizar el gr fico del coche como     */
/* simbolo del n£mero de vidas que se tienen                         */
/*********************************************************************/

PROCESS VIDAS();
BEGIN
    GRAPH=30;
    X=20;
    Y=410;
    Z=-1;
    LOOP
        FRAME;
    END
END

/*********************************************************************/
/*                         PROCESO BOMBAS()                          */
/* Este proceso se usa para visualizar el gr fico de la bomba como   */
/* simbolo del n£mero de bombas que se tienen                        */
/*********************************************************************/

PROCESS BOMBAS();
BEGIN
    GRAPH=90;
    X=20;
    Y=450;
    Z=-1;
    LOOP
        FRAME;
    END
END

/*********************************************************************/
/*                             PROCESO COCHE()                       */
/* Este proceso se usa para manejar el coche en funci¢n de las teclas*/
/* que est‚n pulsadas                                                */
/*********************************************************************/

PROCESS COCHE();
PRIVATE
    INC_X=8;
    INC_Y=8;
BEGIN
    GRAPH=30;
    Z=-1;
    X=65;
    Y=65;

    //Se realizara el bucle infinito que viene a continuacion hasta que
    //maten al coche (colisione con una bola)

    LOOP

    //En funcion de las teclas pulsadas el coche tendr  un grafico distinto
    //ademas se determina la direccion que tiene el coche por si se realiza
    //un disparo, para las teclas habr  que comprobar si son diagonales o
    //son teclas simples

    IF (TECLADO)
        IF ((KEY(_UP)) AND (NOT((KEY(_LEFT)) OR (KEY(_RIGHT)))) AND Y>65)
            GRAPH=30;
            Y-=9;
            DIRECCION=1;
        END
        IF ((KEY(_DOWN)) AND (NOT((KEY(_LEFT)) OR (KEY(_RIGHT)))) AND Y<325)
            GRAPH=31;
            Y+=9;
            DIRECCION=2;
        END
        IF ((KEY(_RIGHT)) AND (NOT((KEY(_UP)) OR (KEY(_DOWN)))) AND X<575)
            GRAPH=32;
            X+=9;
            DIRECCION=3;
        END
        IF ((KEY(_LEFT)) AND (NOT((KEY(_UP)) OR (KEY(_DOWN)))) AND X>65)
            GRAPH=33;
            X-=9;
            DIRECCION=4;
        END
        IF ((KEY(_UP)) AND (KEY(_LEFT)) AND Y>65 AND X>65)
            GRAPH=35;
            X-=9;
            Y-=9;
            DIRECCION=5;
        END
        IF ((KEY(_UP)) AND (KEY(_RIGHT)) AND Y>65 AND X<575)
            GRAPH=34;
            X+=9;
            Y-=9;
            DIRECCION=6;
        END
        IF ((KEY(_DOWN)) AND (KEY(_LEFT)) AND Y<325 AND X>65)
            GRAPH=37;
            X-=9;
            Y+=9;
            DIRECCION=7;
        END
        IF ((KEY(_DOWN)) AND (KEY(_RIGHT)) AND Y<325 AND X<575)
            GRAPH=36;
            X+=9;
            Y+=9;
            DIRECCION=8;
        END

        //Si se pulsa la barra espaciadora y el flag PUEDE_DISPARAR esta a 1
        //(el coche no est  encima de un bloque marron) se genera un disparo
        //ademas se comprueba que no se deja pulsada la tecla de disparo con
        //el flag DISPARANDO

        IF (KEY(_SPACE) AND PUEDE_DISPARAR)
            IF (DISPARANDO==0)
                DISPARO(X,Y,DIRECCION);
            END
            DISPARANDO=1;
        ELSE
            DISPARANDO=0;
        END

        //Si se pulsa la tecla ALT se genera una bomba, siempre y cuando haya
        //bombas disponibles, adem s se comprueba que se suelta la tecla ALT
        //para generar una nueva bomba (con el flag PONIENDO_BOMBA)

        IF ((KEY(_ALT)) AND (CUANTAS_BOMBAS>0))
            IF (PONIENDO_BOMBA==0)
                CUANTAS_BOMBAS--;
                BOMBA(X,Y);
            END
            PONIENDO_BOMBA=1;
        ELSE
            PONIENDO_BOMBA=0;
        END
    ELSE  //El control elegido es el Joystick
        IF ((JOY.UP) AND (NOT((JOY.LEFT) OR (JOY.RIGHT))) AND Y>65)
            GRAPH=30;
            Y-=9;
            DIRECCION=1;
        END
        IF ((JOY.DOWN) AND (NOT((JOY.LEFT) OR (JOY.RIGHT))) AND Y<325)
            GRAPH=31;
            Y+=9;
            DIRECCION=2;
        END
        IF ((JOY.RIGHT) AND (NOT((JOY.UP) OR (JOY.DOWN))) AND X<575)
            GRAPH=32;
            X+=9;
            DIRECCION=3;
        END
        IF ((JOY.LEFT) AND (NOT((JOY.UP) OR (JOY.DOWN))) AND X>65)
            GRAPH=33;
            X-=9;
            DIRECCION=4;
        END
        IF ((JOY.UP) AND (JOY.LEFT) AND Y>65 AND X>65)
            GRAPH=35;
            X-=9;
            Y-=9;
            DIRECCION=5;
        END
        IF ((JOY.UP) AND (JOY.RIGHT) AND Y>65 AND X<575)
            GRAPH=34;
            X+=9;
            Y-=9;
            DIRECCION=6;
        END
        IF ((JOY.DOWN) AND (JOY.LEFT) AND Y<325 AND X>65)
            GRAPH=37;
            X-=9;
            Y+=9;
            DIRECCION=7;
        END
        IF ((JOY.DOWN) AND (JOY.RIGHT) AND Y<325 AND X<575)
            GRAPH=36;
            X+=9;
            Y+=9;
            DIRECCION=8;
        END

        //Si se pulsa el boton 1 del joystick y el flag PUEDE_DISPARAR esta a 1
        //(el coche no est  encima de un bloque marron) se genera un disparo
        //ademas se comprueba que no se deja pulsada el boton de disparo con
        //el flag DISPARANDO

        IF ((JOY.BUTTON1) AND PUEDE_DISPARAR)
            IF (DISPARANDO==0)
                DISPARO(X,Y,DIRECCION);
            END
            DISPARANDO=1;
        ELSE
            DISPARANDO=0;
        END

        //Si se pulsa el boton 2 del joystick se genera una bomba, siempre y cuando haya
        //bombas disponibles, adem s se comprueba que se suelta el boton
        //para generar una nueva bomba (con el flag PONIENDO_BOMBA)

        IF ((JOY.BUTTON2) AND (CUANTAS_BOMBAS>0))
            IF (PONIENDO_BOMBA==0)
                CUANTAS_BOMBAS--;
                BOMBA(X,Y);
            END
            PONIENDO_BOMBA=1;
        ELSE
            PONIENDO_BOMBA=0;
        END

    END
    //Comprobamos si colisiona con un obstaculo (bloque marron) en cuyo caso
    //se pone el flag PUEDE_DISPARAR a 0 para que no se pueda disparar y ademas
    //se introduce un retardo para que el coche vaya m s despacio en estos
    //bloques

    IF (COLLISION(TYPE OBSTACULO))
        FRAME;
        PUEDE_DISPARAR=0;
        FOR (CONT1=0;CONT1<=25;CONT1++) END
    ELSE
        PUEDE_DISPARAR=1;
    END

    //Comprobamos si colisiona con algun tipo de bola, en cuyo caso pone los
    //graficos de la explosion del coche, pone el flag MUERTO a 1 para que
    //salga del bucle que va por pantallas y decrementa la varible CUANTAS_VIDAS

    IF (((COLLISION(TYPE BOLA_GRANDE)) OR (COLLISION(TYPE BOLA_MEDIANA)) OR (COLLISION(TYPE BOLA_PEQUE¥A))))
        SOUND (SONIDO_MUERTO,50,256);
        FROM GRAPH=80 TO 85;
            FRAME;
        END
        MUERTO=1;
        CUANTAS_VIDAS--;
        BREAK;
    END
    FRAME;
    END
END

/*********************************************************************/
/*                    PROCESO INICIA_BORDE()                         */
/* Este proceso se usa para iniciar el borde de la pantalla          */
/*********************************************************************/

PROCESS INICIA_BORDE();
BEGIN
    GRAPH=52;
    X=321;
    Y=241;
    LOOP
        FRAME;
    END
END

/*********************************************************************/
/*                      PROCESO INCIA_FONDO (STAGE)                  */
/* Este proceso inicia los gr ficos del fondo (verdes y marrones)    */
/* En funcion de la pantalla en que nos encontremos (parametro que   */
/* recibe el proceso crea una pantalla u otra                        */
/*********************************************************************/

PROCESS INICIA_FONDO(STAGE);
BEGIN
    SWITCH (STAGE)
        CASE 1:
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 2:
                OBSTACULO(108,108);OBSTACULO(538,108);
                OBSTACULO(108,280);OBSTACULO(538,280);
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 3:
                OBSTACULO (323,108);OBSTACULO(108,194);
                OBSTACULO (538,194);OBSTACULO(323,280);
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 4:
                OBSTACULO(108,108);OBSTACULO(323,108);
                OBSTACULO(538,108);OBSTACULO(108,194);
                OBSTACULO(538,194);OBSTACULO(108,280);
                OBSTACULO(323,280);OBSTACULO(538,280);
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 5:
                OBSTACULO(108,108);OBSTACULO(323,108);
                OBSTACULO(538,108);OBSTACULO(108,194);
                OBSTACULO(538,194);OBSTACULO(108,280);
                OBSTACULO(323,280);OBSTACULO(538,280);
                OBSTACULO(323,194);
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 6:
                FOR (CONT1=108;CONT1<=538;CONT1+=86)
                    OBSTACULO(CONT1,65);
                    OBSTACULO(CONT1,194);
                    OBSTACULO(CONT1,323);
                END
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 7:
                FOR (CONT1=194;CONT1<=452;CONT1+=43);
                    OBSTACULO(CONT1,194);
                END
                OBSTACULO(323,151);OBSTACULO(323,237);
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 8:
                OBSTACULO(151,108);OBSTACULO(495,108);
                FOR (CONT1=108;CONT1<=194;CONT1+=43);
                    OBSTACULO(CONT1,151);
                END
                FOR (CONT1=452;CONT1<=538;CONT1+=43);
                    OBSTACULO(CONT1,151);
                END
                OBSTACULO (151,194);OBSTACULO(323,194);OBSTACULO(495,194);
                FOR (CONT1=280;CONT1<=366;CONT1+=43);
                    OBSTACULO(CONT1,237);
                END
                OBSTACULO (323,280);
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 9:
                FOR (CONT1=65;CONT1<=151;CONT1+=43)
                    OBSTACULO (151,CONT1);
                END
                FOR (CONT1=409;CONT1<=581;CONT1+=43)
                    OBSTACULO (CONT1,151);
                END
                FOR (CONT1=65;CONT1<=237;CONT1+=43)
                    OBSTACULO (CONT1,237);
                END
                FOR (CONT1=237;CONT1<=323;CONT1+=43)
                    OBSTACULO (495,CONT1);
                END
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
        CASE 10:
                FOR (CONT1=151;CONT1<=495;CONT1+=43)
                    OBSTACULO (CONT1,108);
                END
                FOR (CONT1=151;CONT1<=237;CONT1+=43)
                    OBSTACULO (151,CONT1);
                    OBSTACULO (495,CONT1);
                END
                FOR (CONT1=151;CONT1<=495;CONT1+=43)
                    OBSTACULO (CONT1,280);
                END
                FOR (CONT1=65;CONT1<=597;CONT1+=43)
                    FOR (CONT2=65;CONT2<366;CONT2+=43)
                        FONDO(CONT1,CONT2);
                    END
                END
          END
    END

END

/*********************************************************************/
/*                          PROCESO FONDO(X,Y)                       */
/* Este proceso lo que hace es poner el gr fico verde en pantalla    */
/*********************************************************************/

PROCESS FONDO(X,Y);
BEGIN
    GRAPH=51;
    LOOP
        FRAME;

        //Si este proceso colisiona con el coche es destruido incrementando
        //la variable porcentaje, los puntos y comprueba si el porcentaje
        //es mayor que 80 para pasar de pantalla (poner el flag PASADA a 1)

        IF (COLLISION(TYPE COCHE))
            SOUND (SONIDO_COGE_FONDO,35,256);
            PORCENTAJE++;
            PUNTOS+=5;
            IF (PORCENTAJE>=80) PASADA=1; END
            BREAK;
        END

        //Si al iniciarse el fondo coinciden un fondo con un obstaculo se
        //destruye el fondo para que no haya dos procesos superpuestos

        IF (COLLISION(TYPE OBSTACULO)) SIGNAL (ID,S_KILL); END
    END
END

/*********************************************************************/
/*                         PROCESO OBSTACULO(X,Y)                    */
/* Este proceso lo que hace es poner el gr fico marr¢n en pantalla   */
/*********************************************************************/

PROCESS OBSTACULO(X,Y);
BEGIN
    GRAPH=50;
    LOOP
        FRAME;
    END
END

/*********************************************************************/
/*                        PROCESO BOMBA(X,Y)                         */
/* Este proceso genera una bomba en funci¢n de las coordenadas del   */
/* coche (las recibe como parametros                                 */
/*********************************************************************/

PROCESS BOMBA (X,Y)
BEGIN
    GRAPH=90;
    Z=-1;
    LOOP
        FRAME;
    END
END

/*********************************************************************/
/*                        PROCESO DISPARO(X,Y,DIRE)                  */
/* Este proceso genera un disparo en funci¢n de la direcci¢n que     */
/* tenga el coche cuando es invocado, el disparo surge de las coorde-*/
/* nadas del coche (primeros dos parametros) y sigue la direccion que*/
/* tenga el coche (diagonal,arriba,...)                              */
/*********************************************************************/

PROCESS DISPARO (X,Y,DIRE)
BEGIN
    SOUND(SONIDO_DISPARO,50,256);
    GRAPH=38;
    Z=-1;
    LOOP

    //En funcion de la direccion del coche determinamos la trayectoria del
    //disparo

    SWITCH (DIRE)
        CASE 1:
                Y-=16;
                END
        CASE 2:
                Y+=16;
                END
        CASE 3:
                X+=16;
                END
        CASE 4:
                X-=16;
                END
        CASE 5:
                Y-=16;
                X-=16;
                END
        CASE 6:
                Y-=16;
                X+=16;
                END
        CASE 7:
                Y+=16;
                X-=16;
                END
        CASE 8:
                Y+=16;
                X+=16;
                END
        DEFAULT:
                Y-=16;
                END
    END

        //Si el disparo llega a ciertas coordenadas se destruye

        IF (X<=43 OR X>=610 OR Y<=43 OR Y>=343)
            BREAK;
        END

        //Si colisiona con un obstaculo destruye el obstaculo y el disparo
        //incrementando la variable PORCENTAJE y la variable PUNTOS adem s
        //comprobamos si al matar ese obstaculo se obtiene que el porcentaje
        //es mayor o igual que 80 (a lo mejor no quedan procesos fondo y el
        //que hace pasar de pantalla es un proceso obstaculo)

        IF (ID_COLISION1=COLLISION(TYPE OBSTACULO))
            SOUND (SONIDO_OBSTACULO,25,256);
            PUNTOS+=5;
            PORCENTAJE++;
            SIGNAL (ID_COLISION1,S_KILL);
            IF (PORCENTAJE>=80) PASADA=1; BREAK; END
            BREAK;
        END
        FRAME;
    END
END

/*********************************************************************/
/*                   PROCESO INICIO_BOLA_GRANDE(X,Y)                 */
/* Este proceso genera las estrellas que indican el inicio de una    */
/* bola grande                                                       */
/*********************************************************************/

PROCESS INICIO_BOLA_GRANDE (X,Y)
PRIVATE
    CONTADOR;
BEGIN
    Z=-1;
    FOR (CONTADOR=0;CONTADOR<=3;CONTADOR++)
        FROM GRAPH=70 TO 74;
            FRAME;
        END
    END
    BOLA_GRANDE (X,Y);
END

/*********************************************************************/
/*                         PROCESO BOLA GRANDE                       */
/* Este proceso genera una bola grande, si colisiona con un disparo  */
/* genera dos bolas medianas                                         */
/*********************************************************************/

PROCESS BOLA_GRANDE(X,Y);
PRIVATE
    INC_X=8;
    INC_Y=8;
BEGIN
    GRAPH=60;
    Z=-1;
    LOOP

        //Si colisiona la bola grande con un disparo genera dos bolas medianas

        IF (ID_COLISION2=COLLISION(TYPE DISPARO))
            SIGNAL (ID_COLISION2,S_KILL);
            SOUND (SONIDO_BOLA,50,256);
            PUNTOS+=5;
            BOLA_MEDIANA (X,Y,INC_X,INC_Y);
            BOLA_MEDIANA (X,Y,-INC_X,-INC_Y);
            BREAK;
        END

        //Si colisiona con una bomba se destruye la bola completamente

        IF (ID_COLISION2=COLLISION(TYPE BOMBA))
            SOUND (SONIDO_EXPLOSIONA_BOMBA,50,256);
            PUNTOS+=5;
            SIGNAL (ID_COLISION2,S_KILL);
            BREAK;
        END
        FRAME;
        X+=INC_X;
        Y+=INC_Y;
        IF (X<=107 OR X>=533) INC_X=-INC_X; END
        IF (Y<=107 OR Y>=288) INC_Y=-INC_Y; END
    END
END

/*********************************************************************/
/*                          PROCESO BOLA_MEDIANA                     */
/* Este proceso genera una bola mediana, si colisiona con un disparo */
/* genera dos bolas peque¤as                                         */
/*********************************************************************/

PROCESS BOLA_MEDIANA (X,Y,INC_X,INC_Y);
BEGIN
    GRAPH=61;
    Z=-1;
    LOOP

        //Si colisiona con un disparo genera dos bolas peque¤as

        IF (ID_COLISION2=COLLISION(TYPE DISPARO))
            SIGNAL (ID_COLISION2,S_KILL);
            SOUND (SONIDO_BOLA,50,256);
            PUNTOS+=5;
            BOLA_PEQUE¥A (X,Y,INC_X,INC_Y);
            BOLA_PEQUE¥A (X,Y,-INC_X,-INC_Y);
            BREAK;
        END

        //Si colisiona con una bomba se destruye completamente

        IF (ID_COLISION2=COLLISION(TYPE BOMBA))
            SOUND (SONIDO_EXPLOSIONA_BOMBA,50,256);
            PUNTOS+=5;
            SIGNAL (ID_COLISION2,S_KILL);
            BREAK;
        END
        FRAME;
        X+=INC_X;
        Y+=INC_Y;
        IF (X<=85 OR X>=555) INC_X=-INC_X; END
        IF (Y<=85 OR Y>=300) INC_Y=-INC_Y; END
    END
END

/*********************************************************************/
/*                           PROCESO BOLA_PEQUE¥A                    */
/* Este proceso genera una bola peque¤a, si colisiona con un disparo */
/* el proceso es destruido                                           */
/*********************************************************************/

PROCESS BOLA_PEQUE¥A (X,Y,INC_X,INC_Y);
BEGIN
    GRAPH=62;
    Z=-1;
    LOOP

        //Si colisiona con un disparo se destruye la bola

        IF (ID_COLISION2=COLLISION(TYPE DISPARO))
            SOUND (SONIDO_BOLA,50,256);
            PUNTOS+=5;
            SIGNAL (ID_COLISION2,S_KILL);
            BREAK;
        END

        //Si colisiona con una bomba tambien se destruye la bola
        IF (ID_COLISION2=COLLISION(TYPE BOMBA))
            SOUND(SONIDO_EXPLOSIONA_BOMBA,50,256);
            PUNTOS+=5;
            SIGNAL (ID_COLISION2,S_KILL);
            BREAK;
        END
        FRAME;
        X+=INC_X;
        Y+=INC_Y;
        IF (X<=64 OR X>=576) INC_X=-INC_X; END
        IF (Y<=64 OR Y>=320) INC_Y=-INC_Y; END
    END
END

/***********************************************************************/
/*                                                                     */
/*                          FIN DEL JUEGO                              */
/*                                                                     */
/***********************************************************************/
