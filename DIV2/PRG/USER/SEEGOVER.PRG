//---------------------------------------------------------------------------
// Programa:   SEEGOVER
// Autor:      LUIS HERRADOR GONZALEZ
// E-mail:     herrador@arrakis.es
// Empezado:   21 Febrero 98
// Terminado:  29 Marzo 98
//---------------------------------------------------------------------------

PROGRAM seegover;

GLOBAL
    STRUCT tablero[24]      // Tablero
        color;              // Color de la pieza
        id_pieza;           // Identificador del proceso
    END

    fin_fase_1=FALSE;       // Marca los finales de las fases
    fin_fase_2=FALSE;
    fin_movimiento=FALSE;
    fin_retorno=FALSE;
    menu_activo=FALSE;
    finalizando_p=FALSE;
    vuelta_al_menu=FALSE;   // Marca la vuelta al men£ al final de una partida
    n_blancas=0;            // N£mero de piezas del jugador 1 (blanco)
    n_rojas=0;              // N£mero de piezas del jugador 2 (rojo)
    v_jug1=0;               // Victorias del primer jugador
    v_jug2=0;               // Victorias del segundo jugador
    casilla_libre=12;
    modo_juego=1;           // 1=Humano vs ordenador
                            // 2=Ordenador vs humano
                            // 3=Humano vs humano
                            // 4=Ordenador vs ordenador
    s_colocar;              // Sonido al colocar una pieza
    s_mover;                // Sonido al mover una pieza
    STRUCT arbol[7]         // Arbol de jugadas (posiciones)
        posicion[24];           //Posici¢n
    END
    mejor_jug;              // Mejor jugada encontrada para una posici¢n
    f_piezas;               // Fuente para n£mero de piezas
    f_intro;                // Fuente de la pantalla de introducci¢n
    f_marcador;             // Fuente del marcador
    hay_retorno=FALSE;      // Hay movimiento de retorno
    nivel_dif=1;            // 0=Nivel de dificultad f cil
                            // 1=Id. normal
                            // 2=Id. dif¡cil
    profundidad=3;          // Profundidad de an lisis del ordenador
    ult_jugadas[49];        // Para comprobar empate por repeticion de jugadas
    empate=FALSE;           // Indica si hay empate por repetici¢n de jugadas

LOCAL
    casilla;                // Casilla del tablero sobre la que est  la pieza

PRIVATE

BEGIN
    inicializaciones();
    LOOP
        menu();
        WHILE (menu_activo);
            FRAME;
        END
        REPEAT
            nueva_partida();
            primera_fase();
            REPEAT
                FRAME;
            UNTIL (fin_fase_1);
            segunda_fase();
            REPEAT
                FRAME;
            UNTIL (fin_fase_2);
            fin_partida();
            WHILE (finalizando_p)
                FRAME;
            END
        UNTIL (vuelta_al_menu);
    END
END


//---------------------------------------------------------------------------
// Proceso Inicializaciones
// Carga ficheros y define resolucion de pantalla
//---------------------------------------------------------------------------

PROCESS inicializaciones();

BEGIN
    max_process_time=5000;           // Para dar tiempo a pensar al ordenador
    f_piezas=load_fnt("seegover\f3.fnt");
    f_intro=load_fnt("seegover\f5.fnt");
    f_marcador=load_fnt("seegover\f4.fnt");
    s_colocar=load_pcm("seegover\colocar.pcm",0);
    s_mover=load_pcm("seegover\mover.pcm",0);
    load_fpg("seegover\seega2.fpg");
    load_fpg("seegover\intro.fpg");
    load_pal("seegover\intro.fpg");
    set_mode(m640x480);
END


//---------------------------------------------------------------------------
// Proceso Menu
// Presenta el men£ inicial
//---------------------------------------------------------------------------

PROCESS menu();

PRIVATE
    opcion=0;                   // Opci¢n elegida del men£ de opciones
    t1,t2;                      // Identificadores de los textos

