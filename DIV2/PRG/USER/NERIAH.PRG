/*
               *** NERIAH EN EL OESTE. (FASE 1: TNT GIRLS) ***

Este es mi primer programa con DIV. Lo he hecho en una semana (es el tiempo
que hace que lo compr‚). Es s¢lo una prueba de una fase de lo que podr¡a
ser un juego m s extenso. Seguro que hay partes del programa que se podr¡an
haber solucionado mejor pero mi base de programaci¢n es de ToolBook y me
ha costado un poco cambiar el chip (de programaci¢n de objetos a esto de
los procesos). No obstante estoy bastante satisfecho del resultado.

        Por cierto, me llamo Xavi y mi e-mail es ippuh@cc.uab.es



                           ­Saludos a todos!
*/

PROGRAM neriah;
GLOBAL
// Variables globales

    titol;
    cursor;
    posicio;
    lloccursor;
    ficheroneriah;
    crea;chica;p;
    balas=6;
    vidas=5;
    lletres1;lletres2;
    puntuacio;
    x_punto; y_punto;       //Posici¢n de la pistola
    actiurifle=0;tinc_rifle=0;tiros_rifle;avisrifle;escritrifle;

//Variables de sonido
    sinbala;disparo;gemido1;gemido2;gemido3;gemido4;boom;musica;fondo;

//Variables para saber si el enemigo est  en pantalla

    actiu1=0;actiu2=0;actiu3=0;actiu4=0;actiu5=0;actiu6=0;
    actiu7=0;actiu8=0;actiu9=0;actiu10=0;actiu11=0;actiu12=0;
    actiu13=0;actiu14=0;actiu15=0;actiu16=0;actiu17=0;actiu18=0;


LOCAL
// Variables Locales
dades;


BEGIN
//Modo de video a 640 x 480
    set_mode(m640x480);
//Carga el fichero con el gr fico de presentaci¢n
    titol=load_fpg ("USER\neriah\titolner.fpg");
//Carga las letras del inicio
    lletres2=load_fnt("USER\neriah\neriah.fnt");
//Carga las letras del marcador
    lletres1=load_fnt("USER\neriah\neriah2.fnt");
//Carga los sonidos
    sinbala=load_pcm("USER\neriah\sinbala.pcm",0);
    disparo=load_pcm("USER\neriah\tiro.pcm",0);
    gemido1=load_pcm("USER\neriah\a1.pcm",0);
    gemido2=load_pcm("USER\neriah\a2.pcm",0);
    gemido3=load_pcm("USER\neriah\a3.pcm",0);
    gemido4=load_pcm("USER\neriah\a4.pcm",0);
    boom=load_pcm("USER\neriah\explosio.pcm",0);
    musica=load_pcm("USER\neriah\musica.pcm",1);
//Ejecuta el proceso llamado menu (Presentaci¢n del Juego)
    menu ();
END


PROCESS menu()
//Presentaci¢n del Juego
BEGIN
//Muestra el fondo
put_screen (titol,1);
//Muestra instrucciones en pantalla
write (lletres2,320,340,4,"Bot¢n izquierdo: Disparo");
write (lletres2,320,380,4,"Bot¢n derecho: Recarga");
write (lletres2,320,440,4,"Pulsa 'espacio' para empezar");
LOOP
//Espera a que se pulse el espacio para empezar
 IF (key(_space))
//Descarga el fichero de graficos
    unload_fpg (titol);
//Hace sonar un disparo
sound(disparo,128,256);
//Carga la nueva paleta de colores
    load_pal ("USER\neriah\neriah.fpg");
    ficheroneriah=load_fpg ("USER\neriah\neriah.fpg");//Carga los gr ficos del juego
    put_screen (ficheroneriah,14);//Pone el nuevo fondo
    cartell(x,y);
    fondo=sound(musica,100,256);
    BREAK;
 END
FRAME;
END
END

PROCESS cartell(x,y)
BEGIN
//Borra los textos
   delete_text(all_text);
//Inicia el crono
   timer[0]=0;
   x=320;y=240;
   LOOP
       graph=44;
