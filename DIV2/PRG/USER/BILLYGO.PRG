PROGRAM marciano;
GLOBAL
// variables del Juego
  id_joc;
  id_su22;
  local_su22_x;
  local_su22_y;
  size_su22;
  activacio;         // variable del fuego de la nave
  risas;             // las risas del principio
  id_snd_warp;       // identificador del sonido Warp
  id_boss_cupula;    // identificador de la cupula del enemigo final
  id_pal;            // ident. de la paleta
  id_fpg_marciano;   // ident. del archivo fpg llamado marciano
  id_fpg2;           // ident. del segundo fpg
  id_fnt_marciano;   // ident. de las fuentes
  inc_verde;         // almacena el incremento del disparo verde
  inc_amarillo;      // almacena el incremento del disparo amarillo
  estado_programa=1; // inicializa el estado del programa
  puntuacio=0;       // pone a cero la puntuacion
// sonidos
  snd_BBird;          // ident. sonido del BBird
  snd_haha;          // ident. del sonido de las risas
  snd_explosio;      // ident. del sonido de la explosio
  snd_heli;          // ident. del sonido del helicoptero
  snd_bigboom;       // ident. del sonido de la explosion final
  snd_laser;         // ident. del sonido del laser
  snd_disparo;       // ident. del sonido del disparo
  palmao;            // estado de la nave vivo=0 muerto=1
  idnave;            // identificador de la nave principal
  local_x;           // variable que guardara la x de la nave principal
  local_y;           //   "       "      "    "  y "  "   "      "
  local_boss_x;      //   "       "      "    "  x del enemigo final
  local_boss_y;      //   "       "      "    "  y  "     "      "
  graf_disp=3;       // grafico del disparo
  vidas_nave;        // vidas de la nave;


      //TABLA DE LOS ENEMIGOS(helicoptero=1, mig29=2, BBird=3, final boss=99)

  TAULA_ENEMICS[]= 2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,1,1,2,2,
                   2,2,1,2,2,2,2,1,1,2,2,2,2,2,1,2,1,2,2,2,
                   1,2,2,2,1,2,1,2,2,1,3,2,1,2,2,3,3,3,3,1,
                   1,2,1,1,3,1,1,1,2,2,2,2,2,2,1,1,3,1,3,4,
                   1,2,1,3,2,2,1,3,2,1,3,4,4,2,1,2,1,1,4,3,
                   1,1,4,1,2,1,2,4,3,3,2,1,2,3,4,4,2,2,4,4,
                   1,3,1,1,3,4,1,1,1,2,3,1,2,4,2,4,2,4,1,4,
                   2,2,2,2,2,2,2,1,4,1,2,2,4,4,4,4,4,4,4,4,99;




 //TABLA DEL TIEMPO(los numeros indican los frames que hay entre enemigo y enemigo)

  TAULA_TEMPS[]=   50,20,20,20,0,20,20,10,20,20,20,20,20,20,40,10,30,30,10,0,
                   20,30,20,10,20,10,40,20,30,10,10,10,20,40,40,0,15,20,20,30,
                   5,10,5,0,10,10,10,25,0,30,30,20,10,20,20,40,5,10,5,20,
                   10,10,25,0,20,10,10,10,50,5,5,5,5,10,50,10,10,10,10,20,
                   10,10,10,20,10,0,0,10,10,10,5,5,20,30,0,0,30,20,20,10,
                   30,0,10,0,15,10,20,20,10,10,10,20,0,10,10,10,20,10,30,0,
                   20,10,20,10,10,10,20,20,15,10,20,10,20,30,20,20,20,20,10,20,
                   50,10,10,0,10,10,30,20,10,20,20,40,0,0,0,50,0,0,0,150;


  //TABLA DE LOS POWER_UPS(nada=0, verde=37, amarillo=38)

  TAULA_POWER[]=   0,0,0,0,0,0,0,37,0,0,0,0,0,0,0,0,0,0,0,0,
                   0,0,0,0,0,0,0,0,0,0,0,0,38,0,0,0,0,0,0,0,
                   0,0,0,0,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,0,0,0,0,0,
                   0,0,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,
                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,37,
                   0,0,0,0,0,0,0,0,38,0,0,0,0,0,0,0,0,0,0,0,
                   0,0,0,0,37,0,0,0,0,0,0,0,0,0,0,0,38,0,0,0,0;



LOCAL
  tipo_power;
  VIDA;              // vida de los enemigos
  contador;          // contadores
  contador2;
  contador3;
  id_colision;       // identificador de las colisiones
  inc_x;             // incremento de x
  inc_y;             // incremento de y
  animacio;

   BEGIN
     set_mode(m640x480);           // modo de video
     LOOP
       SWITCH(estado_programa);
         CASE 1: PRESENTACIO();  END       // presentacion
         CASE 2: id_joc=JOC();  END               // jugar al marciano
         CASE 3: CREDITS();  END           // los creditos
         CASE 4: GAMEOVER();  END          // pues nada, GAME OVER
       END
     FRAME;
     END
   END
//-------------------------------------------------------
PROCESS joc()
PRIVATE
  snd_fondo;        //ident. del sonido de fondo, osea la banda sonora del juego
  TIPO_ENEMICS;     //ident. del tipo de enemigos
  TEMPS;            //ident. de los frames entre enemigos
  contador4;        // un contador
BEGIN
  puntuacio=0;      // inicia la puntuacion a 0
  inc_verde=0;      //    "   el incremento de disparo verde
  inc_amarillo=0;   //    "    "     "       "    "    amarillo
  graf_disp=3;
  vidas_nave=3;     // maximo de vida
  palmao=0;         // palmao=0 estas vivo, palmao=1 ta muerto
  estado_programa=0;   // deja el estado del programa inactivo
  SIGNAL(TYPE diari,s_kill);   // mata el processo diari
  set_fps(25,0);     // eso es!!, a 25 frames por segundo!!
  load_pal("USER\BILLYGO\div.pal"); // carga la paleta
  fade_on();                        // enciende la pantalla
  snd_fondo=Load_pcm("USER\BILLYGO\fondo.pcm",1);      // carga los sonidos del juego
  snd_BBird=Load_pcm("USER\BILLYGO\BBird.pcm",0);
  snd_laser=Load_pcm("USER\BILLYGO\laser.pcm",0);
  snd_explosio=load_pcm("USER\BILLYGO\explosio.pcm",0);
  snd_heli=load_pcm("USER\BILLYGO\helico.pcm",1);
  snd_bigboom=Load_pcm("USER\BILLYGO\bigboom.pcm",0);
  snd_disparo = Load_pcm("USER\BILLYGO\disparo.pcm",0);
  start_scroll(0,0,19,2,0,10);     // inicia el scroll
  vidas(600,40,35,90000);          // inicia el processo de las vidas
  punts();                         // inicia el processo de puntos
  idnave=nave(x,y,90000,90);       // inicia el processo nave
  SCROLL[0].y0=0;                  // pone en marcha el scroll
  SCROLL[0].y1=960;
  sound(snd_fondo,250,256);        // empieza a sonar el sonido de fondo
  LOOP