BEGIN

    // Inicia los marcadores
    v_jug1=0;
    v_jug2=0;

    load_pal("seegover\intro.fpg");
    menu_activo=TRUE;
    put_screen(1,100);
    fade_on();
    WHILE (fading)
        FRAME;
    END

    // Escribe las opciones del menu
    SWITCH (modo_juego)
        CASE 1:
            t1=write(f_intro,23,330,0,"Jugador 1  vs  Ordenador");
        END
        CASE 2:
            t1=write(f_intro,23,330,0,"Ordenador  vs  Jugador 2");
        END
        CASE 3:
            t1=write(f_intro,23,330,0,"Jugador 1  vs  Jugador 2");
        END
        CASE 4:
            t1=write(f_intro,23,330,0,"Ordenador  vs  Ordenador");
        END
    END
    SWITCH (nivel_dif)
        CASE 0:
            t2=write(f_intro,23,355,0,"Nivel:  Facil");
        END
        CASE 1:
            t2=write(f_intro,23,355,0,"Nivel:  Normal");
        END
        CASE 2:
            t2=write(f_intro,23,355,0,"Nivel:  Dificil");
        END
    END
    write(f_intro,23,380,0,"Instrucciones");
    write(f_intro,23,405,0,"Jugar");
    write(f_intro,23,430,0,"Salir");
    write(f_intro,616,405,2,"Luis Herrador");
    write(f_intro,616,430,2,"herrador@arrakis.es");
    FRAME;

    // Hace visible el puntero del rat¢n
    mouse.file=1;
    mouse.graph=10;

    LOOP
        // Comprueba si se pulsa sobre alguna opci¢n del men£
        REPEAT
            opcion=0;
            IF (mouse.x>22 and mouse.x<295 and mouse.y>329 and mouse.y<352)
                opcion=1;
            END
            IF (mouse.x>22 and mouse.x<158 and mouse.y>354 and mouse.y<377)
                opcion=2;
            END
            IF (mouse.x>22 and mouse.x<170 and mouse.y>379 and mouse.y<401)
                opcion=3;
            END
            IF (mouse.x>22 and mouse.x<83 and mouse.y>404 and mouse.y<427)
                opcion=4;
            END
            IF (mouse.x>22 and mouse.x<73 and mouse.y>429 and mouse.y<452)
                opcion=5;
            END
            FRAME;
        UNTIL (opcion!=0 and mouse.left);

        //Espera que se deje de pulsar el bot¢n del rat¢n
        WHILE (mouse.left)
            FRAME;
        END

        SWITCH (opcion)

            CASE 1:                 // Cambio de jugadores
                modo_juego++;
                IF (modo_juego==5) modo_juego=1; END
                delete_text(t1);
                SWITCH (modo_juego)
                    CASE 1:
                        t1=write(f_intro,23,330,0,"Jugador 1  vs  Ordenador");
                    END
                    CASE 2:
                        t1=write(f_intro,23,330,0,"Ordenador  vs  Jugador 2");
                    END
                    CASE 3:
                        t1=write(f_intro,23,330,0,"Jugador 1  vs  Jugador 2");
                    END
                    CASE 4:
                        t1=write(f_intro,23,330,0,"Ordenador  vs  Ordenador");
                    END
                END
            END

            CASE 2:                 // Cambio de nivel de dificultad
                nivel_dif=(nivel_dif+1)%3;
                delete_text(t2);
                SWITCH (nivel_dif)
                    CASE 0:
                        t2=write(f_intro,23,355,0,"Nivel:  Facil");
                    END
                    CASE 1:
                        t2=write(f_intro,23,355,0,"Nivel:  Normal");
                    END
                    CASE 2:
                        t2=write(f_intro,23,355,0,"Nivel:  Dificil");
                    END
                END
            END

            CASE 3:                 // Instrucciones
                delete_text(all_text);
                put_screen(1,101);
                REPEAT
                    FRAME;
                UNTIL (mouse.left OR key(_esc));
                WHILE (mouse.left)  // Espera a que se deje de pulsar el rat¢n
                    FRAME;
                END
                put_screen(1,100);
                SWITCH (modo_juego)
                    CASE 1:
                        t1=write(f_intro,23,330,0,"Jugador 1  vs  Ordenador");
                    END
                    CASE 2:
                        t1=write(f_intro,23,330,0,"Ordenador  vs  Jugador 2");
                    END
                    CASE 3:
                        t1=write(f_intro,23,330,0,"Jugador 1  vs  Jugador 2");
                    END
                    CASE 4:
                        t1=write(f_intro,23,330,0,"Ordenador  vs  Ordenador");
                    END
                END
                SWITCH (nivel_dif)
                    CASE 0:
                        t2=write(f_intro,23,355,0,"Nivel:  Facil");
                    END
                    CASE 1:
                        t2=write(f_intro,23,355,0,"Nivel:  Normal");
                    END
                    CASE 2:
                        t2=write(f_intro,23,355,0,"Nivel:  Dificil");
                    END
                END
                write(f_intro,23,380,0,"Instrucciones");
                write(f_intro,23,405,0,"Jugar");
                write(f_intro,23,430,0,"Salir");
                write(f_intro,616,405,2,"Luis Herrador");
                write(f_intro,616,430,2,"herrador@arrakis.es");
            END

            CASE 4:                 // Jugar
                fade_off();
                delete_text(all_text);
                BREAK;
            END

            CASE 5:                 // Salir
                fade_off();
                delete_text(all_text);
                exit("Gracias por jugar a Seegover!",0);
            END
        END
        FRAME;
    END
    mouse.graph=0;                  // Oculta el puntero del rat¢n
    menu_activo=FALSE;
END


//---------------------------------------------------------------------------
// Proceso nueva_partida
// Inicializa las variables para el comienzo de una nueva partida
//---------------------------------------------------------------------------

PROCESS nueva_partida();

PRIVATE
    m;                      // Para bucles

BEGIN

    // Limpia de piezas el tablero
    FROM m=0 TO 24;
        tablero[m].color=0;
        tablero[m].id_pieza=0;
    END

    // Inicializa variables
    fin_fase_1=false;
    fin_fase_2=false;
    n_blancas=0;
    n_rojas=0;
    casilla_libre=12;
    IF (nivel_dif==0 OR nivel_dif==1)
        profundidad=3;
    ELSE
        profundidad=7;
    END
    FROM m=0 TO 49;
        ult_jugadas[m]=0;
    END
    empate=FALSE;

    // Carga gr ficos
    load_pal("seegover\seega2.fpg");
    put_screen(0,100);
    put(0,3,500,316);
    put(0,4,500,372);

    // Escribe el n£mero de piezas de cada bando
    write_int(f_piezas,580,316,5,&n_blancas);
    write_int(f_piezas,580,372,5,&n_rojas);

    // Pone los textos del marcador
    IF (modo_juego==1 or modo_juego==3)
        write(f_piezas,533,56,1,"JUGADOR 1");
    ELSE
        write(f_piezas,533,56,1,"ORDENADOR");
    END
    IF (modo_juego==2 or modo_juego==3)
        write(f_piezas,533,168,1,"JUGADOR 2");
    ELSE
        write(f_piezas,533,168,1,"ORDENADOR");
    END
    write_int(f_marcador,533,105,4,&v_jug1);
    write_int(f_marcador,533,217,4,&v_jug2);

    fade_on();
    while (fading)
        FRAME;
    end
END


//---------------------------------------------------------------------------
// Proceso primera_fase
// Primera fase de la partida (Colocaci¢n de las piezas)
//---------------------------------------------------------------------------

PROCESS primera_fase();

PRIVATE
    num_piezas=0;           // N£mero de piezas colocadas

