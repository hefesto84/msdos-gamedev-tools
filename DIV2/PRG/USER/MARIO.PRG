COMPILER_OPTIONS _EXTENDED_CONDITIONS;
//**************************************************************************
// TITULO:      MARIO WORLD, PC VERSION  BETA 2.03
// AUTOR:       WALTER J. (HELLSPAWN)
// FECHA:       30/03/98
// E-MAIL:      hellspawn@iberonline.es  ¢ simmons@mediaweb.es
//**************************************************************************


PROGRAM Mariow;

GLOBAL
    fase=1;           // Numero de fases
    vidas=0;          // Numero de vidas
    puntos=0;         // Variable para la puntuaci¢n
    monedas=0;        // Numero de monedas recogidas
    pts_barra=0;      // Variable para la puntuaci¢n de la barra de fin de fase
    reserva=0;        // Variable para poder tener una seta en la reserva
    graficos2;        // Identificador del fichero FPG1
    fasex;
    id_mario;         // Identificador de MARIO
    id_vida;          // Identificador para las vidas
    yoshy=0;          // Variable para saber si tengo a Yoshy
    fin_fase=false;   // Variable para controlar si se ha terminado un fase
    mi_contador=0;    // Variable para controlar el timer
    total=60;         // Variable para controlar el tiempo del que disponemos
    cuenta_atras=0;   // Para conseguir que nuestro reloj vaya hacia el cero
    canal1;           // Sirve para activar o desactivar la banda sonora
    canal2;           // Cuando sea necesario
    canal3;
    sonando=0;        // Evita una repeticion en el sonido cuando esta dentro
                      // de un LOOP

    estado_mario;   // Guarda el Estado de Mario
    conseguido=0;

    s_musica;       // ID para Banda Sonora del juego ;-)
    s_salto;        // ID el sonido de salto
    s_moneda1;      // ID el sonido de monedas Yoshy
    s_moneda2;      // ID el sonido de monedas normales
    s_1vida;        // ID el sonido de las vidas
    s_coge_seta;    // ID el sonido de la seta en reserva
    s_seta;         // ID el sonido de la seta
    s_coger;        // ID el sonido de cuando cogemos la seta de la reserva
    s_m_inicio;     // ID el sonido del inicio de la fase
    s_m_mata;       // ID el sonido de cuando matamos con Mario
    s_m_muerto;     // ID el segundo sonido de salto
    s_m_trans;      // ID el sonido de ca¡da
    s_yoshy1;       // ID el sonido de las burbujas
    s_yoshy2;       // ID el sonido de recogida de monedas
    s_yoshy_co;     // ID el sonido de cuando Yoshy se va corriendo
    s_y_mata;       // ID el sonido de cuando matamos con Yoshy

    s_m_salto2;     // ID el sonido de cuando matamos con Mario
    s_pantalla;     // ID la musica del Menu
    s_m_dtrans;     // ID el sonido de destransformacion
    s_y_come;       // ID el sonido de Yoshy comiendo
    s_tiempo;       // ID el sonido de poco tiempo
    s_gameover;     // ID el sonido de cuando nos matan o acabamos el juego
    s_viento;       // ID el sonido del viento para el tunel


    fuente;
    fuente2;
    fuente3;

    VEL=0;IDF;IDFU;
    IND=0;MAXE=0;MAXT=0;DIF=0;
    P_X=0;P_Y=0;

LOCAL
    velocidad_gravedad; // Variable para guardar la velocidad de ca¡da (gravedad)
    velocidad=0;        // Variable para guardar la velocidad horizontal de Mario
    muerte;             // Variable para guardar si Mario esta muerto
    angulo2;            // Identificador para las setas
    id_seta;            // Identificador del huevo
    id_huevo;


PRIVATE
    pantalla=200;         // Pantalla que se ve como men£
    salir=0;              // Para volver al menu sin salir del todo


BEGIN

//-----------------------------------------------------------------------
//     Comienza el programa principal y la carga de ficheros
//-----------------------------------------------------------------------

        id_mario=id;
        set_fps(16,0);
        set_mode(m320x240);
        load_fpg("USER\MARIO\mario.fpg");
        graficos2=load_fpg("USER\MARIO\objetos.fpg");
        fuente=load_fnt("USER\MARIO\mario.fnt");
        fuente2=load_fnt("USER\MARIO\mario2.fnt");
        fuente3=load_fnt("USER\MARIO\mario3.fnt");


    //Carga Sonidos

    s_musica=load_pcm("USER\MARIO\m_sound2.pcm",1);     // Banda Sonora
    s_salto=load_pcm("USER\MARIO\salto.pcm",0);         // Banda Sonora
    s_moneda1=load_pcm("USER\MARIO\moneda1.pcm",0);     // Banda Sonora
    s_moneda2=load_pcm("USER\MARIO\moneda2.pcm",0);     // Banda Sonora
    s_1vida=load_pcm("USER\MARIO\1vida.pcm",0);         // Banda Sonora
    s_coge_seta=load_pcm("USER\MARIO\coge_seta.pcm",0); // Banda Sonora
    s_coger=load_pcm("USER\MARIO\coger.pcm",0);         // Banda Sonora
    s_seta=load_pcm("USER\MARIO\seta.pcm",0);           // Banda Sonora
    s_m_inicio=load_pcm("USER\MARIO\m_inicio.pcm",0);   // Banda Sonora
    s_m_mata=load_pcm("USER\MARIO\m_mata1.pcm",0);      // Banda Sonora
    s_m_muerto=load_pcm("USER\MARIO\m_muerto.pcm",0);   // Banda Sonora
    s_m_trans=load_pcm("USER\MARIO\m_trans.pcm",0);     // Banda Sonora
    s_yoshy1=load_pcm("USER\MARIO\yoshy1.pcm",0);       // Banda Sonora
    s_yoshy2=load_pcm("USER\MARIO\yoshy2.pcm",0);       // Banda Sonora
    s_yoshy_co=load_pcm("USER\MARIO\yoshy_co.pcm",0);   // Banda Sonora
    s_y_mata=load_pcm("USER\MARIO\y_mata1.pcm",0);      // Banda Sonora

    s_m_salto2=load_pcm("USER\MARIO\m_salto2.pcm",0);   // ID el sonido de cuando matamos con Mario
    s_pantalla=load_pcm("USER\MARIO\pantalla.pcm",1);   // ID la musica del Menu
    s_m_dtrans=load_pcm("USER\MARIO\m_dtrans.pcm",0);   // ID el sonido de destransformacion
    s_y_come=load_pcm("USER\MARIO\y_come.pcm",0);       // ID el sonido de Yoshy comiendo
    s_tiempo=load_pcm("USER\MARIO\tiempo.pcm",0);       // ID el sonido de poco tiempo
    s_gameover=load_pcm("USER\MARIO\gameover.pcm",0);   // ID el sonido de gameover
    s_viento=load_pcm("USER\MARIO\viento.pcm",0);       // ID el sonido de viento


//-----------------------------------------------------------------------
//          Inicializa las pantallas que se ven como menu
//-----------------------------------------------------------------------

    LOOP                    // Bucle principal
    xput(0,201,160,120,0,100,0,0);
    sound(s_moneda2,256,260);   // Sonido del LOG
    FRAME(2400);
        timer[9]=1000;         // Se inicializa fuera de tiempo para cambiar nada mas entrar
        pantalla=202;         // Elige la otra pantalla porque cambia nada mas entrar
        canal3=sound(s_pantalla,256,256);
        REPEAT                              // Bucle de pantallas de presentaci¢n
            IF (timer[9]>500)                  // Si pasa 5 segundos cambia de pantalla
                timer[9]=0;
                IF (pantalla==200)
                    pantalla=202;             //  3:Pantalla cr‚ditos
                ELSE
                    pantalla=200;
                END                         //  1:Pantalla de men£
                fade_off();
                xput(0,105,160,110,0,100,0,0);
                xput(0,pantalla,160,120,0,100,0,0);
                fade_on();
            END
            angle+=10000;
            FRAME;

        // Si has pulsado ESCAPE o E sal directamente del programa
        IF (key(_esc) or key(_e))
            fade_off();
            exit("E-mail: hellspawn@iberonline.es Gracias por jugar!",0);
         END

IF (key(_r))

END

        // Se ha elegido una opci¢n
            UNTIL (key(_a) OR key(_b) OR key (_c))


        fade_off();                     // Apaga pantalla
        stop_sound(canal3);   // Para la musica de presentacion
        stop_scroll(0);

        // Inicializa variables de juego
        fase=1;
        vidas=3;
        puntos=0;
        monedas=0;
        timer=0;
        pts_barra=0;



        REPEAT              // Bucle para pantalla de espera de carga
            fade_on();      // Enciende la pantalla
            xput(0,999,160,120,0,100,0,0);
            // Pone un mensaje de de la fase
            xput(0,170,160,120,0,100,0,0);
            write(fuente,138,77,4,"FASE");
            write_int(fuente,208,77,4,&fase);
            sound(s_m_inicio,250,256);   // Pone la musica de inicio de Fase
            FRAME(3900);

            WHILE (fading)  // Espera mientras inicia todo, que
                FRAME;      // que ser  al acabar el apagado de pantalla
            END
            delete_text(all_text);


            // Coloca marcadores de puntos, vidas y monedas
            objeto(0,98,160,19,-512,0);     // Pinta vidas, seta Verde
            write_int(fuente2,60,25,5,&vidas);
            write_int(fuente2,300,24,5,&puntos);  // Pinta puntuaci¢n
            write_int(fuente2,300,15,5,&monedas);
            write_int(fuente3,221,24,4,&cuenta_atras);
            write_int(fuente2,135,19,4,&pts_barra);
            fin_fase=false;                      // Inicializa variable
                                                 // controladora de fin de fase



//-----------------------------------------------------------------------
//              Inicializa las  SEIS fases que tiene el juego
//-----------------------------------------------------------------------


            SWITCH (fase)      // Bucle para las distintas fases


//-----------------------------------------------------------------------
//                    Inicializa fase UNO
//-----------------------------------------------------------------------

            CASE 1:         // Inicializa fase uno

            fasex=load_fpg("USER\MARIO\nivel1.fpg");
            canal1=sound(s_musica,100,256);   // Banda Sonora
            start_scroll(0,fasex,100,101,0,4);
            timer=0;
            tuberia(x,y);
            // Inicializa el objeto del protagonista
            mario(20,202,1);


            // Inicializa objetos de las monedas
            moneda(460,60);  // Gr ficos de las monedas
            moneda(593,133); // Gr ficos de las monedas
            moneda(631,100); // Gr ficos de las monedas
            moneda(872,122); // Gr ficos de las monedas
            moneda(302,200); // Gr ficos de las monedas
            moneda(1312,199); // Gr ficos de las monedas
            moneda(113,162); // Gr ficos de las monedas
            moneda(1300,119); // Gr ficos de las monedas
            moneda(1309,180); // Gr ficos de las monedas
            moneda(337,87); // Gr ficos de las monedas
            moneda(417,89); // Gr ficos de las monedas
            moneda(379,96); // Gr ficos de las monedas
            moneda(634,120); // Gr ficos de las monedas
            moneda(690,142); // Gr ficos de las monedas
            moneda(706,171); // Gr ficos de las monedas
            moneda(707,119); // Gr ficos de las monedas
            moneda(120,185); // Gr ficos de las monedas
            moneda(557,131); // Gr ficos de las monedas
            moneda(490,143); // Gr ficos de las monedas
            moneda(1189,116); // Gr ficos de las monedas
            moneda(1146,123); // Gr ficos de las monedas
            moneda(32,89); // Gr ficos de las monedas
            moneda(206,145); // Gr ficos de las monedas
            moneda(266,155); // Gr ficos de las monedas
            moneda(432,185); // Gr ficos de las monedas


            //Conjunto de monedas que forman la palabra DIV
            moneda(860,125);  // Gr ficos de las monedas
            moneda(860,141); // Gr ficos de las monedas
            moneda(860,157); // Gr ficos de las monedas
            moneda(860,173); // Gr ficos de las monedas
            moneda(872,125); // Gr ficos de las monedas
            moneda(872,141); // Gr ficos de las monedas
            moneda(872,157); // Gr ficos de las monedas
            moneda(872,173); // Gr ficos de las monedas
            moneda(884,141); // Gr ficos de las monedas
            moneda(884,157); // Gr ficos de las monedas
            moneda(916,125); // Gr ficos de las monedas
            moneda(916,141); // Gr ficos de las monedas
            moneda(916,157);  // Gr ficos de las monedas
            moneda(916,173); // Gr ficos de las monedas
            moneda(948,125); // Gr ficos de las monedas
            moneda(957,141); // Gr ficos de las monedas
            moneda(966,157); // Gr ficos de las monedas
            moneda(975,173); // Gr ficos de las monedas
            moneda(984,157); // Gr ficos de las monedas
            moneda(993,141); // Gr ficos de las monedas
            moneda(1002,125); // Gr ficos de las monedas


            // Inicializa bloques giratorios
            bl_gira(87,175);
            bl_gira(109,175);
            bl_gira(146,175);

            //Inicializa bloques que contienen Setas
            bloque_sorpresa(1182,159);

            //Inicializa Bloques que contienen a Yoshy ¢ Vida
            bloque_sorpresa2(140,43);


            //Pone la tuber¡a que tapar  a la planta
            Tapa_planta1(1137,175);
            Tapa_planta2(647,184);

            //Pone la planta en pantalla
            planta(646,201);
            planta(1136,201);


            // Inicializa objetos tortugas normales
            tortuga2(107,112,1);
            tortuga2(139,160,1);
            tortuga2(167,80,1);
            tortuga2(426,224,1);
            tortuga2(718,208,1);
            tortuga2(1108,208,1);
            tortuga2(1188,208,1);
            tortuga2(1416,208,1);


            // Inicializa objetos tortugas voladoras
            tortugas(0,0,600,140,110,64,100,64);
            tortugas(0,0,900,140,110,64,160,100);
            tortugas(0,0,1300,170,90,48,200,50);
            tortugas(0,0,1320,150,95,60,150,75);
            tortugas(0,0,1820,100,48,36,64,64);
            tortugas(0,0,370,170,90,48,200,50);
            tortugas(0,0,320,150,95,60,150,75);
            tortugas(0,0,300,200,48,36,64,64);