//Si pasan 3 segundos empieza la animaci¢n
       IF (timer[0]>300) y=y-3;size=size-3;angle=angle-40000;END
//Cuando el cartel es peque¤o, sale del bucle
       IF (size<0) BREAK;END
       FRAME;
   END


//Procesos de inicio del juego
    mouse.x=320;mouse.y=140;
//Cursor del mouse = Punto de mira
    mouse.graph=13;
//Puntuaci¢n a 0
    Puntuacio=0;
    chica=neri(320,350);//Inicia el proceso que muestra el personaje del jugador
    barra(0,0);//Situa la barra sobre el escenario

    porche(77,263,-100);//A¤ade elementos sobre el escenario
    roda(163,153,-90);//A¤ade elementos sobre el escenario
    ventana(191,95,-100);//A¤ade elementos sobre el escenario
    salon(498,207,-100);//A¤ade elementos sobre el escenario

//Variables para indicar que el enemigo no est  en pantalla
    actiu1=0;actiu2=0;actiu3=0;actiu4=0;actiu5=0;actiu6=0;
    actiu7=0;actiu8=0;actiu9=0;actiu10=0;actiu11=0;actiu12=0;
    actiu13=0;actiu14=0;actiu15=0;actiu16=0;actiu17=0;actiu18=0;
    actiurifle=0;tinc_rifle=0;


END



PROCESS barra(x,y)
BEGIN
LOOP
    graph=24;
    x=320;
    y=15;
    z=-1000;
    municion(x,y);
    estrellas(x,y);
    marcador(x,y);
    tipoarma(x,y);
    FRAME;
END
END

PROCESS municion(x,y)
PRIVATE
t;soroll;
BEGIN
x=450;
   FROM t = 1 to 6;
    IF (t > balas); BREAK; END;
    graph=31;
    x=x+25;
    y=20;
    size=75;
    z=-1100;
    FRAME;

   END

END

PROCESS estrellas(x,y)
PRIVATE
t;
BEGIN
x=0;
   FROM t = 1 to 5;

    IF (t > vidas) BREAK; END;
    graph=40;
    x=x+40;
    y=20;
    size=75;
    z=-1100;
    FRAME;

   END
IF (vidas==0)
    graph=21;
    FADE(100,0,0,4);
    REPEAT
        FRAME;
    UNTIL (NOT FADING)
    finjoc();
END

END

PROCESS marcador(x,y)
PRIVATE
textpunts;
BEGIN
delete_text(textpunts);
textpunts=write_int(lletres1,320,20,4,OFFSET puntuacio);
text_z=-1100;
FRAME;
END

PROCESS tipoarma(x,y)
BEGIN
x=425;y=20;
if (tinc_rifle==1) graph=45;
else graph=48 ;END
size=70;
z=-1100;
FRAME;
END




PROCESS porche(x,y,z)
BEGIN
LOOP
    graph=28;
    FRAME;
END
END

PROCESS roda(x,y,z)
BEGIN
LOOP
    graph=29;
    FRAME;
END
END



PROCESS ventana(x,y,z)
BEGIN
LOOP
    graph=27;
    FRAME;
END
END




PROCESS salon(x,y,z)
BEGIN
LOOP
/*
IF (KEY(_up)) y=y-1;END
IF (KEY(_down)) y=y+1;END
IF (KEY(_right)) x=x+1;END
IF (KEY(_left)) x=x-1;END
IF (KEY(_space))
    write_INT(0,320,240,4,OFFSET x);
    write_INT(0,340,240,4,OFFSET y);
END
*/
    graph=26;
    FRAME;
END
END

PROCESS finjoc()
BEGIN

LOOP
    let_me_alone();