BEGIN
    REPEAT
        IF (modo_juego==1 or modo_juego==3)
            colocar_jugador(1);
            REPEAT
                FRAME;
            UNTIL (son.graph==1);
            colocar_jugador(1);
            REPEAT
                FRAME;
            UNTIL (son.graph==1);
        ELSE
            timer=0;
            WHILE (timer<50)
                FRAME;
            END
            colocar_pc(1);
            timer=0;
            WHILE (timer<50)
                FRAME;
            END
            colocar_pc(1);
        END
        IF (modo_juego==2 or modo_juego==3)
            colocar_jugador(2);
            REPEAT
                FRAME;
            UNTIL (son.graph==2);
            colocar_jugador(2);
            REPEAT
                FRAME;
            UNTIL (son.graph==2);
        ELSE
            timer=0;
            WHILE (timer<50)
            FRAME;
            END
            colocar_pc(2);
            timer=0;
            WHILE (timer<50)
            FRAME;
            END
            colocar_pc(2);
        END
        num_piezas+=4;
    UNTIL (num_piezas==24)
    fin_fase_1=true;
END


//---------------------------------------------------------------------------
// Proceso segunda_fase
// Segunda fase de la partida (Movimiento de las piezas)
//---------------------------------------------------------------------------

PROCESS segunda_fase();

BEGIN
    REPEAT
        fin_movimiento=FALSE;
        IF (modo_juego==1 or modo_juego==3)
            mover_jugador(1);
        ELSE
            mover_pc(1);
        END
        REPEAT
            FRAME;
        UNTIL (fin_movimiento);
        IF (n_blancas==0 or n_rojas==0)
            BREAK;
        END
        fin_movimiento=FALSE;
        IF (modo_juego==2 or modo_juego==3)
            mover_jugador(2);
        ELSE
            mover_pc(2);
        END
        REPEAT
            FRAME;
        UNTIL (fin_movimiento);
    UNTIL (n_blancas==0 or n_rojas==0 or empate);
    fin_fase_2=true;
END


//---------------------------------------------------------------------------
// Proceso fin_partida
// Termina la partida
//---------------------------------------------------------------------------

PROCESS fin_partida();

PRIVATE m,t;

BEGIN

    finalizando_p=TRUE;

    // Escribe texto del vencedor de la partida
    IF (n_rojas==0)
        v_jug1++;
        IF (modo_juego==1 or modo_juego==3)
            t=write(1,225,240,4,"Ha ganado el Jugador 1!");
        ELSE
            t=write(1,225,240,4,"Ha ganado el Ordenador!");
        END
    ELSE
        IF (n_blancas==0)
            v_jug2++;
            IF (modo_juego==2 or modo_juego==3)
                t=write(1,225,240,4,"Ha ganado el Jugador 2!");
            ELSE
                t=write(1,225,240,4,"Ha ganado el Ordenador!");
            END
        ELSE
            t=write(1,225,240,4,"Empate!");
        END
    END

    // Permite al jugador volver al men£ si lo desea
    timer=0;
    LOOP
        IF (key(_esc) or mouse.right)
            vuelta_al_menu=TRUE;
            BREAK;
        END
        IF (mouse.left or timer>1000)
            vuelta_al_menu=FALSE;
            BREAK;
        END
        FRAME;
    END

    // Elimina los procesos de las piezas
    FROM m=0 TO 24;
        IF (tablero[m].id_pieza!=0);
            signal(tablero[m].id_pieza,s_kill);
            FRAME;
        END
    END

    // Borra los textos y apaga la pantalla
    delete_text(all_text);
    fade_off();

    // Desactiva indicador de finalizando la partida y termina el proceso
    finalizando_p=FALSE;
END

//---------------------------------------------------------------------------
// Proceso colocar_jugador
// Colocaci¢n inicial de las piezas del jugador
// Entrada: Color de las piezas
//---------------------------------------------------------------------------

PROCESS colocar_jugador(color);

PRIVATE
    casilla_valida=false;
    color;

BEGIN
    mouse.file=0;
    mouse.graph=color;
    REPEAT

        // Limita el rat¢n al tablero
        IF (mouse.x<80) mouse.x=80; END
        IF (mouse.x>369) mouse.x=369; END
        IF (mouse.y<96) mouse.y=96; END
        IF (mouse.y>385) mouse.y=385; END

        // Averigua sobre qu‚ casilla est  posado el rat¢n
        casilla=(mouse.x-42)/73+((mouse.y-58)/73)*5;

        // Comprueba si es una casilla v lida
        IF (tablero[casilla].color==0 and casilla!=12)
            casilla_valida=true;
        ELSE
            casilla_valida=false;
        END

        FRAME;
    UNTIL (mouse.left and casilla_valida);

    // Dibuja la pieza y borra el rat¢n
    graph=color;
    x=casilla%5*73+79;
    y=(casilla/5)*73+95;
    mouse.graph=0;
    sound(s_colocar,200,256);

    // Carga la estructura tablero
    tablero[casilla].color=color;
    tablero[casilla].id_pieza=id;

    // Actualiza n£mero de piezas
    IF (color==1)
        n_blancas++;
    ELSE
        n_rojas++;
    END
    LOOP
        FRAME;
    END
END


//---------------------------------------------------------------------------
// Proceso colocar_pc
// Colocaci¢n inicial de las piezas del ordenador
// Entradas: Color de la pieza
//---------------------------------------------------------------------------

PROCESS colocar_pc(color);

PRIVATE
    valor_casilla[]=100,80,100,80,100,80,60,40,60,80,100,40,0,40,100,80,60,40,60,80,100,80,100,80,100;
                        // Valor de las casillas
    color;
    m;                  // Para bucles
    mejor_valor=0;      // Puntuaci¢n de la casilla mejor valorada
    v_casilla;          // Valor de una casilla

BEGIN

    // Explora todas las casillas y elige la de mejor valoraci¢n
    FROM m=0 TO 24;
        IF (tablero[m].color==0 AND m!=12)
            IF (nivel_dif==0)           // En dificultad f cil la aleatoriedad es mayor
                v_casilla=valor_casilla[m]+rand(0,69);
            ELSE
                v_casilla=valor_casilla[m]+rand(0,9);
            END
            IF (v_casilla>mejor_valor)
                mejor_valor=v_casilla;
                casilla=m;
            END
        END
    END

    // Dibuja la pieza
    graph=color;
    x=casilla%5*73+79;
    y=(casilla/5)*73+95;
    sound(s_colocar,200,256);

    // Carga la estructura tablero
    tablero[casilla].color=color;
    tablero[casilla].id_pieza=id;

    // Actualiza n£mero de piezas
    IF (color==1)
        n_blancas++;
    ELSE
        n_rojas++;
    END
    LOOP
        FRAME;
    END