END

//-----------------------------------------------------------------------
//                    Inicializa fase DOS
//-----------------------------------------------------------------------

            CASE 2:         // Inicializa fase dos
    fase=2;
    canal1=sound(s_musica,100,256);   // Banda Sonora
    IDF=FONDO();
    Mario_volando();
    FADE_OFF();
    clear_screen();
    FADE_ON();
    start_scroll(0,0,107,0,0,4);

            // Inicializa objetos de las monedas
            estrellitas(280,80); // Gr ficos de las monedas
            estrellitas(300,222);  // Gr ficos de las monedas
            estrellitas(299,113); // Gr ficos de las monedas
            estrellitas(235,100); // Gr ficos de las monedas
            estrellitas(289,144); // Gr ficos de las monedas
            gota(60,60);  // Gr ficos de las monedas
            gota(293,133); // Gr ficos de las monedas
            gota(231,100); // Gr ficos de las monedas
            gota(272,122); // Gr ficos de las monedas
            gota(302,230); // Gr ficos de las monedas
            gota(312,199); // Gr ficos de las monedas
            gota(313,162); // Gr ficos de las monedas
            gota(315,119); // Gr ficos de las monedas
            gota(309,180); // Gr ficos de las monedas
            gota(37,20); // Gr ficos de las monedas
            gota(117,19); // Gr ficos de las monedas
            gota(179,36); // Gr ficos de las monedas
            gota(234,20); // Gr ficos de las monedas
            gota(90,42); // Gr ficos de las monedas
            gota(206,71); // Gr ficos de las monedas
            gota(197,119); // Gr ficos de las monedas
            gota(120,185); // Gr ficos de las monedas
            gota(47,231); // Gr ficos de las monedas
            gota(7,233); // Gr ficos de las monedas
            gota(8,36); // Gr ficos de las monedas
            gota(277,83); // Gr ficos de las monedas
            gota(132,99); // Gr ficos de las monedas
            gota(206,227); // Gr ficos de las monedas
            gota(266,205); // Gr ficos de las monedas
            gota(2,185); // Gr ficos de las monedas


            estrellitas(60,125);  // Gr ficos de las monedas
            estrellitas(60,141); // Gr ficos de las monedas
            estrellitas(60,157); // Gr ficos de las monedas
            estrellitas(60,173); // Gr ficos de las monedas
            estrellitas(72,125); // Gr ficos de las monedas
            estrellitas(72,141); // Gr ficos de las monedas
            estrellitas(72,157); // Gr ficos de las monedas
            estrellitas(72,173); // Gr ficos de las monedas
            estrellitas(84,141); // Gr ficos de las monedas
            estrellitas(84,157); // Gr ficos de las monedas
            estrellitas(116,125); // Gr ficos de las monedas
            estrellitas(116,141); // Gr ficos de las monedas
            estrellitas(116,157);  // Gr ficos de las monedas
            estrellitas(116,173); // Gr ficos de las monedas
            estrellitas(148,125); // Gr ficos de las monedas
            estrellitas(157,141); // Gr ficos de las monedas
            estrellitas(166,157); // Gr ficos de las monedas
            estrellitas(175,173); // Gr ficos de las monedas
            estrellitas(184,157); // Gr ficos de las monedas
            estrellitas(193,141); // Gr ficos de las monedas
            estrellitas(202,125); // Gr ficos de las monedas

END


//-----------------------------------------------------------------------
//                    Inicializa fase TRES
//-----------------------------------------------------------------------

            CASE 3:         // Inicializa fase tres

            fasex=load_fpg("USER\MARIO\nivel2.fpg");
            canal1=sound(s_musica,100,256);   // Inicia sonido
            timer=0;
            fase=3;
            start_scroll(0,fasex,100,103,0,4);
            tuberiax(x,y,z);
            tuberia3(x,y);

            // Inicializa el protagonista
            IF (estado_mario==1)
            mario(10,1,1);
            ELSE
            IF (estado_mario==2)
            mario2(10,1,1);
            ELSE
            IF (estado_mario==3)
            yoshy1(10,1,1);
            ELSE
            IF (estado_mario==4)
            yoshy2(10,1,1);
            FRAME;
            END
            END
            END
            END


            // Inicializa objetos de las monedas
            moneda(80,222);  // Gr ficos de las monedas
            moneda(102,113); // Gr ficos de las monedas
            moneda(135,100); // Gr ficos de las monedas
            moneda(181,144); // Gr ficos de las monedas
            moneda(201,133); // Gr ficos de las monedas
            moneda(231,100); // Gr ficos de las monedas
            moneda(382,122); // Gr ficos de las monedas
            moneda(422,230); // Gr ficos de las monedas
            moneda(432,199); // Gr ficos de las monedas
            moneda(393,162); // Gr ficos de las monedas
            moneda(165,113); // Gr ficos de las monedas
            moneda(539,180); // Gr ficos de las monedas

            moneda(460,60);  // Gr ficos de las monedas
            moneda(593,133); // Gr ficos de las monedas
            moneda(631,100); // Gr ficos de las monedas
            moneda(872,122); // Gr ficos de las monedas
            moneda(902,200); // Gr ficos de las monedas
            moneda(1312,199); // Gr ficos de las monedas
            moneda(913,162); // Gr ficos de las monedas
            moneda(815,119); // Gr ficos de las monedas
            moneda(1309,180); // Gr ficos de las monedas
            moneda(737,87); // Gr ficos de las monedas
            moneda(417,89); // Gr ficos de las monedas
            moneda(379,96); // Gr ficos de las monedas
            moneda(634,120); // Gr ficos de las monedas
            moneda(690,142); // Gr ficos de las monedas
            moneda(706,171); // Gr ficos de las monedas
            moneda(997,119); // Gr ficos de las monedas
            moneda(920,185); // Gr ficos de las monedas
            moneda(557,131); // Gr ficos de las monedas
            moneda(490,143); // Gr ficos de las monedas
            moneda(889,116); // Gr ficos de las monedas
            moneda(977,123); // Gr ficos de las monedas
            moneda(999,89); // Gr ficos de las monedas
            moneda(206,145); // Gr ficos de las monedas
            moneda(266,155); // Gr ficos de las monedas
            moneda(432,185); // Gr ficos de las monedas



            // Inicializa bloques giratorios
            bl_gira(87,175);
            bl_gira(109,175);
            bl_gira(146,175);

            //Inicializa bloques que contienen Setas
            bloque_sorpresa(966,37);
            bloque_sorpresa(877,168);
            bloque_sorpresa(1196,173);

            //Inicializa Bloques que contienen a Yoshy ¢ Vida
            bloque_sorpresa2(190,69);

            //Pone la tuber¡a que tapar  a la planta
            Tapa_planta1(558,176);


            //Pone la planta en pantalla
            planta(557,201);


            // Inicializa objetos tortugas normales
            tortuga2(189,112,1);
            tortuga2(173,160,1);
            tortuga2(219,208,1);
            tortuga2(405,224,1);
            tortuga2(620,208,1);
            tortuga2(826,208,1);
            tortuga2(964,112,1);
            tortuga2(1160,208,1);


            // Inicializa objetos tortugas voladoras
            tortugas(0,0,600,140,110,64,100,64);
            tortugas(0,0,900,140,110,64,160,100);
            tortugas(0,0,1300,170,90,48,200,50);
            tortugas(0,0,1320,150,95,60,150,75);
            tortugas(0,0,1820,100,48,36,64,64);
            tortugas(0,0,370,170,90,48,200,50);
            tortugas(0,0,320,150,95,60,150,75);
            tortugas(0,0,300,200,48,36,64,64);
END


//-----------------------------------------------------------------------
//                    Inicializa fase CUATRO
//-----------------------------------------------------------------------

            CASE 4:         // Inicializa fase cuatro

            fasex=load_fpg("USER\MARIO\nivel1.fpg");
            canal1=sound(s_musica,100,256);   // Banda Sonora
            timer=0;
            fase=4;
            start_scroll(0,fasex,100,102,0,4);
            tuberiax(x,y,z);
            tuberia4(x,y);

            // Inicializa el protagonista
            IF (estado_mario==1)
            mario(10,1,1);
            ELSE
            IF (estado_mario==2)
            mario2(10,1,1);
            ELSE
            IF (estado_mario==3)
            yoshy1(10,1,1);
            ELSE
            IF (estado_mario==4)
            yoshy2(10,1,1);
            FRAME;
            END
            END
            END
            END



            // Inicializa objetos de las monedas
            moneda(460,60);  // Gr ficos de las monedas
            moneda(593,133); // Gr ficos de las monedas
            moneda(631,100); // Gr ficos de las monedas
            moneda(872,122); // Gr ficos de las monedas
            moneda(302,200); // Gr ficos de las monedas
            moneda(1312,199); // Gr ficos de las monedas
            moneda(113,162); // Gr ficos de las monedas
            moneda(1300,119); // Gr ficos de las monedas
            moneda(1309,180); // Gr ficos de las monedas
            moneda(337,87); // Gr ficos de las monedas
            moneda(417,89); // Gr ficos de las monedas
            moneda(379,96); // Gr ficos de las monedas
            moneda(634,120); // Gr ficos de las monedas
            moneda(690,142); // Gr ficos de las monedas
            moneda(706,171); // Gr ficos de las monedas
            moneda(707,119); // Gr ficos de las monedas
            moneda(120,185); // Gr ficos de las monedas
            moneda(557,131); // Gr ficos de las monedas
            moneda(490,143); // Gr ficos de las monedas
            moneda(1189,116); // Gr ficos de las monedas
            moneda(1146,123); // Gr ficos de las monedas
            moneda(32,89); // Gr ficos de las monedas
            moneda(206,145); // Gr ficos de las monedas
            moneda(266,155); // Gr ficos de las monedas
            moneda(432,185); // Gr ficos de las monedas


            //Conjunto de monedas que forman la palabra DIV
            moneda(860,125);  // Gr ficos de las monedas
            moneda(860,141); // Gr ficos de las monedas
            moneda(860,157); // Gr ficos de las monedas
            moneda(860,173); // Gr ficos de las monedas
            moneda(872,125); // Gr ficos de las monedas
            moneda(872,141); // Gr ficos de las monedas
            moneda(872,157); // Gr ficos de las monedas
            moneda(872,173); // Gr ficos de las monedas
            moneda(884,141); // Gr ficos de las monedas
            moneda(884,157); // Gr ficos de las monedas
            moneda(916,125); // Gr ficos de las monedas
            moneda(916,141); // Gr ficos de las monedas
            moneda(916,157);  // Gr ficos de las monedas
            moneda(916,173); // Gr ficos de las monedas
            moneda(948,125); // Gr ficos de las monedas
            moneda(957,141); // Gr ficos de las monedas
            moneda(966,157); // Gr ficos de las monedas
            moneda(975,173); // Gr ficos de las monedas
            moneda(984,157); // Gr ficos de las monedas
            moneda(993,141); // Gr ficos de las monedas
            moneda(1002,125); // Gr ficos de las monedas



            // Inicializa bloques giratorios
            bl_gira(87,175);
            bl_gira(109,175);
            bl_gira(146,175);

            //Inicializa bloques que contienen Setas
            bloque_sorpresa(1182,159);

            //Inicializa Bloques que contienen a Yoshy ¢ Vida
            bloque_sorpresa2(140,43);


            //Pone la tuber¡a que tapar  a la planta
            Tapa_planta1(1137,175);
            Tapa_planta2(647,184);

            //Pone la planta en pantalla
            planta(646,201);
            planta(1136,201);

            // Inicializa objetos tortugas normales
            tortuga2(107,112,1);
            tortuga2(139,160,1);
            tortuga2(167,80,1);
            tortuga2(426,224,1);
            tortuga2(718,208,1);
            tortuga2(1108,208,1);
            tortuga2(1188,208,1);
            tortuga2(1416,208,1);


            // Inicializa objetos tortugas voladoras
            tortugas(0,0,600,140,110,64,100,64);
            tortugas(0,0,900,140,110,64,160,100);
            tortugas(0,0,1300,170,90,48,200,50);
            tortugas(0,0,1320,150,95,60,150,75);
            tortugas(0,0,1820,100,48,36,64,64);
            tortugas(0,0,370,170,90,48,200,50);
            tortugas(0,0,320,150,95,60,150,75);
            tortugas(0,0,300,200,48,36,64,64);
