//-----------------------------------------------------------------------------
//
//            ************************************************
//            *****   Ejemplo de juego de plataformas    *****
//            ************************************************
//
//      Creado por: Carlos Dominguez
//      Para cualquier comentario (que agradeceria)
//      E-Mail: carlosd@lettera.skios.es
//
//      Proceso 'gravedad (offset_sup,offset_inf)' basada en
//      el proceso 'gravedad()' del juego Castle of Dr Malvado de
//      Daniel Navarro (espero que no te importe, funciona muy bien)
//
//      Agradecimientos a todo el equipo de DIV Games Studio, que hace
//      posible a programadores 'no tan buenos' realizar programas GENIALES
//      de una forma sencilla.
//
//
//      * Instrucciones: Usa las granadas atomicas para acabar con todos los
//      tanques de vigilancia enemigos. Si te quedas sin municion te
//      enviaremos otra carga desde el avion nodriza. ­Ten cuidado! Si el
//      enemigo detecta que has destruido todas sus tropas, enviara
//      refuerzos.
//
//
//      Teclas: Arriba - Salto
//              Derecha - Derecha
//              Izquierda - Izquierda
//              Espacio - Disparo
//
//              Esc - Salir del juego
//
//-----------------------------------------------------------------------------

PROGRAM Plataformas;

GLOBAL
fuerza_salto=17;     // Fuerza con que salta el personaje
municion=0;          // Cantidad de municion que le queda al personaje
sin_municion=TRUE;   // Indicador para "mandar" mas municion si te quedas sin balas
enemigos=0;          // Cantidad de enmigos que hay en la pantalla (se podria usar para cambiar de fase cuando llege a 0

LOCAL
incx=0;                  // Cantidad que avanza el objeto
velocidad_gravedad=0;    // Velocidad a la que esta bajando o subiendo el objeto
en_suelo=FALSE;          // Indica si el objeto esta sobre el suelo o en el aire

BEGIN

    set_mode(m640x480);           // Ajustamos el modo de pantalla
    load_fpg ("user\platafor\platafor.fpg");    // Cargamos los graficos
    put_screen (0,500);           // Pintamos el fondo

    write (0,15, 20, 0, "MUNICION:");     // Pintamos los marcadores
    write_int (0, 80, 20, 0, &municion);

    write (0, 520, 20, 0, "N§ Enemigos:");
    write_int (0, 600, 20, 0, &enemigos);

    personaje(70,350);         // Creamos a nuestro personaje
    bonus(590,15);             // Creamos un objeto (en este caso municion)
    enemigo (160,260,90,230);  // Creamos los enemigos
    enemigo (110,35,65,150);
    enemigo (130,400,100,600);

    plataforma (590,230,80,230); // Creamos una plataforma que sube y baja
    control();                   // Pasamos a la rutina de control general del programa

END

//-----------------------------------------------------------------------------
//                  Control general del juego
//-----------------------------------------------------------------------------

PROCESS control();

PRIVATE
id_txt;  // Identficador para poder borrar el texto que escribimos

BEGIN

    timer=0;    // Inicia el timer (cronometro)

    LOOP        // Comienza el bucle

    // Comprueba si no hay enemigos. Si no quedan pone un mensaje
    IF (enemigos==0) id_txt=write (0,320,240,4,"TE LOS HAS CARGADO A TODOS");

    FRAME (10000);                // Pausa
    delete_text(id_txt);          // Borra el texto

    enemigo (160,260,90,230);     // Pone mas enemigos
    enemigo (110,35,65,150);
    enemigo (130,400,100,600);

    END

    // Comprueba que no te quede municion y que no haya ningun objeto de bonus en la pantalla y escribe un mensaje
    IF (municion==0 AND NOT get_id(TYPE bonus)) id_txt=write (0,320,240,4,"­ATENCION! Municion agotada");

    FRAME (10000);                // Pausa
    delete_text(id_txt);          // Borra el texto

    bonus (rand(20,620),-40);     // Crea un nuevo objeto en una posicion (de x) al azar

    END

    IF (timer>100) timer=0; END   // Inicia el timer cada segundo

    IF (key(_esc)) let_me_alone(); BREAK; END // Sale del programa si se pulsa Esc

    FRAME;        // Pasa el control a los demas procesos
    END