END


//---------------------------------------------------------------------------
// Proceso mover_jugador
// Movimiento y capturas del jugador
// Entrada: Color de las piezas
//---------------------------------------------------------------------------

PROCESS mover_jugador(color);

PRIVATE
    casilla_valida=false;
    bloqueado=false;            // No tiene piezas alrededor de la casilla libre
    guarda_casilla;             // Para guardar la casilla libre
    hay_capturas;
    color;

BEGIN
    bloqueado=ver_bloqueo(color);   // Ver si est  bloqueado

    // Inicializa el rat¢n
    mouse.file=0;
    mouse.graph=10;
    REPEAT

        // Limita el rat¢n al tablero
        IF (mouse.x<43) mouse.x=43; END
        IF (mouse.x>405) mouse.x=405; END
        IF (mouse.y<59) mouse.y=59; END
        IF (mouse.y>421) mouse.y=421; END

        // Averigua sobre qu‚ casilla est  posado el rat¢n
        casilla=(mouse.x-42)/73+((mouse.y-58)/73)*5;

        // Comprueba si es una casilla v lida
        IF (tablero[casilla].color==color)
            IF (abs(casilla-casilla_libre)==5)  // Casilla libre adyacente verticalmente
                casilla_valida=TRUE;
            ELSE
                IF (casilla/5==casilla_libre/5 and abs(casilla-casilla_libre)==1)
                                                // Casilla libre adyacente horizontalmente
                    casilla_valida=TRUE;
                ELSE
                    IF (bloqueado)
                        casilla_valida=TRUE;
                    ELSE
                        casilla_valida=FALSE;
                    END
                END
            END
        ELSE
            casilla_valida=FALSE;
        END
        FRAME;
    UNTIL (mouse.left and casilla_valida);
    guarda_casilla=casilla_libre;
    mueve_pieza(casilla);                   // Mueve la pieza
    timer=0;                                // Da tiempo a que se mueva la pieza
    WHILE (timer<50)
        FRAME;
    END
    hay_capturas=capturas(guarda_casilla);  // Realiza las capturas
    timer=0;                                // Da tiempo a que se realicen las capturas
    WHILE (timer<50)
        FRAME;
    END
    ver_empate(casilla);                    // Ver empate por repetici¢n de jugadas

    // Ver posibilidad de movimiento de retorno
    IF (NOT bloqueado AND hay_capturas)
        fin_retorno=FALSE;
        mov_retorno(guarda_casilla);
        REPEAT
            FRAME;
        UNTIL (fin_retorno);
    END
    fin_movimiento=TRUE;
END


//---------------------------------------------------------------------------
// Proceso mov_retorno
// Comprueba si existe posibilidad de realizar movimiento de retorno y permite
// realizarlo si el jugador as¡ lo decide.
// Entradas= Casilla de la £ltima pieza movida.
//---------------------------------------------------------------------------

PROCESS mov_retorno(casilla);

PRIVATE
    m;                  // Para bucles
    color;              // Color de la pieza
    hay_capturas;       // Indicador de si hay capturas
    c_raton;            // Casilla se¤alada por el raton
    guarda_casilla;     // Para guardar la casilla libre

BEGIN
    // Utiliza el primer nivel de la estructura arbol para no modificar
    // la posicion real
    FROM m=0 TO 24;
        arbol[0].posicion[m]=tablero[m].color;
    END
    color=arbol[0].posicion[casilla];
    arbol[0].posicion[casilla]=0;
    arbol[0].posicion[casilla_libre]=color;
    hay_capturas=arbol_capturas(casilla_libre,0);
    IF (hay_capturas)

        // El movimiento de respuesta es posible. Se permite elegir al jugador
        mouse.graph=10;
        tablero[casilla].id_pieza.flags=4;
        REPEAT
            tablero[casilla].id_pieza.flags=4-tablero[casilla].id_pieza.flags;
            FRAME;
        UNTIL (mouse.left OR mouse.right);
        tablero[casilla].id_pieza.flags=0;
        IF (mouse.left)

            // Realiza el movimiento de retorno y las capturas
            guarda_casilla=casilla_libre;
            mueve_pieza(casilla);                   // Mueve la pieza
            timer=0;                                // Da tiempo a que se mueva la pieza
            WHILE (timer<50)
                FRAME;
            END
            hay_capturas=capturas(guarda_casilla);  // Realiza las capturas
            timer=0;                                // Da tiempo a que se realicen las capturas
            WHILE (timer<50)
                FRAME;
            END
        END
    END
    fin_retorno=TRUE;
END


//---------------------------------------------------------------------------
// Proceso mover_pc
// Movimiento y capturas del ordenador
// Entradas: Color de las piezas
//---------------------------------------------------------------------------

PROCESS mover_pc(color);

PRIVATE
    color;              // Color de las piezas del ordenador
    m;                  // Para bucles
    guarda_casilla;     // Para guardar la casilla libre
    hay_capturas;       // Indicador de si ha habido capturas
    bloqueado;          // Indicador de posicion bloqueada

BEGIN

    // Carga el primer nivel(0) con la posici¢n de partida
    FROM m=0 TO 24;
        arbol[0].posicion[m]=tablero[m].color;
    END
    IF (n_blancas<=2 or n_rojas<=2)
        profundidad=7;
    END
    explorar_rama(color,0);
    guarda_casilla=casilla_libre;
    IF (hay_retorno)
        mueve_pieza(mejor_jug/100);         // Mueve la pieza
        ver_empate(mejor_jug/100);          // Ver empate por repetici¢n de jugadas
    ELSE
        mueve_pieza(mejor_jug);
        ver_empate(mejor_jug);              // Ver empate por repetici¢n de jugadas
    END
    timer=0;                                // Da tiempo a que se mueva la pieza
    WHILE (timer<50)
        FRAME;
    END
    hay_capturas=capturas(guarda_casilla);  // Realiza las capturas
    timer=0;                                // Da tiempo a que se realicen las capturas
    WHILE (timer<60)
        FRAME;
    END
    IF (hay_retorno)
        guarda_casilla=casilla_libre;
        mueve_pieza(mejor_jug%100);             // Movimiento de retorno
        timer=0;                                // Da tiempo a que se mueva la pieza
        WHILE (timer<50)
            FRAME;
        END
        hay_capturas=capturas(guarda_casilla);  // Realiza las capturas
        timer=0;                                // Da tiempo a que se realicen las capturas
        WHILE (timer<50)
            FRAME;
        END
    END
    fin_movimiento=TRUE;