END


//-----------------------------------------------------------------------
//                    Inicializa fase CINCO
//-----------------------------------------------------------------------

            CASE 5:         // Inicializa fase cinco

            fasex=load_fpg("USER\MARIO\nivel2.fpg");
            canal1=sound(s_musica,100,256);   // Inicia sonido
            timer=0;

            fase=5;
            start_scroll(0,fasex,100,105,0,4);
            tuberiax(x,y,z);
            tuberia5(x,y);
            paleta(1338,196,256); // Se¤ala que entramos en la ultima fase

            // Inicializa el protagonista
            IF (estado_mario==1)
            mario(10,1,1);
            ELSE
            IF (estado_mario==2)
            mario2(10,1,1);
            ELSE
            IF (estado_mario==3)
            yoshy1(10,1,1);
            ELSE
            IF (estado_mario==4)
            yoshy2(10,1,1);
            FRAME;
            END
            END
            END
            END



            // Inicializa objetos de las monedas
            moneda(80,222);  // Gr ficos de las monedas
            moneda(102,113); // Gr ficos de las monedas
            moneda(135,100); // Gr ficos de las monedas
            moneda(181,144); // Gr ficos de las monedas
            moneda(201,133); // Gr ficos de las monedas
            moneda(231,100); // Gr ficos de las monedas
            moneda(382,122); // Gr ficos de las monedas
            moneda(422,230); // Gr ficos de las monedas
            moneda(432,199); // Gr ficos de las monedas
            moneda(393,162); // Gr ficos de las monedas
            moneda(165,113); // Gr ficos de las monedas
            moneda(539,180); // Gr ficos de las monedas

            moneda(460,60);  // Gr ficos de las monedas
            moneda(593,133); // Gr ficos de las monedas
            moneda(631,100); // Gr ficos de las monedas
            moneda(872,122); // Gr ficos de las monedas
            moneda(902,200); // Gr ficos de las monedas
            moneda(1312,199); // Gr ficos de las monedas
            moneda(913,162); // Gr ficos de las monedas
            moneda(815,119); // Gr ficos de las monedas
            moneda(1309,180); // Gr ficos de las monedas
            moneda(737,87); // Gr ficos de las monedas
            moneda(417,89); // Gr ficos de las monedas
            moneda(379,96); // Gr ficos de las monedas
            moneda(634,120); // Gr ficos de las monedas
            moneda(690,142); // Gr ficos de las monedas
            moneda(706,171); // Gr ficos de las monedas
            moneda(997,119); // Gr ficos de las monedas
            moneda(920,185); // Gr ficos de las monedas
            moneda(557,131); // Gr ficos de las monedas
            moneda(490,143); // Gr ficos de las monedas
            moneda(889,116); // Gr ficos de las monedas
            moneda(977,123); // Gr ficos de las monedas
            moneda(999,89); // Gr ficos de las monedas
            moneda(206,145); // Gr ficos de las monedas
            moneda(266,155); // Gr ficos de las monedas
            moneda(432,185); // Gr ficos de las monedas


            // Inicializa bloques giratorios
            bl_gira(87,175);
            bl_gira(109,175);
            bl_gira(146,175);

            //Inicializa bloques que contienen Setas
            bloque_sorpresa(966,37);
            bloque_sorpresa(877,168);
            bloque_sorpresa(1196,173);

            //Inicializa Bloques que contienen a Yoshy ¢ Vida
            bloque_sorpresa2(190,69);

            //Pone la tuber¡a que tapar  a la planta
            Tapa_planta1(558,176);


            //Pone la planta en pantalla
            planta(557,201);



            // Inicializa objetos tortugas normales
            tortuga2(189,112,1);
            tortuga2(173,160,1);
            tortuga2(219,208,1);
            tortuga2(405,224,1);
            tortuga2(620,208,1);
            tortuga2(826,208,1);
            tortuga2(964,112,1);
            tortuga2(1160,208,1);


            // Inicializa objetos tortugas voladoras
            tortugas(0,0,600,140,110,64,100,64);
            tortugas(0,0,900,140,110,64,160,100);
            tortugas(0,0,1300,170,90,48,200,50);
            tortugas(0,0,1320,150,95,60,150,75);
            tortugas(0,0,1820,100,48,36,64,64);
            tortugas(0,0,370,170,90,48,200,50);
            tortugas(0,0,320,150,95,60,150,75);
            tortugas(0,0,300,200,48,36,64,64);
END


//-----------------------------------------------------------------------
//                    Inicializa fase SEIS
//-----------------------------------------------------------------------

            CASE 6:

            fasex=load_fpg("USER\MARIO\nivel1.fpg");
            canal1=sound(s_musica,100,256);   // Banda Sonora
            timer=0;

            fase=6;
            start_scroll(0,fasex,100,106,0,4);
            tuberiax(x,y,z);
            poste(1402,136);
            barra();
            paleta(1338,196,256); // Se¤ala que entramos en la ultima fase

            // Inicializa el protagonista
            IF (estado_mario==1)
            mario(10,1,1);
            ELSE
            IF (estado_mario==2)
            mario2(10,1,1);
            ELSE
            IF (estado_mario==3)
            yoshy1(10,1,1);
            ELSE
            IF (estado_mario==4)
            yoshy2(10,1,1);
            FRAME;
            END
            END
            END
            END


            // Inicializa objetos de las monedas
            moneda(460,60);  // Gr ficos de las monedas
            moneda(593,133); // Gr ficos de las monedas
            moneda(631,100); // Gr ficos de las monedas
            moneda(872,122); // Gr ficos de las monedas
            moneda(302,200); // Gr ficos de las monedas
            moneda(1312,199); // Gr ficos de las monedas
            moneda(113,162); // Gr ficos de las monedas
            moneda(1300,119); // Gr ficos de las monedas
            moneda(1309,180); // Gr ficos de las monedas
            moneda(337,87); // Gr ficos de las monedas
            moneda(417,89); // Gr ficos de las monedas
            moneda(379,96); // Gr ficos de las monedas
            moneda(634,120); // Gr ficos de las monedas
            moneda(690,142); // Gr ficos de las monedas
            moneda(706,171); // Gr ficos de las monedas
            moneda(707,119); // Gr ficos de las monedas
            moneda(120,185); // Gr ficos de las monedas
            moneda(557,131); // Gr ficos de las monedas
            moneda(490,143); // Gr ficos de las monedas
            moneda(1189,116); // Gr ficos de las monedas
            moneda(1146,123); // Gr ficos de las monedas
            moneda(32,89); // Gr ficos de las monedas
            moneda(206,145); // Gr ficos de las monedas
            moneda(266,155); // Gr ficos de las monedas
            moneda(432,185); // Gr ficos de las monedas


            //Conjunto de monedas que forman la palabra DIV
            moneda(860,125);  // Gr ficos de las monedas
            moneda(860,141); // Gr ficos de las monedas
            moneda(860,157); // Gr ficos de las monedas
            moneda(860,173); // Gr ficos de las monedas
            moneda(872,125); // Gr ficos de las monedas
            moneda(872,141); // Gr ficos de las monedas
            moneda(872,157); // Gr ficos de las monedas
            moneda(872,173); // Gr ficos de las monedas
            moneda(884,141); // Gr ficos de las monedas
            moneda(884,157); // Gr ficos de las monedas
            moneda(916,125); // Gr ficos de las monedas
            moneda(916,141); // Gr ficos de las monedas
            moneda(916,157);  // Gr ficos de las monedas
            moneda(916,173); // Gr ficos de las monedas
            moneda(948,125); // Gr ficos de las monedas
            moneda(957,141); // Gr ficos de las monedas
            moneda(966,157); // Gr ficos de las monedas
            moneda(975,173); // Gr ficos de las monedas
            moneda(984,157); // Gr ficos de las monedas
            moneda(993,141); // Gr ficos de las monedas
            moneda(1002,125); // Gr ficos de las monedas


            // Inicializa bloques giratorios
            bl_gira(87,175);
            bl_gira(109,175);
            bl_gira(146,175);

            //Inicializa bloques que contienen Setas
            bloque_sorpresa(1182,159);

            //Inicializa Bloques que contienen a Yoshy ¢ Vida
            bloque_sorpresa2(140,43);

            //Pone la tuber¡a que tapar  a la planta
            Tapa_planta1(1137,175);
            Tapa_planta2(647,184);


            //Pone la planta en pantalla
            planta(646,201);
            planta(1136,201);


            // Inicializa objetos tortugas normales
            tortuga2(107,112,1);
            tortuga2(139,160,1);
            tortuga2(167,80,1);
            tortuga2(426,224,1);
            tortuga2(718,208,1);
            tortuga2(1108,208,1);
            tortuga2(1188,208,1);
            tortuga2(1416,208,1);


            // Inicializa objetos tortugas voladoras
            tortugas(0,0,600,140,110,64,100,64);
            tortugas(0,0,900,140,110,64,160,100);
            tortugas(0,0,1300,170,90,48,200,50);
            tortugas(0,0,1320,150,95,60,150,75);
            tortugas(0,0,1820,100,48,36,64,64);
            tortugas(0,0,370,170,90,48,200,50);
            tortugas(0,0,320,150,95,60,150,75);
            tortugas(0,0,300,200,48,36,64,64);
END
END



//-----------------------------------------------------------------------
// Ahora van las condiciones generales de todo el juego
//-----------------------------------------------------------------------


            fade_off();             // Apaga pantalla
            clear_screen();
            fade_on();              // Enciende pantalla

            REPEAT                  // Bucle principal del juego
            IF (TIMER>=0)
                mi_contador=timer/100;
                cuenta_atras=60-mi_contador;
            END

// Si se pulsa a la vez todas las letras que forman el
// nombre "Dani" de Daniel Navarro, creador de este magn¡fico programa,
// se pasa de fase
            IF (key(_d) AND key (_a) AND key(_n) AND key(_i))
                fin_fase=true;
            END
            FRAME;

            // Repite hasta que se pase de fase o te quedes sin vidas
            UNTIL (fin_fase OR vidas<0 OR key (_esc))
            IF (key(_esc))
                salir=1;
            END
            IF (fin_fase)        // Si se ha terminado la fase
                fase++;          // Incrementa la fase
                fin_fase=false;  // Inicializa la variable de control de fase
            END

            IF (vidas<0)
            // Termina procesos de objetos que tapan gr ficos
            signal(TYPE objeto,s_kill);
            // Para todos los sonidos
            stop_sound(all_sound);
            // Borra todos los textos que haya
            delete_text(all_text);
            reserva=0;
            stop_scroll(0);
            clear_screen();
            fade_off();
            fade_on();      // Enciende la pantalla
            sound(s_gameover,256,256); // Avisa de que queda poco tiempo
            // Pone fondo Negro
            xput(0,999,160,120,0,100,0,0);
            // Pone mensaje de Game Over
            xput(0,171,160,120,0,100,0,0);
            frame(4700);
            clear_screen();

            END
            IF (fase==7)
                sound(s_gameover,256,256); // Se acab¢ el juegazo
                write(fuente,160,100,4,"!Congratulations!");
                frame(4700);
                clear_screen();
            END

            FRAME(2400);             // Espera un segundo
            fade_off();              // Apaga pantalla
            stop_scroll(0);          // Para scroll

            //Acaba con procesos pendientes para liberar MEMoria
            signal(TYPE objeto,s_kill);
            signal(TYPE Mario_volando,s_kill);
            signal(TYPE fondo,s_kill);
            signal(TYPE gota,s_kill);
            signal(TYPE estrellitas,s_kill);
            signal(TYPE tortugas,s_kill);
            signal(TYPE tortuga2,s_kill);
            signal(TYPE moneda,s_kill);
            signal(TYPE bloque_sorpresa,s_kill);
            signal(TYPE bloque_sorpresa2,s_kill);
            signal(TYPE bl_gira,s_kill);
            signal(TYPE Tapa_planta1,s_kill);
            signal(TYPE Tapa_planta1,s_kill);
            signal(TYPE planta,s_kill);

            // Para todos los sonidos
            stop_sound(all_sound);
            // Borra todos los textos que haya
            delete_text(all_text);
          IF(conseguido==1)
          FIN_FASE=TRUE;
          END

        // Repite hasta que no tengas vidas o llegue a la fase cuatro
        UNTIL (vidas<0 OR fase==7 OR key (_esc) OR salir)
        salir=0;
        reserva=0;  // Si se ha quedado algo en la reserva lo borra
    END
END





//-----------------------------------------------------------------------

PROCESS mario(x,y,graph)

PRIVATE
    saltando=0;      // Evita el salto continuo
    corriendo=0;
    incx=0;          // Variable para comprobar paredes
    ultima_y=0;      // Variable para mirar si sube
    ultima_veloc=0;  // Variable para comprobar la velocidad
    id2;             // Identificador de uso general
    id3;             // Identificador de Mario sin collision muerte_mario
    id4;

