COMPILER_OPTIONS _extended_conditions;
// Pingu y No (Demo Final Marzo 98)
//
// Idea y dise¤o
//      Alejandro Barroso Mu¤oz
//      Miguel Adolfo Barroso Mu¤oz
// Programaci¢n y realizaci¢n
//      Miguel Adolfo Barroso Mu¤oz
// Sonido
//      Leandro
//      Karl Sommer
//      Miguel Adolfo Barroso Mu¤oz
// Pruebas y test
//      Alejandro Barroso Mu¤oz
//
// http://www.columbus-digital.com/barroso/emda.htm
// Miguel Adolfo Barroso Mu¤oz (barroso@columbus-digital.com)
// (R) Todos los derechos reservados
//
// * Este c¢digo fuente, y los ficheros necesarios para su uso, son
// freeware, no pudi‚ndose cobrar por su uso, copia ni distribuci¢n.
// * El autor no se hace responsable de las p‚rdidas o da¤os que se puedan
// producir con el uso de este programa.
// * Todos y cada uno de los ficheros deben ser distribuidos integra e
// inseparablemente de forma gratuita.
// * Este c¢digo fuente, y dem s ficheros necesarios para su uso, se
// distribuyen con fines educativos. Son susceptibles a modificaciones
// y no es necesario el pago por su empleo.
// * Toda modificaci¢n del c¢digo, y dem s ficheros necesarios, est n
// permitidos siempre que:
//        - Forme parte de investigaci¢n y/o aprendizaje
//        - El c¢digo sea reutilizado para la elaboraci¢n de otros
//        programas, tanto en cuanto, no sea plagio
//
// 31-3-1998 Miguel Adolfo Barroso Mu¤oz
//
// Ficherolog¡a:
//     PINGU.000    Sonido   Presentaci¢n 1
//     PINGU.001    Gr ficos Presentaci¢n 1
//     PINGU.002    Gr ficos Presentaci¢n 2
//     PINGU.003    Gr ficos Personajes y utiliter¡a
//     PINGU.004    Gr ficos Bloques
//     PINGU.005    Gr ficos Enemigos
//     PINGU.008    Sonido Presentaci¢n 2
//     PINGU.009    Sonido M£sica en el juego
//     PINGU.010    Sonido no-rompe
//     PINGU.011    Sonido rompe
//     PINGU.012    Sonido choca
//     PINGU.013    Sonido desplaza 1
//     PINGU.014    Sonido desplaza 2
//     PINGU.015    Sonido desplaza 3
//     PINGU.016    Sonido desplaza 4
//     PINGU.018    Fuente letra 1
//     PINGU.019    Fuente letra 2

PROGRAM Pingu;

CONST
  retardo_fin_nivel = 300;          // 5 segundos de retardo al completar un nivel
  delay             = 100;          // tiempo de espera antes de siguiente vida
  delay_agoniza     = 100;          // tiempo que dura ver como te han matado
  max_obj           = 7;            // n§ m ximo de objetos arrastrables
  null              = 0;

  // *** Puntuaciones de cada objeto
  puntos_jugador       = 1000;      // NO PUEDE ser igual a otra puntuaci¢n
  puntos_bloque_blando =   10;      // Bloque blando roto
  puntos_tonto         =  100;      // Enemigo eliminado

  //*** Clases de Objetos (1-30)
  //    (de momento no tiene mucho sentido, s¢lo vale bloque)
  personaje     =   1;
  bloque        =   2;
  enemigo       =   3;
  //***Tipos de Objetos (seg£n ID del gr fico base)
  p1            =   6;                      // Jugador 1
  fuego_p1      =  18;                      // Fuego del jugador 1
  p2            =  22;                      // Jugador 2
  fuego_p2      =  34;                      // Fuego jugador 2
  solido        =   1;                      // ni se mueve ni se rompe (bordes)
  blando        =   5;                      // bloque normal
  tonto         =   1;

  // *** control y direcci¢n para los objetos m¢viles
  horizontal    = TRUE;
  vertical      = FALSE;
  incrementa    = TRUE;
  decrementa    = FALSE;

  //*** Desplazamiento desde el gr fico base para conseguir movimiento
  m_izquierda   = 0;
  m_derecha     = 3;
  m_abajo       = 6;
  m_arriba      = 9;

  //*** ajuste de la rejilla de juego a la pantalla
  ajuste_x = 33;
  ajuste_y = 66;
  //*** Dimensiones de la rejilla de juego
  max_x = 22;
  max_y = 15;