// Borra el cursor (gr fico todo negro).
    stop_sound(fondo);
    mouse.graph=21;
    unload_fpg (ficheroneriah);
    load_pal ("USER\neriah\titolner.fpg");
    titol=load_fpg ("USER\neriah\titolner.fpg");
    put_screen (titol,1);
    delete_text (all_text);
    write (lletres2,320,160,4,"Te cazaron...");
    write (lletres2,320,200,4,"Tu puntuaci¢n ha sido de");
    write_int (lletres2,320,240,4,OFFSET puntuacio);
    write (lletres2,320,310,4,"Pulsa 'espacio'");
    write (lletres2,320,350,4,"para volver a empezar");
    write (lletres2,320,420,4,"Pulsa 'esc' para salir");
    if (key(_space)) vidas=5; balas=6;menu();BREAK;END
    if (key(_esc)) exit("Hasta la vista, forastero",0);END
    FRAME;
END

END


PROCESS NERI(x,y)
PRIVATE
permiso=1;permiso2=1;
BEGIN
x_punto=x;
y_punto=y;
//timer[2]=0;

LOOP
lloccursor=mouse.x;

   FROM graph = 1 to 12; //Neri andando
    //Impide que el cursor pase debajo de la barra superior
      IF (mouse.y<50) mouse.y=50;END
    // Inicia el proceso de creaci¢n de enemigos
      IF (RAND (0,100)>95) crea=aon();END
    //Cursor del mouse = Punto de mira
      if(tinc_rifle==1) mouse.graph=47;
        else mouse.graph=13;END
    //Modifica el tama¤o dependiendo de lo arriba que est‚.
      cursor=((mouse.y*100)/640);
      mouse.size = cursor+20;
    //Coge la posici¢n x del cursor
      lloccursor=mouse.x;
      posicio=x;
      z=-1000;


//Si se aprieta el bot¢n izquierdo del rat¢n...
    //Pone a 0 la variable permiso para evitar la repetici¢n de la
    //pulsaci¢n si no se tiene el rifle
      IF (mouse.left AND permiso)
        permiso=0;
      IF (tinc_rifle==1) permiso=1; tiros_rifle=tiros_rifle-1;
        IF (tiros_rifle<1) balas=6; tinc_rifle=0; actiurifle=0; permiso=0;END
        IF (tiros_rifle>0 AND tiros_rifle<11) balas=1;END
        IF (tiros_rifle>10 AND tiros_rifle<21) balas=2;END
        IF (tiros_rifle>20 AND tiros_rifle<31) balas=3;END
        IF (tiros_rifle>30 AND tiros_rifle<41) balas=4;END
        IF (tiros_rifle>40 AND tiros_rifle<51) balas=5;END
        IF (tiros_rifle>50 AND tiros_rifle<61) balas=6;END
      END
//...y si se tienen balas...
        IF (balas > 0)
    //Muestra a Neri parada...
         graph=15;
    //... a una altura de 350
         y=350;
    //Consigue la posici¢n de la pistola.
         get_real_point(1,offset x_punto,offset y_punto);
    // Genera fuego en el ca¤¢n de la pistola.
         tiro (x_punto,y_punto);
    // Dispara una bala
         bala(mouse.x,mouse.y);
    // Resta una bala a la munici¢n si no se tiene el rifle.
         if (tinc_rifle==0) balas=balas-1;END
         FRAME;
//...Si no queda munici¢n...
    ELSE
    //Carga el sonido "sin balas"

    //Hace sonar el gatillo
         sound(sinbala,128,256);
    //Muestra a Neri parada...
         graph=15;
    //... a una altura de 350
         y=350;
    //Consigue la posici¢n de la pistola.
         get_real_point(1,offset x_punto,offset y_punto);
    //Envia al proceso donde se muestra el texto de "RECARGA"
          recarga(320,140);
         FRAME;
        END
      END

   //Comprueba que se haya soltado el bot¢n izquierdo
   //para permitir un nuevo disparo
      IF (mouse.left <> 1) permiso=1;END

//Si se aprieta el bot¢n derecho del mouse...
       IF (mouse.right AND permiso2) permiso2=0;
    //Consigue la posici¢n de la pistola.
        get_real_point(1,offset x_punto,offset y_punto);
    //Hace sonar el gatillo
         sound(sinbala,128,256);
    // Munici¢n a 6 balas si no tiene el rifle
        if (tinc_rifle==0) balas=6;END
      END
      IF (mouse.right == 0 ) permiso2=1;END