END


//---------------------------------------------------------------------------
// Proceso explorar_rama
// Explora todas las jugadas de una rama del  rbol de jugadas
// Entradas: Color del bando que mueve
//           Nivel de la rama en el  rbol de jugadas
// Devuelve la valoraci¢n de la posici¢n
//---------------------------------------------------------------------------

PROCESS explorar_rama(color,nivel);

PRIVATE
    color;                  // Color de las piezas
    nivel;                  // Nivel de la rama
    m;                      // Para bucles
    n;
    c_libre;                // Casilla libre
    bloqueo=TRUE;           // Situaci¢n de bloqueo
    punt_posicion;          // Puntuaci¢n de una posici¢n
    max_puntuacion=-10000;  // Puntuaci¢n de la mejor jugada
    hay_capturas;           // Indica si se producen capturas
    hay_adyacentes;         // Indica que piezas contrarias adyacentes a una propia

BEGIN

    // Explora todas las jugadas posibles------------------------------------
    // Averigua la casilla libre
    FROM m=0 to 24;
        IF (arbol[nivel].posicion[m]==0)
            c_libre=m;
            BREAK;
        END
    END

    // Comprueba pieza superior
    IF (c_libre>4)
        IF (arbol[nivel].posicion[c_libre-5]==color)
            bloqueo=FALSE;
            FROM m=0 TO 24;         // Copia la posici¢n al siguiente nivel
                arbol[nivel+1].posicion[m]=arbol[nivel].posicion[m]:
            END
            arbol[nivel+1].posicion[c_libre]=color;
            arbol[nivel+1].posicion[c_libre-5]=0;
            hay_capturas=arbol_capturas(c_libre,nivel+1);       // Realiza las capturas
            IF (nivel==profundidad-1)
                // No profundizar m s. Evaluar posici¢n
                punt_posicion=evaluar_posici¢n(color,nivel+1);
            ELSE
                // Profundizar otro nivel
                punt_posicion=explorar_rama(3-color,nivel+1);
            END
            IF (punt_posicion>=max_puntuacion)
                // Encontramos la mejor jugada por ahora
                max_puntuacion=punt_posicion;
                IF (nivel==0)
                    // Guarda la jugada
                    mejor_jug=c_libre-5;
                    hay_retorno=FALSE;
                END
            END

            // Considerar movimiento de retorno
            IF (hay_capturas)
                arbol[nivel+1].posicion[c_libre-5]=color;
                arbol[nivel+1].posicion[c_libre]=0;
                hay_capturas=arbol_capturas(c_libre-5,nivel+1);       // Realiza las capturas
                IF (hay_capturas)
                    IF (nivel==profundidad-1)
                        // No profundizar m s. Evaluar posici¢n
                        punt_posicion=evaluar_posici¢n(color,nivel+1);
                    ELSE
                        // Profundizar otro nivel
                        punt_posicion=explorar_rama(3-color,nivel+1);
                    END
                    IF (punt_posicion>=max_puntuacion)
                        // Encontramos la mejor jugada por ahora
                        max_puntuacion=punt_posicion;
                        IF (nivel==0)
                            // Guarda la jugada
                            mejor_jug=(c_libre-5)*100+c_libre;
                            hay_retorno=TRUE;
                        END
                    END
                END
            END
        END
    END

    // Comprueba pieza inferior
    IF (c_libre<20)
        IF (arbol[nivel].posicion[c_libre+5]==color)
            bloqueo=FALSE;
            FROM m=0 TO 24;         // Copia la posici¢n al siguiente nivel
                arbol[nivel+1].posicion[m]=arbol[nivel].posicion[m]:
            END
            arbol[nivel+1].posicion[c_libre]=color;
            arbol[nivel+1].posicion[c_libre+5]=0;
            hay_capturas=arbol_capturas(c_libre,nivel+1);       // Realiza las capturas
            IF (nivel==profundidad-1)
                // No profundizar m s. Evaluar posici¢n
                punt_posicion=evaluar_posici¢n(color,nivel+1);
            ELSE
                // Profundizar otro nivel
                punt_posicion=explorar_rama(3-color,nivel+1);
            END
            IF (punt_posicion>=max_puntuacion)
                // Encontramos la mejor jugada por ahora
                max_puntuacion=punt_posicion;
                IF (nivel==0)
                    // Guarda la jugada
                    mejor_jug=c_libre+5;
                    hay_retorno=FALSE;
                END
            END

            // Considerar movimiento de retorno
            IF (hay_capturas)
                arbol[nivel+1].posicion[c_libre+5]=color;
                arbol[nivel+1].posicion[c_libre]=0;
                hay_capturas=arbol_capturas(c_libre+5,nivel+1);       // Realiza las capturas
                IF (hay_capturas)
                    IF (nivel==profundidad-1)
                        // No profundizar m s. Evaluar posici¢n
                        punt_posicion=evaluar_posici¢n(color,nivel+1);
                    ELSE
                        // Profundizar otro nivel
                        punt_posicion=explorar_rama(3-color,nivel+1);
                    END
                    IF (punt_posicion>=max_puntuacion)
                        // Encontramos la mejor jugada por ahora
                        max_puntuacion=punt_posicion;
                        IF (nivel==0)
                            // Guarda la jugada
                            mejor_jug=(c_libre+5)*100+c_libre;
                            hay_retorno=TRUE;
                        END
                    END
                END
            END
        END
    END

    // Comprueba pieza a la izquierda
    IF (c_libre%5>0)
        IF (arbol[nivel].posicion[c_libre-1]==color)
            bloqueo=FALSE;
            FROM m=0 TO 24;         // Copia la posici¢n al siguiente nivel
                arbol[nivel+1].posicion[m]=arbol[nivel].posicion[m]:
            END
            arbol[nivel+1].posicion[c_libre]=color;
            arbol[nivel+1].posicion[c_libre-1]=0;
            hay_capturas=arbol_capturas(c_libre,nivel+1);       // Realiza las capturas
            IF (nivel==profundidad-1)
                // No profundizar m s. Evaluar posici¢n
                punt_posicion=evaluar_posici¢n(color,nivel+1);
            ELSE
                // Profundizar otro nivel
                punt_posicion=explorar_rama(3-color,nivel+1);
            END
            IF (punt_posicion>=max_puntuacion)
                // Encontramos la mejor jugada por ahora
                max_puntuacion=punt_posicion;
                IF (nivel==0)
                    // Guarda la jugada
                    mejor_jug=c_libre-1;
                    hay_retorno=FALSE;
                END
            END

            // Considerar movimiento de retorno
            IF (hay_capturas)
                arbol[nivel+1].posicion[c_libre-1]=color;
                arbol[nivel+1].posicion[c_libre]=0;
                hay_capturas=arbol_capturas(c_libre-1,nivel+1);       // Realiza las capturas
                IF (hay_capturas)
                    IF (nivel==profundidad-1)
                        // No profundizar m s. Evaluar posici¢n
                        punt_posicion=evaluar_posici¢n(color,nivel+1);
                    ELSE
                        // Profundizar otro nivel
                        punt_posicion=explorar_rama(3-color,nivel+1);
                    END
                    IF (punt_posicion>=max_puntuacion)
                        // Encontramos la mejor jugada por ahora
                        max_puntuacion=punt_posicion;
                        IF (nivel==0)
                            // Guarda la jugada
                            mejor_jug=(c_libre-1)*100+c_libre;
                            hay_retorno=TRUE;
                        END
                    END
                END
            END
        END
    END

    // Comprueba pieza a la derecha
    IF (c_libre%5<4)
        IF (arbol[nivel].posicion[c_libre+1]==color)
            bloqueo=FALSE;
            FROM m=0 TO 24;         // Copia la posici¢n al siguiente nivel
                arbol[nivel+1].posicion[m]=arbol[nivel].posicion[m]:
            END
            arbol[nivel+1].posicion[c_libre]=color;
            arbol[nivel+1].posicion[c_libre+1]=0;
            hay_capturas=arbol_capturas(c_libre,nivel+1);       // Realiza las capturas
            IF (nivel==profundidad-1)
                // No profundizar m s. Evaluar posici¢n
                punt_posicion=evaluar_posici¢n(color,nivel+1);
            ELSE
                // Profundizar otro nivel
                punt_posicion=explorar_rama(3-color,nivel+1);
            END
            IF (punt_posicion>=max_puntuacion)
                // Encontramos la mejor jugada por ahora
                max_puntuacion=punt_posicion;
                IF (nivel==0)
                    // Guarda la jugada
                    mejor_jug=c_libre+1;
                    hay_retorno=FALSE;
                END
            END

            // Considerar movimiento de retorno
            IF (hay_capturas)
                arbol[nivel+1].posicion[c_libre+1]=color;
                arbol[nivel+1].posicion[c_libre]=0;
                hay_capturas=arbol_capturas(c_libre+1,nivel+1);       // Realiza las capturas
                IF (hay_capturas)
                    IF (nivel==profundidad-1)
                        // No profundizar m s. Evaluar posici¢n
                        punt_posicion=evaluar_posici¢n(color,nivel+1);
                    ELSE
                        // Profundizar otro nivel
                        punt_posicion=explorar_rama(3-color,nivel+1);
                    END
                    IF (punt_posicion>=max_puntuacion)
                        // Encontramos la mejor jugada por ahora
                        max_puntuacion=punt_posicion;
                        IF (nivel==0)
                            // Guarda la jugada
                            mejor_jug=(c_libre+1)*100+c_libre;
                            hay_retorno=TRUE;
                        END
                    END
                END
            END
        END
    END

    // En situaci¢n de bloqueo hay que explorar todas las piezas excepto
    // las que no tengan piezas contrarias adyacentes
    IF (bloqueo)
        FROM n=0 TO 24;
            IF (arbol[nivel].posicion[n]==color)
                hay_adyacentes=FALSE;
                // Comprueba pieza superior
                IF (n>4)
                    IF (arbol[nivel].posicion[n-5]==3-color)
                        hay_adyacentes=TRUE;
                    END
                END
                // Comprueba pieza inferior
                IF (n<20)
                    IF (arbol[nivel].posicion[n+5]==3-color)
                        hay_adyacentes=TRUE;
                    END
                END
                // Comprueba pieza a la izquierda
                IF (n%5>0)
                    IF (arbol[nivel].posicion[n-1]==3-color)
                        hay_adyacentes=TRUE;
                    END
                END
                // Comprueba pieza a la derecha
                IF (n%5<4)
                    IF (arbol[nivel].posicion[n+1]==3-color)
                        hay_adyacentes=TRUE;
                    END
                END
                IF (hay_adyacentes)
                    FROM m=0 TO 24;         // Copia la posici¢n al siguiente nivel
                        arbol[nivel+1].posicion[m]=arbol[nivel].posicion[m]:
                    END
                    arbol[nivel+1].posicion[c_libre]=color;
                    arbol[nivel+1].posicion[n]=0;
                    hay_capturas=arbol_capturas(c_libre,nivel+1);       // Realiza las capturas
                    IF (nivel==profundidad-1)
                        // No profundizar m s. Evaluar posici¢n
                        punt_posicion=evaluar_posici¢n(color,nivel+1);
                    ELSE
                        // Profundizar otro nivel
                        punt_posicion=explorar_rama(3-color,nivel+1);
                    END
                    IF (punt_posicion>=max_puntuacion)
                        // Encontramos la mejor jugada por ahora
                        max_puntuacion=punt_posicion;
                        IF (nivel==0)
                            // Guarda la jugada
                            mejor_jug=n;
                            hay_retorno=FALSE;
                        END
                    END
                END
            END
        END
    END
    return(-max_puntuacion);