END

//-----------------------------------------------------------------------------
//                  Proceso para el personaje principal
//-----------------------------------------------------------------------------

PROCESS personaje(x,y);

PRIVATE
pri_cuad=1;    // Control de la animacion
ult_cuad=1;
disparo=1;     // Bloqueo de la tecla de disparo
BEGIN


    LOOP

        FOR (graph=pri_cuad ; graph<=ult_cuad ; graph=graph+1) // Animacion del personaje


        IF  (key(_left))
        incx=-10; pri_cuad=2; ult_cuad=12; flags=1; // Movimiento a la izquierda

        ELSE IF  (key(_right))                      // Movimiento a la derecha
        incx=10; pri_cuad=2; ult_cuad=12; flags=0;

        ELSE pri_cuad=1;  ult_cuad=1; incx=0; END   // Si no se mueve ni hacia la izquierda ni la derecha ponemos la animacion de quieto

        END



        IF  (key(_up) AND en_suelo) velocidad_gravedad=-fuerza_salto; END // Salta si esta sobre el suelo

        IF (velocidad_gravedad<>0 OR NOT en_suelo) graph=14; END          // Ajusta los graficos segun suba o baje
        IF (velocidad_gravedad > 0 AND NOT en_suelo) graph=15; END


        IF (key(_space))        // Dispara si tiene municion
            IF (disparo)
                disparo=0;
                IF (municion>0) disparo_yo(x,y,flags); municion--; END
            END
        ELSE disparo=1; END


        corrige_coordenadas();   // Si sale por un lado, entra por el otro
        gravedad(16,23);         // Fuerza de gravedad y suelos

        paredes (42);            // Choque con las paredes
        x+=incx;                 // Movimiento horizontal

        FRAME;
        END
    END
END //Fin de PERSONAJE

//-----------------------------------------------------------------------------
//                  Proceso para la muerte del prota
//-----------------------------------------------------------------------------
PROCESS muerto(x,y,direccion);

BEGIN

graph=96;                                   // Pone el grafico inicial del proceso
velocidad_gravedad=-10;                     // Lo hace subir un poco

IF (direccion) incx=-10; ELSE incx=10; END  // Mueve el grafico segun de donde le venga la bala

FROM angle=0 to 90000 STEP 6000;            // Va girando el grafico
IF (angle>40000) graph=97; END              // y cambiandolo segun el angulo
IF (angle>60000) graph=98; END

x+=incx;                                                //Lo hace avanzar
IF (incx>0) incx--; ELSE IF (incx<0) incx++; END END    // y  lo va frenando poco a poco
IF (en_suelo) incx=0; angle=90000; graph=98; END        // Si toca el suelo lo pone horizontal de golpe

corrige_coordenadas();
gravedad(11,11);

FRAME;
END

LOOP

x+=incx;                                               // Es lo mismo de antes, por si antes de llegar
IF (incx>0) incx--; ELSE IF (incx<0) incx++; END END   // al suelo se puso en posicion horizontal
IF (en_suelo) incx=0; END
corrige_coordenadas();
gravedad(11,11);

IF (en_suelo) FRAME(800); fade_off();                    // Cuando llega al suelo hace una pequeña pausa
personaje(70,350); fade_on(); signal (id,s_kill); END    // crea un nuevo personaje, y se auto-destruye

FRAME;

END

END


//-----------------------------------------------------------------------------
//                  Proceso para el disparo del prota
//-----------------------------------------------------------------------------

PROCESS disparo_yo(x,y,direccion);
PRIVATE
id2=0;
BEGIN

velocidad_gravedad=-20;        // Lo hace subir
size=50;

IF (direccion>0) incx=-20; ELSE incx=20; END  // Ajusta incx segun la direccion del muñeco

 WHILE (NOT en_suelo)                  // Repite hasta llegar al suelo

    FROM graph = 52 to 62;             // Animacion
    x+=incx;                           // Avanza en la direccion horizontal
    IF (incx>0) incx--; ELSE IF (incx<0) incx++; END END // Lo frena poco a poco
    corrige_coordenadas();
    gravedad (11,11);
    paredes(23);
    FRAME;
    END

  END