// Si Neri no est  alineada verticalmente con el mouse...
      IF (posicio < lloccursor-11 or posicio > lloccursor+11)
        //Simulaci¢n de movimiento al andar (arriba y abajo)
          if (graph < 7 AND graph>1 ) y-=2; END
          if (graph > 6 ) y+=2; END
          if (graph < 2) y=350; END
        // Si Neri est  a la izquierda de la posici¢n del mouse hace un mirror de la imagen y se acerca al mouse.
          if (x > mouse.x) x-=12; flags=1; END
        // Si Neri est  a la derecha de la posici¢n del mouse, se acerca al mouse
          if (x < mouse.x) x+=12; flags=0; END
          FRAME;
      ELSE
      // Si Neri est  alineada verticalmente con el mouse...
        graph=15;//Muestra a Neri parada
        y=350;
     //Consigue la posici¢n de la pistola.
         get_real_point(1,offset x_punto,offset y_punto);
        FRAME;

      END

   END
END
END

PROCESS recarga(x,y)
BEGIN
if (avisrifle==1) avisrifle=2;signal(escritrifle,s_sleep);END
REPEAT
    graph=32;
    z=-500;
    FRAME;
UNTIL (mouse.right)
if (avisrifle==2) avisrifle=1;signal(escritrifle,s_wakeup);END
END

PROCESS tiro(x,y)
PRIVATE

escopeta;
BEGIN
//Carga el sonido de disparo

//Hace sonar el disparo
    sound(disparo,128,256);
//Muestra el fuego en el ca¤¢n de la pistola
    FROM graph = 16 to 20;
        FRAME;
    END
// Da la posibilidad de mostrar el rifle
    escopeta=RAND(1,50);
    if (escopeta==50 and actiurifle==0) rifle();END

END

PROCESS rifle()
BEGIN
if (actiurifle==0 AND tinc_rifle==0)
timer[1]=0;
  graph=45;
  x=RAND(50,600);
  y=RAND(300,400);
  flags=RAND(0,1);
  avisrifle=1;
  escritrifle=avis_rifle();
REPEAT
actiurifle=1;FRAME;
UNTIL (timer[1]>600);
if (timer[1]>600) actiurifle=0;avisrifle=0;END
END
END

PROCESS avis_rifle()
BEGIN
graph=49; x=320; y=140; z=-110;
LOOP
    IF (avisrifle==0 or tinc_rifle==1); BREAK;
    ELSE
        FRAME;
    END
END
END



PROCESS bala(x,y)
PRIVATE
dinamita;elimina;quegrito;canvi_a_rifle;
f1;f2;f3;f4;
BEGIN
//Da a P el valor de 10
    P=10;
//Da a la bala el tama¤o del mouse
    size=mouse.size;
    IF (size<50) size=50;END
// Borra el cursor (gr fico todo negro).
    mouse.graph=21;
REPEAT
  //Imagen de la bala
     graph = 22;
  //Disminuye el tama¤o
     size=size-5;
  //Plano de la bala
     z=-200;
  //Resta 1 a la variable P
     P=P-1;

// Detecta colisi¢n con el enemigo
IF (P<2)
    elimina=(COLLISION(TYPE aon));
  //Si colisiona, reactiva la creaci¢n de enemigos en esa posici¢n
    IF (elimina<>0)
  //Crea una mancha de sangre en el lugar de impacto de la bala
      sang(x,y);
  //Carga un sonido de gemido aleatoriamente
      quegrito=(RAND(2,4));
      IF (quegrito==2) sound(gemido2,256,562);END
      IF (quegrito==3) sound(gemido3,256,562);END
      IF (quegrito==4) sound(gemido4,256,562);END
  //Comprueba en qu‚ posici¢n est  el enemigo y deja el espacio disponible
  //para otro
      if (elimina.dades==1) actiu1=0;END
      if (elimina.dades==2) actiu2=0;END
      if (elimina.dades==3) actiu3=0;END
      if (elimina.dades==4) actiu4=0;END
      if (elimina.dades==5) actiu5=0;END
      if (elimina.dades==6) actiu6=0;END
      if (elimina.dades==7) actiu7=0;END
      if (elimina.dades==8) actiu8=0;END
      if (elimina.dades==9) actiu9=0;END
      if (elimina.dades==10) actiu10=0;END
      if (elimina.dades==11) actiu11=0;END
      if (elimina.dades==12) actiu12=0;END
      if (elimina.dades==13) actiu13=0;END
      if (elimina.dades==14) actiu14=0;END
      if (elimina.dades==15) actiu15=0;END
      if (elimina.dades==16) actiu16=0;END
      if (elimina.dades==17) actiu17=0;END
      if (elimina.dades==18) actiu18=0;END
   //Aumenta puntuaci¢n dependiendo del tama¤o del enemigo
      if (elimina.size<30) puntuacio+=100;
      else puntuacio+=50;END
   //Elimina al enemigo
      signal(elimina,s_kill);
      FRAME;
   //Elimina la bala
      BREAK;
    END
