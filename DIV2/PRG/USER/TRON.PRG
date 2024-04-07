//---------------------------------------------------------
// TITULO: TRON
// AUTOR: JAVIER SEGURA Y ANTONIO LLORENS
// FECHA: 10/02/98
//---------------------------------------------------------

PROGRAM tron2;

GLOBAL
    fuente;                   //declaro las varibles  necesarias  para el juego
    fuente1;
    fondo;
    estado;
    jug;
    jugador[]=1,1,1,1;        // esta variable se necesita paralos juegos>2jug.
    jugador1;
    jugador2;
    jugador3;
    jugador4;
    equipo1;
    equipo2;

LOCAL
    i;
    dir;
    pulsacion_i;
    pulsacion_d;
    salir;
    giro_i;
    giro_d;
    nuevo_estado;             // para saber  que tipo de juego es

BEGIN
set_fps(70,0);                // inicializo la velocidad del juego
set_mode(m320x200);           // cargo el modo  de video( En realidad no haria falta)
estado=1;                     // estado inical del  juego
fuente=load_fnt("user\tron\tron.fnt");    //cargo las fuentes
fuente1=load_fnt("user\tron\tron2.fnt");
jugador1=0;
jugador2=0;                    // estas  variables son para el marcador
jugador3=0;
jugador4=0;
equipo1=0;
equipo2=0;
play_cd(4,0);
LOOP
    SWITCH (estado)                 //para cargar los juegos
        CASE 1: titulo();END
        CASE 2: juego_solo();END
        CASE 3: juego_doble();END
        CASE 4: juego_triple();END
        CASE 5: juego_4_normal();END
        CASE 6: juego_4_vs();END
        CASE 7: marcador();END
        CASE 8: exit("Gracias por jugar",0);END
    END
FRAME;
END
stop_cd();
END

//-----------------------------------------------------------------------------
// Proceso paraponer la pantalla de titulo
// Entradas: Ninguna
//-----------------------------------------------------------------------------

PROCESS titulo();
BEGIN
estado=-1;                    //cambio el estado
nuevo_estado=0;                 //varible necesaria para  selecionar la opcion
clear_screen();                 //borro la pantalla
delete_text(all_text);          //borro los textos
fondo=load_map("user\tron\tron.map");     //cargo el fondo
put_screen(0,fondo);            //pongo el fondo en la pantalla
FRAME;
write(fuente,160,20,4,"TRON v1.0");
write(fuente,100,100,4,"Elige una Opcion:");     //opciones de  juego
write(fuente,100,120,3,"1. Un jugador");
write(fuente,100,130,3,"2. Dos jugadores");
write(fuente,100,140,3,"3. Tres jugadores");
write(fuente,100,150,3,"4. Cuatro jugadores");
write(fuente,100,160,3,"5. DOS VS DOS");
write(fuente,100,170,3,"6. MARCADOR");
write(fuente,80,180,3,"ESC : SALIR");
WHILE (nuevo_estado==0)
    IF (key(_1)) nuevo_estado=2;END
    IF (key(_2)) nuevo_estado=3;END
    IF (key(_3)) nuevo_estado=4;END
    IF (key(_4)) nuevo_estado=5;END
    IF (key(_5)) nuevo_estado=6;END
    IF (key(_6)) nuevo_estado=7;END
    IF (key(_esc)) nuevo_estado=8;END
    FRAME;
END
unload_map(fondo);                  //descargo el  fondo y borro  los textos
delete_text(all_text);
estado=nuevo_estado;
END

//-----------------------------------------------------------------------------
// Proceso para reirte del jugador 1
// Entradas: Ninguna
//-----------------------------------------------------------------------------

PROCESS juego_solo();
BEGIN
estado=0;
write(fuente,160,100,4,"¨Tu CREES que le he puesto inteligencia?");
write(fuente,160,120,4,"Pulsa ESPACIO para continuar");
WHILE (NOT(key(_space)))
    FRAME;
END
delete_text(all_text);
estado=1;
END


//-----------------------------------------------------------------------------
// Proceso para controlar el juego de dos jugadores
// Entradas: Ninguna
//-----------------------------------------------------------------------------

PROCESS juego_doble();
PRIVATE
    id1;
    id2;

BEGIN
estado=0;
jug=0;                      //varible para salir del juego
jugador[0]=1;               //inicilizo los  contadores  de vida
jugador[1]=1;
unload_map(fondo);
clear_screen();
delete_text(all_text);
bordes(40);                 //llamo  la proceso bordes
id1=nave(20,208,1,0);       // llamo a las naves
id2=nave(300,225,3,1);
REPEAT
    FRAME;
