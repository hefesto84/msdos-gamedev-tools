
//-----------------------------------------------------------------------------
//TITULO:        STEROID
//AUTOR:         DANIEL NAVARRO
//FECHA:         10-5-96
//-----------------------------------------------------------------------------

PROGRAM steroid;

GLOBAL
    puntuacion=0;               // Puntuaci¢n
    vidas=3;                    // Vidas restantes
    vida[2];                    // Identificadores del gr fico de las vidas
    muerte=0;                   // Indica si ha muerto o no (Bandera)
    nivel=1;                    // Numero de Fase
    salir_=0;                   // Bandera de salida (1=Salir del juego)

    sonido_fuego;               // Identificadores de sonidos
    sonido_explosion;
    sonido_aceleracion;
    sonido_hiperespacio;

    volumen=0;                  // Volumen del sonido de aceleraci¢n
    id_sonido;                  // Identificador del sonido de aceleraci¢n

PRIVATE
    contador0=0;                // Contador de uso general
    id2;                        // Identificador de uso general
BEGIN

    // Selecciona modo gr fico y carga de ficheros
    set_mode(m640x480);
    load_fpg("steroid\steroid.fpg");
    load_fnt("steroid\steroid.fnt");

    // Carga los sonidos necesarios
    sonido_fuego=load_pcm("steroid\tubo8.pcm",0);
    sonido_explosion=load_pcm("steroid\tubo5.pcm",0);
    sonido_aceleracion=load_pcm("steroid\nave.pcm",1);
    sonido_hiperespacio=load_pcm("steroid\fx33.pcm",0);

    // Crea las estrellas del fondo de la pantalla
    FROM contador0=0 TO 499;
        put_pixel(rand(0,639),rand(0,479),127);
    END

    // Escribe los textos de presentaci¢n
    write(1,320,0,1,"STEROIDS Version 1.0");
    write(1,320,15,1,"(c) DIV GAMES STUDIO");
    write(1,0,480,6,"LEVEL");
    write(1,640,480,8,"< >:rotar ^:avanzar SPC:disparo H:hiperespacio");
    write_int(1,640,0,2,&puntuacion);
    write_int(1,64,480,6,&nivel);

    // Inicia el sonido del motor de la nave
    id_sonido=sound(sonido_aceleracion,volumen,200);

    LOOP                                          // Bucle principal
        fade_on();                                // Apaga pantalla
        puntuacion=0;                             // Inicia variables necesarias
        nivel=1;                                  // Puntuaci¢n, Fase, Vidas, Volumen
        vidas=3;
        volumen=0;
        // Pone un texto explicativo
        id2=write(1,320,240,1,"PRESIONE UNA TECLA PARA JUGAR");

        scan_code=0;
        REPEAT                              // Repite hasta que se suelte cualquier tecla
            FRAME;
        UNTIL (scan_code==0)

        REPEAT
            IF (key (_esc))                 // Si se pulsa ESCAPE
                fade_off();                 // Apaga la pantalla
                salir_=1;
            END
            FRAME;
        UNTIL (scan_code<>0 OR (salir_==1))     // Si pulsas una tecla, empieza el juego

        IF (salir_==1)
            let_me_alone();             // Elimina todos los dem s procesos
            BREAK;                      // Sale del bucle
        ELSE
            salir_=0;
        END              // Si se pulso escape, sale del juego
        delete_text(id2);

        nave(320,240);                          // Inicia la nave

        // Inicia los asteroides, crea los procesos tipo asteroide
        FOR (contador0=0;contador0<2+nivel;contador0++)
            asteroide(-16,-16,3);
        END

        // Inicia los gr ficos de las vidas
        vida[0]=nave_peque¤a(16,16);
        vida[1]=nave_peque¤a(48,16);
        vida[2]=nave_peque¤a(80,16);
        LOOP
            // Si se pulsa [ESCAPE] se sale del juego
            IF (key(_esc))
                vidas=0;
                let_me_alone();
                fade_off();
                BREAK;
            END

            IF (muerte)                             // Si te matan
                signal(vida[--vidas],s_kill);       // Se borra un gr fico de las vidas
                signal(TYPE asteroide,s_kill);      // Tambi‚n los asteroides
                signal(TYPE disparo_nave,s_kill);   // Y los disparos
                muerte=0;

                IF (vidas==0)                       // Si no tienes vidas
                    fade_off();                     // Acaba el juego
                    let_me_alone();
                    BREAK;
                END
                fade(0,0,0,8);                      // Hace apagado de pantalla
                WHILE (fading)                      // De un modo especial
                    FRAME;
                END

                nave(320,240);                      // Crea nave protagonista
                FOR (contador0=0;contador0<2+nivel;contador0++)             // Crea los asteroides
                    asteroide(-16,-16,3);           // Dependiendo de la fase
                END

                fade(100,100,100,8);                // Enciende la pantalla
            END

            IF (get_id(TYPE asteroide)==0)          // Si no quedan asteroides
                fade(0,0,0,8);                      // Apaga pantalla
                WHILE (fading)                      // Espera
                    FRAME;
                END
                nivel++;                            // Cambia de nivel
                id2=get_id(TYPE disparo_nave);
                WHILE (id2)   // Elimina los disparos actuales
                    signal(id2,s_kill);
                    id2=get_id(TYPE disparo_nave);
                END
                signal(get_id(TYPE nave),s_kill);   // Elimina la nave
                nave(320,240);                      // Crea una nueva nave
                FOR (contador0=0;contador0<2+nivel;contador0++) // Crea nuevos asteroides
                    asteroide(-16,-16,3);
                END
                fade(100,100,100,8);                // Enciende la pantalla
            END

            FRAME;
            change_sound(id_sonido,volumen,200);    // Actualiza el sonido del motor
        END
    END
    fade_off(); // Apaga la pantalla antes de salir del programa
