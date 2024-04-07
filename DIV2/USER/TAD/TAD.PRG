// Demostraci¢n de uso de TAD.DLL
//
// Demostraci¢n de como usar tipos abstractos de datos (pilas y colas)
// mediante tad.DLL
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
// * El archivo TAD.DLL puede ser distribuido gratuitamente junto a cualquier
// programa compilado y realizado en DIV que lo use. En este caso, el resto de
// archivos de este paquete no necesariamente deben acompa¤arlo.
//
// 15-3-1998 Miguel Adolfo Barroso Mu¤oz
//
// Notas:
//   La ruta de IMPORT debe modificarse si es distinta


PROGRAM prueba_tad;

GLOBAL
 cy=0;
 pila, cola;
 tmp;

 import "c:\div\dll\tad.dll";       // Debe modificarse si la ruta es distinta

BEGIN

  // Comprobamos que se usa la versi¢n adecuada de TAD.DLL
  if ((tad_version() !=100))
    EXIT('Requerida versi¢n 1.0 de TAD.DLL', 1);
  END;

  set_mode(m640x480);

  write (0, 340, 0, 0, 'DEMOSTRACION DE TAD.DLL');

  // Creamos los punteros a las estructuras de datos
  write (0,  40, 0, 0, 'creamos estructuras');
  pila = crea_pila();
  cola = crea_cola();

  // Usamos la pila: apilamos 3 elementos de cualquier tipo
  write (0,  40, 20, 0, 'apilamos "12"');
  apila(pila, 12);
  write (0,  40, 30, 0, 'apilamos "hola a todos"');
  apila(pila, "hola a todos");
  write (0,  40, 40, 0, 'apilamos PROCESS');
  apila(pila, nulo());

  // Usamos la pila: desapilamos los 3 elementos
  write (0,  40, 50, 0, 'desapilamos:');
  write (0,  80, 60, 0, 'PROCESS');
  signal (desapila(pila), s_kill);
  write (0,  80, 70, 0, desapila(pila));
  tmp = desapila(pila);
  write_int (0, 140, 80, 0, OFFSET tmp);

  // Usamos la cola: acolamos 3 elementos de cualquier tipo
  write (0,  40,100, 0, 'A¤adimos a la cola "12"');
  acola(cola, 12);
  write (0,  40,110, 0, 'A¤adimos a la cola "hola a todos"');
  acola(cola, "hola a todos");
  write (0,  40,120, 0, 'A¤adimos a la cola PROCESS');
  acola(cola, nulo());

  // Usamos la cola: desacolamos los 3 elementos
  write (0,  40,130, 0, 'Sacamos de la cola:');
  tmp = desacola(cola);
  write_int (0, 140,140, 0, OFFSET tmp);
  write (0, 80,150, 0, desacola(cola));
  write (0, 80,160, 0, 'PROCESS');
  signal (desacola(cola), s_kill);

  // destruimos los punteros a las estructuras, liberamos memoria
  write (0,  40,180, 0, 'destruimos estructuras');
  destruye_pila(pila);
  destruye_cola(cola);


  // Esperamos en bucle para que pueda leerse los textos en la pantalla
  LOOP
    IF (key(_esc))    // Se ha pulsado 'escape'
      BREAK;          // Se sale del bucle
    END
    FRAME;
  END;

END;


// Proceso que no hace nada, devolver  ID, que ser  usado para la PILA y COLA
PROCESS nulo();
  BEGIN
    FRAME;
  END;