// comprueba si la nave esta viva o muerta
    IF(palmao==0);                 // si la nave esta viva
      SCROLL[0].y0-=2; // mueve el primer plano de scroll
      SCROLL[0].y1-=1; // mueve el segundo plano de scroll

// Este bucle va leyendo las tablas y cada x tiempo envia unos valores al
// process enemics con lo que este ultimo llama a otros process que crean
// los enemigos
      IF (contador3<161)
        TEMPS=TAULA_TEMPS[contador3];
        IF(contador2==TEMPS)
          TIPO_ENEMICS=TAULA_ENEMICS[contador];
          TIPO_POWER=TAULA_POWER[contador];
          contador+=1;
          contador2=0;
          contador3+=1;
          ENEMICS(TIPO_ENEMICS,TIPO_POWER);  // llama el processo enemics
        ELSE
          contador2+=1;
        END
      END
      FRAME;
    ELSE     // si la nave esta muerta
      IF(vidas_nave>0);
        IF(contador4==25)   // espera 25 frames
          contador4=0;
          palmao=0;      // la nave esta viva
          Idnave=nave(x,y,90000,90);   // inicia el processo nave
          SIGNAL(TYPE regal,s_wakeup);  // despierta el processo regal
        ELSE
          contador4+=1;
        END
      ELSE
      estado_programa=4;   // Game Over
      END
      FRAME;
    END
    IF(key(_esc)) estado_programa=4; END  // para salir apretar esc
  END
END
//----------------------------------------------------------------------
PROCESS nave(x,y,angle,size)      // processo de la nave principal

PRIVATE
  disparo_permitido=1;  // variable que controla los disparos
  disparo_permitido2=1;
  id_foc_reactor;       // identificador del process foc_reactor
BEGIN
  z=-599;               // coloca el grafico encima de todos los otros
  x=320;                // posicion x de la nave
  graph=1;              // grafico 1
  flags=4;              // transparente
  id_foc_reactor=foc_reactor();  // inicia el processo foc_reactor
  FROM y=500 to 300 step -10;    // este bucle mueve la nave de la y=500
                                 // hasta y=300 de diez en diez
    local_x=x;          // variable que guarda la x de la nave principal
    local_y=y;          // variable que guarga la y de la nave principal
    FRAME;
  END
  LOOP
   IF(contador<100)    // durante 100 frames
     flags=4;          // la nave es transparente
     contador+=1;
   ELSE
     flags=0;          // despues vuelve a la normalidad
   END
// movimiento de la nave
      IF(key(_up) and y>40)  // si apretas el boton flecha arriba y la Y es mas grande que 40
        activacio=31;    // guarda el valor 31 en la variable
        y=y-14;          // mueve la nave 12 pixel para arriba
      ELSE
        activacio=32;     // guarda el valor 32 en la variable
      END
      IF(key(_down) and y<440)  // si apretas el boton flecha abajo y la Y es menor que 440
        y=y+14;           // mueve la nave 12 hacia abajo
      END
      IF(key(_right) and x<615) // boton flecha derecha y la X es menor que 615
        x=x+12;           // mueve 10 pixel hacia la derecha
      END
      IF(key(_left) and x>25)  // boton flecha izquierda y la X es mayor que 25
        x=x-12;           // 10 pixels a la izquierda
      END;

// hace que el disparo de la nave no sea seguido si mantienes la tecla apretada
      IF(key(_control) or key(_space))  // si apretas control
        IF (disparo_permitido)
          disparo_permitido=0;
          IF(graf_disp==3)              // si el grafico es igual a 3
            SOUND(snd_disparo,90,256);  // suena el disparo
            SWITCH(inc_amarillo)        // segun el incremento amarillo
              CASE 0:                   // si es 0 crea 2 disparos
                disparo(x-5,y,90000);
                disparo(x+5,y,90000);
              END
              CASE 1:                   // si es 1 crea 3 disparos
                disparo(x,y,90000);
                disparo(x-5,y,95000);
                disparo(x+5,y,85000);
              END
              CASE 2:                   // si es 2 crea 5 disparos
                disparo(x,y,90000);
                disparo(x-5,y,95000);
                disparo(x+5,y,85000);
                disparo(x-5,y,100000);
                disparo(x+5,y,80000);
              END
              CASE 3:                   // si es 3 crea 7 disparos
                disparo(x,y,90000);
                disparo(x-5,y,95000);
                disparo(x+5,y,85000);
                disparo(x-5,y,100000);
                disparo(x+5,y,80000);
                disparo(x-5,y,105000);
                disparo(x+5,y,75000);
              END
              CASE 4:                  // si es 4 crea 9 disparos
                disparo(x,y,90000);
                disparo(x-5,y,95000);
                disparo(x+5,y,85000);
                disparo(x-5,y,100000);
                disparo(x+5,y,80000);
                disparo(x-5,y,105000);
                disparo(x+5,y,75000);
                disparo(x-5,y,110000);
                disparo(x+5,y,70000);
              END
              CASE 5:
                disparo(x,y,90000);    // si es 5 crea 11 disparos
                disparo(x-5,y,95000);
                disparo(x+5,y,85000);
                disparo(x-5,y,100000);
                disparo(x+5,y,80000);
                disparo(x-5,y,105000);
                disparo(x+5,y,75000);
                disparo(x-5,y,110000);
                disparo(x+5,y,70000);
                disparo(x-5,y,115000);
                disparo(x+5,y,65000);
              END
            END
          ELSE
           SOUND(snd_laser,90,100);    // suena el laser
           disparo(x,y-30,90000);      // crea el laser
          END
        END
      ELSE
        disparo_permitido=1;
      END