END
//-----------------------------------------------------------------------------
// Proceso para controlar el objeto nave
// Entradas: Coordenadas de la nave
//-----------------------------------------------------------------------------
PROCESS nave(x,y);

PRIVATE
    disparo=1;              // 1=disparo permitido, 0=no permitido
    hiper=1;                // 1=hiperespacio permitido, 0=no permitido
    velocidad_x=0;          // incremento x
    velocidad_y=0;          // incremento y

BEGIN
    graph=1;
    LOOP
        // Lee teclas y actualiza el  ngulo de la nave
        IF (key(_right)) angle-=pi/16; END
        IF (key(_left)) angle+=pi/16; END

        IF (key(_up))                       // Calcula el avance con formula
            velocidad_x+=get_distx(angle,10);
            velocidad_y+=get_disty(angle,10);
            // Incrementa el volumen de la nave
            IF ((volumen+=30) > 256) volumen=256; END
        ELSE
            // Decrementa el volumen
            volumen-=10;
            IF (volumen<0) volumen=0; END
        END

        x+=velocidad_x/10;
        y+=velocidad_y/10;

        // Comprueba si se ha salido de la pantalla y lo soluciona
        IF (x<-16) x+=640+32; END
        IF (y<-16)  y+=480+32; END
        IF (x>640+16) x-=640+32; END
        IF (y>480+16) y-=480+32; END

        IF (key(_space) OR key (_control))      // Comprueba la tecla de disparo
            IF (disparo)                        // Y si se puede dispara
                disparo=0;
                disparo_nave(x,y,angle);        // Dispara, creando un proceso tipo disparo nave
            END
        ELSE
            disparo=1;                          // Hace que los disparos salgan de uno en uno
        END

        IF (key(_h))                            // Comprueba la tecla del hiperespacio
            IF (hiper)                          // Y si se puede se hace
                hiper=0;
                hiperespacio(x,y);
                x=rand(0,640);                  // Pon la nave en una posici¢n aleatoria
                y=rand(0,480);
            END
        ELSE
            hiper=1;                            // Hace que los hiperespacios salgan de uno en uno
        END

        FRAME;
    END
END

//-----------------------------------------------------------------------------
// Proceso para manejar los disparos de la nave
// Entradas: Coordenadas de la nave y  ngulo de la misma
//-----------------------------------------------------------------------------

PROCESS disparo_nave(x,y,angle);

PRIVATE
    cont=20;                        // Contador de uso general

BEGIN
    sound(sonido_fuego,100,100);    // Sonido de disparo
    graph=2;                        // Selecciona el gr fico
    REPEAT
        x+=get_distx(angle,16);     // Calcula las coordenadas respecto a la nave
        y+=get_disty(angle,16);

        // Comprueba si se ha salido de la pantalla y los arregla
        IF (x<-16) x+=640+32; END
        IF (y<-16) y+=480+32; END
        IF (x>640+16) x-=640+32; END
        IF (y>480+16) y-=480+32; END

        FRAME;
    UNTIL (--cont==0)               // Avanza 20 pasos antes de desaparecer
END