size=100;
FROM graph = 63 TO 75;                   // Comienza la explosion
    id2=collision (TYPE enemigo);        // Mira si toca algun enemigo
    IF ( id2 )                           // Si lo toca
    explosion (id2.x , id2.y, 50);       // Crea una explosion
    signal (id2,s_kill);                 // Se carga al enemigo
    enemigos--;                          // Quita un enemigo de la cuenta
    END
FRAME;

END


END //Fin de ENEMIGO

//-----------------------------------------------------------------------------
//                  Proceso plataforma
//-----------------------------------------------------------------------------
PROCESS plataforma(x,y,y1,y2);
PRIVATE
id2=0;
direccion=-1;
BEGIN

  graph=99; size=50;

  LOOP

    //Movimiento

      IF (direccion==-1 ) y+=2; IF (y>y2) direccion=1; END    // Mueve la plataforma en la direccion indicada por direccion
                     ELSE y-=2; IF (y<y1) direccion=-1; END   // Si llega al limite cambia direccion

      END

    // Deteccion de colision con personaje

    id2=collision (TYPE personaje);        // Si colisiona con el personaje
    IF ( id2 )
    IF (id2.y < y )                        // y esta por debajo de el,
    id2.y = y - 28;                        // lo sube un poco
    id2.en_suelo=true;                     // le activa en_suelo
    id2.velocidad_gravedad=0;              // y le quita la gravedad
    END
    END

    // Deteccion de colision con disparos personaje

    id2=collision (TYPE disparo_yo);        // Igual que en personaje
    IF ( id2 )
    IF (id2.y < y )
    id2.y = y - 10;
    id2.en_suelo=true;
    id2.velocidad_gravedad=0;
    END
    END


    // Deteccion de colision con objeto

    id2=collision (TYPE bonus);            // Igual que en personaje
    IF ( id2 )
    IF (id2.y < y )
    id2.y = y - 20;
    id2.en_suelo=true;
    id2.velocidad_gravedad=0;
    END
    END

  FRAME;

  END

END //Fin de PLATAFORMA

//-----------------------------------------------------------------------------
//                  Proceso que crea una explosion
//-----------------------------------------------------------------------------

PROCESS explosion(x,y,size);
BEGIN

FROM graph = 76 TO 95;    // Ajusta la animacion
y-=5;                     // La hace subir un poco
FRAME;
END

END


//-----------------------------------------------------------------------------
//                  Proceso para objeto de ejemplo
//-----------------------------------------------------------------------------

PROCESS bonus(x,y);
PRIVATE
id2=0;
BEGIN

  LOOP

    FROM graph = 50 to 51;             // Ajusta la animacion

    id2=collision (TYPE personaje);    // Si el personaje lo toca...
    IF ( id2 )
    municion=5;                        // le da 5 de municion
    signal (id,s_kill);                // Se auto-destruye
    END

   IF (en_suelo) velocidad_gravedad=-10; END //Si toca el suelo, lo hace botar
    gravedad (11,11);
    FRAME;
    END

  END

END //Fin de BONUS

//-----------------------------------------------------------------------------
//                  Proceso para enemigo de ejemplo
//-----------------------------------------------------------------------------
PROCESS enemigo(x,y,x1,x2);
PRIVATE
id2=0;
direccion=-1;
BEGIN

  enemigos++;            // Acaba de crear un enemigo, suma uno al contador

  LOOP

    FROM graph = 30 to 31;             // Pone la animacion

    id2=collision (TYPE personaje);    // Si toca al personaje
    IF ( id2 )
    signal (id2,s_kill);               // se lo carga
    muerto(id2.x, id2.y,direccion);    // y pone un muerto en su lugar
    END

    //Movimiento
    IF (en_suelo)    // Solo se mueve si esta en el suelo
    IF (direccion==-1 ) x+=2; flags=0; IF (x>x2) direccion=1; END
                   ELSE x-=2; flags=1; IF (x<x1) direccion=-1; END
    END
    END

       IF (rand(1, 100) > 95) disparo( x - 10 * direccion , y - 10 ,direccion); END    // Disparo al azar
    // IF (timer<10)  disparo( x - 10 * direccion , y - 10 ,direccion); END        // Disparo cada segundo

    corrige_coordenadas();
    gravedad (15,11);
    FRAME;
    END

  END