// asi se puede saber donde esta la nave siempre que  queramos

      local_x=x;
      local_y=y;
      FRAME;
  END
END
//---------------------------------------------------------------------
PROCESS disparo(X,Y,angle)         // processo disparo de la nave principal
PRIVATE
BEGIN
  z=-597;
  graph=graf_disp;
  REPEAT

// controla si el disparo colisiona con algun enemigo
      id_colision=collision(TYPE mig29);

      IF(id_colision XOR collision(TYPE helicoptero) XOR collision(TYPE shoting_laser_dret) XOR collision(TYPE shoting_laser_esquerra) XOR collision(TYPE ca¤on_boss_dreta_amunt) XOR collision(TYPE ca¤on_boss_esquerra_amunt) XOR collision(TYPE ca¤on_boss_dreta_aball) XOR collision(TYPE ca¤on_boss_esquerra_aball) XOR collision(type boss_cupula) XOR collision(type BBird) XOR collision(type su22))
        SWITCH(graf_disp);     // segun el grafico de disparo
          CASE 3:              // si es grafico es el 3
            IF(id_colision.graph<>48)    // si el grafico con que colisiona es diferente a 48
             IF(id_colision.size==100 or id_joc.contador3==161)
              id_colision.vida-=5;       // resta vida a la nave que ha tocado
              IF(id_colision.vida>0) explosio(x,y,96,100); END   // si la vida con el que ha colisionado es mas grade que 0
              BREAK;    // se suicida el processo
             END
            ELSE
              IF(id_colision.vida>0) explosio(x,y,97,100); END   // si la vida...
              BREAK;
            END
          END

// a partir de aqui hasta el final del SWITCH todo es lo mismo
// pero con diferentes valores

          CASE 38:
            IF(id_colision.graph<>48)
             IF(id_colision.size==100 or id_joc.contador3==161)
              id_colision.vida-=20;
              IF(id_colision.vida>0) explosio(x,y,96,80); END
              BREAK;
             END
            ELSE
              IF(id_colision.vida>0) explosio(x,y,95,80); END
              BREAK;
            END
          END
          CASE 39:
            IF(id_colision.graph<>48)
             IF(id_colision.size==100 or id_joc.contador3==161)
              id_colision.vida-=30;
              IF(id_colision.vida>0) explosio(x,y,96,100); END
              BREAK;
             END
            ELSE
              IF(id_colision.vida>0) explosio(x,y,95,100); END
              BREAK;
            END
          END
          CASE 40:
            IF(id_colision.graph<>48)
             IF(id_colision.size==100 or id_joc.contador3==161)
              id_colision.vida-=40;
              IF(id_colision.vida>0) explosio(x,y,96,120); END
              BREAK;
             END
            ELSE
              IF(id_colision.vida>0) explosio(x,y,95,120); END
              BREAK;
            END
          END
          CASE 41:
            IF(id_colision.graph<>48)
             IF(id_colision.size==100 or id_joc.contador3==161)
              id_colision.vida-=50;
              IF(id_colision.vida>0) explosio(x,y,96,140); END
              BREAK;
            END
           ELSE
              IF(id_colision.vida>0) explosio(x,y,95,140); END
              BREAK;
            END
          END
          CASE 42:
            IF(id_colision.graph<>48)
             IF(id_colision.size==100 or id_joc.contador3==161)
              id_colision.vida-=60;
              IF(id_colision.vida>0) explosio(x,y,96,160); END
              BREAK;
             END
            ELSE
              IF(id_colision.vida>0) explosio(x,y,95,160); END
              BREAK;
            END
          END
        END
      END
      ADVANCE(16);   // avanza 16 en el angulo deseado
      FRAME;
  UNTIL(y<0);     // hasta que Y sea menor que 0
END
//---------------------------------------------------------------------
PROCESS muerte(x,y)   // processo muerte de la nave principal
BEGIN
  palmao=1;
  SIGNAL(TYPE nave,s_kill_tree);     // mata el processo nave
  explosio(x,y,1,100);               // inicia el processo explosio
  WHILE(contador<inc_verde-1)
    regal(x,y,37);
    contador+=1;
  END
  WHILE(contador2<inc_amarillo-1)
    regal(x,y,38);
    contador2+=1;
  END
  vidas_nave-=1;                     // resta una vida
  graf_disp=3;                       // inicia el grafico de disparo
  inc_verde=0;         // inicia los valores de incremeto de disparo verde
  inc_amarillo=0;      //   "     "     "     "     "     "     "    amarillo
END
//-----------------------------------------------------------------------
PROCESS explosio(x,y,id_explosio,size)      // processo explosio
PRIVATE
 explo;
BEGIN
  z=-596;
  SWITCH(id_explosio)      // segun el valor de la variable id_explosio
   CASE 1:
     SOUND(snd_explosio,180,256);     // ejecuta el sonido de la explosion
     FROM explo= 4 to 9;               // anima la explosion
       graph=explo;
       FRAME;
     END
   END
   CASE 95:
     FROM explo=101 to 108;          // anima la explosion
       graph=explo;
       FRAME;
     END
   END
   CASE 96:
     FROM explo=90 to 95;            // anima la explosion
       graph=explo;
       FRAME;
     END
   END
   CASE 97:
     FROM explo=96 to 99;           // anima la explosion
       graph=explo;
       FRAME;
     END
   END
   CASE 98:
     SOUND(snd_explosio,180,256);     // ejecuta el sonido de la explosion
     FROM explo= 80 to 89;               // anima la explosion
       graph=explo;
       FRAME;
     END
   END
   CASE 99:
     idnave.contador=0;    // pone a 0 el contador de la nave con lo cual se vuelve transparente
     SOUND(snd_bigboom,200,256);     // ejecuta el sonido de la explosion
     SIGNAL(TYPE ca¤on_boss_dreta_aball,s_kill);  // mata estos processos
     SIGNAL(TYPE ca¤on_boss_esquerra_aball,s_kill);
     SIGNAL(TYPE ca¤on_boss_dreta_amunt,s_kill);
     SIGNAL(TYPE ca¤on_boss_esquerra_amunt,s_kill);
     SIGNAL(TYPE shoting_laser_dret,s_kill);
     SIGNAL(TYPE shoting_laser_esquerra,s_kill);
     SIGNAL(TYPE enemigo_final,s_kill);
     FROM explo=69 to 80;     // anima la explosion
       graph=explo;
       FRAME;
     END
     LEWINSKY(local_boss_x,local_boss_y+100);   // inicia el process LEWINSKY
   END
  END