UNTIL (jug>0);
IF (jugador[0]==1) write(fuente,160,20,4,"Ha ganado el jugador 1");jugador1++;END
IF (jugador[1]==1) write(fuente,160,20,4,"Ha ganado el jugador 2");jugador2++;END
signal (id1,s_kill);
signal (id2,s_kill);
write(fuente1,160,100,4,"¨Quieres jugar otra?(S/N)");
nuevo_estado=0;
WHILE (nuevo_estado==0);
    IF (key(_n)) nuevo_estado=1;END
    IF (key(_s)) nuevo_estado=3;END
FRAME;
END
delete_text(all_text);
estado=nuevo_estado;
END


//-----------------------------------------------------------------------------
// Proceso para controlar el juego de tres jugadores
// Entradas: Ninguna
//-----------------------------------------------------------------------------

PROCESS juego_triple();
PRIVATE
    id1;
    id2;
    id3;

BEGIN
jugador[0]=1;
jugador[1]=1;
jugador[2]=1;
estado=0;
jug=0;
unload_map(fondo);
clear_screen();
delete_text(all_text);
bordes(40);
id1=nave(20,208,1,0);
id2=nave(160,217,2,1);
id3=nave(300,225,3,2);
REPEAT
    FRAME;
UNTIL (jug>1);
IF (jugador[0]==1) write(fuente,160,20,4,"Ha ganado el jugador 1");jugador1++;END
IF (jugador[1]==1) write(fuente,160,40,4,"Ha ganado el jugador 2");jugador2++;END
IF (jugador[2]==1) write(fuente,160,60,4,"Ha ganado el jugador 3");jugador3++;END
signal (id1,s_kill);
signal (id2,s_kill);
signal (id3,s_kill);
nuevo_estado=0;
write(fuente1,160,100,4,"¨Quieres jugar otra?(S/N)");
WHILE (nuevo_estado==0);
    IF (key(_n)) nuevo_estado=1;END
    IF (key(_s)) nuevo_estado=4;END
FRAME;
END
delete_text(all_text);
estado=nuevo_estado;
END


//-----------------------------------------------------------------------------
// Proceso para controlar el juego de cuatro jugadores
// Entradas: Ninguna
//-----------------------------------------------------------------------------

PROCESS juego_4_normal();
PRIVATE
    id1;
    id2;
    id3;
    id4;

BEGIN
jugador[0]=1;
jugador[1]=1;
jugador[2]=1;
jugador[3]=1;
estado=0;
jug=0;
unload_map(fondo);
clear_screen();
delete_text(all_text);
bordes(40);
id1=nave(40,208,1,0);
id2=nave(120,217,2,1);
id3=nave(200,133,3,2);
id4=nave(280,225,4,3);
REPEAT
    FRAME;
UNTIL (jug>2);
IF (jugador[0]==1) write(fuente,160,20,4,"Ha ganado el jugador 1");jugador1++;END
IF (jugador[1]==1) write(fuente,160,40,4,"Ha ganado el jugador 2");jugador2++;END
IF (jugador[2]==1) write(fuente,160,60,4,"Ha ganado el jugador 3");jugador3++;END
IF (jugador[3]==1) write(fuente,160,60,4,"Ha ganado el jugador 4");jugador4++;END
signal (id1,s_kill);
signal (id2,s_kill);
signal (id3,s_kill);
signal (id4,s_kill);
nuevo_estado=0;
write(fuente1,160,100,4,"¨Quieres jugar otra?(S/N)");
WHILE (nuevo_estado==0);
    IF (key(_n)) nuevo_estado=1;END
    IF (key(_s)) nuevo_estado=5;END
FRAME;
END
delete_text(all_text);
estado=nuevo_estado;
END


//-----------------------------------------------------------------------------
// Proceso para controlar el juego de cuatro jugadores
// Entradas: Ninguna
//-----------------------------------------------------------------------------

PROCESS juego_4_vs();
PRIVATE
    id1;
    id2;
    id3;
    id4;

BEGIN
jugador[0]=1;
jugador[1]=1;
jugador[2]=1;
jugador[3]=1;
estado=0;
jug=0;
unload_map(fondo);
clear_screen();
delete_text(all_text);
bordes(40);
id1=nave(40,208,1,0);
id2=nave(120,235,2,1);
id3=nave(200,79,3,2);
id4=nave(280,225,4,3);
REPEAT
    FRAME;