END


//---------------------------------------------------------------------------
// Proceso arbol_capturas
// Realiza las capturas en el arbol de posiciones partiendo de una casilla dada
// Entradas: Casilla de partida
//           Nivel de la rama
// Retorna TRUE en caso de capturas y FALSE en caso contrario
//---------------------------------------------------------------------------

PROCESS arbol_capturas(casilla,nivel);

PRIVATE
    hay_capturas=FALSE;
    color;                  // Color del bando que captura
    encerrona=FALSE;        // Hay piezas encerradas en una l¡nea determinada
    m=0;                    // Para bucles
    nivel                   // Nivel de la rama

BEGIN
    color=arbol[nivel].posicion[casilla];

    // Ver capturas hacia arriba
    IF (casilla>9)
        IF (arbol[nivel].posicion[casilla-5]==3-color)
            encerrona=FALSE;
            FOR (m=casilla-10;m>=0;m-=5)
                IF (arbol[nivel].posicion[m]==0)
                    BREAK;
                END
                IF (arbol[nivel].posicion[m]==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla-5;m>=m%5;m-=5)
                    IF (arbol[nivel].posicion[m]==3-color)
                        // Realiza la captura
                        arbol[nivel].posicion[m]=color;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END

    // Ver capturas hacia abajo
    IF (casilla<15)
        IF (arbol[nivel].posicion[casilla+5]==3-color)
            encerrona=FALSE;
            FOR (m=casilla+10;m<=24;m+=5)
                IF (arbol[nivel].posicion[m]==0)
                    BREAK;
                END
                IF (arbol[nivel].posicion[m]==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla+5;m<=24;m+=5)
                    IF (arbol[nivel].posicion[m]==3-color)
                        // Realiza la captura
                        arbol[nivel].posicion[m]=color;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END

    // Ver capturas hacia la izquierda
    IF (casilla%5>1)
        IF (arbol[nivel].posicion[casilla-1]==3-color)
            encerrona=FALSE;
            FOR (m=casilla-2;(m+5)%5<=(casilla-2)%5;m--)
                IF (arbol[nivel].posicion[m]==0)
                    BREAK;
                END
                IF (arbol[nivel].posicion[m]==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla-1;m%5<=(casilla-1)%5;m--)
                    IF (arbol[nivel].posicion[m]==3-color)
                        // Realiza la captura
                        arbol[nivel].posicion[m]=color;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END

    // Ver capturas hacia la derecha
    IF (casilla%5<3)
        IF (arbol[nivel].posicion[casilla+1]==3-color)
            encerrona=FALSE;
            FOR (m=casilla+2;m%5>=(casilla+2)%5;m++)
                IF (arbol[nivel].posicion[m]==0)
                    BREAK;
                END
                IF (arbol[nivel].posicion[m]==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla+1;m%5>=(casilla+1)%5;m++)
                    IF (arbol[nivel].posicion[m]==3-color)
                        // Realiza la captura
                        arbol[nivel].posicion[m]=color;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END
    return(hay_capturas);