FRAME;
END


  //Si la bala colisiona con un cartucho de dinamita la destruye
    dinamita=(COLLISION (TYPE tnt));
    IF (dinamita) signal(dinamita,s_kill);
  //Hace sonar la explosi¢n
    sound(boom,128,256);
  //A¤ade 10 a la puntuaci¢n
    puntuacio=puntuacio+10;
  //Destruye la bala
    BREAK; END
    FRAME;

  //Comprueba si la bala colisiona con el rifle
    canvi_a_rifle=(COLLISION (TYPE rifle));
  //Destruye el rifle
    IF (canvi_a_rifle) signal(canvi_a_rifle,s_kill);
  //Prepara para disparos en r faga
    tinc_rifle=1;
  //Recarga la munici¢n
    balas=6;
  //Tiros del rifle al m ximo
    tiros_rifle=60;
  //Borra aviso "RECOGE RIFLE"
    signal(escritrifle,s_kill);avisrifle=0;
  //Muestra aviso de r faga
    aviso();
  //Destruye la bala
    BREAK; END
    FRAME;


//Destruye la bala cuando es menor que 0 si no ha impactado.
UNTIL (P<0);
//Vuelve a mostrar el Punto de mira en el cursor
      if(tinc_rifle==1) mouse.graph=47;
        else mouse.graph=13;END
FRAME;

END

PROCESS aviso()
BEGIN
REPEAT
graph=46; x=320; y=140; z=-110;
FRAME;
UNTIL (tiros_rifle<50)
END

PROCESS tnt(x,y,size)
PRIVATE
trajecte;rotacio;
BEGIN
IF (size > 25) size=size-25;END
rotacio=(RAND(20000,40000));
   IF (x_punto>x) trajecte=(x_punto-x)/2;
       LOOP
         graph=33;
         size=size+1;
         z=-275;
         angle=angle+rotacio;
         x=x+10;
         IF (x > 680) BREAK;END
         IF (x> trajecte)
            y=y+10;
         ELSE
            y=y-10;
         END
         FRAME;
         IF (y>410) explosio(x,y);BREAK;END
        END
   ELSE
        trajecte=(x-x_punto)/2;
       LOOP
         graph=33;
         size=50;
         z=-200;
         angle=angle-rotacio;
         x=x-10;
         IF (x < -20) BREAK;END
         IF ((700-x)>trajecte)
            y=y+10;
         ELSE
            y=y-10;
         END
         FRAME;
         IF (y>410) explosio(x,y);BREAK;END
        END

    END
END

PROCESS explosio(x,y)
PRIVATE
impacto;permitido;
BEGIN
permitido=1;
//Hace sonar la explosi¢n
  sound(boom,128,256);
    FROM graph=34 to 39;
    //Comprueba si la explosi¢n toca a Neriah
      impacto=(COLLISION (TYPE neri));
    //Si toca...
    IF (impacto AND permitido)
      //Variable que solo permite activar el sonido 1 vez
        permitido=0;
      //Hace sonar el gemido
        sound(gemido1,256,256);
      //Resta una vida si no le acaban de restar una
      IF (NOT FADING) vidas=vidas-1;END
    FADE(100,50,50,8);
    REPEAT
        FRAME;
    UNTIL (NOT FADING)
    FADE(100,100,100,8);
    REPEAT
        FRAME;
    UNTIL (NOT FADING)

    END

       FRAME;
    END