GLOBAL
  //*** Datos necesarios para interpretar el nivel en juego
  STRUCT Nivel
    STRUCT Fila[max_y]
      STRUCT Columna[max_x]
        objeto;                             // Clase de Objeto
        tipo;                               // Tipo de objeto
      END;
    END;
  END =
            // [0]
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            // [1]
            bloque, solido,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            bloque, solido,
            // [2]
            bloque, solido,
            null,   null,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [3]
            bloque, solido,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [4]
            bloque, solido,
            null,   null,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [5]
            bloque, solido,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [6]
            bloque, solido,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            bloque, solido,
            // [7]
            bloque, solido,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            bloque, solido,
            // [8]
            bloque, solido,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            bloque, solido,
            // [9]
            bloque, solido,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            bloque, blando,
            bloque, solido,
            // [10]
            bloque, solido,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [11]
            bloque, solido,
            null,   null,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [12]
            bloque, solido,
            null,   null,
            bloque, blando,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [13]
            bloque, solido,
            null,   null,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            bloque, blando,
            null,   null,
            bloque, solido,
            // [14]
            bloque, solido,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            null,   null,
            bloque, solido,
            // [15]
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido,
            bloque, solido;

  //*** Ficheros
  f_personajes;                                 // Galer¡a de personajes
  f_bloques;                                    // Galer¡a de bloques
  f_enemigos;                                   // Galer¡a de enemigos

  //*** Sonidos
    sonido_roce;                                // n§ sonido roce a emitir(0-3)
    s_musica;                                   // M£sica en juego
    s_no_rompe;                                 // El bloque no se rompe
    s_rompe;                                    // El bloque se rompe
    s_choca;                                    // El bloque choca con otro
    s_desplaza1;                                // El bloque se mueve 1
    s_desplaza2;                                // El bloque se mueve 2
    s_desplaza3;                                // El bloque se mueve 3
    s_desplaza4;                                // El bloque se mueve 4
    s_fuego;                                    // Disparo

  marcador1;                                    // marcador puntos jugador 1
  marcador2;                                    // marcador puntos jugador 2
  t_marcador1;                                  // id marcador jugador 1
  t_marcador2;                                  // id marcador jugador 2
  f_1;                                          // Fuente de letra 1

  p1_x; p1_y;                                   // Coordenadas iniciales jugador 1
  p2_x; p2_y;                                   // Coordenadas iniciales jugador 2

  p1_n_vidas;                                   // n§ vidas jugador 1
  p1_vidas[2];                                  // id vidas jugador 1
  p2_n_vidas;                                   // n§ vidas jugador 2
  p2_vidas[2];                                  // id vidas jugador 2
  n_p;                                          // n§ jugadores
  n_enemigos;                                   // n§ de enemigos

LOCAL
  n_jugador;                                    // n1 de jugador
  puntos;                                       // puntos del objeto
  jugador;

PRIVATE
  i,j;
  id_s_presenta;                                // id sonido presentaciones
  s_presenta;                                   // Sonido presentaciones
  f_presenta;                                   // Fichero presentaci¢n 1
  f_presenta2;                                  // Fichero presentaci¢n 1
  f_2;                                          // Fuente de letra 2
  o1, o2, o3, o4, o5, o6, o7, o8;               // id objetos en las presentaciones
  r_presenta = 100;                             // retardo presentaci¢n
  fin_presenta;                                 // ¨acabar la presentaci¢n?