END //Fin de ENEMIGO



//-----------------------------------------------------------------------------
//                  Proceso para el disparo de los enemigos
//-----------------------------------------------------------------------------


PROCESS disparo(x,y,direccion);
PRIVATE
id2=0;
BEGIN

graph=32;

IF (direccion>0) incx=-25; ELSE incx=25; END  // Ajusta la direccion del proyectil

    WHILE (NOT out_region(id,0))              // Mientras este en la pantalla

    id2=collision (TYPE personaje);           // Si toca al personaje
    IF ( id2 )
    signal (id2,s_kill);                      // Se lo carga y
    muerto(id2.x, id2.y,direccion);           // pone un muerto en su sitio
    END

    x+=incx;                    // Avance horizontal
    FRAME;
    END

END

//-----------------------------------------------------------------------------
//                  Proceso para continuidad horizontal
//-----------------------------------------------------------------------------

PROCESS corrige_coordenadas()
BEGIN
    IF (father.x<1) father.x=father.x+640; END       // Si se va a salir por un lado, lo mete por el otro
    IF (father.x>639) father.x=father.x-640; END
END


//-----------------------------------------------------
//  Proceso para calcular el choque con las paredes
//-----------------------------------------------------

PROCESS paredes(alto);

PRIVATE
c_pared=73;               // Posicion en la paleta del color para las paredes
contx,new_incx=0;
BEGIN

IF (father.incx <> 0)     // Solo se efectua la comprobacion si se mueve

contx=father.incx/abs(father.incx);    // Hallamos la direccion del movimiento 1 o -1

FOR (new_incx=0; new_incx <> father.incx ; new_incx=new_incx + contx) // Recorremos los puntos entre el grafico y la posicion que ocupara

// Si encuentra una pared, ajusta el nuevo incx y sale
// Realiza comprobaciones a distintas alturas del grafico

IF (map_get_pixel (0,999, father.x + new_incx + contx, father.y + alto /2 ) == c_pared
 OR map_get_pixel (0,999, father.x + new_incx + contx, father.y +  alto /4) == c_pared
 OR map_get_pixel (0,999, father.x + new_incx + contx, father.y  ) == c_pared
 OR map_get_pixel (0,999, father.x + new_incx + contx, father.y - alto /4) == c_pared
 OR map_get_pixel (0,999, father.x + new_incx + contx, father.y - alto /2) == c_pared)

father.incx=new_incx; BREAK; END

END

END
END


//-----------------------------------------------------------------------------
//                      Proceso para la gravedad
//-----------------------------------------------------------------------------

PROCESS gravedad(offset_sup, offset_inf);

PRIVATE
    c_suelo=228;
    c_techo=40;
    gravedad_temp;

BEGIN
    father.en_suelo=FALSE;
    gravedad_temp=(father.velocidad_gravedad+=2);  // Aumenta la gravedad del objeto
    IF (gravedad_temp>15)                          // Y establece un limite de gravedad de 15
    father.velocidad_gravedad=15;
    gravedad_temp=15;
    END

    IF (gravedad_temp<0)         // Si el objeto sube

    WHILE (gravedad_temp++!=0)   // Hasta que deje de subir

    IF (map_get_pixel(0,999,father.x,(father.y-offset_sup))<>c_techo) // Comprueba que no da con el techo
    father.y--;                                                       // Lo mueve, o
    ELSE father.velocidad_gravedad=0; BREAK; END                      // lo para y sale

    END

    ELSE  father.y+=gravedad_temp;     // Si el objeto baja

   FROM gravedad_temp=-15 TO 8;        // Establece un rango de comprobacion
   IF (map_get_pixel(0,999,father.x,(father.y+gravedad_temp+offset_inf))==c_suelo) // Si toca el suelo
   father.en_suelo=TRUE;  BREAK;       // Pone en_suelo y sale
   END
   END

   IF (gravedad_temp<9)                // Ajusta los nuevos valores de y
   father.y+=gravedad_temp;
   father.velocidad_gravedad=0;

   END
   END

END


//-----------------------------------------------------
//                 Fin del listado
//-----------------------------------------------------