END

PROCESS aon()
PRIVATE
posa;quin;cap;permitido,sonido;
BEGIN
cap=0;
posa=(rand(1,18));
z=-99;
/*
    Elige aleatoriamente una de las 18 posiciones, asign ndole
    su correspondiente tama¤o, reflejo y profundidad.
    Adem s, pone a 1 la variable correspondiente que indica que esa posici¢n
    ya est  asignada para que no se superponga otro enemigo.
    La variable "dades" servir  para indicar que puesto a dejado libre un
    enemigo destruido.
*/

IF (posa==1 AND actiu1==0)  x=50;y=110;size=39;flags=0;actiu1=1;cap=1;dades=posa;END
IF (posa==2 AND actiu2==0)  x=202;y=82; size=20;flags=0;actiu2=1;cap=1;dades=posa;END
IF (posa==3 AND actiu3==0)  x=208;y=216;size=22;flags=0;z=-80;actiu3=1;cap=1;dades=posa;END
IF (posa==4 AND actiu4==0)  x=379;y=179;size=10;flags=1;actiu4=1;cap=1;dades=posa;END
IF (posa==5 AND actiu5==0)  x=418;y=105;size=16;flags=1;actiu5=1;cap=1;dades=posa;END
IF (posa==6 AND actiu6==0)  x=541;y=89; size=26;flags=1;actiu6=1;cap=1;dades=posa;END
IF (posa==7 AND actiu7==0)  x=586;y=94; size=30;flags=1;z=-160;actiu7=1;cap=1;dades=posa;END
IF (posa==8 AND actiu8==0)  x=562;y=225;size=39;flags=1;actiu8=1;cap=1;dades=posa;END
IF (posa==9 AND actiu9==0)  x=471;y=211;size=28;flags=1;actiu9=1;cap=1;dades=posa;END
IF (posa==10 AND actiu10==0) x=443;y=196;size=23;flags=1;actiu10=1;cap=1;dades=posa;END
IF (posa==11 AND actiu11==0) x=371;y=117;size=16;flags=1;actiu11=1;cap=1;cap=1;dades=posa;END
IF (posa==12 AND actiu12==0) x=338;y=202;size=24;flags=1;actiu12=1;cap=1;dades=posa;END
IF (posa==13 AND actiu13==0) x=240;y=238;size=31;flags=0;actiu13=1;cap=1;dades=posa;END
IF (posa==14 AND actiu14==0) x=259;y=202;size=24;flags=0;z=300;actiu14=1;cap=1;dades=posa;END
IF (posa==15 AND actiu15==0) x=158;y=233;size=24;flags=0;actiu15=1;cap=1;dades=posa;END
IF (posa==16 AND actiu16==0) x=42; y=285;size=50;flags=0;actiu16=1;cap=1;dades=posa;END
IF (posa==17 AND actiu17==0) x=620;y=260;size=50;flags=1;actiu17=1;cap=1;dades=posa;END
IF (posa==18 AND actiu18==0) x=120;y=120;size=25;flags=0;actiu18=1;cap=1;dades=posa;END

//  Elige al azar uno de las cinco TNT girls
quin=(rand(1,5));
IF (quin== 1) graph=23;END
IF (quin== 2) graph=25;END
IF (quin== 3) graph=41;END
IF (quin== 4) graph=42;END
IF (quin== 5) graph=43;END

// Si se ha creado un nuevo enemigo...
IF (cap==1)
    LOOP
     FRAME;
        LOOP
// Inicia el proceso de lanzar dinamita aleatoriamente.
            IF (RAND(0,150)==150) tnt(x,y,size);END
            FRAME;
        END
    END
END
END




PROCESS sang(x,y)
PRIVATE
elimina;
BEGIN
    FROM graph = 16 to 20;
     z=-200;
     FRAME;
    END
END