BEGIN
  set_fps(10, 1);

  // *********************************
  // ** Presentaci¢n (introducci¢n) **
  // *********************************
  set_mode(m640x480);

  // Carga ficheros necesarios para la presentaci¢n (introducci¢n)
  f_personajes  = load_fpg("user/pingu/pingu003.FPG");
  f_bloques     = load_fpg("user/pingu/pingu004.FPG");
  f_enemigos    = load_fpg("user/pingu/pingu005.FPG");
  f_1           = load_fnt("user/pingu/pingu018.FNT");
  f_2           = load_fnt("user/pingu/pingu019.FNT");
  s_presenta    = load_pcm("user/pingu/pingu000.PCM",0);
  f_presenta    = load_fpg("user/pingu/pingu001.FPG");
  f_presenta2   = load_fpg("user/pingu/pingu002.FPG");

  // Inicia pantalla, sonidos y variables
  load_pal("user/pingu/pingu001.FPG");
  put_screen(f_presenta,1);                     // Pone la pantalla de fondo
  id_s_presenta = sound(s_presenta,256,256);
  fin_presenta=FALSE;
  timer=0;

  REPEAT                                        // Bucle de la introducci¢n
    IF (scan_code<>0) break; END;               // Salimos del bucle al pulsar una tecla

    // Primera pantalla
    IF ((timer>450) AND (timer<470))
      fade(0, 0, 0, 20);
      put_screen(f_presenta,2);
      fade(100, 100, 100, 20);
    END;

    // Segunda pantalla
    IF ((timer>650) AND (timer<670))
      fade_off();               FRAME(r_presenta);
      put_screen(f_presenta,3); FRAME(r_presenta);
      fade_on();                FRAME(r_presenta);
    END;

    // Tercera pantalla (animaci¢n personajes)
    IF ((timer>1000))
      fade_off();
      clear_screen();
      load_pal("user/pingu/pingu003.FPG");
      fade_on();
      o1 = objeto3(-33, 240, p1, f_personajes);
      o8 = objeto3(330, 240, blando, f_bloques);
      o4 = objeto3(363, 240, blando, f_bloques);
      o6 = objeto3(330,  33, blando, f_bloques);
      j=1;
      FOR (i=-33; i<298; i+=15)
        j = (++j) mod 2;
        o1.graph = p1 + m_derecha + j +1;
        o1.x=i;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      o1.graph = p1 + m_derecha;
      FRAME(600); IF (scan_code<>0) break; END;
      o3 = objeto3(330, 240, fuego_p1, f_personajes);
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o3.graph++; o8.graph++;
      o2 = objeto3(627, 240, p2, f_personajes);
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o3.graph++; o8.graph++;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o3.graph++; o8.graph++;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      signal(o8, s_kill);
      signal(o3, s_kill);
      FOR (; i<332; i+=15)
        j = (++j) mod 2;
        o1.graph = p1 + m_derecha + j + 1;
        o1.x=i;
        o2.graph = p2 + m_izquierda + j + 1;
        o2.x-=15;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      o1.graph = p1 + m_derecha;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      FRAME(600); IF (scan_code<>0) break; END;
      o3 = objeto3(363, 240, fuego_p1, f_personajes);
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o3.graph++; o4.x+=11;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      o7 = objeto3(627,  66, tonto,  f_enemigos);
      o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o3.graph++; o4.x+=11;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o3.graph++; o4.x+=11;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      signal(o3, s_kill);
      o4.x+=11;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o4.x+=11;
      j = (++j) mod 2; o2.graph = p2 + m_izquierda + j + 1; o2.x-=11;
      o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o4.x+=11;
      o2.graph = p2 + m_izquierda;
      o1.graph = p2 + m_abajo;
      o5 = objeto3(o2.x-33, 240, fuego_p2, f_personajes);
      j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      FOR (i=0; i<2; i++)
        j = (++j) mod 2;
        o1.graph = p1 + m_abajo + j + 1;
        o1.y+=11;
        o5.graph++;
        o4.x-=11;
        j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      signal(o5, s_kill);
      o4.x-=11;
      o1.y+=11;
      o2.graph = p2 + m_izquierda +1;
      j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o1.graph = p1 + m_arriba;
      o4.x-=11;
      j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o4.x-=11;
      j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o4.x-=11;
      j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      o4.x-=11;
      o5 = objeto3(o1.x, o1.y-33, fuego_p1, f_personajes);
      j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      FOR (i=0; i<2; i++)
        o5.graph++;
        o4.x-=11;
        j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      o1.graph = p1 + m_arriba + 1;
      signal(o5, s_kill);
      FOR (i=0; i<13; i++)
        o4.y-=11;
        j = (++j) mod 2; o7.graph = tonto + m_izquierda + j +1; o7.x-=11;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      FOR (i=0; i<3; i++)
        o4.y-=11;
        o7.y-=11;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      signal(o7, s_kill);
      FOR (i=0; i<33; i+=6)
        y-=4;
        o7 = Write(f_1, 330, 33-i, 4, "x 1");
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
        delete_text(o7);
      END;
      IF (fin_presenta) break; END;
      j = (++j) mod 2;
      o2.graph = p2 + m_abajo + 1 + j;
      o1.graph = p1 + m_izquierda + 1 + j;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      j = (++j) mod 2;
      o2.y-=11;
      o2.graph = p2 + m_abajo + 1 + j;
      o1.graph = p1 + m_abajo + 1 + j;
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      FOR (i=0; i<10; i++);
        j = (++j) mod 2;
        o2.y+=11;
        o2.graph = p2 + m_abajo + 1 + j;
        o1.y-=11;
        o1.graph = p1 + m_abajo + 1 + j;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
        j = (++j) mod 2;
        o2.y-=11;
        o2.graph = p2 + m_abajo + 1 + j;
        o1.y+=11;
        o1.graph = p1 + m_abajo + 1 + j;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      signal(o1, s_kill);
      signal(o2, s_kill);
      signal(o3, s_kill);
      signal(o4, s_kill);
      signal(o6, s_kill);
      fade_off();
      FOR (i=0; i<3; i++);
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      fade_on();
      o5 = objeto3(320, 240, 2, f_presenta2);
      put_screen(f_presenta2,1);
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      FOR (i=0; i<6; i++);
        o5.graph++;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      signal(o5, s_kill);
      FOR (i=0; i<6; i++);
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      FOR (i=10; i<1000; i+=3);
        write(f_2, 320, 500-i, 4, "Idea y dise¤o");
        write(f_1, 320, 520-i, 4, "Alejandro Barroso Mu¤oz");
        write(f_1, 320, 540-i, 4, "Miguel Adolfo Barroso Mu¤oz");
        write(f_2, 320, 580-i, 4, "Programaci¢n y realizaci¢n");
        write(f_1, 320, 600-i, 4, "Miguel Adolfo Barroso Mu¤oz");
        write(f_2, 320, 640-i, 4, "Sonido");
        write(f_1, 320, 660-i, 4, "Leandro");
        write(f_1, 320, 680-i, 4, "Karl Sommer");
        write(f_1, 320, 700-i, 4, "Miguel Adolfo Barroso Mu¤oz");
        write(f_2, 320, 740-i, 4, "Pruebas y test");
        write(f_1, 320, 760-i, 4, "Alejandro Barroso Mu¤oz");
        write(f_2, 320, 800-i, 4, "Agradecimientos");
        write(f_1, 320, 820-i, 4, "A mi hermano, que cuando le regal‚ DIV nunca");
        write(f_1, 320, 840-i, 4, "pens‚ que lo usase m s que ‚l");
        write(f_1, 320, 880-i, 4, "A mis amig@s, que me apoyan y conf¡an en m¡");
        write(f_1, 320, 900-i, 4, "(y sufren probando mis demos ;-})");
        write(f_1, 320, 940-i, 4, "A todos los DIVmaniacos que comparten,");
        write(f_1, 320, 960-i, 4, "porque as¡ todos tenemos m s");
        write(f_1, 320,1000-i, 4, "A m¡, porque me estimo mucho :-D");
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
        delete_text(all_text);
      END;
      IF (fin_presenta) break; END;
      FOR (i=0; i<5; i++);
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      o5 = objeto3(320, 240, 6, f_presenta2);
      put_screen(f_presenta2,1);
      delete_text(all_text);
      FOR (i=6; i>2; i--);
        o5.graph--;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      fade_off();
      FOR (i=0; i<5; i++);
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      clear_screen();
      signal(o5, s_kill);
      load_pal("user/pingu/pingu001.FPG");
      put_screen(f_presenta,1);
      fade_on();
      o5 = objeto3(320, 240, 2, f_presenta2);
      FRAME(r_presenta); IF (scan_code<>0) break; END;
      FOR (i=0; i<6; i++);
        o5.graph++;
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      signal(o5, s_kill);
      FOR (i=0; i<20; i++);
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
      fade_off();
      FOR (i=0; i<5; i++);
        FRAME(r_presenta); IF (scan_code<>0) fin_presenta=TRUE; break; END;
      END;
      IF (fin_presenta) break; END;
    ELSE
      FRAME;
    END;
  UNTIL (scan_code<>0 OR timer[0]>5500)
  stop_sound(id_s_presenta);
  delete_text(all_text);
  let_me_alone();

  // Descargamos todos los ficheros (parece una tonter¡a, pero no lo es)
  unload_fpg(f_personajes);
  unload_fpg(f_bloques);
  unload_fpg(f_enemigos);
  unload_fnt(f_1);
  unload_fnt(f_2);
  unload_pcm(s_presenta);
  unload_fpg(f_presenta);
  unload_fpg(f_presenta2);