BEGIN
    estado_mario=1;
    yoshy=0;
    ctype=c_scroll;
    scroll.camera=id;
    graph=1;
    priority=1;
    FRAME;

    LOOP

    IF ((key(_b) OR joy.button2) AND reserva==1)
    seta3(x,y);
    END


        // Lee el teclado y el joystick

        IF ((key(_right) OR joy.right) AND velocidad<5)
        IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo=0;
           ELSE
            corriendo--;
        END
        ELSE
        IF ((key(_left) OR joy.left) AND velocidad>-5)
        IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo=0;
           ELSE
            corriendo--;

           END
            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END


IF ((key(_right) or joy.right) AND (key(_v) or joy.button1) AND velocidad<17)
            IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
           END

        ELSE
IF ((key(_left) or joy.left) AND (key(_v) or joy.button1) AND velocidad>-17)
            IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
           END

            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END



        // Mira si el protagonista choca con las paredes
        ultima_veloc=velocidad;                // Guarda la velocidad en 'ultima_veloc'
        IF (ultima_veloc<>0)                   // Y si se mueve
            incx=ultima_veloc/abs(ultima_veloc);          // Halla la direcci¢n
            WHILE (ultima_veloc<>0)            // 'ultima_veloc' sirve de contador
                // Comprueba que no de por debajo
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-2)/2)<>120)
               // Ni por encima
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-32)/2)<>120)


                        x+=incx;    // Y si no da, mueve al mu¤eco
                    END
                END
                ultima_veloc-=incx;     // Resta 'ultima_veloc' hasta que sea cero
            END
        END




        gravedad();             // Funci¢n para hallar la gravedad

        IF (en_pantalla())      // Si el protagonista no baja
            IF (velocidad<>0)   // Y se mueve
                graph++;        // Cambia de gr fico para animarlo
                IF (graph>4)
                    graph=1;
                END
            ELSE
                graph=1;      // Si no se mueve ponen el gr fico por defecto
            END

 //Ahora si Mario va hacia la derecha o izquierda y de repente cambia
 //de direcci¢n, pone gr fico de frenado

         IF (velocidad>0 AND (key(_left) OR joy.left)) graph=7; flags=1; END
         IF (velocidad<0 AND (key(_right) OR joy.right)) graph=7; flags=0; END


        IF ((key(_up) OR joy.up))
        graph=10;
        ELSE
        IF ((key(_down) OR joy.down))
        graph=9;
         END
         END


        IF ((key(_up) or joy.up) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_up) or joy.up) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END


// Lee que las teclas Control o el joystick
            IF (key(_control) OR joy.button3)
            IF (saltando==0)
            sound(s_salto,256,265);   // Inicia sonido
                velocidad_gravedad=-18;         // y salta
              END
            saltando++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            saltando=0;       // Tecla soltada
        END
     END

// Mientras Mario est‚ subiendo pon el gr fico de subir
        IF (ultima_y>y) graph=5; END
//Y si Mario est  agachado y salta, saltar  agachado
        IF ((key(_down) OR joy.down) AND (key(_control) OR joy.button3))
        graph=9;
         END


        IF (ultima_y<y)         // Comprueba si esta bajando
            graph=6;            // Pon el gr fico de bajar
            muerte=true;        // Si baja puede matar
//Y si Mario est  agachado y salta, saltar  agachado Y BAJARA agachado.
        IF ((key(_down) OR joy.down) AND (ultima_y<y))
        graph=9;
         END
        ELSE
            muerte=false;       // Si sube o esta quieto le pueden matan
        END

        // Comprueba colisi¢n con tortugas voladoras
            IF (id2=collision(TYPE tortugas))
            puntos+=400;
            ELSE
        // Comprueba colisi¢n con tortugas2
            IF (id2=collision(TYPE tortuga2))
            puntos+=400;
          END
          END


        // Comprueba colisi¢n con bloques sorpresa
            IF (id3=collision(TYPE bloque_sorpresa))
            puntos+=400;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);
            seta(id3.x+3,id3.y-5);
            END

        // Comprueba colisi¢n con bloques sorpresa2
            IF (id3=collision(TYPE bloque_sorpresa2))
            puntos+=400;
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);

            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            id_huevo=Huevo(id3.x,id3.y);
            END


        // Comprueba colisi¢n con setas
            IF (id3=collision(TYPE seta))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            transformacion(id3.x,id3.y);
            sound(s_m_trans,256,256);   // Inicia sonido
            ELSE
            IF (id3=collision(TYPE seta3))
            puntos+=0;
            signal(id3,s_kill);        // Elimina seta de pantalla
            transformacion(id3.x,id3.y);
            sound(s_m_trans,256,256);   // Inicia sonido
            END
            END



        // Comprueba colisi¢n con setas verdes
            IF (id3=collision(TYPE seta_vida))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            vidas++;                // Y suma una vida
            vida(id3.x,id3.y);
            END



        // Comprueba colisi¢n con bloques piedra
            IF (id3=collision(TYPE piedra))
            puntos+=0;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
          END


        // Comprueba colisi¢n con Plantas Carnivoras
        IF (collision (TYPE planta))
        muerte_mario();
        END


        // Comprueba la colision con la barra de final del juego
        IF (collision (TYPE barra))
        pts_barra+=100;
        mario_stage_clear1();
        END

        // Comprueba si hay colisi¢n con los monedas
            IF (id3=collision(TYPE moneda))
            monedas++;                    // Incrementa numero de monedas
            puntos+=50;               // Suma puntos
            signal(id3,s_kill);         // Elimina los monedas de pantalla
            sound(s_moneda2,256,260);   // Inicia sonido
            IF (monedas>=100)              // Y si tiene mas de 10 monedas
                monedas-=100;              // Resta 10 monedas en la cuenta
                vidas++;                // Y suma una vida
                // Pone un mensaje de vida conseguida
                vida(id3.x,id3.y);
            END
        END


        IF (id2)        // Comprueba si ha habido cualquier tipo de colisi¢n
            IF (muerte)     // Y si esta bajando y puede matar
            IF (corriendo==0)
                velocidad_gravedad=-12; // Hace botar al protagonista + alto
                golpes(id2.x,id2.y);    // Crea estrella del golpe
                signal(id2,s_kill);     // Elimina al monstruo que ha originado la colisi¢n
                sound(s_m_mata,256,256);   // Inicia sonido

            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
            velocidad_gravedad=-20;   // Hace botar al protagonista + bajo
            golpes(id2.x,id2.y);      // Crea estrella del golpe
            signal(id2,s_kill);       // Elimina al monstruo que ha originado la colisi¢n
            sound(s_m_mata,256,256);   // Inicia sonido
        END
            ELSE       // Si no puede matar, entonces el protagonista
                muerte_mario();              // Esta muerto
            END
        END

        ultima_y=y;   // Guarda y en ultima_y para comprobar luego si baja
                      // Controla que Mario solo mate cuando este bajando
        IF (out_region(id,0)) // Si se sale de la pantalla se muere
            muerte_mario();   // Esta muerto
        END

        IF (cuenta_atras<=0) // Si el tiempo supera el minuto
            muerte_mario(); // Esta muerto
            timeup(x,y);
        ELSE
        IF (cuenta_atras<=30) // Si el tiempo supera el minuto
        IF (sonando==0)
            sound(s_tiempo,256,256); // Avisa de que queda poco tiempo
        END
            sonando++;
        ELSE
            sonando=0;
        END
        END
        FRAME;
     END
END



//-----------------------------------------------------------------------

PROCESS mario2(x,y,graph)

PRIVATE
    saltando=0;      // Evita el salto continuo
    corriendo=0;
    incx=0;          // Variable para comprobar paredes
    ultima_y=0;      // Variable para mirar si sube
    ultima_veloc=0;  // Variable para comprobar la velocidad
    id2;             // Identificador de uso general
    id3;             // Identificador de Mario sin collision muerte_mario
    id4;

BEGIN
    estado_mario=2;
    yoshy=0;
    ctype=c_scroll;
    scroll.camera=id;
    graph=20;
    priority=1;
    FRAME;

    LOOP
    IF ((key(_b) OR joy.button2) AND reserva==1)
    seta3(x,y);
    END




        // Lee el teclado y el joystick

        IF ((key(_right) OR joy.right) AND velocidad<5)
        IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo=0;
           ELSE
            corriendo--;
        END
        ELSE
        IF ((key(_left) OR joy.left) AND velocidad>-5)
        IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo=0;
           ELSE
            corriendo--;

           END
            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END


IF ((key(_right) or joy.right) AND (key(_v) or joy.button1) AND velocidad<17)
            IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
           END

        ELSE
IF ((key(_left) or joy.left) AND (key(_v) or joy.button1) AND velocidad>-17)
            IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
           END

            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END



   // Mira si el protagonista choca con las paredes
   ultima_veloc=velocidad;                // Guarda la velocidad en 'ultima_veloc'
   IF (ultima_veloc<>0)                   // Y si se mueve
       incx=ultima_veloc/abs(ultima_veloc);          // Halla la direcci¢n
       WHILE (ultima_veloc<>0)            // 'ultima_veloc' sirve de contador
                // Comprueba que no de por debajo
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-2)/2)<>120)
               // Ni por encima
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-32)/2)<>120)

                        x+=incx;    // Y si no da, mueve al mu¤eco
                END
                END
                ultima_veloc-=incx;     // Resta 'ultima_veloc' hasta que sea cero
            END
        END


        gravedad();             // Funci¢n para hallar la gravedad del protagonista

        IF (en_pantalla())      // Si el protagonista no baja
            IF (velocidad<>0)   // Y se mueve
                graph++;        // Cambia de gr fico para animarlo
                IF (graph>22)
                    graph=20;
                END
            ELSE
                graph=20;      // Si no se mueve ponen el gr fico por defecto
            END

 //Ahora si Mario va hacia la derecha o izquierda y de repente cambia
 //de direcci¢n, pone gr fico de frenado

         IF (velocidad>0 AND (key(_left) OR joy.left)) graph=25; flags=1; END
         IF (velocidad<0 AND (key(_right) OR joy.right)) graph=25; flags=0; END


        IF ((key(_up) OR joy.up))
        graph=30;
        ELSE
        IF ((key(_down) OR joy.down))
        graph=29;
         END
         END



        IF ((key(_up) or joy.up) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_up) or joy.up) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END


// Lee que las teclas Control o el joystick
            IF (key(_control) OR joy.button3)
            IF (saltando==0)
            sound(s_salto,256,265);   // Inicia sonido
                velocidad_gravedad=-18;         // y salta
              END
            saltando++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            saltando=0;       // Tecla soltada
        END
     END

// Mientras Mario est‚ subiendo pon el gr fico de subir
        IF (ultima_y>y) graph=23; END
//Y si Mario est  agachado y salta, saltar  agachado
        IF ((key(_down) OR joy.down) AND (key(_control) OR joy.button3))
        graph=29;
         END



        IF (ultima_y<y)         // Comprueba si esta bajando
            graph=24;            // Pon el gr fico de bajar
            muerte=true;        // Si baja puede matar
//Y si Mario est  agachado y salta, saltar  agachado Y BAJARA agachado.
        IF ((key(_down) OR joy.down) AND (ultima_y<y))
        graph=29;
         END
        ELSE
            muerte=false;       // Si sube o esta quieto le pueden matan
        END



        // Comprueba colisi¢n con tortugas voladoras
            IF (id2=collision(TYPE tortugas))
            puntos+=400;
            ELSE
        // Comprueba colisi¢n con tortugas2
            IF (id2=collision(TYPE tortuga2))
            puntos+=400;
          END
          END

        // Comprueba colisi¢n con bloques sorpresa
            IF (id3=collision(TYPE bloque_sorpresa))
            puntos+=400;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);
            seta(id3.x+3,id3.y-5);
            END



        // Comprueba colisi¢n con bloques sorpresa2
            IF (id3=collision(TYPE bloque_sorpresa2))
            puntos+=400;
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            id_huevo=Huevo(id3.x,id3.y);
            END



        // Comprueba colisi¢n con setas
            IF (id3=collision(TYPE seta))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            reserva=1;
            id_seta=seta2(x,y,ctype);
            ELSE
            IF (id3=collision(TYPE seta3))
            puntos+=0;
            signal(id3,s_kill);        // Elimina seta de pantalla
            reserva=1;
            id_seta=seta2(x,y,ctype);
            END
            END


        // Comprueba colisi¢n con setas verdes
            IF (id3=collision(TYPE seta_vida))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            vidas++;                // Y suma una vida
            vida(id3.x,id3.y);
            END



            // Comprueba colisi¢n con bloques piedra
            IF (id3=collision(TYPE piedra))
            puntos+=0;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
          END


        // Comprueba colisi¢n con Plantas Carnivoras
        IF (collision (TYPE planta))
        muerte_mario2();
        END


        // Comprueba la colision con la barra de final del juego
        IF (collision (TYPE barra))
        pts_barra+=100;
        mario_stage_clear2();
        END



        // Comprueba si hay colisi¢n con los monedas
            IF (id3=collision(TYPE moneda))
            monedas++;                    // Incrementa numero de monedas
            puntos+=50;               // Suma puntos
            signal(id3,s_kill);         // Elimina los monedas de pantalla
            sound(s_moneda2,256,260);   // Inicia sonido
            IF (monedas>=100)              // Y si tiene mas de 10 monedas
                monedas-=100;              // Resta 10 monedas en la cuenta
                vidas++;                // Y suma una vida
                // Pone un mensaje de vida conseguida
                vida(id3.x,id3.y);
            END
        END




        IF (id2)        // Comprueba si ha habido cualquier tipo de colisi¢n
            IF (muerte)     // Y si esta bajando y puede matar
            IF (corriendo==0)
                velocidad_gravedad=-12; // Hace botar al protagonista + alto
                golpes(id2.x,id2.y);    // Crea estrella del golpe
                signal(id2,s_kill);     // Elimina al monstruo que ha originado la colisi¢n
                sound(s_m_mata,256,256);   // Inicia sonido
            corriendo++;      // Tecla pulsada
        ELSE
            corriendo=0;       // Tecla soltada
            velocidad_gravedad=-20;   // Hace botar al protagonista + bajo
            golpes(id2.x,id2.y);      // Crea estrella del golpe
            signal(id2,s_kill);       // Elimina al monstruo que ha originado la colisi¢n
            sound(s_m_mata,256,256);   // Inicia sonido
        END
            ELSE       // Si no puede matar, entonces el protagonista
                transformacion2(x,y);      // Se vuelve peque¤o otra vez
                sound(s_m_dtrans,256,256);   // Inicia sonido
            END
        END

        ultima_y=y;   // Guarda y en ultima_y para comprobar luego si baja
                      // Controla que Mario solo mate cuando este bajando

        IF (out_region(id,0)) // Si se sale de la pantalla se muere
            muerte_mario2();   // Esta muerto
        END

        IF (cuenta_atras<=0) // Si el tiempo supera el minuto
            muerte_mario2(); // Esta muerto
            timeup(x,y);
        ELSE
        IF (cuenta_atras<=30) // Si el tiempo supera el minuto
        IF (sonando==0)
            sound(s_tiempo,256,256); // Avisa de que queda poco tiempo
        END
            sonando++;
        ELSE
            sonando=0;
        END
        END
        FRAME;
     END