END

//---------------------------------------------------------------------------
// Proceso evaluar_posici¢n
// Devuelve la valoraci¢n de una posici¢n dada. Esta ser  positiva si la
// posici¢n es ventajosa y negativa en caso contrario
// Entradas: Nivel de la rama y color del bando que se eval£a
//---------------------------------------------------------------------------

PROCESS evaluar_posicion(color,nivel)

PRIVATE
    color;              // Color del bando que se eval£a
    nivel;              // Nivel de la rama
    m;                  // Para bucles
    puntuacion=0;       // Valor de la posici¢n

BEGIN
    FROM m=0 TO 24;
        IF (arbol[nivel].posici¢n[m]==color)
            puntuacion+=100;
        ELSE
            puntuacion-=100;
        END
    END
    IF (nivel_dif==0)           // En dificultad f cil la aleatoriedad es mayor
        return(puntuacion+rand(0,499));
    ELSE
        return(puntuacion+rand(0,9));
    END
END


//---------------------------------------------------------------------------
// Proceso ver_bloqueo
// Comprueba si existen piezas de un color dado alrededor de la casilla libre
// Entradas= Color a examinar
// Retorna TRUE en caso de bloqueo y FALSE en caso contrario
//---------------------------------------------------------------------------

PROCESS ver_bloqueo(color);

PRIVATE color;

BEGIN
    // Comprueba pieza superior
    IF (casilla_libre>4)
        IF (tablero[casilla_libre-5].color==color)
            return(FALSE);
        END
    END

    // Comprueba pieza inferior
    IF (casilla_libre<20)
        IF (tablero[casilla_libre+5].color==color)
            return(FALSE);
        END
    END

    // Comprueba pieza a la izquierda
    IF (casilla_libre%5>0)
        IF (tablero[casilla_libre-1].color==color)
            return(FALSE);
        END
    END

    // Comprueba pieza a la derecha
    IF (casilla_libre%5<4)
        IF (tablero[casilla_libre+1].color==color)
            return(FALSE);
        END
    END
    return(TRUE);
END


//---------------------------------------------------------------------------
// Proceso mueve_pieza
// Mueve una pieza a la casilla libre
// Entradas: Casilla donde esta la pieza a mover
//---------------------------------------------------------------------------

PROCESS mueve_pieza(casilla);

PRIVATE
    id_pieza;           // Identificador del proceso de la pieza que se mueve
    m;                  // Para bucles
    inc_x;              // Incremento coordenadas para el movimiento de la pieza‡
    inc_y;