// ******************************************
// ** Bucle principal del juego (infinito) **
// ******************************************

LOOP
  // **********************************************
  // ** Presentaci¢n 2 (Selecci¢n tipo de juego) **
  // **********************************************
  set_mode(m640x480);
  load_pal("/USEr/pingu/pingu002.FPG");

  // Cargamos ficheros de la presentaci¢n 2
  f_personajes  = load_fpg("user/pingu/pingu003.FPG");
  f_presenta2   = load_fpg("user/pingu/pingu002.FPG");
  s_presenta    = load_pcm("user/pingu/pingu008.PCM",1);

  // Crea objetos temporales y pone a sonar la m£sica
  o1 = objeto3(319, 363, p1, f_personajes);
  o2 = objeto3(297, 407, p1, f_personajes);
  o3 = objeto3(341, 407, p2, f_personajes);
  id_s_presenta = sound(s_presenta, 150, 256);

  fade_on();
  put_screen(f_presenta2,1);
  i=0; j=0;

  // Bucle que permite elegir la modalidad de juego mientras bailan los personajes ;-)
  REPEAT
    j = (++j) mod 12;
    IF (i==0)
      o1.graph = p1 + j;
      o2.graph = p1 + m_abajo + 1;
      o3.graph = p2 + m_abajo + 1;
      n_p = 1;
    ELSE
      o1.graph = p1 + m_abajo + 1;
      o2.graph = p1 + j;
      o3.graph = p2 + j;
      n_p = 2;
    END;
    IF (key(_up) OR key(_down) OR key(_w) OR key(_s) )
      i = (++i) mod 2;
      REPEAT FRAME; UNTIL (scan_code==0);       // Espera a que se deje en paz al teclado
    END;
    FRAME(r_presenta);

    // Si se puls¢ ESC, pues nos vamos y se acab¢
    IF (key(_esc))
      exit("Pingu Y No Demo 1.0, por Miguel Adolfo Barroso Mu¤oz 1998", 0);
    END;
  UNTIL (key(_alt) OR key(_control))

  // Destruye objetos temporales y detiene la m£sica
  signal(o1, s_kill);
  signal(o2, s_kill);
  signal(o3, s_kill);
  stop_sound(id_s_presenta);

  // Descargamos ficheros de la presentaci¢n 2
  unload_fpg(f_personajes);
  unload_fpg(f_presenta2);
  unload_pcm(s_presenta);

  // Carga paleta y ficheros de gr ficos/sonidos principales del juego
  load_pal("USeR/pingu/pingu003.FPG");
  f_personajes  = load_fpg("user/pingu/pingu003.FPG");
  f_bloques     = load_fpg("user/pingu/pingu004.FPG");
  f_enemigos    = load_fpg("user/pingu/pingu005.FPG");
  s_musica      = load_pcm("user/pingu/pingu009.PCM",1);
  s_no_rompe    = load_pcm("user/pingu/pingu010.PCM",0);
  s_rompe       = load_pcm("user/pingu/pingu011.PCM",0);
  s_choca       = load_pcm("user/pingu/pingu012.PCM",0);
  s_desplaza1   = load_pcm("user/pingu/pingu013.PCM",0);
  s_desplaza2   = load_pcm("user/pingu/pingu014.PCM",0);
  s_desplaza3   = load_pcm("user/pingu/pingu015.PCM",0);
  s_desplaza4   = load_pcm("user/pingu/pingu016.PCM",0);
  s_fuego       = load_pcm("user/pingu/pingu017.PCM",0);
  f_1           = load_fnt("user/pingu/pingu018.FNT");

  // Inicializa Pantalla y Variables globales
  set_mode(m800x600);
  pantalla();
  n_enemigos= 4;
  marcador1 = 0;
  marcador2 = 0;

  // Inicializa iconos de n§ jugador, vidas y personajes
  p1_n_vidas= 3;
                objeto3(33, 17, 4, f_personajes);
  p1_vidas[0] = objeto3(65,  20, 3, f_personajes);
  p1_vidas[1] = objeto3(86,  20, 3, f_personajes);
  p1_vidas[2] = objeto3(107, 20, 3, f_personajes);
  p1_x=66;
  p1_y=99;
  o_jugador(puntos_jugador, p1, fuego_p1, p1_x, p1_y, f_personajes);

  IF (n_p==2)                                   // Hay 2§ jugador
    p2_n_vidas= 3;
                  objeto3(757, 17, 5, f_personajes);
    p2_vidas[0] = objeto3(725, 20, 3, f_personajes);
    p2_vidas[1] = objeto3(704, 20, 3, f_personajes);
    p2_vidas[2] = objeto3(683, 20, 3, f_personajes);
    p2_x=726;
    p2_y=528;
    o_jugador(puntos_jugador, p2, fuego_p2, p2_x, p2_y, f_personajes);
  ELSE
    p2_n_vidas= 0;                              // No hay -> no tienes vidas
  END;

  // Inicializa enemigos y sonido durante el juego
  o_enemigo(puntos_tonto, tonto, 132, 165, f_enemigos);
  o_enemigo(puntos_tonto, tonto, 660, 462, f_enemigos);
  o_enemigo(puntos_tonto, tonto, 132, 396, f_enemigos);
  o_enemigo(puntos_tonto, tonto, 660, 231, f_enemigos);
  id_s_presenta = sound(s_musica,  60, 255);

  // *******************************
  // ** Bucle principal del juego **
  // *******************************
  // Hasta que se maten todos los enemigos, se agoten todas las vidas o se cancele
  WHILE ((n_enemigos>0) AND ((p1_n_vidas>0) OR (p2_n_vidas>0)))
    // Imprime marcadores
    t_marcador1 = write_int(f_1, 130, 18, 3, &marcador1);
    IF (n_p==2)
      t_marcador2 = write_int(f_1, 660, 18, 5, &marcador2);
    END;

    FRAME;

    // Elimina marcadores
    delete_text(t_marcador1);
    IF (n_p==2)
      delete_text(t_marcador2);
    END;
    IF (key(_esc))                              // ¨Cancelar actual partida?
      BREAK;
    END
  END;

    IF ((n_enemigos<1) OR ((p1_n_vidas<1) AND (p2_n_vidas<1))) // No quedan enemigos o vidas
      // Imprime marcadores (que est n actualmente borrados)
      t_marcador1 = write_int(f_1, 130, 18, 3, &marcador1);
      IF (n_p==2)
        t_marcador2 = write_int(f_1, 660, 18, 5, &marcador2);
      END;
      // Un retardillo, que queda muy bien y hasta asusta a veces ;-)
      timer=0;
      LOOP
        FRAME;
        IF (timer>retardo_fin_nivel) break; END;
      END;
      fade_off();
      let_me_alone();
      stop_sound(id_s_presenta);
      delete_text(all_text);

      //descarga ficheros innecesarios
      unload_fpg(f_bloques);
      unload_fpg(f_enemigos);
      unload_pcm(s_musica);
      unload_pcm(s_no_rompe);
      unload_pcm(s_rompe);
      unload_pcm(s_choca);
      unload_pcm(s_desplaza1);
      unload_pcm(s_desplaza2);
      unload_pcm(s_desplaza3);
      unload_pcm(s_desplaza4);
      unload_pcm(s_fuego);

      FRAME;                                    // Si no se pone, algunas veces falla

      //Carga ficheros
      f_presenta2   = load_fpg("user/pingu/pingu002.FPG");
      s_presenta    = load_pcm("user/pingu/pingu008.PCM",1);

      set_mode(m640x480);
      put_screen(f_presenta2, 1);
      o1 = objeto3(297, 363, p1, f_personajes);
      IF (n_p==2) o2 = objeto3(297, 407, p2, f_personajes); END;

      t_marcador1 = write_int(f_1, 429, 363,    5, &marcador1);
      IF (n_p==2) t_marcador2 = write_int(f_1, 429, 407, 5, &marcador2); END;
      id_s_presenta = sound(s_presenta, 150, 256);

      REPEAT FRAME; UNTIL (scan_code==0); // Espera a que se suelte el teclado

      fade_on();
      // Hacemos una simp tica animaci¢n
      j=0;
      REPEAT
        j = (++j) mod 12;
        o1.graph = p1 + j;
        IF (n_p==2) o2.graph = p2 + j; END;
        FRAME(r_presenta);
      UNTIL (scan_code<>0)
      fade_off();
      signal(o1, s_kill);
      IF (n_p==2) signal(o2, s_kill); END;
      stop_sound(id_s_presenta);
      unload_fpg(f_personajes);
      unload_fpg(f_presenta2);
      unload_pcm(s_presenta);
    ELSE
      // De todas formas, detiene el sonido y descarga ficheros
      fade_off();
      stop_sound(id_s_presenta);
      unload_fpg(f_personajes);
      unload_fpg(f_bloques);
      unload_fpg(f_enemigos);
      unload_pcm(s_musica);
      unload_pcm(s_no_rompe);
      unload_pcm(s_rompe);
      unload_pcm(s_choca);
      unload_pcm(s_desplaza1);
      unload_pcm(s_desplaza2);
      unload_pcm(s_desplaza3);
      unload_pcm(s_desplaza4);
      unload_pcm(s_fuego);
    END;
  unload_fnt(f_1);
  delete_text(all_text);
  let_me_alone();
  FRAME;                                        // Si no se pone, algunas veces falla