END
//--------------------------------------------------------------
PROCESS misil(tipo_disparo,x,y,bala_angle,SIZE)  // es el process del misil que disparan los enemigos

PRIVATE
id_nave_flags;
id_fum_misil;
BEGIN
// cada tipo de disparo tiene unas caracteristica diferentes
  z=-597;
  SWITCH(tipo_disparo)
  CASE 1:
    graph=17;
    REPEAT
      x+=get_distx(bala_angle,5);
      y+=get_disty(bala_angle,5);    // mueve la bala hacia la nave pricipal
      SIZE+=6;                       // va augmentando el tama¤o del disparo
      IF(collision(type nave) and idnave.flags==0)      // si colisiona con la nave principal la mata
        muerte(local_x,local_y);     // llama el process muerte con la posicion de la nave principal
      END
      FRAME;
    UNTIL((x>635 OR x<-5)OR(y>475 OR y<-5) OR SIZE>500)
  END
  CASE 2:
    SOUND(snd_laser,50,256); //suena el laser del helicoptero
    graph=38;
    angle=fget_angle(x,y,local_x,local_y);  //calcula el angulo entre el enemigo y la nave
    REPEAT
      x+=get_distx(bala_angle,12);    // mueve la bala hacia la nave principal
      y+=get_disty(bala_angle,12);
      IF(collision(type nave) and idnave.flags==0)       // si la toca esta se muere
        muerte(local_x,local_y);
      END
      FRAME;
    UNTIL((x>635 OR x<-5)OR(y>485 OR y<-5))
  END
  CASE 3:
    graph=21;
    sound(snd_disparo,40,200); //suena el disparo del mig 29
      REPEAT   //  este bucle crea el disparo del mig
        y+=10;
        IF(collision(type nave) and idnave.flags==0)   // si la toca esta se muere
          muerte(local_x,local_y);
        END
        FRAME;
      UNTIL(y>485)
  END
  CASE 97:
    graph=26;
    SIGNAL(TYPE Boss_cupula,s_freeze);   // congela la c£pula del enemigo final
    REPEAT    //  este bucle crea el megadisparo
      y+=20;
      IF(collision(type nave) and idnave.flags==0)       // si la toca esta se muere
        muerte(local_x,local_y);
      END
      FRAME;
    UNTIL(y>500);
    SIGNAL(TYPE boss_cupula,s_wakeup);  // despierta la cupula del enemigo final
  END
  CASE 98:
    graph=25;
    //z=-509;
    REPEAT
      y+=10;
      IF(collision(type nave) and idnave.flags==0)       // si la toca esta se muere
        muerte(local_x,local_y);
      END
      FRAME;
    UNTIL(y>500);
  END
  CASE 99:
    graph=24;
    x+=get_distx(bala_angle,30);    // mueve la bala hacia la nave principal
    y+=get_disty(bala_angle,30);
    REPEAT      // este bucle crea los disparos de las torretas del enemigo final
      x+=get_distx(bala_angle,10);    // mueve la bala hacia la nave principal
      y+=get_disty(bala_angle,10);
      IF(collision(type nave) and idnave.flags==0)       // si la toca esta se muere
        muerte(local_x,local_y);
      END
      FRAME;
    UNTIL((x>635 OR x<-5)OR(y>475 OR y<-5))
  END
  END
END
//---------------------------------------------------------------
PROCESS regal(x,y,tipo_power)      // processo regal  o sea los power ups
BEGIN
  REPEAT
    IF(inc_x==0 and inc_y==0)    //si incremento X y incremento Y  son 0
      inc_x=rand(-5,5);         // da un valor aleatorio entre -4 y 4 a incremento de X
      inc_y=rand(-5,5);         // "   "   "     "          "  "  " " "      "      " Y
    END
    graph=tipo_power;    // gr fico es igual al tipo de power up
    contador+=1;
    IF(contador==60)     // cada 60 frames cambia el tipo de power up
      tipo_power+=1;
      contador=0;
    END
    IF(tipo_power>37)   // a partir del grafico 37 vuleve a reiniciar el tipo de power up
    tipo_power=36;
    END
    IF(contador2<2)      // este bucle hace qeu el power up rebote
      IF(x>630 or x<10)  // dos veces cuando llega al borde de la pantalla
        inc_x=-inc_x;
        contador2+=1;
      END
      IF(y>470 or y<10)
        inc_y=-inc_y;
        contador2+=1;
      END
    END
    x+=inc_x;
    y+=inc_y;

// este bucle controla los power_ups de la nave principal

    IF(collision(type nave))
      IF(tipo_power==37)
        IF(inc_verde<5)
          graf_disp=38+inc_verde;
          inc_verde+=1;
        END
      ELSE
        IF(inc_amarillo<5)
          graf_disp=3;
          inc_amarillo+=1;
        END
      END
      BREAK;
    END
    FRAME;
  UNTIL(y>480 or x>640 or y<0 or x<0);
END
//-----------------------------------------
PROCESS PUNTS();     // processo de la puntuacion
PRIVATE
BEGIN
  DELETE_TEXT(all_text);
  WRITE(0,615,15,4,"SCORE");    // escribe "SCORE" en la esquina de arriba a la derecha
  WRITE_INT(0,615,25,4,OFFSET puntuacio);  // escribe los puntos ganados
  WRITE_INT(0,615,40,4,OFFSET vidas_nave); // escribe las vidas de la nave
  FRAME;
END
//-----------------------------------------------
PROCESS mig29(x,y,tipo_power)  // processo de un tipo de nave enemiga (mig 29)
PRIVATE
  MOV=0;
  inc=0;
BEGIN
  VIDA=20;  // tiene 20 puntos de vida
  Angle=90000;
  REPEAT
    graph=34;
    IF(VIDA=<0)     // sila vida es <=0
      explosio(x,y,1,110);           //llama al proceso  que crea la explosi¢n
      IF(tipo_power>0) regal(x,y,tipo_power); END  // mira si tiene power up
      puntuacio+=200;  // da 200 puntos
      punts();  //llama al proceso que escribe los puntos en pantalla
      BREAK;  // se mata a si mismo
    END