//-----------------------------------------------------------------------------
// Proceso para manejar los objetos de los asteroides
// Entradas: Coordenadas y gr fico necesario (cambian de tama¤o)
//-----------------------------------------------------------------------------

PROCESS asteroide(x,y,graph);

PRIVATE
    velocidad;                // Velocidad de los asteroides
    id2=0;                    // Identificador de uso general
    incr_ ngulo;              // Velocidad de giro
     ngulo2;                  // Direcci¢n de avance

BEGIN
    angle= ngulo2=rand(0,2*pi);     // Selecciona la direcci¢n del asteroide
    incr_ ngulo=rand(-pi/32,pi/32); // Y la velocidad del giro
    velocidad=graph+nivel;          // La velocidad depende de la fase del juego y del tama¤o del asteroide

    LOOP

        // Comprueba si se ha chocado con un disparo
        id2=collision(TYPE disparo_nave);
        IF (id2)
            puntuacion+=25*graph+(nivel-1)*25;  // Suma puntuaci¢n
            signal(id2,s_kill);                 // Elimina el disparo
            sound(sonido_explosion,30*(6-graph),33*graph);
            IF (graph<5)                        // Si el asteroide es muy grande
                asteroide(x,y,graph+1);         // Crea dos m s peque¤os
                asteroide(x,y,graph+1);
            END
            IF (graph==3)                       // Si es el asteroide m s grande
                asteroide(x,y,graph+1);         // Crea uno m s (3 en total)
            END
            signal(ID,s_kill);                  // Elimina el asteroide actual
        END

        // Comprueba si se ha chocado con la nave
        id2=collision(TYPE nave);
        IF (id2)
            signal(id2,s_kill);             // Elimina el proceso de la nave
            sound(sonido_explosion,200,100);// Hace sonido de destrucci¢n
            volumen=0;

            pieza(id2.x,id2.y,id2.angle,6); // Destruye la nave en piezas
            pieza(id2.x,id2.y,id2.angle,7);
            pieza(id2.x,id2.y,id2.angle,8);
            pieza(id2.x,id2.y,id2.angle,9);
        END

        x+=get_distx( ngulo2,velocidad);    // Mueve los asteroides
        y+=get_disty( ngulo2,velocidad);

        // Comprueba que si se ha salido de pantalla y actualiza la posici¢n en consecuencia
        IF (x<-16)  x+=640+32;   END
        IF (y<-16)  y+=480+32;   END
        IF (x>640+16) x-=640+32; END
        IF (y>480+16) y-=480+32; END

        angle+=incr_ ngulo;                 // Gira el asteroide
        FRAME;
    END
END


//-----------------------------------------------------------------------------
// Proceso para los objetos de las piezas cuando la nave ha sido destruida
// Entradas: Coordenadas,  ngulo de la nave, necesarios, y gr fico
//-----------------------------------------------------------------------------


PROCESS pieza(x,y,angle,graph);

PRIVATE
     ngulo2;                        // Angulo aleatorio
    incr_ ngulo;                    // Velocidad de giro

BEGIN
     ngulo2=rand(0,2*pi);           // Reinicia variables al azar para los  ngulos
    incr_ ngulo=rand(-pi/32,pi/32);

    REPEAT
        angle+=incr_ ngulo;         // Gira los gr ficos
        x+=get_distx( ngulo2,2);    // Los mueve
        y+=get_disty( ngulo2,2);
        size-=2;                    // Y los re-escala (cada vez m s peque¤os)
        FRAME;
    UNTIL (size<=0)

    muerte=1;                       // Actualiza la variable para indicar que has muerto
END

//-----------------------------------------------------------------------------
// Proceso para el objeto que las naves del marcador de vidas
// Entradas: Coordenadas del gr fico
//-----------------------------------------------------------------------------

PROCESS nave_peque¤a(x,y);

BEGIN
    graph=1;                    // Elige el gr fico
    size=75;                    // Hace que sea m s peque¤o que con la nave que juegas

    LOOP
        angle+=pi/64;           // Gira el gr fico
        FRAME;
    END

END


//-----------------------------------------------------------------------------
// Proceso para el objeto del hiper-espacio, solo muestra el gr fico y el sonido
// Entradas: Coordenadas del gr fico
//-----------------------------------------------------------------------------

PROCESS hiperespacio(x,y);

BEGIN
    sound(sonido_hiperespacio,180,400); // Realiza el sonido
    graph=10;                           // Selecciona el gr fico
    WHILE (size>0)                      // Repite hasta que desaparezca
        size-=5;                        // Hace que el gr fico sea m s peque¤o
        FRAME;
    END
END