END




//-----------------------------------------------------------------------

PROCESS yoshy1(x,y,graph)

PRIVATE
    saltando=0;      // Evita el salto continuo
    corriendo=0;
    incx=0;          // Variable para comprobar paredes
    ultima_y=0;      // Variable para mirar si sube
    ultima_veloc=0;  // Variable para comprobar la velocidad
    id2;             // Identificador de uso general
    id3;             // Identificador de Mario sin collision muerte_mario
    id4;

BEGIN
    estado_mario=3;
    yoshy=1;
    file=graficos2;
    ctype=c_scroll;
    scroll.camera=id;
    graph=1;
    priority=1;
    FRAME;

    LOOP
    IF ((key(_b) OR joy.button2) AND reserva==1)
    seta3(x,y);
    END




        // Lee el teclado y el joystick

        IF ((key(_right) OR joy.right) AND velocidad<5)
        IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo=0;
           ELSE
            corriendo--;
        END
        ELSE
        IF ((key(_left) OR joy.left) AND velocidad>-5)
        IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo=0;
           ELSE
            corriendo--;

           END
            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END


IF ((key(_right) or joy.right) AND (key(_v) or joy.button1) AND velocidad<17)
            IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo++;      // Tecla pulsada
        ELSE
            corriendo=0;       // Tecla soltada
           END

        ELSE
IF ((key(_left) or joy.left) AND (key(_v) or joy.button1) AND velocidad>-17)
            IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
           END

            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END



   // Mira si el protagonista choca con las paredes
   ultima_veloc=velocidad;                // Guarda la velocidad en 'ultima_veloc'
   IF (ultima_veloc<>0)                   // Y si se mueve
       incx=ultima_veloc/abs(ultima_veloc);          // Halla la direcci¢n
       WHILE (ultima_veloc<>0)            // 'ultima_veloc' sirve de contador
                // Comprueba que no de por debajo
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-2)/2)<>120)
               // Ni por encima
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-32)/2)<>120)


                        x+=incx;    // Y si no da, mueve al mu¤eco
                    END
                END
                ultima_veloc-=incx;     // Resta 'ultima_veloc' hasta que sea cero
            END
        END


        gravedad();             // Funci¢n para hallar la gravedad del protagonista

        IF (en_pantalla())      // Si el protagonista no baja
            IF (velocidad<>0)   // Y se mueve
                graph++;        // Cambia de gr fico para animarlo
                IF (graph>3)
                    graph=1;
                END
            ELSE
                graph=1;      // Si no se mueve ponen el gr fico por defecto
            END

 //Ahora si Mario va hacia la derecha o izquierda y de repente cambia
 //de direcci¢n, pone gr fico de frenado

         IF (velocidad>0 AND (key(_left) OR joy.left))
        graph=6;
        graph++;
        IF (graph>7)
            graph=6;
        flags=1;
        END
        END

         IF (velocidad<0 AND (key(_right) OR joy.right))
        graph=6;
        graph++;
        IF (graph>7)
            graph=6;
        flags=0;
        END
        END


        IF ((key(_down) OR joy.down))
        graph=9;
         END


        IF ((key(_c) OR joy.button4))
        mario(x-25,y-25,graph);
        signal((TYPE yoshy1),s_kill);
        sound(s_m_salto2,256,256);
        yoshy_prepared(x,y);
        yoshy=0;
        END


        IF ((key(_up) or joy.up) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_up) or joy.up) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END


// Lee que las teclas Control o el joystick
            IF (key(_control) OR joy.button3)
            IF (saltando==0)
            sound(s_salto,256,265);   // Inicia sonido
                velocidad_gravedad=-18;         // y salta
              END
            saltando++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            saltando=0;       // Tecla soltada
        END
     END

// Mientras Mario est‚ subiendo pon el gr fico de subir
        IF (ultima_y>y) graph=5; END
//Y si Mario est  agachado y salta, saltar  agachado
        IF ((key(_down) OR joy.down) AND (key(_control) OR joy.button3))
        graph=9;
         END



        IF (ultima_y<y)         // Comprueba si esta bajando
            graph=3;            // Pon el gr fico de bajar
            muerte=true;        // Si baja puede matar
//Y si Mario est  agachado y salta, saltar  agachado Y BAJARA agachado.
        IF ((key(_down) OR joy.down) AND (ultima_y<y))
        graph=9;
         END
        ELSE
            muerte=false;       // Si sube o esta quieto le pueden matan
        END



        // Comprueba colisi¢n con tortugas voladoras
            IF (id2=collision(TYPE tortugas))
            puntos+=400;
            ELSE
        // Comprueba colisi¢n con tortugas2
            IF (id2=collision(TYPE tortuga2))
            puntos+=400;
          END
          END

        // Comprueba colisi¢n con bloques sorpresa
            IF (id3=collision(TYPE bloque_sorpresa))
            puntos+=400;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);
            seta(id3.x+3,id3.y-5);
            END



        // Comprueba colisi¢n con setas
            IF (id3=collision(TYPE seta))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            sound(s_m_trans,256,256);   // Inicia sonido
            transformacion_con_Yoshy(id3.x,id3.y);
            ELSE
            IF (id3=collision(TYPE seta3))
            puntos+=0;
            signal(id3,s_kill);        // Elimina seta de pantalla
            sound(s_m_trans,256,256);   // Inicia sonido
            transformacion_con_Yoshy(id3.x,id3.y);
            END
            END



        // Comprueba colisi¢n con setas verdes
            IF (id3=collision(TYPE seta_vida))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            vidas++;                // Y suma una vida
            vida(id3.x,id3.y);
            END



        // Comprueba colisi¢n con bloques sorpresa2
            IF (id3=collision(TYPE bloque_sorpresa2))
            puntos+=400;
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            id_vida=seta_vida(id3.x+8,id3.y-5);
            END



            // Comprueba colisi¢n con bloques piedra
            IF (id3=collision(TYPE piedra))
            puntos+=0;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
          END



        // Comprueba colisi¢n con Plantas Carnivoras
        IF (collision (TYPE planta))
        muerte_con_Yoshy(x,y);
        END



        // Comprueba la colision con la barra de final del juego
        IF (collision (TYPE barra))
        pts_barra+=100;
        mario_stage_clear3();
        END



        // Comprueba si hay colisi¢n con los monedas
            IF (id3=collision(TYPE moneda))
            monedas++;                    // Incrementa numero de monedas
            puntos+=50;               // Suma puntos
            signal(id3,s_kill);         // Elimina los monedas de pantalla
            sound(s_moneda2,256,260);   // Inicia sonido
            IF (monedas>=100)              // Y si tiene mas de 10 monedas
                monedas-=100;              // Resta 10 monedas en la cuenta
                vidas++;                // Y suma una vida
                // Pone un mensaje de vida conseguida
                vida(id3.x,id3.y);
            END
        END




        IF (id2)        // Comprueba si ha habido cualquier tipo de colisi¢n
            IF (muerte)     // Y si esta bajando y puede matar
            IF (corriendo==0)
                velocidad_gravedad=-12; // Hace botar al protagonista + alto
                golpes(id2.x,id2.y);    // Crea estrella del golpe
                signal(id2,s_kill);     // Elimina al monstruo que ha originado la colisi¢n
                sound(s_y_mata,256,256);   // Inicia sonido
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
            velocidad_gravedad=-20;   // Hace botar al protagonista + bajo
            golpes(id2.x,id2.y);      // Crea estrella del golpe
            signal(id2,s_kill);       // Elimina al monstruo que ha originado la colisi¢n
            sound(s_y_mata,256,256);   // Inicia sonido
        END
            ELSE       // Si no puede matar, entonces el protagonista
            mario(x-25,y-25,graph);
            sound(s_yoshy_co,256,256);   // Inicia sonido
            signal((TYPE yoshy1),s_kill);
            yoshy_prepared(x,y);
            END
        END

        ultima_y=y;   // Guarda y en ultima_y para comprobar luego si baja
                      // Controla que Mario solo mate cuando este bajando

        IF (out_region(id,0)) // Si se sale de la pantalla se muere
            muerte_con_yoshy(x,y);   // Esta muerto
        END


        IF (cuenta_atras<=0) // Si el tiempo supera el minuto
            muerte_con_yoshy(x,y); // Esta muerto
            timeup(x,y);
        ELSE
        IF (cuenta_atras<=30) // Si el tiempo supera el minuto
        IF (sonando==0)
            sound(s_tiempo,256,256); // Avisa de que queda poco tiempo
        END
            sonando++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            sonando=0;       // Tecla soltada
        END
        END
        FRAME;
     END
END



//-----------------------------------------------------------------------

PROCESS yoshy2(x,y,graph)

PRIVATE
    saltando=0;      // Evita el salto continuo
    corriendo=0;
    incx=0;          // Variable para comprobar paredes
    ultima_y=0;      // Variable para mirar si sube
    ultima_veloc=0;  // Variable para comprobar la velocidad
    id2;             // Identificador de uso general
    id3;             // Identificador de Mario sin collision muerte_mario
    id4;

BEGIN
    estado_mario=4;
    yoshy=1;
    file=graficos2;
    ctype=c_scroll;
    scroll.camera=id;
    graph=20;
    priority=1;
    FRAME;

    LOOP
    IF ((key(_b) OR joy.button2) AND reserva==1)
    seta3(x,y);
    END




        // Lee el teclado y el joystick

        IF ((key(_right) OR joy.right) AND velocidad<5)
        IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo=0;
           ELSE
            corriendo--;
        END
        ELSE
        IF ((key(_left) OR joy.left) AND velocidad>-5)
        IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo=0;
           ELSE
            corriendo--;

           END
            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END


IF ((key(_right) or joy.right) AND (key(_v) or joy.button1) AND velocidad<17)
            IF (corriendo==0)
            velocidad+=2;           // Sube velocidad a la derecha
            flags=0;                // Mira a la derecha
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
           END

        ELSE
IF ((key(_left) or joy.left) AND (key(_v) or joy.button1) AND velocidad>-17)
            IF (corriendo==0)
                velocidad-=2;       // Sube velocidad a la izquierda
                flags=1;            // Mira a la izquierda
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
           END

            ELSE                    // Si no hay nada pulsado
                IF (velocidad>0)    // Frena a la derecha
                    velocidad--;
                END
                IF (velocidad<0)    // O frena a la izquierda
                    velocidad++;
                END
            END
        END



   // Mira si el protagonista choca con las paredes
   ultima_veloc=velocidad;                // Guarda la velocidad en 'ultima_veloc'
   IF (ultima_veloc<>0)                   // Y si se mueve
       incx=ultima_veloc/abs(ultima_veloc);          // Halla la direcci¢n
       WHILE (ultima_veloc<>0)            // 'ultima_veloc' sirve de contador
                // Comprueba que no de por debajo
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-2)/2)<>120)
               // Ni por encima