// bucle que controla el movimiento del mig 29
    IF(MOV==0)
      x+=3;
      contador+=3;
      IF(contador>120) MOV=1;  END
    END
    IF(MOV==1);
      x-=3;
      contador-=3;
      IF(contador<0)  MOV=0;  END
    END
    y+=8;
      IF(collision(TYPE nave) and idnave.flags==0)    // si colisiona con la nave prinicipal
      MUERTE(local_x,local_y);   // la mata
    END
    IF(y>0)
      IF(rand(0,100)<7) misil(3,x,y+30,get_angle(idnave),100); END
    END
  FRAME;
  UNTIL(y>650)
END
//---------------------------------------------
PROCESS helicoptero(x,inc_y,tipo_power)   // el proceso helicoptero
PRIVATE
 id_helice;      // identificador de la helice
 id_snd_heli;       // identificador del sonido de la helice
BEGIN
  VIDA=30;
  inc_x=rand(4,-4);
  y=-20;
  id_helice=helice(); // inicia el proceso helice
  id_snd_heli=sound(snd_heli,160,256);   //suena la helice
  REPEAT
      graph=109;
      angle=fget_angle(x,y,local_x,local_y);   // el helicoptero va girando hacia la posicion de la nave principal
      contador2+=1;
      IF(VIDA=<0)      // si vida igual o menor que 0
        stop_sound(id_snd_heli);    // para el sonido de la helice
        SIGNAL(id_helice,s_kill);  // mata el proceso helice
        explosio(x,y,1,130);       //llama al proceso de la explosion
        IF(tipo_power>0) regal(x,y,tipo_power); END  //mira si tiene power ups
        puntuacio+=400;  //da 400 puntos
        punts();         // llama al proceso punts
        BREAK;
      END
      IF(contador2==40)     // cada 40 frames este bucle cambia la direccion de la nave
        inc_x=rand(3,-3);
        contador2=0;
      END
      // si se acerca a los bordes horizontales de la pantalla impide  que salga
      IF(x<35) inc_x=rand(0,3); END
      IF(x>605) inc_x=rand(0,-3); END
      x=x+inc_x;
      y=y+inc_y;
      IF(collision(TYPE nave) and idnave.flags==0)
        MUERTE(local_x,local_y);
      END
      IF(y>0)
        IF(rand(0,150)<8)
          misil(2,x-15,y,get_angle(idnave),100); // dispara los misiles
          misil(2,x+15,y,get_angle(idnave),100);
        END
      END
      id_helice.x=x;     //almacena la posicion de las helices
      id_helice.y=y;
      IF(y>490 or x<-10 or x>650)
        SIGNAL(id_helice,s_kill);   // destuye el processo helice
        stop_sound(id_snd_heli);    // para el sonido
      END
      FRAME;
  UNTIL (y>490 OR x<-10 OR x>650);
END
//------------------------------------------------
PROCESS vidas(x,y,size,angle)   //este processo pone el grafico de la vida
  BEGIN
    graph=1;
    LOOP
     FRAME;
    END
  END
//-----------------------------------------------------
PROCESS ENEMICS(tipo_enemics,tipo_power) // llama los processos de los enemigos
 BEGIN
   SWITCH(tipo_enemics)
     CASE 1:  helicoptero(rand(20,620),rand(2,5),tipo_power); END
     CASE 2:  mig29(rand(60,520),-20,tipo_power); END
     CASE 3:  BBird(rand(20,620),tipo_power); END
     CASE 4:  id_su22=SU22(rand(20,620),tipo_power); END
     CASE 99: ENEMIGO_FINAL(80); END
   END
 END
//--------------------------------------------------------
PROCESS PRESENTACIO()  // este processo es... la presentacion!!!
  PRIVATE;
   id_fnt_presentacio;
   id_fpg;
   id_today;
   id_lies;
  BEGIN
   SIGNAL(TYPE GAMEOVER,S_KILL);
   estado_programa=0;
   id_fpg=load_fpg("USER\BILLYGO\billywar.fpg");       // carga el fichero de los dibujos
   risas=snd_haha=Load_pcm("USER\BILLYGO\rr.pcm",0);   // carga el fichero las risas
   id_pal=load_pal("USER\BILLYGO\div.pal");           // carga la paleta
   id_fnt_presentacio=load_fnt("USER\BILLYGO\presenta.fnt");  // carga las fuentes
   put_screen(id_fpg,29);  //pone el grafico en pantalla
   fade_on();
   LOOP
     contador+=1;
     SWITCH(contador)
       CASE 10:
         sound(snd_haha,100,256);  //suenan las risas
       END
       CASE 80:
         id_pal=load_pal("USER\BILLYGO\presentacio.pal");    // carga la paleta de la presentacion
         id_fpg2=load_fpg("USER\BILLYGO\presentacio.fpg");       // carga el fichero de los dibujos
         Fade_off();
         unload_fpg(id_fpg);  // descarga el fpg marciano
         clear_screen();
         write(id_fnt_presentacio,330,400,4,"Today a terrible danger threatens USA");
         fade_on();    // enciende la pantalla
       END
       CASE 150:
         fade_off();   // apaga la pantalla
         delete_text(all_text);      // borra todos los textos
         PUT(id_fpg2,1,320,220);     // pone el grafico de BIll Clinton
         WRITE(id_fnt_presentacio,320,380,4,"The future of the entire States");  // escribe...
         WRITE(id_fnt_presentacio,320,400,4,"lies on the hands of a single man,");
         WRITE(id_fnt_presentacio,320,420,4,"a man who governs the states of the light");
         WRITE(id_fnt_presentacio,320,65,4,"BILLY CLINTON");
         Fade_on();
       END
       CASE 310:
         delete_text(all_text);
         WRITE(id_fnt_presentacio,320,65,4,"BILLY CLINTON");
         WRITE(id_fnt_presentacio,320,380,4,"To destroy the darkness he must seek");
         WRITE(id_fnt_presentacio,320,400,4,"the help of all who are around him");
       ENd
       CASE 420:
         delete_text(all_text);
         WRITE(id_fnt_presentacio,320,65,4,"BILLY CLINTON");
         WRITE(id_fnt_presentacio,320,380,4,"His beloved HILARY");
         PUT(id_fpg2,2,120,120);   // pone el grafico de La peazo Hilari
       END
       CASE 480:
         delete_text(all_text);
         WRITE(id_fnt_presentacio,320,65,4,"BILLY CLINTON");
         WRITE(id_fnt_presentacio,320,380,4,"Aznar, his brother in arms");
         PUT(id_fpg2,3,520,120);  // el bigotes
       END
       CASE 560:
         delete_text(all_text);
         WRITE(id_fnt_presentacio,320,64,4,"BILLY CLINTON");
         WRITE(id_fnt_presentacio,320,380,4,"If they fail, the light of the sun god");
         WRITE(id_fnt_presentacio,320,400,4,"will never shine again over USA");
       END
       CASE 680:
         diari();   // llama el process diari
       END
     END
     IF(key(_esc) OR key(_enter) OR key(_space)) DIARI();  END  // salta la presentacion
     FRAME;
   END
  END