END; // del LOOP infinito

END

// ***************************************************************************
// ** PROCEDIMIENTO objeto(coord_x, coord_y, grafico, fichero)              **
// **                                                                       **
// ** Procedimiento gen‚rico para objetos pasivos                           **
// ***************************************************************************
PROCESS objeto3(x,y, graph, file);
  BEGIN
    LOOP
        FRAME;
    END;
  END

// ***************************************************************************
// ** PROCEDIMIENTO pantalla()                                              **
// **                                                                       **
// ** Genera la pantalla del nivel                                          **
// ***************************************************************************
PROCESS pantalla();
  PRIVATE
    i,j;
  BEGIN
    FOR (i=0; i<max_x+1; i++)
      FOR (j=0; j<max_y+1; j++)
        SWITCH (Nivel.Fila[j].Columna[i].Objeto)
          CASE null:                            // Si es nulo, no hacemos nada
          END;
          CASE bloque:
            o_bloque(ajuste_x + i*33, ajuste_y + j*33 , Nivel.Fila[j].Columna[i].Tipo, f_bloques);
          END;
          DEFAULT:
            o_bloque(ajuste_x + i*33, ajuste_y + j*33 , 3, f_personajes);
         END;
        END;
      END;
    END;
  END;

// ***************************************************************************
// ** PROCEDIMIENTO marcador(numero_jugador, puntos)                        **
// **                                                                       **
// ** Actualiza el marcador del jugador                                     **
// ***************************************************************************
PROCESS marcador(jugador, puntos)
  BEGIN
    IF (jugador==p1)
      marcador1+=puntos;
    ELSE
      marcador2+=puntos;
    END;
  END;