IF (map_get_pixel(fasex,99,(x+incx)/2,(y-32)/2)<>120)

                        x+=incx;    // Y si no da, mueve al mu¤eco
                    END
                END
                ultima_veloc-=incx;     // Resta 'ultima_veloc' hasta que sea cero
            END
        END


        gravedad();             // Funci¢n para hallar la gravedad de Yoshy

        IF (en_pantalla())      // Si el protagonista no baja
            IF (velocidad<>0)   // Y se mueve
                graph++;        // Cambia de gr fico para animarlo
                IF (graph>22)
                    graph=20;
                END
            ELSE
                graph=20;      // Si no se mueve ponen el gr fico por defecto
            END

 //Ahora si Mario va hacia la derecha o izquierda y de repente cambia
 //de direcci¢n, pone gr fico de frenado

         IF (velocidad>0 AND (key(_left) OR joy.left))
        graph=24;
        graph++;
        IF (graph>25)
            graph=24;
        flags=1;
        END
        END

         IF (velocidad<0 AND (key(_right) OR joy.right))
        graph=24;
        graph++;
        IF (graph>25)
            graph=24;
        flags=0;
        END
        END


        IF ((key(_down) OR joy.down))
        graph=29;
         END


        IF ((key(_c) OR joy.button4))
        mario2(x-25,y-25,graph);
        signal((TYPE yoshy2),s_kill);
        sound(s_m_salto2,256,256);
        yoshy_prepared(x,y);
        yoshy=0;
        END



        IF ((key(_up) or joy.up) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_right) or joy.right) AND velocidad>0)
         velocidad=FALSE;
         END
        IF ((key(_up) or joy.up) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END
        IF ((key(_down) or joy.down) AND (key(_left) or joy.left) AND velocidad<0)
         velocidad=FALSE;
         END


// Lee que las teclas Control o el joystick
            IF (key(_control) OR joy.button3)
            IF (saltando==0)
            sound(s_salto,256,265);   // Inicia sonido
                velocidad_gravedad=-18;         // y salta
              END
            saltando++;       //Tecla pulsada
        ELSE
            saltando=0;       // Tecla soltada
        END
     END

// Mientras Mario est‚ subiendo pon el gr fico de subir
        IF (ultima_y>y) graph=23; END
//Y si Mario est  agachado y salta, saltar  agachado
        IF ((key(_down) OR joy.down) AND (key(_control) OR joy.button3))
        graph=29;
         END



        IF (ultima_y<y)         // Comprueba si esta bajando
            graph=22;            // Pon el gr fico de bajar
            muerte=true;        // Si baja puede matar
//Y si Mario est  agachado y salta, saltar  agachado Y BAJARA agachado.
        IF ((key(_down) OR joy.down) AND (ultima_y<y))
        graph=29;
         END
        ELSE
            muerte=false;       // Si sube o esta quieto le pueden matan
        END



        // Comprueba colisi¢n con tortugas voladoras
            IF (id2=collision(TYPE tortugas))
            puntos+=400;
            ELSE
        // Comprueba colisi¢n con tortugas2
            IF (id2=collision(TYPE tortuga2))
            puntos+=400;
          END
          END


        // Comprueba colisi¢n con bloques sorpresa
            IF (id3=collision(TYPE bloque_sorpresa))
            puntos+=400;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);
            seta(id3.x+3,id3.y-5);
            END


        // Comprueba colisi¢n con bloques sorpresa2
            IF (id3=collision(TYPE bloque_sorpresa2))
            puntos+=400;
            signal(id3,s_kill);        // Elimina bloque_sorpresa de pantalla
            piedra(id3.x+2,id3.y+2);
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
            id_vida=seta_vida(id3.x+8,id3.y-5);
            END



        // Comprueba colisi¢n con setas
            IF (id3=collision(TYPE seta))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            reserva=1;
            id_seta=seta2(x,y,ctype);
            ELSE
            IF (id3=collision(TYPE seta3))
            puntos+=0;
            signal(id3,s_kill);        // Elimina seta de pantalla
            reserva=1;
            id_seta=seta2(x,y,ctype);
            END
            END



        // Comprueba colisi¢n con setas verdes
            IF (id3=collision(TYPE seta_vida))
            pts_seta(id3.x,id3.y);    // Crea estrella del golpe
            puntos+=1000;
            signal(id3,s_kill);        // Elimina seta de pantalla
            vidas++;                // Y suma una vida
            vida(id3.x,id3.y);
            END



            // Comprueba colisi¢n con bloques piedra
            IF (id3=collision(TYPE piedra))
            puntos+=0;
            velocidad_gravedad=+2;   // Hace botar al protagonista + bajo
          END




        // Comprueba colisi¢n con Plantas Carnivoras
        IF (collision (TYPE planta))
        muerte_yoshy();
        END



        // Comprueba la colision con la barra de final del juego
        IF (collision (TYPE barra))
        pts_barra+=100;
        mario_stage_clear4();
        END


        // Comprueba si hay colisi¢n con los monedas
            IF (id3=collision(TYPE moneda))
            monedas++;                    // Incrementa numero de monedas
            puntos+=50;               // Suma puntos
            signal(id3,s_kill);         // Elimina los monedas de pantalla
            sound(s_moneda2,256,260);   // Inicia sonido
            IF (monedas>=100)              // Y si tiene mas de 10 monedas
                monedas-=100;              // Resta 10 monedas en la cuenta
                vidas++;                // Y suma una vida
                // Pone un mensaje de vida conseguida
                vida(id3.x,id3.y);
            END
        END




        IF (id2)        // Comprueba si ha habido cualquier tipo de colisi¢n
            IF (muerte)     // Y si esta bajando y puede matar
            IF (corriendo==0)
                velocidad_gravedad=-12; // Hace botar al protagonista + alto
                golpes(id2.x,id2.y);    // Crea estrella del golpe
                signal(id2,s_kill);     // Elimina al monstruo que ha originado la colisi¢n
                sound(s_y_mata,256,256);   // Inicia sonido
            corriendo++;      // Tecla pulsada
        ELSE                  // Si el protagonista si baja
            corriendo=0;       // Tecla soltada
            velocidad_gravedad=-20;   // Hace botar al protagonista + bajo
            golpes(id2.x,id2.y);      // Crea estrella del golpe
            signal(id2,s_kill);       // Elimina al monstruo que ha originado la colisi¢n
            sound(s_y_mata,256,256);   // Inicia sonido
        END
            ELSE       // Si no puede matar, entonces el protagonista
            mario2(x-25,y-25,graph);
            sound(s_yoshy_co,256,256);   // Inicia sonido
            signal((TYPE yoshy2),s_kill);
            yoshy_prepared(x,y);
            sound(s_yoshy_co,256,256);   // Inicia sonido
            END
        END

        ultima_y=y;   // Guarda y en ultima_y para comprobar luego si baja
                      // Controla que Mario solo mate cuando este bajando

        IF (out_region(id,0)) // Si se sale de la pantalla se muere
            muerte_yoshy();   // Esta muerto
        END


        IF (cuenta_atras<=0) // Si el tiempo supera el minuto
            muerte_yoshy(); // Esta muerto
            timeup(x,y);
        ELSE
        IF (cuenta_atras<=30) // Si el tiempo supera el minuto
        IF (sonando==0)
            sound(s_tiempo,256,256); // Avisa de que queda poco tiempo
        END
            sonando++;
        ELSE
            sonando=0;
        END
        END
        FRAME;
     END
END




//-----------------------------------------------------------------------------
// Proceso para controlar la gravedad
//-----------------------------------------------------------------------------
PROCESS gravedad();

PRIVATE
    vgravedad;        // Variable para guardar temporalmente la gravedad

BEGIN

    // Coge la coordenada y a partir de la gravedad del protagonista (padre)
    vgravedad=(father.velocidad_gravedad+=2);   // Pero incrementada en 2
    IF (vgravedad>16)                           // Si es mayor de 16
        father.velocidad_gravedad=16;           // Haz que sea 16 la gravedad del protagonista
        vgravedad=16;                           // Y tambi‚n la variable temporal
    END

    IF (vgravedad<0)                        // Si la gravedad es negativa
        WHILE (vgravedad++!=0)              // Mientras no sea cero, la incrementa
            // Si no se da con el techo
IF (map_get_pixel(fasex,99,father.x/2,(father.y-24)/2)<>252)
                father.y--;                 // Haz que el protagonista suba
            ELSE                            // Si no pon la gravedad a cero
                father.velocidad_gravedad=0;
                BREAK;
            END
        END
    ELSE                                    // Si la gravedad es cero o positiva
        father.y+=vgravedad;                // Se suma a la coordenada del protagonista
        // Comprueba el rango vertical del movimiento del protagonista
        FROM vgravedad=-16 TO 7 STEP 2;     // Acaba el proceso si se toca el suelo
IF (map_get_pixel(fasex,99,father.x/2,(father.y+vgravedad)/2)==136)

               BREAK;                      // Sal si se encuentra con el suelo
          END
        END
        IF (vgravedad<8)                    // Si se encuentra con el suelo
            father.y+=vgravedad;            // Se actualiza con el m ximo que se pueda mover
            father.velocidad_gravedad=0;    // Y se pone la gravedad a 0
        END
    END
END

//-----------------------------------------------------------------------------
// Proceso para comprobar si se toca suelo
// Salidas: True/False, seg£n si se toca suelo o no
//-----------------------------------------------------------------------------

PROCESS en_pantalla()

BEGIN

IF (map_get_pixel(fasex,99,father.x/2,father.y/2)==136)
        RETURN(TRUE);   // Si se toca suelo el proceso devuelve true
    ELSE
        RETURN(FALSE);  // Si no, devuelve false
    END
END




//------------------------------------------------------------------------

PROCESS tortugas(graph,flags,xrelativa,yrelativa,incx,incy,distx,disty);

PRIVATE
    angulo3;

BEGIN
    ctype=c_scroll;      // haz que este dentro del scroll
    graph=156;            // Elige el gr fico inicial apropiado

    LOOP

        angulo2+=pi/incx;                 // Halla el tama¤o de los incrementos
        angulo3+=pi/incy;                 // que se har n en las dos coordenadas
        x=xrelativa+get_distx(angulo2,distx);    // Incrementa la coordenada x
        y=yrelativa+get_disty(angulo3,disty);    // Incrementa la coordenada y

        // Realiza la animaci¢n de los gr ficos
        graph++;
        IF (graph>158)
            graph=156;
        END
        FRAME;
    END
END




//-----------------------------------------------------------------------

PROCESS tortuga2(x,y,incr_x);

BEGIN
    graph=153;      // N£mero de gr fico
    ctype=c_scroll; // Lo mete dentro del scroll
    LOOP
        x+=incr_x;             // Mueve el gr fico horizontalmente
        // Si ha llegado al final de la plataforma
        IF (NOT en_pantalla())
            incr_x=-incr_x;        // Cambia el incremento horizontal
            x+=incr_x*2;       // Mueve un poco el gr fico
            // Da la vuelta al gr fico
            IF (flags==0)
                flags=1;
            ELSE
                flags=0;
            END
        END
        // Realiza la animaci¢n de los gr ficos
        graph++;
            IF (graph>155) graph=153; END
        FRAME;
    END
END



//----------------------------------------------------------------------

PROCESS Planta(x,y);

PRIVATE
Paso;
idp;

// Variable para controlar el descenso y aumento de la Planta
mov_y[]=87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,
        107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,
        124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,
        141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,
        158,159,160,161,162,163,164,165,166,167,168,169,170,171,

        170,169,168,167,
        166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,
        149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,
        132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,
        115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,
        98,97,96,95,94,93,92,91,90,89,88;


BEGIN
    graph=76;      // N£mero de gr fico
    ctype=c_scroll; // Lo mete dentro del scroll

    LOOP
        graph++;
        IF (graph>78)
            graph=76;
        END
        FRAME;
        y=mov_y[paso++];        // Anima el gr fico
        IF (paso==sizeof(mov_y))    // Si no hay mas gr ficos en la animaci¢n
        paso=0;                     // Sube la barra
        FRAME;
        END
    END
END

//----------------------------------------------------------------------

PROCESS Tapa_planta1(x,y);

BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    z=-512;
    graph=116;              // Pon el gr fico inicial del gr fico

    LOOP                    // Realiza la animaci¢n de los gr ficos
        FRAME;
    END
END



//----------------------------------------------------------------------

PROCESS Tapa_planta2(x,y);

BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    z=-512;
    graph=112;              // Pon el gr fico inicial del gr fico

    LOOP                    // Realiza la animaci¢n de los gr ficos
        FRAME;
    END
END




//-----------------------------------------------------------------------

PROCESS moneda(x,y);

BEGIN
    ctype=c_scroll;
    graph=40;              // Pon el gr fico inicial del gr fico
    LOOP                    // Realiza la animaci¢n de los gr ficos
        graph++;
        IF (graph>43)
            graph=40;
        END
        FRAME;
    END
END


//-----------------------------------------------------------------------

PROCESS gota(x,y);

BEGIN
    file=graficos2;
    ctype=c_scroll;
    graph=60;              // Pon el gr fico inicial del gr fico
    LOOP                    // Realiza la animaci¢n de los gr ficos
        graph++;
        IF (graph>62)
            graph=60;
        END
        FRAME;
    END
END


//-----------------------------------------------------------------------

PROCESS estrellitas(x,y);

BEGIN
    file=graficos2;
    ctype=c_scroll;
    graph=63;              // Pon el gr fico inicial del gr fico
    LOOP                    // Realiza la animaci¢n de los gr ficos
        graph++;
        IF (graph>65)
            graph=63;
        END
        FRAME;
    END
END




//------------------------------------------------------------------

PROCESS poste(x,y);

BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=305;              // Pon el gr fico inicial del gr fico

    LOOP                    // Realiza la animaci¢n de los gr ficos
        FRAME;
    END