//-------------------------------------------------------
PROCESS GAMEOVER()   // si llegas a este process es que no eres muy bueno
  BEGIN
    z=-600;          // por encima de todo se ve este process
    estado_programa=0;   // el estado de programa es nulo
    SIGNAL(TYPE joc,s_kill_tree); // mata el process joc
    DELETE_TEXT(ALL_TEXT);        // borra el texto
    x=320;  //las coordenadas
    y=200;
    LOOP
      graph=35;
      IF(KEY(_ENTER))
        estado_programa=1;
        FADE_OFF();    // cierra la pantalla
        CLEAR_SCREEN();        // borra toda la pantalla
        SIGNAL(TYPE helicoptero,s_kill_tree);  // mata los process...
        SIGNAL(TYPE mig29,s_kill_tree);
        SIGNAL(TYPE misil,s_kill_tree);
        SIGNAL(TYPE BBird,s_kill_tree);
        SIGNAL(TYPE su22,s_kill_tree);
        SIGNAL(TYPE enemigo_final,s_kill_tree);
        SIGNAL(TYPE regal,s_kill);
        stop_sound(all_sound);    // para todo el sonido
        STOP_SCROLL(0);        // para el scroll
        unload_fpg(id_fpg_marciano);
      END
      FRAME;
    END
  END
//------------------------------------------------------------
PROCESS ENEMIGO_FINAL(size);   // enemigo final

PRIVATE
BEGIN
  x=320;    // coordenadas
  y=-300;
  graph=43;
  local_boss_x=x;    // almacena las coordenadas del BOSS
  local_boss_y=y;
  CA¥ON_BOSS_esquerra_aball(); // inicia los processos que formaran la nave
  CA¥ON_BOSS_dreta_aball();
  CA¥ON_BOSS_esquerra_amunt();
  CA¥ON_BOSS_dreta_amunt();
  SHOTING_LASER_dret(80);
  SHOTING_LASER_esquerra(80);
  id_boss_cupula=BOSS_CUPULA(80);
  LOOP
    local_boss_x=x;       // almacena las coordenadas del BOSS
    local_boss_y=y;
    IF(collision(TYPE nave) and idnave.flags==0)  // si colisiona con la nave
      MUERTE(local_x,local_y);          // esta ultima muere
    END
    IF(y<0);      //mueve la nave
      y+=3;
    END
    FRAME;
  END
END
//----------------------------------------------------
PROCESS CA¥ON_BOSS_dreta_aball();
  PRIVATE
  id_fire;    // identificador de la cadencia de fuego
  BEGIN
  z=-593;
  vida=300;          // la vida
  id_fire=rand(15,30);      // almacena el valor de la cadencia de fuego
    LOOP
      x=local_boss_x+99;    // coloca la torreta en la posicion que le toca
      y=local_boss_y+193;
      graph=20;
      IF(vida<=0)          // si la vida es igual o menos que 0 ­­CABUM!!
        explosio(x,y,1,100);
        BREAK;
      END
      angle=fget_angle(x,y,local_x,local_y)+90000; // la torreta sigue la posicion de la nave principal
      IF(contador==id_fire)
        misil(99,x+10,y,get_angle(idnave),100); // dispara
        misil(99,x-10,y,get_angle(idnave),100);
        contador=0;
        id_fire=rand(15,30);   // da otro valor a la cadencia de fuego
      ELSE
        contador+=1;
      END
      FRAME;
    END
  END
//----------------------------------------------
PROCESS CA¥ON_BOSS_esquerra_aball();   // lo mismo que el process anterior
  PRIVATE
  id_fire;
  BEGIN
  z=-593;
  vida=300;
  id_fire=rand(15,30);
    LOOP
      x=local_boss_x-99;
      y=local_boss_y+193;
      graph=20;
      IF(vida<=0)
        explosio(x,y,1,100);
        BREAK;
      END
      angle=fget_angle(x,y,local_x,local_y)+90000;
      IF(contador==id_fire)
        misil(99,x-10,y,get_angle(idnave),100);
        misil(99,x+10,y,get_angle(idnave),100);
        contador=0;
        id_fire=rand(15,30);
      ELSE
        contador+=1;
      END
      FRAME;
    END
  END
//----------------------------------------------------
PROCESS CA¥ON_BOSS_dreta_amunt();      // lo mismo que el process anterior
  PRIVATE
  id_fire;
  BEGIN
  z=-593;
  vida=300;
  id_fire=rand(15,30);
    LOOP
      x=local_boss_x+110;
      y=local_boss_y+82;
      graph=20;
      IF(vida<=0)
        explosio(x,y,1,100);
        BREAK;
      END
      angle=fget_angle(x,y,local_x,local_y)+90000;
      IF(contador==id_fire)
        misil(99,x+10,y,get_angle(idnave),100);
        misil(99,x-10,y,get_angle(idnave),100);
        contador=0;
        id_fire=rand(15,30);
      ELSE
        contador+=1;
      END
      FRAME;
    END
  END