// ***************************************************************************
// ** PROCEDIMIENTO fuego(puntos, numero, disparo, coor_x, coor_y, fichero) **
// **                                                                       **
// ** Proceso gen‚rico de jugadores                                         **
// ***************************************************************************
PROCESS o_jugador(puntos, jugador, fire, x, y, file);
  PRIVATE
   m         = 1;
   dir       = m_abajo;
   direccion = vertical;
   sentido   = incrementa;
   mueve     = FALSE;                           // Valdr  TRUE si estamos en movimiento
   idbloque;
  BEGIN
    graph = jugador + dir + m;
    // si al aparecer topamos con un bloque, lo destruimos y no sumamos puntos
    // si fuera un enemigo, mala suerte, nos matar  :-(
    IF (idbloque=COLLISION(TYPE o_bloque))
      signal(idbloque, s_kill);
    END;
    LOOP
      IF (!mueve)                               // Si no nos movemos, ¨debemos hacerlo?
        // Debemos asegurarnos que el jugador no est  pulsando FUEGO para movernos
        IF ((Key(_left) AND (jugador==p1)) OR (Key(_a) AND (jugador==p2)))
          direccion = horizontal;
          sentido   = decrementa;
          mueve = TRUE;
        ELSE
          IF ((Key(_right) AND (jugador==p1)) OR (Key(_d) AND (jugador==p2)))
            direccion = horizontal;
            sentido   = incrementa;
            mueve = TRUE;
          ELSE
            IF ((Key(_up) AND (jugador==p1)) OR (Key(_w) AND (jugador==p2)))
              direccion = vertical;
              sentido   = decrementa;
              mueve = TRUE;
            ELSE
              IF ((Key(_down) AND (jugador==p1)) OR (Key(_s) AND (jugador==p2)))
                direccion = vertical;
                sentido   = incrementa;
                mueve = TRUE;
              END;
            END;
          END;
        END;
      END;

      // Si estamos en movimiento (porque MUEVE o porque no estamos alineados)
      //   calculamos el desplazamiento del sprite a usar
      IF ((((x mod 33)<>0) OR ((y mod 33)<>0) OR mueve))
        IF (direccion)
          IF (sentido)
            IF (!((Key(_alt) AND (jugador==p1)) OR (Key(_control) AND (jugador==p2)))) x=x+11; END;
            dir = m_derecha;
          ELSE
            IF (!((Key(_alt) AND (jugador==p1)) OR (Key(_control) AND (jugador==p2)))) x=x-11; END;
            dir = m_izquierda;
          END;
        ELSE
          IF (sentido)
            IF (!((Key(_alt) AND (jugador==p1)) OR (Key(_control) AND (jugador==p2)))) y=y+11; END;
            dir = m_abajo;
          ELSE
            IF (!((Key(_alt) AND (jugador==p1)) OR (Key(_control) AND (jugador==p2)))) y=y-11; END;
            dir = m_arriba;
          END;
        END;
        m++;
      END;

      IF (COLLISION(TYPE o_bloque))
        // Si topamos con un bloque, hemos de recuperar una posici¢n v lida (multiplo de 33)
        WHILE (((x mod 33)<>0) OR ((y mod 33)<>0))
          IF (direccion)
            IF (sentido)
              x=x-11;
            ELSE
              x=x+11;
            END;
          ELSE
            IF (sentido)
              y=y-11;
            ELSE
              y=y+11;
            END;
          END;
        END;
        mueve=FALSE;
      END;

      // Si tenemos una posici¢n v lida (m£ltiplo de 33), podemos dejar de movernos
      IF (((x mod 33)==0) AND ((y mod 33)==0))
        mueve = FALSE;
      END;

      // Si se pulsa fuego y no hay (son==0), usamos la secadora :-))
      IF ((Key(_alt) AND (jugador==p1)) OR (Key(_control) AND (jugador==p2)) AND (son==0))
        graph = jugador + dir;
        IF (direccion)
          IF (sentido)
            fuego(jugador, x+30, y, fire);
          ELSE
            fuego(jugador, x-30, y, fire);
          END;
        ELSE
          IF (sentido)
            fuego(jugador, x, y+30, fire);
          ELSE
            fuego(jugador, x, y-30, fire);
          END;
        END;
      ELSE
        // Si no, calculamos siguiente sprite de movimiento
        if (m>2) m=1; END;
        graph = jugador + dir + m;
      END;
      FRAME;
    END;
  END;