END



//-----------------------------------------------------------------------

PROCESS barra();

PRIVATE
Paso;

// Variable para controlar el descenso de la barra
mov_y[]=87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,
        107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,
        124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,
        141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,
        158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,
        175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
        192,193,194,195,196,197,198,199,200,201,

        200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,
        183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,
        166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,
        149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,
        132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,
        115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,
        98,97,96,95,94,93,92,91,90,89,88;


BEGIN
    graph=306;      // N£mero de gr fico
    z=-512;
    ctype=c_scroll; // Lo mete dentro del scroll
    x=1398;

    LOOP
        y=mov_y[paso++];        // Anima el gr fico
        IF (paso==sizeof(mov_y))    // Si no hay mas gr ficos en la animaci¢n
        paso=0;                     // Sube la barra
        FRAME;
        END
        FRAME;
    END
END


//-----------------------------------------------------------------------

PROCESS  mario_stage_clear1();

PRIVATE
id2;
incr_x;


BEGIN
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=1;                  // Coge el gr fico de Mario Stage Clear
    id2=get_id(TYPE mario);    // Coge identificador de protagonista
    signal(id2,s_kill);       // Y du‚rmelo.
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    graph=18;
    FRAME(2400);
    REPEAT
        graph++;
        IF (graph>4)
            graph=1;
        END
        x+=incr_x;                // Mueve gr fico hacia el lado
        incr_x++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla
    FRAME;
    fin_fase=TRUE;
END



//-----------------------------------------------------------------------

PROCESS  mario_stage_clear2();

PRIVATE
id2;
incr_x;


BEGIN
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=20;                  // Coge el gr fico de Mario Stage Clear
    id2=get_id(TYPE mario2);    // Coge identificador de protagonista
    signal(id2,s_kill);       // Y du‚rmelo.
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    graph=38;
    FRAME(2400);
    REPEAT
        graph++;
        IF (graph>22)
            graph=20;
        END
        x+=incr_x;                // Mueve gr fico hacia el lado
        incr_x++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla
    FRAME;
    fin_fase=TRUE;
END




//-----------------------------------------------------------------------

PROCESS  mario_stage_clear3();

PRIVATE
id2;
incr_x;


BEGIN
    file=graficos2;
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=1;                  // Coge el gr fico de Mario Stage Clear
    id2=get_id(TYPE yoshy1);    // Coge identificador de protagonista
    signal(id2,s_kill);       // Y du‚rmelo.
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    graph=18;
    FRAME(2400);
    REPEAT
        graph++;
        IF (graph>3)
            graph=1;
        END
        x+=incr_x;                // Mueve gr fico hacia el lado
        incr_x++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla
    FRAME;
    fin_fase=TRUE;
END




//-----------------------------------------------------------------------

PROCESS  mario_stage_clear4();

PRIVATE
id2;
incr_x;


BEGIN
    file=graficos2;
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=20;                  // Coge el gr fico de Mario Stage Clear
    id2=get_id(TYPE yoshy2);    // Coge identificador de protagonista
    signal(id2,s_kill);       // Y du‚rmelo.
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    graph=38;
    FRAME(2400);
    REPEAT
        graph++;
        IF (graph>22)
            graph=20;
        END
        x+=incr_x;                // Mueve gr fico hacia el lado
        incr_x++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla
    FRAME;
    fin_fase=TRUE;
END



//-----------------------------------------------------------------------

PROCESS objeto(file,graph,x,y,z,ctype);

BEGIN
    LOOP
        FRAME;
    END
END




//-----------------------------------------------------------------------

PROCESS muerte_mario()

PRIVATE
    incr_y=-13;                // Variable para mover gr fico poco a poco

BEGIN
    stop_sound(canal1);   // Inicia sonido
    canal2=sound(s_m_muerto,256,256);   // Inicia sonido
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=8;                   // Coge el gr fico de Mario muerto
    priority=1;                          // Le da prioridad m xima
    signal(father,s_sleep);              // Duerme al proceso padre (el del protagonista)
    x=father.x;                          // Coge x del protagonista (proceso padre)
    y=father.y;                          // Coge y

    // Si esta en la fase uno
    IF (fase==1)
        scroll.camera=id;                // Hace que la c mara le siga
    END
    y-=13;                               // Mueve el gr fico hacia abajo

    REPEAT
            // Da la vuelta al gr fico
            IF (flags==0)
                flags=1;
            ELSE
                flags=0;
            END
        y+=incr_y;                // Mueve gr fico hacia arriba
        incr_y++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla

    // Apaga pantalla si le quedan vidas
    IF (vidas>0)
        fade_off();
    FRAME(2400);
    canal1=sound(s_musica,100,256);
    ELSE
        id_mario.x=father.x;
        id_mario.y=father.y;
        scroll.camera=id_mario;
    END
    vidas--;
    IF (vidas>-1)                       // Si no quedan vidas
      mario(10,198,1);
      END
        signal(father,s_kill);          // Elimina el protagonista anterior
        //enciende pantalla
        fade_on();
END



//-----------------------------------------------------------------------

PROCESS muerte_mario2()

PRIVATE
    id2;
    incr_y=-13;                // Variable para mover gr fico poco a poco

BEGIN
    stop_sound(canal1);   // Inicia sonido
    canal2=sound(s_m_muerto,256,256);   // Inicia sonido
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=8;                   // Coge el gr fico de Mario muerto
    priority=1;                          // Le da prioridad m xima
    id2=get_id(TYPE mario2);    // Coge identificador de protagonista
    signal(id2,s_sleep);       // Y du‚rmelo.
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario

    REPEAT
            // Da la vuelta al gr fico
            IF (flags==0)
                flags=1;
            ELSE
                flags=0;
            END
        y+=incr_y;                // Mueve gr fico hacia arriba
        incr_y++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla

    // Apaga pantalla si le quedan vidas
    IF (vidas>0)
    fade_off();
    FRAME(2400);
    canal1=sound(s_musica,100,256);
    ELSE
        id_mario.x=father.x;
        id_mario.y=father.y;
        scroll.camera=id_mario;
    END
    vidas--;
    IF (vidas>-1)                       // Si no quedan vidas
      mario(10,198,1);
       END
        signal(father,s_kill);          // Elimina el protagonista anterior
        //enciende pantalla
        fade_on();
    IF (vidas<0)
     fase=0;
    END
END




//-----------------------------------------------------------------------

PROCESS muerte_con_Yoshy(x,y)

PRIVATE
    id2;
    incr_y=-13;                // Variable para mover gr fico poco a poco

BEGIN
    file=graficos2;
    stop_sound(canal1);
    canal2=sound(s_m_muerto,256,256);
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=8;                   // Coge el gr fico de Mario muerto
    priority=1;                          // Le da prioridad m xima
    id2=get_id(TYPE yoshy1);    // Coge identificador de protagonista
    signal(id2,s_sleep);       // Y du‚rmelo.
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario

    REPEAT
        y+=incr_y;                // Mueve gr fico hacia arriba
        incr_y++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla

    // Apaga pantalla si le quedan vidas
    IF (vidas>0)
    fade_off();
    FRAME(2400);
    canal1=sound(s_musica,100,256);
    END
    vidas--;
    IF (vidas>-1)                       // Si no quedan vidas
      mario(10,198,1);
       END
        signal(father,s_kill);          // Elimina el protagonista anterior
        //enciende pantalla
        fade_on();
    IF (vidas<0)
     fase=0;
    END
END



//-----------------------------------------------------------------------

PROCESS muerte_yoshy()

PRIVATE
    id2;
    incr_y=-13;                // Variable para mover gr fico poco a poco

BEGIN
    stop_sound(canal1);   // Inicia sonido
    canal2=sound(s_m_muerto,256,256);   // Inicia sonido
    file=graficos2;
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=8;                   // Coge el gr fico de Mario muerto
    priority=1;                          // Le da prioridad m xima
    id2=get_id(TYPE yoshy2);    // Coge identificador de protagonista
    signal(id2,s_sleep);       // Y du‚rmelo.
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario

    REPEAT
        y+=incr_y;                // Mueve gr fico hacia arriba
        incr_y++;                 // Cada vez mas lento
        FRAME(125);
    UNTIL (out_region(id,0))    // Hasta que se salga de pantalla

    // Apaga pantalla si le quedan vidas
    IF (vidas>0)
    fade_off();
    FRAME(2400);
    canal1=sound(s_musica,100,256);   // Inicia sonido
    END
    vidas--;
    IF (vidas>-1)                       // Si no quedan vidas
      mario(10,198,1);
       END
        signal(father,s_kill);          // Elimina el protagonista anterior
        //enciende pantalla
        fade_on();
    IF (vidas<0)
     fase=0;
    END
END


//------------------------------------------------------------------------


PROCESS estrellas(x,y);

BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=140;              // Pon el gr fico inicial del gr fico

    LOOP                    // Realiza la animaci¢n de los gr ficos
        graph++;
        IF (graph>143)
            graph=140;
        END
        FRAME;
    END
END


//--------------------------------------------------------------------------

PROCESS vida(x,y);

PRIVATE
incr_y;

BEGIN
    sound(s_1vida,256,256);   // Inicia sonido
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=133;              // Pon el gr fico inicial del gr fico
    REPEAT
        y-=15;                               // Mueve el gr fico hacia abajo
        y+=incr_y;                       // Sigue bajando el gr fico
        incr_y++;                        // Cada vez mas despacio
        angle+=pi/4;
        size=500;
        FRAME;
    UNTIL (out_region(id,0));              // Mientras o salga de pantalla
END


//--------------------------------------------------------------------------

PROCESS timeup(x,y);

PRIVATE
incr_y;

BEGIN
    timer=0;
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=172;              // Pon el gr fico inicial del gr fico
    REPEAT
        y-=15;                               // Mueve el gr fico hacia abajo
        y+=incr_y;                       // Sigue bajando el gr fico
        incr_y++;                        // Cada vez mas despacio
        size=300;
        FRAME;
    UNTIL (out_region(id,0));              // Mientras o salga de pantalla
END


//-----------------------------------------------------------------------

PROCESS golpes(x,y);

PRIVATE
    incr_x;                    // Incremento horizontal

BEGIN
    ctype=c_scroll;            // Haz que este dentro del scroll
    graph=130;                 // Pon el gr fico inicial de la estrella
        FRAME(400);
    graph=131;
        FRAME(300);
END


//----------------------------------------------------------------------

PROCESS pts_seta(x,y);

PRIVATE
    incr_x;                    // Incremento horizontal

BEGIN
    ctype=c_scroll;            // Haz que este dentro del scroll
    graph=130;                 // Pon el gr fico inicial de la estrella
        FRAME(400);
    graph=136;
        FRAME(300);
END



//----------------------------------------------------------------------

PROCESS bl_gira(x,y);

BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=148;              // Pon el gr fico inicial del gr fico

    LOOP                    // Realiza la animaci¢n de los gr ficos
        IF (collision (TYPE mario))
        IF (collision (TYPE mario2))
        graph++; END
        IF (graph>151)
            graph=148;
        END
        END
        FRAME;
     END
   END


//-------------------------------------------------------------------------

PROCESS bloque_sorpresa(x,y);

PRIVATE
id_bs;

BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=144;              // Pon el gr fico inicial del gr fico
    LOOP
        graph++;
        IF (graph>147)
            graph=144;
          END
        FRAME;
    END
END



//------------------------------------------------------------------------

PROCESS bloque_sorpresa2(x,y);

PRIVATE
id_bs;

BEGIN

    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=144;              // Pon el gr fico inicial del gr fico
    LOOP
        graph++;
        IF (graph>147)
            graph=144;
          END
        FRAME;
    END
END



//-----------------------------------------------------------------------

PROCESS piedra(x,y);

PRIVATE
id_piedra;

BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=152;
     LOOP
        FRAME;
    END
END



//------------------------------------------------------------------------

PROCESS Huevo(x,y);

BEGIN
    sound(s_yoshy1,256,256);   // Inicia sonido
    file=graficos2;
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=50;                   // Coge el gr fico de Mario muerto
    z=-512;


        graph=50;
        FRAME(400);

        graph=51;
        FRAME(400);

        graph=52;
        FRAME(400);


        graph=40;
        FRAME(400);

        graph=41;
        FRAME(400);

        graph=40;
        FRAME(400);

        graph=41;
        FRAME(400);

        graph=40;
        FRAME(400);

        graph=41;
        FRAME(400);

        graph=42;
        FRAME(400);

        graph=43;
        FRAME(400);
        Yoshy_prepared(x,y);
END


//-----------------------------------------------------------------------

PROCESS Yoshy_prepared(x,y);

PRIVATE
    id3;
    x_inicial;
    y_inicial;
    x_final;
    y_final;