BEGIN

    // Pasa la pieza a la casilla libre en la estructura tablero
    id_pieza=tablero[casilla].id_pieza;
    tablero[casilla_libre].color=tablero[casilla].color;
    tablero[casilla_libre].id_pieza=id_pieza;
    tablero[casilla].color=0;       // Libera la casilla donde estaba la pieza
    tablero[casilla].id_pieza=0;

    // Mueve la pieza en incrementos de 7 pixels
    inc_x=(casilla_libre%5*73+79-id_pieza.x)/10;
    inc_y=((casilla_libre/5)*73+95-id_pieza.y)/10;
    IF (abs(inc_x)+abs(inc_y)<=7)
        sound(s_mover,256,256);
    END
    id_pieza.z=-1;
    FOR (m=0;m<=9;m++)
        id_pieza.x+=inc_x;
        id_pieza.y+=inc_y;
        FRAME;
    END
    id_pieza.x=casilla_libre%5*73+79;
    id_pieza.y=(casilla_libre/5)*73+95;
    FRAME;
    IF (abs(inc_x)+abs(inc_y)>7)
        sound(s_colocar,200,256);
    END
    id_pieza.z=0;
    casilla_libre=casilla;
END


//---------------------------------------------------------------------------
// Proceso capturas
// Realiza las capturas partiendo de una casilla dada
// Entradas: Casilla de partida
// Retorna TRUE en caso de capturas y FALSE en caso contrario
//---------------------------------------------------------------------------

PROCESS capturas(casilla);

PRIVATE
    hay_capturas=FALSE;
    color;                  // Color del bando que captura
    encerrona=FALSE;        // Hay piezas encerradas en una l¡nea determinada
    m=0;                    // Para bucles

BEGIN
    color=tablero[casilla].color;

    // Ver capturas hacia arriba
    IF (casilla>9)
        IF (tablero[casilla-5].color==3-color)
            encerrona=FALSE;
            FOR (m=casilla-10;m>=0;m-=5)
                IF (tablero[m].color==0)
                    BREAK;
                END
                IF (tablero[m].color==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla-5;m>=m%5;m-=5)
                    IF (tablero[m].color==3-color)
                        // Realiza la captura
                        tablero[m].color=color;             // Modifica la estructura tablero
                        tablero[m].id_pieza.graph=color;    // Cambia el gr fico del proceso
                        IF (color==1)                       // Actualiza num. piezas
                            n_blancas++;
                        ELSE
                            n_blancas--;
                        END
                        n_rojas=24-n_blancas;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END

    // Ver capturas hacia abajo
    IF (casilla<15)
        IF (tablero[casilla+5].color==3-color)
            encerrona=FALSE;
            FOR (m=casilla+10;m<=24;m+=5)
                IF (tablero[m].color==0)
                    BREAK;
                END
                IF (tablero[m].color==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla+5;m<=24;m+=5)
                    IF (tablero[m].color==3-color)
                        // Realiza la captura
                        tablero[m].color=color;             // Modifica la estructura tablero
                        tablero[m].id_pieza.graph=color;    // Cambia el gr fico del proceso
                        IF (color==1)                       // Actualiza num. piezas
                            n_blancas++;
                        ELSE
                            n_blancas--;
                        END
                        n_rojas=24-n_blancas;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END

    // Ver capturas hacia la izquierda
    IF (casilla%5>1)
        IF (tablero[casilla-1].color==3-color)
            encerrona=FALSE;
            FOR (m=casilla-2;(m+5)%5<=(casilla-2)%5;m--)
                IF (tablero[m].color==0)
                    BREAK;
                END
                IF (tablero[m].color==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla-1;m%5<=(casilla-1)%5;m--)
                    IF (tablero[m].color==3-color)
                        // Realiza la captura
                        tablero[m].color=color;             // Modifica la estructura tablero
                        tablero[m].id_pieza.graph=color;    // Cambia el gr fico del proceso
                        IF (color==1)                       // Actualiza num. piezas
                            n_blancas++;
                        ELSE
                            n_blancas--;
                        END
                        n_rojas=24-n_blancas;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END

    // Ver capturas hacia la derecha
    IF (casilla%5<3)
        IF (tablero[casilla+1].color==3-color)
            encerrona=FALSE;
            FOR (m=casilla+2;m%5>=(casilla+2)%5;m++)
                IF (tablero[m].color==0)
                    BREAK;
                END
                IF (tablero[m].color==color)
                    encerrona=TRUE;
                    BREAK;
                END
            END
            IF (encerrona)
                hay_capturas=TRUE;
                FOR (m=casilla+1;m%5>=(casilla+1)%5;m++)
                    IF (tablero[m].color==3-color)
                        // Realiza la captura
                        tablero[m].color=color;             // Modifica la estructura tablero
                        tablero[m].id_pieza.graph=color;    // Cambia el gr fico del proceso
                        IF (color==1)                       // Actualiza num. piezas
                            n_blancas++;
                        ELSE
                            n_blancas--;
                        END
                        n_rojas=24-n_blancas;
                    ELSE
                        BREAK;
                    END
                END
            END
        END
    END
    return(hay_capturas);
END


//---------------------------------------------------------------------------
// Proceso ver_empate
// Comprueba si se produce empate por repetici¢n de jugadas
// Entradas: Casilla de donde parte la pieza en la £ltima jugada
// Pone a TRUE la variable empate si se detecta repetici¢n de jugadas
//---------------------------------------------------------------------------

PROCESS ver_empate(casilla);

PRIVATE
    m;                  // Para bucles
    num_repet=0;        // N£mero de repeticiones de jugadas halladas

BEGIN

    // Guarda la £ltima jugada en la £ltima posici¢n de la tabla y desplaza
    // las anteriores una posici¢n
    FROM m=0 TO 48;
        ult_jugadas[m]=ult_jugadas[m+1];
    END
    ult_jugadas[49]=casilla;

    // Compara las £ltimas cuatro jugadas con las anteriores y suma las
    // repeticiones encontradas
    FROM m=0 TO 42;
        IF (ult_jugadas[m]==ult_jugadas[46] AND ult_jugadas[m+1]==ult_jugadas[47] AND ult_jugadas[m+2]==ult_jugadas[48] AND ult_jugadas[m+3]==ult_jugadas[49])
            num_repet++;
        END
    END

    // Si hay dos o m s repeticiones, se pone a TRUE el indicador de empate
    IF (num_repet>=2)
        empate=TRUE;
    END

END