//----------------------------------------------
PROCESS CA¥ON_BOSS_esquerra_amunt();   // lo mismo que el process anterior
  PRIVATE
  id_fire;
  BEGIN
  z=-593;
  vida=300;
  id_fire=rand(15,30);
    LOOP
      x=local_boss_x-107;
      y=local_boss_y+81;
      graph=20;
      IF(vida<=0)
        explosio(x,y,1,100);
        BREAK;
      END
      angle=fget_angle(x,y,local_x,local_y)+90000;
      IF(contador==id_fire)
        misil(99,x-10,y,get_angle(idnave),100);
        misil(99,x+10,y,get_angle(idnave),100);
        contador=0;
        id_fire=rand(10,30);
      ELSE
        contador+=1;
      END
      FRAME;
    END
  END
//------------------------------------------------
PROCESS SHOTING_LASER_DRET(size);  // EL laser del BOSS
 BEGIN
  vida=1000;   // esa vida, que es mucha
  contador=0;
  animacio=46;
  LOOP
    x=local_boss_x+70;     // coloca el Laser en su posicion
    y=local_boss_y+112;
    graph=animacio;
    IF(vida<=0)
      explosio(x,y,98,100);
      BREAK;
    END
    IF(contador==40) animacio=47; END //lo anima
    IF(contador==50)
      misil(98,x-12,y+50,0,60);   // y dispara
      contador=0;
      animacio=46;
    ELSE
      contador+=1;
    END
    FRAME;
  END
 END
//-------------------------------------------------------
PROCESS SHOTING_LASER_ESQUERRA(size);   // Idem Anterior
 BEGIN
  vida=1000;
  contador=0;
  animacio=44;
  LOOP
    x=local_boss_x-69;
    y=local_boss_y+112;
    graph=animacio;
    IF(vida<=0)
      explosio(x,y,98,100);
      BREAK;
    END
    IF(vida<=0) BREAK; END
    IF(contador==40) animacio=45; END
    IF(contador==50)
      misil(98,x+12,y+50,0,60);
      contador=0;
      animacio=44;
    ELSE
      contador+=1;
    END
    FRAME;
  END
 END
//----------------------------------------------------
PROCESS BOSS_CUPULA(size);   // La gran cupula
 PRIVATE
 snd_warp;
 inc_ani=1;
 aleatori;
 BEGIN
   snd_warp=load_pcm("USER\BILLYGO\warp2.pcm",0); // carga el sonido del beam
   vida=3000;  // casi Immortal
   aleatori=rand(50,100);   // almacena el valor de la cadencia de fuego
   z=-595;
   animacio=48;  // inicia el valor de la animacion
   LOOP
     x=local_boss_x;   // lo coloca es su posicion
     y=local_boss_y+130;
     graph=animacio;   // pone el grafico
     IF(vida<=0)
       explosio(x,y,99,100);
       break;
     END
     IF(contador==aleatori)   // Este bucle controla la cadencia de fuego
       IF(contador2==3)
         IF(animacio==62 and inc_ani==1)
           id_snd_warp=SOUND(snd_warp,190,256);  // ejecuta el sonido del beam
         END
         animacio+=inc_ani;  // anima los graficos
         contador2=0;
         IF(animacio==69)
           animacio=64;
           inc_ani=-1;
           misil(97,x,y+60,0,80);   // dispara
         END
       ELSE
         contador2+=1;
       END
     ELSE
       contador+=1;
     END
     IF(animacio<48)     // controla la animacion
       animacio=48;
       inc_ani=1;
       contador=0;
       aleatori=rand(50,100);   // reinica los valores de cadencia de fuego
     END
     FRAME;
   END
 END
//-----------------------------------------------
PROCESS LEWINSKY(x,y);
 PRIVATE
 id_fnt_billpc;
 BEGIN
   stop_sound(id_snd_warp); // para el sonido del beam
   DELETE_TEXT(all_text);        // borra el texto
   SIGNAL(TYPE vidas,s_kill);              // mata el grafico de las vidas
   id_fnt_billpc=load_fnt("USER\BILLYGO\billpc.fnt");    // carga la fuente
   size=125;
   graph=33;
   SIGNAL(TYPE joc,s_freeze);  // congela el processo joc
   punto_mira(x,y);
   WRITE(id_fnt_billpc,200,100,4,"TARGET LOCATED ==>");
   LOOP
     frame;
     contador+=1;
     IF(contador==5)
       FOTO_LEWY();
       WRITE(id_fnt_billpc,350,425,4,"Monica Lewinsky: One of most important arms of Sadam");
       FROM size=125 to 0;
         angle-=10000;
         FRAME;
       END
       Credits();       // llama al processo creditos
       BREAK;
     END
   END
 END
//------------------------------------------
PROCESS PUNTO_MIRA(x,y);
 BEGIN
   animacio=27;
   LOOP
     graph=animacio;       // controla la animacion del punto de mira
     IF(contador==5);
       animacio=28;
       contador=0;
       IF(contador2==2);
         animacio=27;
         contador2=0;
       ELSE
         contador2+=1;
       END
     ELSE
       contador+=1;
     END
     FRAME;
   END
 END
//------------------------------------------------
PROCESS DIARI();
PRIVATE
BEGIN
  fade_off();            // apaga la pantalla
  delete_text(all_text); // borra todo el texto
  unload_fpg(id_fpg2);   // descarga el fpg de la presentacion
  stop_sound(risas);     // para el sonido risas
  unload_pcm(snd_haha);  // descarga el pcm
  id_fpg_marciano=load_fpg("USER\BILLYGO\billywar.fpg");     //carga el fpg
  id_fnt_marciano=load_fnt("USER\BILLYGO\billywar.fnt");     //carga las fuentes
  load_pal("USER\BILLYGO\div.pal");                    // carga la paleta
  SIGNAL(TYPE PRESENTACIO,s_kill);   // mata el processo presentacion
  clear_screen();                    // borra la pantalla
  put_screen(id_fpg_marciano,30);    // pone la pantalla de inico
  fade_on();                      // enciende la pantalla
  contador+=1;
  WRITE(id_fnt_marciano,420,350,4,"1)  JUGAR");    // escribe las opciones
  WRITE(id_fnt_marciano,435,373,4,"2) CREDITOS");
  WRITE(id_fnt_marciano,413,396,4,"3) SALIR");
  LOOP  // este bucle controla el estado del programa segun el numero que se pulse
   IF(key(_1))
     estado_programa=2;
     Fade_off();      // apaga la pantalla
     clear_screen();  // borra  "    "
     unload_fnt(id_fnt_marciano);  // descarga  la fuente
   END
   IF(key(_2))      // lo mismo que antes
     estado_programa=3;
     Fade_off();
     clear_screen();
     unload_fnt(id_fnt_marciano);
   END
   IF(key(_3))     // lo mismo que antes
     Fade_off();
     clear_screen();
     unload_fnt(id_fnt_marciano);
     EXIT("GRACIAS POR JUGAR",0);
   END
   FRAME;
  END