// ***************************************************************************
// ** PROCEDIMIENTO fuego(puntos_enemigo, tipo, coord_x, coord_y, fichero)  **
// **                                                                       **
// ** Proceso gen‚rico de enemigos                                          **
// ***************************************************************************
PROCESS o_enemigo(puntos, tipo_enemigo, x, y, file);
  PRIVATE
   m         = 1;                               // Sprite de movimiento
   dir       = m_abajo;                         // desplazamiento del sprite
   direccion = horizontal;
   sentido   = decrementa;
   mueve     = TRUE;
   n         = 0;                               // caso de decisi¢n de movimiento
   idjugador;

  BEGIN
    graph = tipo_enemigo + dir + m;
    LOOP
      // si tocamos un jugador, lo fulminamos :-(
      IF (idjugador=COLLISION(TYPE o_jugador))
        muere_o_jugador(idjugador, TRUE);       // Matamos al jugador
      END;

      // Si no estamos en movimiento, buscamos un nuevo sentido y direcci¢n
      IF (!mueve AND ((x mod 33)==0) AND ((y mod 33)==0))
       n = (n+1) mod 8;
       SWITCH (n)
         CASE 0, 5:
          direccion = horizontal;
          sentido   = decrementa;
          dir = m_izquierda;
         END;
         CASE 1, 7:
          direccion = horizontal;
          sentido   = incrementa;
          dir = m_derecha;
         END;
         CASE 2, 4:
           direccion = vertical;
           sentido   = decrementa;
           dir = m_arriba;
         END;
         CASE 3, 6:
           direccion = vertical;
           sentido   = incrementa;
           dir = m_abajo;
         END;
       END;
       mueve = TRUE;
      END;

      // Si nos movemos, o no estamos alineados, calculamos movimiento
      IF (((x mod 33)<>0) OR ((y mod 33)<>0) OR mueve)
        IF (direccion)
          IF (sentido)
            x=x+11;
            dir = m_derecha;
          ELSE
            x=x-11;
            dir = m_izquierda;
          END;
        ELSE
          IF (sentido)
            y=y+11;
            dir = m_abajo;
          ELSE
            y=y-11;
            dir = m_arriba;
          END;
        END;
        m++;
        if (m>2) m=1; END;
      END;

      // Si topamos con un bloque, nos alineamos y nos paramos (para decidir otro camino)
      IF (COLLISION(TYPE o_bloque))  // Rectifica la posici¢n
        WHILE (((x mod 33)<>0) OR ((y mod 33)<>0))
          IF (direccion)
            IF (sentido)
              x=x-11;
            ELSE
              x=x+11;
            END;
          ELSE
            IF (sentido)
              y=y-11;
            ELSE
              y=y+11;
            END;
          END;
        END;
        mueve=FALSE;
      END;
      graph = tipo_enemigo + dir + m;
      FRAME;
    END;
  END;


// ***************************************************************************
// ** PROCEDIMIENTO fuego(numero_jugador, coord_x, coord_y, imagen)         **
// **                                                                       **
// ** Hace el efecto de disparo de los personajes                           **
// ***************************************************************************
PROCESS fuego(n_jugador, x, y, imagen);
  PRIVATE
    i;
  BEGIN
    sound(s_fuego, 100, 255);
    FOR (i=0; i<4; i++)
      graph=imagen+i;
      FRAME;
    END;
  END;

// ***************************************************************************
// ** PROCEDIMIENTO o_bloque(cordenada_x, coordenada_y, tipo, fichero)      **
// **                                                                       **
// ** Imprime por cuanto multiplica los puntos                              **
// ***************************************************************************
PROCESS o_bloque(x,y, tipo, file);
  PRIVATE
    HuboColision = FALSE;
    Destruye = FALSE;
    nos_dan = FALSE;
    nos_movemos = FALSE;
    n_frame=0;
    direccion = null;
    sentido   = null;
    mueve = FALSE;
    empujador;
    jugador_empujador;
    arrastrado;
    arrastrados[max_obj];
    n_arrastrados=0;
    anterior_colision = FALSE;
    i;
    existe;
  BEGIN
    graph = tipo;
    LOOP
      IF (((x mod 33)==0) AND ((y mod 33)==0) AND (n_arrastrados<1))
        IF (empujador=COLLISION(TYPE fuego) AND (!HuboColision))
          jugador_empujador=empujador.n_jugador;
          Hubocolision = TRUE;
          mueve=TRUE;
          IF (tipo==solido)
            IF (!nos_dan)
              sound(s_no_rompe, 255, 255);
              nos_dan=TRUE;
            END;
            mueve = FALSE;                            // somos solido, no nos movemos
            destruye = TRUE;                          // fingimos destruinos
          ELSE                                        // No somos solido, nos movemos
            IF (empujador.y==y)
              direccion = horizontal;
              IF (empujador.x<x);
                sentido   = incrementa;
              ELSE
                sentido   = decrementa;
              END;
            ELSE
              direccion = vertical;
              IF (empujador.y<y);
                sentido   = incrementa;
              ELSE
                sentido   = decrementa;
              END;
            END;
          END;
        END;
      END;

      IF (mueve)
        IF (!(arrastrado = COLLISION(TYPE o_enemigo)))
          IF (arrastrado = COLLISION(TYPE o_jugador))
            // Si es el jugador que nos empuja, ignora
            IF (arrastrado.n_jugador==jugador_empujador) arrastrado = FALSE; END;
          ELSE
            arrastrado = FALSE;                       // Ni jugador ni enemigo
          END;
        END;
        anterior_colision = arrastrado;
        WHILE (arrastrado)
          existe=FALSE;
          FOR (i=0; i<n_arrastrados; i++)
            IF (arrastrados[i] == arrastrado) existe=TRUE; END;
          END;
          IF (!existe)
            arrastrados[n_arrastrados++] = arrastrado;
            signal(arrastrado, s_freeze);
          END;
          IF (!(arrastrado = COLLISION(TYPE o_enemigo)))
            IF (arrastrado = COLLISION(TYPE o_jugador))
              // Si es el jugador que nos empuja, ignora
              IF (arrastrado.n_jugador==jugador_empujador) arrastrado = FALSE; END;
            ELSE
              arrastrado = FALSE;                       // Ni jugador ni enemigo
            END;
          END;
          IF (anterior_colision == arrastrado)
            arrastrado = FALSE;                         // Ya se ha evaluado
          END;
        END;

        IF (direccion)
          IF (sentido)
            x=x+11;
          ELSE
            x=x-11;
          END;
        ELSE
          IF (sentido)
            y=y+11;
          ELSE
            y=y-11;
          END;
        END;
      END;

      IF (destruye)
        n_frame++;
        IF (n_frame>3)
          nos_dan=FALSE;
          n_frame = 0;
          destruye=FALSE;
          HuboColision=FALSE;
          nos_dan=FALSE;
          IF (tipo==blando)             // Si es blando, destruyelo
            marcador (jugador_empujador, puntos_bloque_blando);
            break;
          END;
        END;
        graph=tipo+n_frame;
      END;

      // arrastramos a los objetos
      FOR (i=0; i<n_arrastrados; i++)
        IF (direccion)
          IF (sentido)
            arrastrados[i].x+=11;
          ELSE
            arrastrados[i].x-=11;
          END;
        ELSE
          IF (sentido)
            arrastrados[i].y+=11;
          ELSE
            arrastrados[i].y-=11;
          END;
        END;
      END;

      IF (!mueve)                       // En movimiento, es el doble de r pido
        FRAME;
      ELSE
        FRAME(50);

        // Ahora veremos si hemos chocado con otra piedra
        IF (COLLISION(TYPE o_bloque))    // Rectifica la posici¢n
          nos_movemos=FALSE;
          WHILE (((x mod 33)<>0) OR ((y mod 33)<>0))
            IF (direccion)
              IF (sentido)
                x=x-11;
              ELSE
                x=x+11;
              END;
            ELSE
              IF (sentido)
                y=y-11;
              ELSE
                y=y+11;
              END;
            END;
          END;
          mueve=FALSE;
          IF (HuboColision)               // como estabamos recien empujados nos destruimos
            Destruye=TRUE;
            IF (!nos_dan)
              sound(s_rompe,  90, 255);
              nos_dan = TRUE;
            END;
          ELSE
            sound(s_choca, 160, 255);     // si no, suena el choque

            // se machacan los objetos arrastrados
            FOR (i=0; i<n_arrastrados; i++)
              marcador (jugador_empujador, arrastrados[i].Puntos * n_arrastrados);
              IF (arrastrados[i].Puntos==puntos_jugador)
                muere_o_jugador(arrastrados[i], FALSE);// Es un jugador, matalo con su procedimiento
              ELSE
                signal(arrastrados[i], s_kill);      // no es jugador, matalo de cualquier forma
                n_enemigos--;                        // un enemigo menos :-;
              END;
            END;
            IF (n_arrastrados>0)
              bonos(x, y, n_arrastrados);
              n_arrastrados=0;
            END;
          END;
        ELSE                              // como no hemos chocado, dejan de empujarnos
          HuboColision=FALSE;
          IF (!nos_movemos)
            nos_movemos=TRUE;
            sonido_roce = (sonido_roce+1) mod 4;
            SWITCH (sonido_roce)
              CASE 0: sound(s_desplaza1, 160, 255); END;
              CASE 1: sound(s_desplaza2, 160, 255); END;
              CASE 2: sound(s_desplaza3, 255, 255); END;
              CASE 3: sound(s_desplaza4, 255, 255); END;
            END;
          END;
        END;
      END;
    END;
  END