BEGIN
    file=graficos2;
    ctype=c_scroll;            // Integra el gr fico en el scroll
    graph=43;                   // Coge el gr fico de Mario muerto
    z=-512;

        LOOP
        graph++;
        IF (graph>44)
            graph=43;
        FRAME;
        END
        x_inicial=x;
        y_inicial=y;
        x_final=x;
        y_final=y--;
        FRAME;
        IF (y_final>1)
        y_final=y++;
        FRAME(400);
        gravedad();
        FRAME;
        END

     IF (collision (TYPE mario))
    sound(s_yoshy2,256,256);   // Inicia sonido
    id3=get_id(TYPE mario);    // Coge identificador de protagonista
    x=id3.x;                   // Coge la x de Mario
    y=id3.y;                   // Coge la y de Mario
    yoshy1(id3.x,id3.y,graph);
    signal(id3,s_kill);          // Elimina el protagonista anterior
    break;
    ELSE
     IF (collision (TYPE mario2))
    sound(s_yoshy2,256,256);   // Inicia sonido
    id3=get_id(TYPE mario2);    // Coge identificador de protagonista
    x=id3.x;                   // Coge la x de Mario
    y=id3.y;                   // Coge la y de Mario
    yoshy2(id3.x,id3.y,graph);
    signal(id3,s_kill);          // Elimina el protagonista anterior
    break;
    FRAME;
    END
    END
    END
END



//-----------------------------------------------------------------------

PROCESS seta(x,y);

PRIVATE
incr_x;

BEGIN
    sound(s_seta,256,256);   // Inicia sonido
    sound(s_coger,256,256);   // Inicia sonido
    graph=70;      // N£mero de gr fico
    ctype=c_scroll; // Lo mete dentro del scroll
    incr_x=5;

    LOOP
        gravedad();    // Funci¢n para hallar la gravedad de la seta
    // Mira si la seta choca con las paredes
    // Comprueba que no de por debajo
            IF (map_get_pixel(fasex,99,(x)/2,(y-2)/2)<>120)
    // Ni que de por encima
            IF (map_get_pixel(fasex,99,(x)/2,(y-32)/2)<>120)
            x+=incr_x;    // Y si no da, mueve a la seta
        END
        END
        IF (out_region(id,0)) // Si se sale de la pantalla desaparece
        break;
        END
        FRAME;
    END
END



//----------------------------------------------------------------------

PROCESS seta_vida(x,y);

PRIVATE
incr_x;

BEGIN
    sound(s_seta,256,256);   // Inicia sonido
    graph=71;                // N£mero de gr fico
    ctype=c_scroll;          // Lo mete dentro del scroll
    incr_x=5;

    LOOP
        gravedad();          // Funci¢n para hallar la gravedad de la seta
    // Mira si la seta choca con las paredes
    // Comprueba que no de por debajo
            IF (map_get_pixel(fasex,99,(x)/2,(y-2)/2)<>120)
    // Ni que de por encima
            IF (map_get_pixel(fasex,99,(x)/2,(y-32)/2)<>120)
            x+=incr_x;    // Y si no da, mueve a la seta
        END
        END
        IF (out_region(id,0)) // Si se sale de la pantalla desaparece
        break;
        END
        FRAME;
    END
END



//-----------------------------------------------------------------------

PROCESS seta2(x,y,ctype);
BEGIN
sound(s_coger,256,256);
sound(s_seta,256,256);
reserva=1;
    graph=70;      // N£mero de gr fico
    x=160;
    y=19;
    ctype=0; // Lo mete dentro del scroll
    LOOP
    IF (reserva==0)
    BREAK;
    END
    FRAME;
    END
END



//------------------------------------------------------------------------

PROCESS seta3(x,y);

PRIVATE
incr_y;
id2;

BEGIN
    sound(s_coge_seta,256,256);
    reserva=0;
    ctype=0;  // Lo mete dentro del scroll
    graph=70;      // N£mero de gr fico
    id2=get_id(TYPE seta2);    // Coge identificador de protagonista
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    incr_y=5;

    LOOP
    y+=incr_y;    // Y si no da, mueve a la seta

            // Hace la seta tranparente
            IF (flags==0)
                flags=4;
            ELSE
                flags=0;
            END

    IF (out_region(id,0)) // Si se sale de la pantalla desaparece
    break;
    END
    FRAME;
    END
END



//-----------------------------------------------------------------------

PROCESS transformacion(x,y);

PRIVATE
    id2;

BEGIN
    ctype=c_scroll;        // Haz que este dentro del scroll
    graph=1;
    id2=get_id(TYPE mario);    // Coge identificador de protagonista
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    mario2(id2.x,id2.y,graph);
    signal(father,s_kill);          // Elimina el protagonista anterior
        graph=14;
        FRAME;

        graph=1;
        FRAME;

        graph=14;
        FRAME;

        graph=1;
        FRAME;

        graph=14;
        FRAME;

        graph=15;
        FRAME;

        graph=14;
        FRAME;

        graph=15;
        FRAME;
END




//-----------------------------------------------------------------------

PROCESS transformacion2(x,y);

PRIVATE
    id2;

BEGIN
    IF (reserva==1)
    seta3(x,y);
    ELSE
    reserva=0;
    FRAME;
    END

    ctype=c_scroll;        // Haz que este dentro del scroll
    graph=15;              // Pon el gr fico inicial del gr fico
    id2=get_id(TYPE mario2);    // Coge identificador de protagonista
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    mario(id2.x,id2.y,graph);
    signal(father,s_kill);          // Elimina el protagonista anterior
        graph=14;
        FRAME;
        graph=15;
        FRAME;
        graph=14;
        FRAME;
        graph=15;
        FRAME;
        graph=14;
        FRAME;
        graph=1;
        FRAME;
        graph=14;
        FRAME;
        graph=1;
        FRAME;
END





//-------------------------------------------------------------------------

PROCESS transformacion_con_Yoshy(x,y);

PRIVATE
    id2;

BEGIN
    ctype=c_scroll;        // Haz que este dentro del scroll
    graph=1;
    id2=get_id(TYPE yoshy1);    // Coge identificador de protagonista
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    yoshy2(id2.x,id2.y,graph);
    signal(father,s_kill);          // Elimina el protagonista anterior
        graph=14;
        FRAME;

        graph=1;
        FRAME;

        graph=14;
        FRAME;

        graph=1;
        FRAME;

        graph=14;
        FRAME;

        graph=15;
        FRAME;

        graph=14;
        FRAME;

        graph=15;
        FRAME;
END



//-------------------------------------------------------------------------

PROCESS Destransformacion_con_Yoshy(x,y);      // Se vuelve peque¤o otra vez

PRIVATE
    id2;

BEGIN
    IF (reserva==1)
    seta3(x,y);
    ELSE
    reserva=0;
    FRAME;
    END

    ctype=c_scroll;        // Haz que este dentro del scroll
    graph=15;              // Pon el gr fico inicial del gr fico
    id2=get_id(TYPE yoshy2);    // Coge identificador de protagonista
    x=id2.x;                   // Coge la x de Mario
    y=id2.y;                   // Coge la y de Mario
    mario(id2.x,id2.y,graph);
    signal(father,s_kill);          // Elimina el protagonista anterior
        graph=14;
        FRAME;
        graph=15;
        FRAME;
        graph=14;
        FRAME;
        graph=15;
        FRAME;
        graph=14;
        FRAME;
        graph=1;
        FRAME;
        graph=14;
        FRAME;
        graph=1;
        FRAME;
END



//------------------------------------------------------------------------

PROCESS paleta(x,y,z);
BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=135;
     LOOP
        FRAME;
    END
END



//-------------------------------------------------------------------------

PROCESS tuberiax(x,y,z)
BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    x=15;
    y=0;
    z=-512;
    graph=119;
    LOOP
    FRAME;
END
END



//-------------------------------------------------------------------------

PROCESS tuberia(x,y);
BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=118;              // Pon el gr fico inicial del gr fico
    x=1447;
    y=193;
     LOOP
     IF (collision (type mario))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (TYPE mario2))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (type yoshy1))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (type yoshy2))
     fin_fase=true;         // Acaba la fase

     END
     END
     END
     END
        FRAME;
    END
END



//-----------------------------------------------------------------------

PROCESS tuberia2(x,y);
BEGIN
    graph=118;              // Pon el gr fico inicial del gr fico
     ctype=c_scroll;
     LOOP

     IF (collision (type mario))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (TYPE mario2))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (type yoshy1))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (type yoshy2))
     fin_fase=true;         // Acaba la fase

     END
     END
     END
     END
        FRAME;
    END
END



//-----------------------------------------------------------------------

PROCESS tuberia3(x,y);
BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=118;              // Pon el gr fico inicial del gr fico
    x=1447;
    y=193;
     LOOP

     IF (collision (type mario))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (TYPE mario2))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (type yoshy1))
     fin_fase=true;         // Acaba la fase

     ELSE
     IF (collision (type yoshy2))
     fin_fase=true;         // Acaba la fase

     END
     END
     END
     END
        FRAME;
    END
END



//-----------------------------------------------------------------------

PROCESS tuberia4(x,y);
BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=118;              // Pon el gr fico inicial del gr fico
    x=1447;
    y=193;
     LOOP

     IF (collision (type mario))
     fin_fase=true;   // Acaba la fase

     ELSE
     IF (collision (TYPE mario2))
     fin_fase=true;   // Acaba la fase

     ELSE
     IF (collision (type yoshy1))
     fin_fase=true;   // Acaba la fase

     ELSE
     IF (collision (type yoshy2))
     fin_fase=true;   // Acaba la fase

     END
     END
     END
     END
        FRAME;
    END
END




//-----------------------------------------------------------------------

PROCESS tuberia5(x,y);
BEGIN
    ctype=c_scroll;         // Haz que este dentro del scroll
    graph=118;              // Pon el gr fico inicial del gr fico
    x=1447;
    y=193;
     LOOP

     IF (collision (type mario))
     fin_fase=true;   // Acaba la fase

     ELSE
     IF (collision (TYPE mario2))
     fin_fase=true;   // Acaba la fase

     ELSE
     IF (collision (type yoshy1))
     fin_fase=true;   // Acaba la fase

     ELSE
     IF (collision (type yoshy2))
     fin_fase=true;   // Acaba la fase

     END
     END
     END
     END
        FRAME;
    END
END


//-----------------------------------------------

PROCESS FONDO();

BEGIN

            // Termina procesos de objetos que tapan gr ficos
            signal(TYPE objeto,s_kill);
            // Borra todos los textos que haya
            delete_text(all_text);
            reserva=0;// Pone la reserva a cero

    graph=110;
    WRITE(fuente3,10,05,0,'POSICION  :');WRITE_INT(fuente3,80,05,0,OFFSET P_X);
    WRITE(fuente3,115,05,0,'(MAX:11860)');
    WRITE(fuente3,10,15,0,'TIEMPO    :');WRITE_INT(fuente3,80,15,0,OFFSET IND);
    WRITE(fuente3,115,15,0,'(MAX:1200)');
    WRITE(fuente3,10,190,0,'VELOCIDAD :');WRITE_INT(fuente3,95,190,0,OFFSET VEL);
    SIZE=190;
    LOOP
        X=220-P_X;
        Y=105-P_Y;
        FRAME;
    END;
END;


//-----------------------------------

PROCESS Mario_volando();

PRIVATE
IDST;
INDI;

BEGIN
    file=graficos2;
    X=30;
    Y=100;
    GRAPH=70;

    LOOP
        graph++;
        IF (graph>73)
            graph=70;
        END

        // Haz un sonido de viento de vez en cuando
        IF (rand(0,48)==0) sound(s_viento,256,256);
        END


        P_X=P_X+VEL;
        IF (KEY(_UP) or joy.up AND (VEL>0))
            P_Y=P_Y-5;
        END;
        IF (KEY(_DOWN) or joy.down AND (VEL>0))
            P_Y=P_Y+5;
        END;
        IF (KEY(_LEFT) or joy.left)
            IF (VEL>3)
                VEL-=1;
            END;
        END;
        IF (KEY(_RIGHT) or joy.right)
            IF (VEL<15)
                VEL+=1;
            END;
        END;
        IF (VEL>0)
            IND+=1;
        END;
        IF (COLLISION(TYPE FONDO) )
            sound(s_y_mata,256,256);
            BREAK;
        END;
        IF (IND>12000)
            IDST=WRITE(fuente3,40,60,0,'TIEMPO');
            FROM INDI=1 TO 30;
                FRAME;
            END;
            DELETE_TEXT(IDST);
            BREAK;
        END;

        IF (P_X>11860)
        IDST=WRITE(fuente,40,60,0,'­Congratulations!');
        FIN_FASE=TRUE;
        END
        FRAME;
        END

    VEL=0;
    FROM GRAPH=3 TO 11;
        FRAME;
    END;
    P_X=0;
    P_Y=0;
    IND=0;
    DIF=0;
    Mario_volando();
END;



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//--------- FINAL DEL JUEGO: JOER SI HA COSTAO DE HACERLO!! ------------
//-----------------------------------------------------------------------
//----------  Y ESO QUE EL MARIO A SIMPLE VISTA PARECE SENCILLO ---------
//----------  PUES ALE QUE NO, QUE INTENTE ALGUIEN A HACERLO Y  ---------
//----------  SE DARA CUENTA DE LO QUE CUESTA, Y MAS A MI QUE NO --------
//----------  TENIA NI ZORRA EN PROGRAMACION... -------------------------
//
//            E-MAIL: hellspawn@iberonline.es
//
//    MARIO, YOSHY Y TODOS LOS DEMAS PERSONAJES QUE APARECEN EN ESTE JUEGO
//    SON MARCAS REGISTRADAS POR "NINTENDO"
//
//
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------