END
//--------------------------------------------------------
PROCESS foc_reactor();  // controla el fuego del avion
BEGIN
  angle=90000;
  flags=4;
  LOOP
    graph=activacio;
    x=local_x;
    y=local_y+40;
    FRAME;
  END
END
//------------------------------------------
PROCESS FOTO_LEWY();    // pone la foto de la lewy en pantalla
BEGIN
  x=65;
  y=400;
  graph=100;
  LOOP
    FRAME;
  END
END
//---------------------------------------
PROCESS CREDITS();           // los creditos
PRIVATE
 id_credits;
BEGIN
  estado_programa=0;
  FADE_OFF();
  CLEAR_SCREEN();
  SIGNAL(TYPE DIARI,s_kill);       // mata los processos necesarios
  SIGNAL(TYPE joc,s_kill);
  SIGNAL(TYPE punto_mira,s_kill);
  SIGNAL(TYPE nave,s_kill_tree);
  SIGNAL(TYPE foto_lewy,s_kill);
  SIGNAL(TYPE helicoptero,s_kill_tree);
  SIGNAL(TYPE mig29,s_kill_tree);
  SIGNAL(TYPE enemigo_final,s_kill_tree);
  SIGNAL(TYPE regal,s_kill);
  DELETE_TEXT(ALL_TEXT);      // borra todo el texto
  STOP_SCROLL(0);           // para el scroll
  id_credits=load_fpg("USER\BILLYGO\creditos.fpg");    // carga el fichero de los creditos
  PUT_SCREEN(id_credits,1);  // los pone en pantalla
  FADE_ON();        // enciende la pantalla
  LOOP     // este bucle para los creditos si se pulsa enter, espacio o esc
    IF(key(_enter) OR key(_space) OR key(_esc));
      FADE_OFF();   // apaga la pantalla
      clear_screen();  // borra la pantalla
      stop_sound(all_sound);    // para todos los sonidos
      unload_fpg(id_fpg_marciano);     // descarga el fichero
      estado_programa=1; // inicia el estado del programa
      BREAK;
    END
    FRAME;
  END
END
//--------------------------------------------------------
PROCESS BBird(x,tipo_power)     // este peazo de Black Bird
PRIVATE
 angulo_BBird;
BEGIN
  VIDA=10;
  y=-20;
  angulo_BBird=get_angle(idnave);     // almacena el angulo entre el bird y la nave
  angle=fget_angle(x,y,local_x,local_y)+270000;  // el bird se encara a la nave
  sound(snd_BBird,140,200);  // suena
  graph=111;
  REPEAT       // este bucle controla la vida y el mavimiento del bird
      IF(VIDA=<0)
        explosio(x,y,1,100);
        IF(tipo_power>0) regal(x,y,tipo_power); END
        puntuacio+=200;
        punts();
        BREAK;
      END
      x+=get_distx(angulo_BBird,40); // mueve el black bird hacia la nave
      y+=get_disty(angulo_BBird,40); // a una velocidad increible
      IF(collision(TYPE nave) and idnave.flags==0)
        MUERTE(local_x,local_y);
      END
      FRAME;
  UNTIL (y>490 OR x<-10 OR x>650);
END
//----------------------------------------------
PROCESS helice();   //hace girar la helice
BEGIN
 graph=110;
 LOOP
  angle-=15000;
  FRAME;
 END
END
//--------------------------------------------------------
PROCESS SU22(x,tipo_power)  // el dificultoso su22
PRIVATE
  id_ca¤on_su22;
BEGIN
  graph=112;
  VIDA=100;
  inc_x=rand(3,-3);
  size=24;
  y=500;
  size_su22=size;
  local_su22_x=x;
  local_su22_y=y;
  id_ca¤on_su22=ca¤on_su22(); // inicia el processo del ca¤on
  LOOP       // controla los movimientos de la nave
      IF(y>100 and size<100) y-=5; END    //se encarga de que la nave de la
      IF(y<340 and size<100) size+=2; END //sensacion de que sube desde lejos
      IF(x<35) inc_x=rand(0,3); END   // limita em movimiento de la nave
      IF(x>605) inc_x=rand(0,-3); END // por la pantalla
      IF(size==100);
        IF(y>280) inc_y=rand(0,-3); END // tambien lo limita
        IF(y<60) inc_y=rand(0,3); END
      END
      contador2+=1;
      IF(contador2==30)    // cada 30 frames canvia de direccion
        inc_x=rand(3,-3);
        inc_y=rand(3,-3);
        contador2=0;
      END
      x=x+inc_x;
      y=y+inc_y;
      IF(VIDA=<0)
        SIGNAL(id_ca¤on_su22,s_kill); // mata el processo del a¤on
        explosio(x,y,1,100);
        IF(tipo_power>0) regal(x,y,tipo_power); END
        puntuacio+=600;
        punts();
        BREAK;
      END
      IF(collision(TYPE nave) and idnave.flags==0 and size==100)
        MUERTE(local_x,local_y);
      END
      size_su22=size;
      local_su22_x=x;
      local_su22_y=y;
      FRAME;
  END
END
//-------------------------------------------
PROCESS CA¥ON_SU22();
PRIVATE
  id_fire2;
BEGIN
  z=-593;
  id_fire2=rand(5,20);// guarda en la variable la cadencia de fuego
  graph=113;
    LOOP
      size=size_su22;
      x=local_su22_x; // posiciona la torreta donde le corresponde
      y=local_su22_y;
      angle=fget_angle(x,y,local_x,local_y)+90000; // gira la torreta para encararla
      IF(size==100);
        IF(contador==id_fire2)
          misil(99,x+6,y,get_angle(idnave),75); // dispara
          misil(99,x-6,y,get_angle(idnave),75);
          contador=0;
          id_fire2=rand(5,20); // reinicia la cadencia de fuego
        ELSE
          contador+=1;
        END
      END
      FRAME;
    END
END