// ***************************************************************************
// ** PROCEDIMIENTO bonos(coordenada_x, coordenada_y, bonos)                **
// **                                                                       **
// ** Imprime por cuanto multiplica los puntos                              **
// ***************************************************************************
PROCESS bonos(x, y, n);
  PRIVATE
    t;
    i;
  BEGIN
    FOR (i=0; i<66; i+=4)
      y-=4;
      SWITCH (n)
        CASE 1: t = Write(f_1, x, y, 4, "x 1"); END;
        CASE 2: t = Write(f_1, x, y, 4, "x 2"); END;
        CASE 3: t = Write(f_1, x, y, 4, "x 3"); END;
        CASE 4: t = Write(f_1, x, y, 4, "x 4"); END;
        DEFAULT: t = Write(f_1, x, y, 4, "x bonos"); END;
      END;
      FRAME;
      delete_text(t);
    END;
  END;

// ***************************************************************************
// ** PROCEDIMIENTO muere_o_jugador(idjugador, BOOLEAN)                       **
// **                                                                       **
// ** Controla la muerte del jugador, y si agoniza, lo deja dando vueltas   **
// ***************************************************************************
PROCESS muere_o_jugador(idjugador, agoniza);
  PRIVATE
    idagoniza;
  BEGIN
    IF (idjugador.jugador==p1)
      IF (agoniza)
        idagoniza = objeto3(idjugador.x, idjugador.y, idjugador.graph, f_personajes);
        signal(idjugador, s_kill);
        timer[1]=0;
        WHILE (timer[1]<delay_agoniza)
          idagoniza.angle+=22500;
          FRAME(50);
        END;
        signal(idagoniza, s_kill);
      ELSE
        signal(idjugador, s_kill);
      END;
      p1_n_vidas--;
      signal(p1_vidas[p1_n_vidas], s_kill);
      IF (p1_n_vidas>0)
        timer[1]=0;
        WHILE (timer[1]<delay)
          FRAME;
        END;
        o_jugador(puntos_jugador, p1, fuego_p1, p1_x, p1_y, f_personajes);
      END;
    ELSE
      IF (agoniza)
        idagoniza = objeto3(idjugador.x, idjugador.y, idjugador.graph, f_personajes);
        signal(idjugador, s_kill);
        timer[2]=0;
        WHILE (timer[2]<delay_agoniza)
          idagoniza.angle+=22500;
          FRAME(50);
        END;
        signal(idagoniza, s_kill);
      ELSE
        signal(idjugador, s_kill);
      END;
      p2_n_vidas--;
      signal(p2_vidas[p2_n_vidas], s_kill);
      IF (p2_n_vidas>0)
        signal(idjugador, s_kill);
        timer[2]=0;
        WHILE (timer[2]<delay)
          FRAME;
        END;
        o_jugador(puntos_jugador, p2, fuego_p2, p2_x, p2_y, f_personajes);
      END;
    END;
  END;