UNTIL (jug>2);
IF ((jugador[0]==1) or (jugador[2]==1)) write(fuente,160,40,4,"Ha ganado el equipo Rojo");equipo2++;END
IF ((jugador[1]==1) or (jugador[3]==1)) write(fuente,160,60,4,"Ha ganado el equipo Azul");equipo1++;END
signal (id1,s_kill);
signal (id2,s_kill);
signal (id3,s_kill);
signal (id4,s_kill);
nuevo_estado=0;
write(fuente1,160,100,4,"¨Quieres jugar otra?(S/N)");
WHILE (nuevo_estado==0);
    IF (key(_n)) nuevo_estado=1;END
    IF (key(_s)) nuevo_estado=6;END
FRAME;
END
delete_text(all_text);
estado=nuevo_estado;
END



//-----------------------------------------------------------------------------
// Proceso para controlar la nave de los jugadores
// Entradas: x,color y teclas que se usaran
//-----------------------------------------------------------------------------

PROCESS nave(x,color,teclas,tipo);
BEGIN
dir=1;  //direccion de la "moto"
y=170;
salir=0;
IF (teclas==1) giro_i=16;giro_d=17;END //Teclas que usar  dicho proceso
IF (teclas==2) giro_i=37;giro_d=38;END
IF (teclas==3) giro_i=75;giro_d=80;END
IF (teclas==4) giro_i=55;giro_d=74;END
REPEAT
    IF (key(giro_i))
        IF (pulsacion_i==0)   //proporcioan un retardo al juego
            dir-=1;
        END
        pulsacion_i++;
    ELSE
        pulsacion_i=0;
    END
    IF (dir<1) dir=4;END
    IF (key(giro_d))
        IF (pulsacion_d==0)
            dir+=1;
        END
        pulsacion_d++;
    ELSE
        pulsacion_d=0;
    END
    IF (dir>4) dir=1;END
IF (dir==1) y=y-1;END
IF (dir==2) x=x+1;END
IF (dir==3) y=y+1;END
IF (dir==4) x=x-1;END
IF ((get_pixel(x,y)==235) or (get_pixel(x,y)==79) or (get_pixel(x,y)==225) or (get_pixel(x,y)==208) or (get_pixel(x,y)==217) or (get_pixel(x,y)==133)) salir=1;END
IF ((x>319) OR (x<0)) salir=1;END
IF ((y>199) OR (y<0)) salir=1;END
put_pixel(x,y,color);                //es la  intruci¢n  para  dibujarla estela
FRAME;
UNTIL (salir)
jug+=1;
jugador[tipo]=0;
END


//-----------------------------------------------------------------------------
// Proceso para mostrar los puntos de los jugadores
// Entradas: Ninguna
//-----------------------------------------------------------------------------

PROCESS marcador();
BEGIN
estado=0;
clear_screen();
delete_text(all_text);
fondo=load_map("user\tron\tron.map");
put_screen(0,fondo);
FRAME;
write(fuente1,160,30,4,"MARCADOR");
write(fuente1,100,50,4,"Jugadores");
write(fuente1,210,50,4,"Puntos");

write(fuente,100,70,4,"Jugador 1");
write(fuente,100,80,4,"Jugador 2");
write(fuente,100,90,4,"Jugador 3");
write(fuente,100,100,4,"Jugador 4");
write(fuente,100,110,4,"Equipo Azul");
write(fuente,100,120,4,"Equipo Rojo");

write_int(fuente,210,70,4,&jugador1);
write_int(fuente,210,80,4,&jugador2);
write_int(fuente,210,90,4,&jugador3);
write_int(fuente,210,100,4,&jugador4);
write_int(fuente,210,110,4,&equipo1);
write_int(fuente,210,120,4,&equipo2);

write(fuente1,160,150,4,"Pulsa ESPACIO para salir");
WHILE (NOT(key(_space)))
    FRAME;
END
delete_text(all_text);
unload_map(fondo);
estado=1;
END

//-----------------------------------------------------------------------------
// Proceso para dibujar los bordes de la pantalla
// Entradas: el  color del borde
//-----------------------------------------------------------------------------

PROCESS bordes(color);
BEGIN
FROM i=0 to 320;
    put_pixel(i,0,color);
    put_pixel(i,199,color);
END

FROM i=0 to 200;
    put_pixel(0,i,color);
    put_pixel(319,i,color);
END

END // END del final  del proceso
