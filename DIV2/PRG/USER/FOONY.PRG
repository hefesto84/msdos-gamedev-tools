//------------------------------------------------------------------------
// TITULO....:  FOONY
// AUTOR.....:  JAVIER PEREZ GARCIA
// E-MAIL....:  F.PEREZ2@CGAC.ES
// TFNO......:  942-218901
// FECHA.....:  20/2/98
// VERSION...:  1.3
//------------------------------------------------------------------------

program FOONY;

global

   posx[3]=320,477,320,162; // Posiciones x de los cuatro cuadros de juegos
   posy[3]=77,205,327,205;  // Posiciones y de los cuatro cuadros de juegos
   pasado[3]=0,0,0,0;      // Juegos pasados

   pas=0;        // Contador de veces que se consigue pasar un juego;
   fuente;       // Tipo de letra1
   fuente2;      // Tipo de letra2
   fuente3;      // Tipo de letra3
   fuente4;      // Tipo de letra4
   sn1,sn2,sn3,sn4,sn5,sn6,sn7,sn8,sn9,sn10,sn11,sn12,sn13;  // Sonidos
   fich;         // Tichero de imagenes
   vidas=5;      // Numero de vidas
   ptos=0;       // Varible para la puntuacion
   tiempo=0;     // Contador de tiempo
   rtn;          // Reduccion de tiempo inicial por nivel
   nivel=1;      // nivel en el que estamos
   sel;          // Numero de posicion de icono seleccionado en juego2
   dir;          // Direcion de iconos seleccionados en juego2
   tabla[215];   // Array para almacenar los iconos del juego2
   sele[4];      // Array para almacenar iconos a encontrar en juego2 y numeros en juego3
   tab[59];      // Array para almacenar numeros en juego3
   id_texto;     // Id de texto en pantalla
   id_sn;        // Id de canal de sonido
   f;            // Variable para for
   sw=0;         // Conmutador si entra en un if
   cont_jueg=0;  // Contador de juegos pasados
   lugar;        // Posicion de copi en los sombreros
   tot;          // Total procesos terminados

begin

    set_mode(m640x480);                                 // Modo 640x480
    fich=load_fpg('USER\FOONY\foony.fpg');         // Fichero graficos

    // Carga de letras utilizadas en los juegos

    fuente =load_fnt('USER\FOONY\foon4.fnt');
    fuente2=load_fnt('USER\FOONY\foon3.fnt');
    fuente3=load_fnt('USER\FOONY\foon2.fnt');
//    fuente4=load_fnt('USER\FOONY\tmp.fnt');

    // Carga de sonidos

    sn1=load_pcm('USER\FOONY\foongir1.pcm',0);
    sn2=load_pcm('USER\FOONY\foongir2.pcm',0);
    sn3=load_pcm('USER\FOONY\foongir3.pcm',0);
    sn4=load_pcm('USER\FOONY\foongir4.pcm',0);
    sn5=load_pcm('USER\FOONY\foonfoto.pcm',0);
    sn6=load_pcm('USER\FOONY\fooncur2.pcm',0);
    sn7=load_pcm('USER\FOONY\foonsele.pcm',0);
    sn8=load_pcm('USER\FOONY\foonok.pcm',0);
    sn9=load_pcm('USER\FOONY\foonno.pcm',0);
    sn10=load_pcm('USER\FOONY\foonbar.pcm',0);
    sn11=load_pcm('USER\FOONY\fooncur1.pcm',0);
    sn12=load_pcm('USER\FOONY\foonhomb.pcm',1);
    sn13=load_pcm('USER\FOONY\foontime.pcm',1);

    menu();  // Llamada al menu de eleccion de juego

end;


//------------------------------------------------------------------------
// PROCESO.............:  MENU
// DESCRIPCION.........:  Visualiza los iconos representando los posibles
//                        juegos y llama al proceso eleccion para elegir.
// PROCESOS QUE LLAMA..:  pantalla, eleccion
//------------------------------------------------------------------------

process menu()


begin

    let_me_alone();
    clear_screen();
    pas=0;
    fade_on();
    put_screen(0,5);


    // Textos de puntuacion y vidas

    write(fuente,110,420,0,'VIDAS:');
    write_int(fuente2,170,410,0,&vidas);
    write(fuente,240,420,0,'NIVEL:');
    write_int(fuente2,310,410,0,&nivel);
    write(fuente,360,420,0,'PUNTOS:');
    write_int(fuente2,460,410,0,&ptos);
    write(fuente4,40,20,0,"F1  - AYUDA");
    write(fuente4,600,40,2,"ALT+X - SALIR");
    write(fuente4,40,40,0,"F2 - CREDITOS");

    cont_jueg=0;

    if (vidas>0)                 // Si le quedan vidas puede elegir otro juego

        for (f=0;f<=3;f++)       // Cuento el numero de juegos pasados

            if (pasado[f]==1)

                cont_jueg++;

            end;

        end;

        if (cont_jueg==4)        // Si ha pasado los 4 juegos...

            vidas++;             // aumentar una vida

            for (f=0;f<=3;f++)   // Cuento el numero de juegos pasados

                pasado[f]=0;     // Borra los juegos pasados

            end;

            aumento_de_nivel();   // mensaje en pantalla

        else

            set_fps(10,0);   // Que se ande despacio para el movimiento de eleccion

            for (f=0;f<=3;f++)

                pantalla(f+1,posx[f],posy[f]);     // Visualiza los 4 posibles juegos

                if (pasado[f]==1)                  // Si el juego esta pasado...

                    pantalla(66,posx[f],posy[f]);  // ...Pongo signo encima

                end;

            end;

            eleccion();  // Eleccion de juego

        end;

    else

        fin();

    end;

end;



//------------------------------------------------------------------------
// PROCESO.............:  PANTALLA
// DESCRIPCION.........:  Visualiza el grafico pasado en la posicion indi-
//                        cada.
// ENTRADAS............:  Graph : Grafico a visualizar
//                        x     : Coordenada de columna
//                        y     : Coordenada de fila
//------------------------------------------------------------------------

process pantalla(graph,x,y)

begin

    loop

        frame;

    end;

end;



//------------------------------------------------------------------------
// PROCESO.............:  ELECCION
// DESCRIPCION.........:  Mueve recuadro alrededor de los juegos hasta que
//                        que se elija uno.
// PROCESOS QUE LLAMA..:  Parpadeo
//------------------------------------------------------------------------

process eleccion()

private

    i;             // Variable para for

begin

    graph=50;
    x=posx[0];     // Coge las posiciones establecidas para los iconos de juegos
    y=posy[0];     // Coge las posiciones establecidas para los iconos de juegos

    if (cont_jueg<=2)

        repeat


            frame;

        until(key(_space)==false);    // Esperar no este pulsado el espacio

        repeat

            for (i=0;i<=3 and not key(_space) and not mouse.left;i++) // Se sale con espacio


                if (key(_F1))         // Si pide ayuda

                    ayuda(0);

                end;

                if (key(_F2))         // Creditos

                    ayuda(5);

                end;


                if (pasado[i]==1)

                    continue;

                end;

                x=posx[i];            // Movemos el cuadro
                y=posy[i];            // Movemos el cuadro
                sound(sn11,128,256);  // Sonido
                frame;

            end;

        until (key(_space) or mouse.left);  // Cuando se pulsa dispar se acaba la eleccion

    else                      // Solo queda un juego


        for (i=0;i<=3;i++)    // Buscar el juego que queda

                if (pasado[i]==0)

                    x=posx[i];            // Visualizar el cuadro
                    y=posy[i];            // Visualizar el cuadro
                    frame;
                    break;

                end;

        end;

        timer[4]=0;

        repeat

            frame;

        until (timer[4]>40)   // Pausa

        i=4;

    end;

    sound(sn7,128,256);  // Sonido
    i--;

    while (pasado[i]!=0)  // por si pulsa espacio y la f no valida por ser un juego ya pasado

        i--;
        sw=1;

        if (f<0)

                f=3;
        end;

    end;


    parpadeo(x,y,i+1);      // Parpadea el cuadro de seleccion

end;


//------------------------------------------------------------------------
// PROCESO.............:  PARPADEO
// DESCRIPCION.........:  Parpadea unos segundos el cuadro de seleccion
//                        y lo manda al juego apropiado.
// ENTRADAS............:  x     : Coordenada de columna
//                        y     : Coordenada de fila
//                        k     : Numero de Juego elegido
// PROCESOS QUE LLAMA..:  Juego1, Juego2, Juego3, Juego4
//------------------------------------------------------------------------

process parpadeo(x,y,k)

begin

    // cuatro parpadeos

    for (f=0;f<=2;f++)

       graph=50;              // Mostrar cursor
       timer[4]=0;

       repeat

        frame;

       until (timer[4]>=40);

       graph=51;              // Borrar cursor
       timer[4]=0;

       repeat

        frame;

       until (timer[4]>=40);

    end;

    set_fps(70,0);            // 70 imagenes
    let_me_alone();           // Eliminar todos los procesos activos
    delete_text(all_text);    // Borrar todos los textos activos

    switch (k)                // Dependiendo de k mandar a juego diferente

        case 1:

            juego1();         // Juego 1 : Fotografia

        end;

        case 2:

            juego2();         // Juego 2 : Iconos

        end;

        case 3:

            juego3();         // Juego 3 : Numeros

        end;

        case 4:

            juego4();         // Juego 4 : Sombreros

        end;

     end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  FIN
// DESCRIPCION.........:  Visualiza pantalla de fin de juego.
// PROCESOS QUE LLAMA..:  Boton, raton
//------------------------------------------------------------------------

Process fin()

begin

    sound(sn9,108,400);  // Sonido
    x=320;
    y=240;
    graph=500;
    raton();             // Mostrar puntero de raton

    boton(0);            // Mostrar boton de aceptar

    write(fuente4,320,340,4,"ACEPTAR");
    write (fuente4,320,130,1,"J U E G O    T E R M I N A D O");
    write (fuente4,320,190,1,"  Se te han acabado las vidas");
    sound(sn9,108,400);  // Sonido

    repeat

        frame;           // Mostrar ventana

    until (key(_enter) or key(_a) or key(_A) or (mouse.x>243 and mouse.x<397 and mouse.y>316 and mouse.y<364 and mouse.left))

    fade_off();
    exit ("Gracias por jugar",0);

end;


//------------------------------------------------------------------------
// PROCESO.............:  BOTON
// DESCRIPCION.........:  Pone en pantalla un boton
// ENTRADAS............:  opcion : si es cero el boton esta desapretado
//                                 si es uno el boton esta apretado
//------------------------------------------------------------------------

process boton(opcion)

begin

    z=-15;
    x=320;
    y=340;
    graph=501;

    if (opcion==1)          // Boton apretado


        angle+=180000;      // Giro el boton hasta darle la vuelta

    end;

    loop

        frame;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  RATON
// DESCRIPCION.........:  Activa en pantalla el raton
//------------------------------------------------------------------------

process raton()

begin

    z=-500;
    graph=502;

    loop

        x=mouse.x;      // mover puntero con movimiento de raton
        y=mouse.y;      // mover puntero con movimiento de raton
        frame;

    end;

end;



//------------------------------------------------------------------------
// PROCESO.............:  AUMENTO_DE_NIVEL
// DESCRIPCION.........:  Visualiza pantalla de aumento de nivel.
// PROCESOS QUE LLAMA..:  Boton, raton
//------------------------------------------------------------------------

process aumento_de_nivel()
private

    t1,t2,t3;           // identificadores de texto
    id_b;               // identificador de proceso

begin

    sound(sn8,108,506); // Sonido
    x=320;
    y=240;
    graph=500;

    raton();

    id_b=boton(0);

    t1=write (fuente4,320,130,1,"L O   H A S   C O N S E G U I D O");

    switch (nivel)

        case 1:         // Nivel 1

            t2=write (fuente4,320,190,1,"Pero eso no es nada, ahora veras");

        end;

        case 2:         // Nivel 2

            t2=write (fuente4,320,190,1,"    Pero ahora vas a flipar");

        end;

        case 3:         // Nivel 3

            t2=write (fuente4,320,190,1,"  Pero dudo que salgas de esta");

        end;

        default:        // Nivel 4 o mas

            t2=write (fuente4,320,190,1,"Si la pasas te devuelvo el importe");

        end;


    end;

    t3=write (fuente4,320,340,4,"ACEPTAR");
    sound(sn8,108,506);  // Sonido

    repeat

        frame;

    until (key(_enter) or key(_a) or key(_A) or (mouse.x>243 and mouse.x<397 and mouse.y>316 and mouse.y<364 and mouse.left))

    nivel++;             // aumentar el nivel
    fade_off();


    // Borrar textos

    delete_text(t1);
    delete_text(t2);
    delete_text(t3);
    signal(id_b, s_kill);
    menu();

end;




//------------------------------------------------------------------------
// PROCESO.............:  JUEGO1 : fotografiando a un corredor
// DESCRIPCION.........:  Pone en pantalla elementos de juego: fondo,
//                        corredor y objetivo de la camara.
// PROCESOS QUE LLAMA..:  keko, objetivo
// COMENTARIOS.........:  En el juego uno se hace uso de la funcion
//                        Map_block_copy para poder hacer la foto
//------------------------------------------------------------------------

process juego1()

begin

    clear_screen();
    put(fich,64,320,200);   // Fondo de nubes
    put(fich,61,320,200);   // Fondo de edificios


    // Textos de puntuacion y vidas

    write(fuente,150,420,0,'VIDAS:');
    write(fuente,300,420,0,'PUNTOS:');
    write_int(fuente2,215,410,0,&vidas);
    write_int(fuente2,400,410,0,&ptos);
    write(fuente,287,10,0,'ACIERTOS');
    write_int(fuente2,320,48,4,&pas);
    write(fuente4,40,40,0,"F1 - AYUDA");


    define_region(1,320-75,240-75,150,150);  // Region valida para la foto

    set_fps(50,1);                // Velocidad 50 frames
    keko();                       // Proceso de hombre corriendo
    objetivo();                   // Proceso de Objetivo de la camara

end;


//------------------------------------------------------------------------
// PROCESO.............:  OBJETIVO
// DESCRIPCION.........:  Visualiza el objetivo de la camara
//------------------------------------------------------------------------

process objetivo()

begin

    graph=62;
    x=320;
    y=220;

    loop

        frame;

    end;

end;



//------------------------------------------------------------------------
// PROCESO.............:  KEKO
// DESCRIPCION.........:  Dibuja a hombre corriendo por pantalla y espera
//                        a que se active el disparo para llamar a foto.
// PROCESOS QUE LLAMA..:  Foto
//------------------------------------------------------------------------

process keko()

begin

    graph=100;
    x=0;
    y=200;

    // Movimiento de hombre recorriendo la pantalla

    id_sn=sound(sn12,20,477);               // Sonido

    repeat

    // Corredor mas rapido segun nivel

        for(x=1;x<=640 and not(mouse.left) and not key(_space);x+=23+nivel*2)

            if (key(_f1))                   // Pulsa ayuda

                stop_sound(id_sn);
                ayuda(1);

            end;

            frame;
            graph++;

            if( graph>105)  // Si llega al ultimo grafico ...

                graph=100;  // ... Activo el primero

            end;

         end;

    until (mouse.left or key(_space))      // se dispara la foto

    stop_sound(id_sn);
    foto(x,y,graph);        // proceso que visualiza la fotografia

end;


//------------------------------------------------------------------------
// PROCESO.............:  FOTO
// DESCRIPCION.........:  Saca la foto con lo que se encontraba en el
//                        sitio en el momento de hacerla.
// ENTRADAS............:  px    : Coordenada x donde estaba el hombre
//                        py    : Coordenada y donde estaba el hombre
//                        ng    : Numero de grafico fotografiado
// PROCESOS QUE LLAMA..:  okno
//------------------------------------------------------------------------


process foto(px,py,ng);

private

    ancho;        // Ancho de grafico de hombre fotografiado

begin


    let_me_alone();    // Parar todo
    region=1;          // La foto solo salga en esta region
    graph=63;

    // Copio a la foto un trozo de fondo de nubes

    map_block_copy(fich,63,10,10,64,320-75+10,120-70+10,130,130);

    // Copio a la foto un trozo de fondo de edificios

    map_block_copy(fich,63,10,10,61,320-75+10,120-70+10,130,130);

    ancho = graphic_info(fich, ng, g_wide);  // Mirar ancho de hombre

    // Copio a la foto la imagen de ese momento de hombre corriendo
    // en la posicion correcta dentro de la foto

    map_block_copy(fich,63,(px-245-ancho),30,ng,0,0,70,90);

    x=245;
    set_fps(35,0);                           // 35 frames

    sound(sn5,256,510);                      // Sonido

    for (y=240-75;y<=240+75;y++)             // Bajo la foto lentamente

        sound(sn5,80,510);                   // Sonido
        frame;

    end;

    if (((px-ancho)>245)and(px<395))         // Si ha acertado

         sound(sn8,128,256);                 // Sonido
         okno(66,px);

    else                                     // Si ha fallado

         sound(sn9,128,256);                 // Sonido
         okno(65,px);

    end;

    loop

        frame;

    end;

end;



//------------------------------------------------------------------------
// PROCESO.............:  OKNO
// DESCRIPCION.........:  Dibujo el simbolo correspondiente (un circulo si
//                        ha tenido exito y una equis si no). Si ha pasado
//                        el juego 3 veces vuelvo al menu, en caso contrario
//                        repito el juego.
// ENTRADAS............:  graph : Grafico de hombre que ha sido fotografiado
//                        px    : Coordenada x donde estaba el hombre
// PROCESOS QUE LLAMA..:  Juego1, menu
//------------------------------------------------------------------------

process okno(graph,px);

begin

    region=1;
    x=318;
    y=240;

    timer[4]=0;

    repeat

        frame;

    until(timer[4]>=180);     // Pausa

    let_me_alone();           // Elimina todos los procesos
    delete_text(all_text);    // Elimina todos los textos

    unload_fpg(fich);         // Descargo los graficos para borrar la foto
    fich=load_fpg('USER\FOONY\foony.fpg');   // vuelvo a cargar los graficos

    if (graph==66)            // Si ha acertado

            ptos+=200-abs((320-px));   // puntuacion dependiendo de mejor foto
            pas++;            // Aumento el numero de veces que ha pasado el juego

    else                      // Si ha fallado

            vidas--;          // Pierde una vida

    end;

    repeat


        frame;

    until(key(_space)==false); //  Esperar no este pulsado el espacio

    if (pas<=2 and vidas>0)   // Si no lo ha pasado 3 veces y le quedadan vidas

        juego1();             // repite el juego

    else

        if (vidas>0)

            pasado[0]=1;      // Activador de juego pasado

        end;

        menu();          // vuelve al menu

    end;

end;



//------------------------------------------------------------------------
// PROCESO.............:  JUEGO2 : Encontrando iconos en panel
// DESCRIPCION.........:  Pone en pantalla elementos de juego: iconos,
//                        clave a averiguar y llama al cursor de seleccion.
// PROCESOS QUE LLAMA..:  azar, arriba, visual, recuadro
// COMENTARIOS.........:  En el juego dos se utiliza una tabla unidimensional
//                        como si fuera bidimensional (aunque con esta
//                        hubiera sido mas facil)
//------------------------------------------------------------------------

process juego2();

begin

    // Textos de puntuacion, vidas y tiempo

    put_screen(0,67);
    write(fuente,419,10,0,'VIDAS');
    write(fuente,524,10,0,'PUNTOS');
    write_int(fuente2,442,48,4,&vidas);
    write_int(fuente2,552,48,4,&ptos);
    timer[4]=0;                       // Contador de tiempo
    rtn=400*nivel;                    // Descenso de tiempo inicial por nivel

    if (rtn>2400)                      // Dejar siempre algo de tiempo minimo

        rtn=2400;

    end;


    tiempo=(3400-rtn-timer[4])/100;  // Se desplaza en cuenta atras (menos tiempo cuanto mas nivel)
    write(fuente,37,10,0,'TIEMPO');
    write_int(fuente2,62,48,4,&tiempo);
    write(fuente,122,10,0,'ACIERTOS');
    write_int(fuente2,155,48,4,&pas);

    set_fps(70,0);                     // 70 imagenes
    azar();                            // Eleccion de iconos para pantalla
    arriba();                          // Eleccion de iconos a encontrar
    visual();                          // Visualiza iconos



    recuadro();     // Muestra y permite mover el cursor para buscar iconos


end;


//------------------------------------------------------------------------
// PROCESO.............:  AZAR
// DESCRIPCION.........:  Mete en un array los numeros correspondientes a
//                        iconos del panel grande del juego.
//------------------------------------------------------------------------

process azar()

begin

    for(f=0;f<=215;f++)

            tabla[f]=rand(8,18);   //  Numero a suertes
    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  ARRIBA
// DESCRIPCION.........:  Busca en el array de iconos llenada en el proce-
//                        dimiento Azar, un grupo de cinco en cualquier
//                        direccion y sin que se salga por los extremos
//                        y los mostramos en panel de arriba.
// PROCESOS QUE LLAMA..:  Casilla
//------------------------------------------------------------------------


process arriba()

begin

    sel=rand(0,215);   // Numero del que parto para seleccionar
    dir=rand(1,4);     // Direccion en la que coger iconos, (si se puede)

    if (((sel mod 18)>13) and (dir==1))  // No se puede a la derecha

        dir=2;

    end;

    if (((sel/18)>7) and (dir==2))       // No se puede abajo

        dir=3;

    end;

    if (((sel mod 18)<4) and (dir==3))   // No se puede a la izquierda

        dir=4;

    end;

    if (((sel/18)<4) and (dir==4))       // No se puede arriba

        dir=1;

    end;

    if (((sel mod 18)>14) and (dir==1))  // No se puede a la derecha (por si se cambia en la anterior)

        dir=2;

    end;

    // Ya se sabe la direccion en dir

    for (f=0;f<=4;f++)

        if (dir==1)

            sele[f]=tabla[sel+f];        // Seleccionar 5 iconos a la derecha

        else

            if (dir==3)

                sele[f]=tabla[sel-f];    // Seleccionar 5 iconos a la izquierda

            else

                if (dir==2)

                    sele[f]=tabla[sel+(f*18)];    // Seleccionar 5 iconos hacia abajo

                else

                    sele[f]=tabla[sel-(f*18)];    // Seleccionar 5 iconos hacia arriba

                end;

            end;

        end;


        pantalla(sele[f],f*32+241,40);      // Visualizar iconos en panel de arriba

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  VISUAL
// DESCRIPCION.........:  Visualiza el panel de abajo con todos los iconos
//------------------------------------------------------------------------

process visual()

begin

    for(f=0;f<=215;f++)

            pantalla(tabla[f],(f mod 18)*32+38,(f/18)*32+95);

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  RECUADRO
// DESCRIPCION.........:  Permite mover un recuadro por el panel de iconos
//                        y seleccionar icono.
// PROCESOS QUE LLAMA..:  Acierto
//------------------------------------------------------------------------

process recuadro()

private

    xx,yy;          // Variables intermedias para almacenar posicion antigua

begin

    graph=7;
    mouse.x=326;                         // Valor inicial raton x
    mouse.y=287;                         // Valor inicial raton y
    xx=x;
    yy=y;
    sw=0;

    loop

        if (key(_f1))                    // Si pulsa ayuda

            ayuda(2);

        end;

        tiempo=(3400-rtn-timer[4])/ 100; // Se desplaza en cuenta atras (menos tiempo cuanto mas nivel)

        if (tiempo<=3 and sw==0)

            id_sn=sound(sn13,128,96);        // Sonido indicador de poco tiempo
            sw=1;

        end;

        if (key(_right))                 // Si utiliza cursor a la derecha

            mouse.x+=32;

        end;

        if (key(_left))                 // Si utiliza cursor a la izquierda

            mouse.x-=32;

        end;


        if (mouse.x>581)                // No deja salirse por la derecha

            mouse.x=581;

        end;

        if (mouse.x<39)                 // No deja salirse por la izquierda

            mouse.x=39;

        end;

        // actualiza la posicion x con el raton, moviendose de icono en icono

        x=(mouse.x /32)*32+6;

        if (key(_down))                 // Si utiliza cursor hacia abajo

            mouse.y+=32;

        end;

        if (key(_up))                   // Si utiliza cursor hacia arriba

            mouse.y-=32;

        end;

        if (mouse.y>426)                 // No deja salirse por abajo

            mouse.y=421;

        end;

        if (mouse.y<75)                  // No deja salirse por arriba

            mouse.y=70;

        end;

        // Actualiza la posicion y con el raton, moviendose de icono en icono

        y=(mouse.y/32)*32+31;

        if (xx<>x or yy<>y)              // Si se ha movido el cursor

            sound(sn6,128,256);          // Sonido

        end;

        xx=x;
        yy=y;

        if(mouse.left or tiempo<=0 or key(_space)) // Si se acaba el tiempo o dispara

                stop_sound(id_sn);
                sound(sn7,128,256);       // Sonido
                acierto(x,y);             // Mirar si acierta

        end;

        frame;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  ACIERTO
// DESCRIPCION.........:  Comprueba si el lugar pulsado es valido respecto
//                        a la combinacion a buscar.
// ENTRADAS............:  x    : Coordenada x donde se ha pulsado
//                        y    : Coordenada y donde se ha pulsado
// PROCESOS QUE LLAMA..:  rcuadro, okno2, mostrar_donde, juego2, menu
//------------------------------------------------------------------------


process acierto(x,y)

private

   aciertos;                        // Contador de iconos acertados
   pos;                             // Posicion en el array

begin

    aciertos=0;

    // Calculo para sabiendo las coordenadas de pantalla, saber la posicion en el array

    pos=((((x-23)/32))+(((y-75)/32)*18));

    // Si se cumple se puede comprobar la derecha, ya que hay suficientes iconos

    if ((pos mod 18)<15)

        for (f=0;f<=4;f++)

            if(tabla[pos+f]==sele[f]) // Si es igual es correcto (coincide con la clave)

                 aciertos++;
                 rcuadro(7,x+(f*32),y);

            else

                 aciertos=0;          // En cuanto fallo lo pongo a cero y busco otra direccion
                 break;

            end;

        end;

     end;

    // Si se cumple se puede comprobar la izquierda, ya que hay suficientes iconos

    if (((pos mod 18)>3) and (aciertos<5))

        signal(type rcuadro,s_kill);  // Borra si se marco alguno sin llegar a 5 aciertos

        for (f=0;f<=4;f++)

            if(tabla[pos-f]==sele[f]) // Si es igual es correcto (coincide con la clave)

                 aciertos++;
                 rcuadro(7,x-(f*32),y);

            else

                aciertos=0;
                break;                // En cuanto fallo lo pongo a cero y busco otra direccion

            end;

        end;

     end;

    // Si se cumple se puede comprobar arriba, ya que hay suficientes iconos

    if (((pos / 18)>3) and (aciertos<5))

        signal(type rcuadro,s_kill);        // Borra si se marco alguno sin llegar a 5 aciertos

        for (f=0;f<=4;f++)

            if(tabla[pos-(f*18)]==sele[f])  // Si es igual es correcto (coincide con la clave)

                 aciertos++;
                 rcuadro(7,x,y-(f*32));     // En cuanto fallo lo pongo a cero y busco otra direccion

            else

                 aciertos=0;
                 break;

            end;

        end;

     end;

    // Si se cumple se puede comprobar la abajo, ya que hay suficientes iconos

    if (((pos / 18)<8) and (aciertos<5))

        signal(type rcuadro,s_kill);        // Borra si se marco alguno sin llegar a 5 aciertos

        for (f=0;f<=4;f++)

            if(tabla[pos+(f*18)]==sele[f])  // Si es igual es correcto (coincide con la clave)

                 rcuadro(7,x,y+(f*32));
                 aciertos++;

            else

                 break;

            end;

        end;

     end;

     if (aciertos>=5)          // Si se ha acertado al completo

         sound(sn8,128,256);   // Sonido
         ptos+=100+tiempo;     // Mas puntos cuanto antes se resuelva
         pas++;
         okno2(66);            // Sacar simbolo de correcto

     else

         sound(sn9,128,256);   // Sonido
         vidas--;
         mostrar_donde();
         okno2(65);            // Sacar simbolo de no correcto

     end;

     signal(type recuadro,s_kill);   // Eliminar los recuadros

     timer[4]=0;

     repeat

        frame;

     until(timer[4]>=200);      // Pausa

     let_me_alone();
     delete_text(all_text);     // Borrar todos los textos

     if (pas<=2 and vidas>0)    // si quedan vidas y no se ha pasado dos veces

        juego2();               // repito el juego

     else

        if (vidas>0)

            pasado[1]=1;        // Activador de juego pasado

        end;

        menu();            // vuelvo al menu

     end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  OKNO2
// DESCRIPCION.........:  Dibujo el simbolo correspondiente (un circulo si
//                        ha tenido exito y una equis si no).
//------------------------------------------------------------------------

process okno2(graph)

begin

    x=318;
    y=240;
    z=-100;

    loop

        frame;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  RCUADRO
// DESCRIPCION.........:  Dibujo el Cursor usado en el juego en la casilla
//                        indicada.
// COMENTARIOS.........:  No utilizo el procedimiento pantalla ya que este
//                        debe ser eliminado en algun momento con el comando
//                        s_kill, mientras que el otro continua.
//------------------------------------------------------------------------

process rcuadro(graph,x,y)

begin

    loop

        frame;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  MOSTRAR_DONDE
// DESCRIPCION.........:  En caso de no acertar, muestra donde estaban las
//                        casillas a buscar.
// PROCESOS QUE LLAMA..:  rcuadro
//------------------------------------------------------------------------

process mostrar_donde()

begin

    for (f=0;f<=4;f++)

        if (dir==1)              // Derecha

            rcuadro(7,((sel+f) mod 18)*32+38,((sel+f)/18)*32+95);

        else

            if (dir==3)          // Izquierda

                rcuadro(7,((sel-f) mod 18)*32+38,((sel-f)/18)*32+95);

            else

                if (dir==2)      // Abajo

                    rcuadro(7,((sel+18*f) mod 18)*32+38,((sel+18*f)/18)*32+95);

                else             // Arriba

                    rcuadro(7,((sel-18*f) mod 18)*32+38,((sel-18*f)/18)*32+95);

                end;

            end;

        end;

    end;

end;



//------------------------------------------------------------------------
// PROCESO.............:  JUEGO3 : Encontrando el numero
// DESCRIPCION.........:  Pone en pantalla elementos de juego: textos,
//                        opciones de numeros y llama al movimiento y la
//                        eleccion.
// PROCESOS QUE LLAMA..:  elegir, mostrar_opciones, movimiento, elec
// COMENTARIOS.........:  En el juego tres se utiliza una tabla unidimen-
//                        sional como si fuera bidimensional (aunque con
//                        esta hubiera sido mas facil)
//------------------------------------------------------------------------

process juego3()

begin

    timer[4]=0;                             // Contador de tiempo
    rtn=100*nivel;                          // Descenso de tiempo inicial por nivel

    if (rtn>600)                            // Dejar siempre algo de tiempo minimo

        rtn=600;

    end;

    tiempo=(1000-rtn-timer[4])/100;         // Se desplaza en cuenta atras (menos tiempo cuanto mas nivel)

    // Textos de puntuacion, vidas y tiempo

    write(fuente,180,420,0,'VIDAS:');
    write(fuente,300,420,0,'PUNTOS:');
    write_int(fuente2,245,410,0,&vidas);
    write_int(fuente2,400,410,0,&ptos);
    write(fuente,445,15,0,'TIEMPO');
    write_int(fuente2,475,52,4,&tiempo);
    write(fuente,322,15,0,'ACIERTOS');
    write_int(fuente2,355,52,4,&pas);
    write(fuente4,90,450,1,"F1 - AYUDA");


    set_fps(15,0);                       // Para que se mueva lento
    elegir();                            // Elegir clave
    mostrar_opciones();                  // Mostrar todos los numeros
    movimiento();                        // Mover cursor
    elec();                              // Elegir opcion

end;


//------------------------------------------------------------------------
// PROCESO.............:  ELEGIR
// DESCRIPCION.........:  Escoge los numeros para la clave sin repetir
//                        ningun numero.
//------------------------------------------------------------------------

process elegir()

private

    t;             // Variable para for

begin

    for(f=0;f<=4;f++)

        repeat

            sw=0;
            sele[f]=rand(0,9);

            for(t=0;t<=(f-1);t++)    // Mirar si el numero ya esta

                if (sele[f]==sele[t])

                    sw=1;

                end;

            end;

        until (sw==0)                // Si no esta repetido vale

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  MOSTRAR_OPCIONES
// DESCRIPCION.........:  Elige los numeros para bloque de la izquierda
//                        e incluyo entre ellos el de la clave.
//------------------------------------------------------------------------

process mostrar_opciones()

private

    pos;       // Posicion donde colocar el numero correcto
    d;         // Variable para for
    t;         // Variable para for

begin

    pos=rand(0,11);         // Posicion de la clave entre los otros numeros

    for(f=0;f<=11;f++)      // Once filas

        for(d=0;d<=4;d++)   // Cinco columnas

            if (f==pos)     // Si es la posicion de la clave lo meto dentro

                tab[(f*5)+d]=sele[d];  // Es el que hay que averiguar

            else

                repeat      // Tampoco dejar que se repitan los numeros

                    sw=0;
                    tab[(f*5)+d]=rand(0,9);

                    for(t=0;t<=(d-1);t++)  // Mirar si el numero ya esta

                        if (tab[(f*5)+d]==tab[(f*5)+t])

                            sw=1;

                        end;

                    end;

                until (sw==0)  // Hasta que el numero no este repetido

            end;

            write_int(fuente2,(45+(d*30)),(41+(f*32)),4,&tab[(f*5)+d]);

        end;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  MOVIMIENTO
// DESCRIPCION.........:  Muestra la clave numero a numero a la derecha de
//                        la pantalla.
//------------------------------------------------------------------------

process movimiento()

private

    cont;        // Contador de numeros acertado

begin

    f=rand(0,4); // Para que el numero no comienze siempre por el primero, ya que en ese caso con fijarse en el primero vale

    loop


            timer[2]=0;
            id_texto=write_int(fuente3,408,200,4,&sele[f]);
            sound(sn10,128,256);    // Sonido

            repeat

                frame;

            until (
            timer[2]>8)      // Pausa para ver el numero

            delete_text(id_texto);
            f++;

            if (f>4)

                f=0;                // Que inicie el numero

            end;

     end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  ELEC
// DESCRIPCION.........:  Permite mover el cursor de seleccion para escoger
//                        numero. comprueba si es correcto y llama a cores-
//                        pondientes procesos.
// PROCESOS QUE LLAMA..:  Resultado, juego3, menu
//------------------------------------------------------------------------

process elec()

private

    cont;                                 // Contador  de numeros acertados

begin

    graph=150;
    x=105;
    y=44;
    sw=0;

    repeat

        if (key(_f1))

            ayuda(3);

        end;

        tiempo=(1000-rtn-timer[4])/ 100;  // Se desplaza en cuenta atras (menos tiempo cuanto mas nivel)

        if (tiempo<=3 and sw==0)

            id_sn=sound(sn13,128,96);     // Sonido indicador de poco tiempo
            sw=1;

        end;

        if (key(_down)or mouse.right)     // Abajo

            y+=32;

            if (y>396)      // Se pasa de abajo - arriba

                y=44;

            end;

        else

            if (key(_up)or mouse.left)   // Arriba

                y-=32;

                if (y<44)   // Se pasa de arriba - abajo

                    y=396;

                end;

            end;

        end;

        frame;

     until(key(_space) or tiempo<=0);  // Acabar cuando pulse o se acabe el tiempo

     stop_sound(id_sn);
     sound(sn7,128,256);      // Sonido
     cont=0;

     for (f=0;f<=4;f++)       // Contar el numero de aciertos

        if (sele[f]==tab[(((y-44)/32)*5)+f]) //si el numero es correcto

            cont++;

        else

            break;            // Si falla uno no seguir contando

        end;

     end;

    if (cont>=5)              // Si le ha acertado por completo

         sound(sn8,128,256);  // Sonido
         ptos+=100+tiempo;    // Mas puntos cuanto antes se resuelva         pas++;
         pas++;

     else

         sound(sn9,128,256);  // Sonido
         vidas--;

     end;

     delete_text(id_texto);            // Borrar texto
     signal(type movimiento,s_kill);   // Parar el movimiento
     resultado();                      // Separar las letras para ver el resultado

     timer[3]=0;

     repeat

        frame;

     until(timer[3]>=550);             // Pausa

     let_me_alone();
     delete_text(all_text);            // Borrar todos los textos

     if (pas<=2 and vidas>0)           // Si no se ha pasado 3 veces y quedan vidas

        juego3();                      // Repito el juego

     else

        if (vidas>0)

            pasado[2]=1;               // Activador de juego pasado

        end;

        menu();                        // Vuelvo al menu de juegos

     end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  RESULTADO
// DESCRIPCION.........:  Separa las cifras de la clave para mostrar el
//                        resultado
//------------------------------------------------------------------------

process resultado()

private

    t;               // Variable para for
    id_tex[4];       // Array con identificadores de textos

begin

        for(f=0;f<=4;f++)

            id_tex[f]=write_int(fuente3,408,200,4,&sele[f]); // Escribir los numeros unos encima de otros

        end;

        for(t=0;t<=89;t+=3)

            for(f=0;f<=1;f+=1)

                     // Separar progresivamente

                move_text(id_tex[f],408-(2-f)*t,200);   // Separa las letras a la izquierda de forma porgresiva
                move_text(id_tex[f+3],408+(f+1)*t,200); // Separa las letras a la derecha de forma porgresiva
                frame;

            end;

        end;

        loop

            frame;

        end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  JUEGO4 : Encontrando a Copito el peluche
// DESCRIPCION.........:  Pone en pantalla elementos de juego: sombreros,
//                        copi y baraja.
// PROCESOS QUE LLAMA..:  Copi, Sombreros, barajar
// COMENTARIOS.........:  En el juego tres se utiliza para barajar los
//                        Sombreros la varible angle, pero sin girar el
//                        grafico; y avance para que vayan de un lado a
//                        otro. Hasta ahora para realizar este movimiento
//                        siempre habia utilizado senos y cosenos.
//------------------------------------------------------------------------

process juego4()

begin


      put_screen(0,201);             // Fondo

      // Textos en pantalla

      write(fuente,400,10,0,'VIDAS:');
      write(fuente,500,10,0,'PUNTOS:');
      write_int(fuente2,427,48,4,&vidas);
      write_int(fuente2,530,48,4,&ptos);
      write(fuente,122,10,0,'ACIERTOS');
      write_int(fuente2,155,48,4,&pas);
      write(fuente4,320,450,1,"F1 - AYUDA");

      set_fps(40,0);        // Poner lento de nuevo despues de barajar

      lugar=rand(0,2);      // Colocacion del peluche
      copi(lugar*200+125);  // Peluche
      tot=0;                // Total sombreros que han llegado abajo

      for(f=120;f<=521;f+=200)       // crear tres sombreros en posicion

        sombreros(f,rand(-10,50));   // Cada sombrero empieza en altura diferente

      end;

      loop

          if (tot==3)       // Si ya han llegado todos abajo

            timer[4]=0;

            repeat

                frame;

            until (timer[4]>60);           // Pausa

            signal(type copi,s_kill);       // Elimino al peluche
            signal(type sombreros,s_kill);  // Elimino los sombreros
            barajar();                      // Barajo los sombreros

           end;

           frame;

      end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  COPI
// DESCRIPCION.........:  Muestra al peluche en una posicion concreta
// ENTRADAS............:  x    : posicion x donde se coloca al peluche                        y    : Coordenada y donde se ha pulsado
//------------------------------------------------------------------------

process copi(x)

begin

    y=260;
    z=250;                   // Que quede por detras de los sombreros
    graph=251;

    loop

        for (f=1;f<=8;f++)   // Para no cambiar de grafico muy rapido

            frame;

        end;

        graph++;             // Cambiar grafico

        if (graph>254)       // Si he llegado al ultimo grafico...

            graph=251;       // ...Pongo el primero

        end;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  SOMBREROS
// DESCRIPCION.........:  Bajamos el sombrero, rebotando un poco abajo
// ENTRADAS............:  x    : Coordenada x del sombrero
//                        y    : Coordenada y inicial del sombrero
//------------------------------------------------------------------------

process sombreros(x,y)

private

    reb; // Variable para contar los rebotes de los sombreros al caer

begin

    graph=200;
    reb=0;

    for (y=y;reb<=3;y+=5)  // Hasta que rebote 4 veces

        frame;

        if (key(_f1))      // Si se pulsa ayuda

            ayuda(4);

        end;

        if (y>=180)        // Lega abajo

            reb++;

            for(y=y;(y>=(150+(reb*10)));y-=4)  // Subir, cada rebote menos

                frame;

            end;

        end;

    end;

    tot++;                 // Sombrero llego abajo

    loop

        frame;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  BARAJAR
// DESCRIPCION.........:  Realiza los movimientos de los sombreros
// PROCESOS QUE LLAMA..:  mov_somb_derarr, mov_somb_izqaba, mov_somb_deraba
//                        mov_somb_izqarr, ele
//------------------------------------------------------------------------

process barajar()

private

    s1,s2;    // Variables que almacenan los sombreros a intercambiar
    dire;     // Sentido de intercambio
    tmp;      // Variable temporal por si se necesita intercambiar s1 y s2
    px;       // Posicion x de sombrero a mover

begin

     set_fps(8000,1000);    // Que se muevan rapido
     let_me_alone();        // Eliminar el resto

     for (f=0;f<=12+rand(1,8);f++)  // Barajar un numero de veces

        dire=rand(0,1);     // Sentido en el que girar
        s1=rand(0,2);       // Primer sombrero a mover

        repeat

            s2=rand(0,2);   // Segundo Sombrero a mover

        until (s2<>s1);     // Tienen que ser sombreros diferentes

        // Poner siempre el menor en s1

        if (s1>s2)

            tmp=s1;         // Intercambiar
            s1=s2;
            s2=tmp;

        end;

        if (s1==lugar)      // Si se mueve el sombrero con copi...

            lugar=s2;       // ...cambio posicion

        else

            if (s2==lugar)  // Si se mueve el sombrero con copi...

                lugar=s1;   // ...cambio posicion

            end;

        end;

        tot=0;              // Numero de sombreros moviendose

        if (dire==0)        // Si la direccion es 0

            mov_somb_derarr(s1,s2-s1);   // Un sombrero a la derecha y arriba
            mov_somb_izqaba(s2,s2-s1);   // Un sombrero a la izquierda y abajo

        else

            mov_somb_deraba(s1,s2-s1);   // Un sombrero a la derecha y abajo
            mov_somb_izqarr(s2,s2-s1);   // Un sombrero a la izquierda y arriba

        end;

        most_somb(3-(s1+s2)); // Forma de calcular el sombrero que no se mueve y mostrarlo

        repeat

            frame;

            if (key(_f1))     // Si pulsa ayuda

                ayuda(4);

            end;

        until (tot==1);       // Se terminen de mover los sombreros

        signal(type most_somb,s_kill);  // Eliminar sombreros

     end;

    most_somb(0);            // Mostrar los tres sombreros en su posicion
    most_somb(1);            // Mostrar los tres sombreros en su posicion
    most_somb(2);            // Mostrar los tres sombreros en su posicion
    ele();                   // Elegir sombrero

end;


//------------------------------------------------------------------------
// PROCESO.............:  MOV_SOMB_DERARR
// DESCRIPCION.........:  Avanza sombrero en curva hasta destino, hacia
//                        la derecha y arriba.
// ENTRADAS............:  s1 :     Numero de sombrero a mover
//                        dis:     Numero de distancia a la que se tiene
//                                 que desplazar (1- sombrero cercano,
//                                 2- Sombrero lejano)
//------------------------------------------------------------------------

process mov_somb_derarr(s1,dis)

private

    tangle;                // Almacen intermedio de angulo

begin

    graph=200;
    x=(s1)*200+120;        // Posicion inicial del sombrero
    y=180;
    sound(sn1,128,256);  // Sonido

    // Se mueven mas rapido cuanto mas nivel

    for (angle=90000;angle>=-90000;angle-=11000+nivel*1000)

        advance((20+(2*nivel))*dis);
        tangle=angle;      // Almacenar el angulo
        angle=0;           // Poner el angulo a cero para que no gire el sombrero
        frame;
        angle=tangle;      // Vuelvo a poner el angulo, para que siga girando

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  MOV_SOMB_IZQABA
// DESCRIPCION.........:  Avanza sombrero en curva hasta destino, hacia
//                        la izqyuierda y abajo.
// ENTRADAS............:  s2 :     Numero de sombrero a mover
//                        dis:     Numero de distancia a la que se tiene
//                                 que desplazar (1- sombrero cercano,
//                                 2- Sombrero lejano)
//------------------------------------------------------------------------

process mov_somb_izqaba(s2,dis)

private

    tangle;               // Almacen intermedio de angulo

begin

    graph=200;
    x=(s2)*200+120;       // Posicion inicial del sombrero
    y=180;
    sound(sn2,128,256);  // Sonido

    // Se mueven mas rapido cuanto mas nivel

    for (angle=270000;angle>=90000;angle-=11000+nivel*1000)

        advance((20+(2*nivel))*dis);
        tangle=angle;      // Almacenar el angulo
        angle=0;           // Poner el angulo a cero para que no gire el sombrero
        frame;
        angle=tangle;      // Vuelvo a poner el angulo, para que siga girando

    end;
    tot=1;                 // Movimiento terminado

end;

//------------------------------------------------------------------------
// PROCESO.............:  MOV_SOMB_DERABA
// DESCRIPCION.........:  Avanza sombrero en curva hasta destino, hacia
//                        la derecha y abajo.
// ENTRADAS............:  s1 :     Numero de sombrero a mover
//                        dis:     Numero de distancia a la que se tiene
//                                 que desplazar (1- sombrero cercano,
//                                 2- Sombrero lejano)
//------------------------------------------------------------------------

process mov_somb_deraba(s1,dis)

private

    tangle;                  // Almacen intermedio de angulo

begin

    graph=200;
    x=(s1)*200+120;          // Posicion inicial del sombrero
    y=180;
    sound(sn3,128,256);  // Sonido

    // Se mueven mas rapido cuanto mas nivel

    for (angle=270000;angle<=450000;angle+=11000+nivel*1000)

        advance((20+(2*nivel))*dis);
        tangle=angle;      // Almacenar el angulo
        angle=0;           // Poner el angulo a cero para que no gire el sombrero
        frame;
        angle=tangle;      // Vuelvo a poner el angulo, para que siga girando

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  MOV_SOMB_IZQARR
// DESCRIPCION.........:  Avanza sombrero en curva hasta destino, hacia
//                        la derecha y arriba.
// ENTRADAS............:  s2 :     Numero de sombrero a mover
//                        dis:     Numero de distancia a la que se tiene
//                                 que desplazar (1- sombrero cercano,
//                                 2- Sombrero lejano)
//------------------------------------------------------------------------

process mov_somb_izqarr(s2,dis)

private

    tangle;                    // Almacen intermedio de angulo

begin

    graph=200;
    x=(s2)*200+120;            // Posicion inicial del sombrero
    y=180;
    sound(sn4,128,256);  // Sonido

    // Se mueven mas rapido cuanto mas nivel

    for (angle=90000;angle<=270000;angle+=11000+nivel*1000)

        advance((20+(2*nivel))*dis);
        tangle=angle;      // Almacenar el angulo
        angle=0;           // Poner el angulo a cero para que no gire el sombrero
        frame;
        angle=tangle;      // Vuelvo a poner el angulo, para que siga girando

    end;

    tot=1;                 // Movimiento terminado
end;



//------------------------------------------------------------------------
// PROCESO.............:  MOST_SOMB
// DESCRIPCION.........:  Mostrar el sombrero especificado
// ENTRADAS............:  s3 :     Numero de sombrero a mostrar
//------------------------------------------------------------------------

process most_somb(s3)

begin

    graph=200;
    x=s3*200+120;         // Posicion del sombrero
    y=180;

    loop

        frame;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  ELE
// DESCRIPCION.........:  Permite mover el cursor y seleccionar sombrero
//------------------------------------------------------------------------

process ele()

begin

    graph=260;
    x=320;
    y=350;
    set_fps(8,0);                 // Lo pongo lento de nuevo despues de barajar

    repeat

       if (key(_f1))

            ayuda(4);

       end;

       if (key(_right) or mouse.right)           // Derecha

            sound(sn6,128,256);  // Sonido
            x+=200;

            if (x>520)            // Si nos pasamos por la derecha aparecer izquierda

                x=120;

            end;

       else

        if (key(_left) or mouse.left)           // Izquierda

            sound(sn6,128,256);  // Sonido
            x-=200;

            if (x<120)            // Si nos pasamos por la izquierda aparecer derecha

                x=520;

            end;

        end;

       end;

       frame;

    until(key(_space));           // Disparador

    sound(sn7,128,256);           // Sonido
    set_fps(35,0);                // Poner un poco mas rapido

    signal(type most_somb,s_kill);// Eliminar sombreros

    if (lugar==(x-320)/200+1)     // Si se acierta

         sound(sn8,128,256);  // Sonido
         ptos+=140;
         pas++;

    else

         sound(sn9,128,256);  // Sonido
         vidas--;

    end;

    for(f=120;f<=521;f+=200)      // Posiciones de los 3 sombreros

        subir_sombreros(f);       // Subir los sombreros

    end;

    copi(lugar*200+125);          // Mostrar el peluche

    loop

        frame;

    end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  SUBIR_SOMBREROS
// DESCRIPCION.........:  Alza los sombreros para ver el peluche.
// ENTRADAS............:  x :     posicion de sombrero a mover
// PROCESOS QUE LLAMA..:  Juego4, menu
//------------------------------------------------------------------------

process subir_sombreros(x)

begin

     graph=200;

     for (y=180;y>=1;y-=5)        // Subir

        if (key(_f1))             // Si se pulsa ayuda

            ayuda(4);

        end;

        frame;

     end;

     timer[4]=0;

     repeat

        frame;

     until(timer[4]>=180);        // Pausa


     let_me_alone();              // Eliminar todos los procesos
     delete_text(all_text);       // Borrar textos

     if (pas<=2 and vidas>0)      // Si quedan vidas y no se ha pasado 3 veces

        juego4();                 // Repetir juego

     else

        if (vidas>0)

            pasado[3]=1;          // Activador de juego pasado

        end;

        menu();                   // Volver al menu

     end;

end;


//------------------------------------------------------------------------
// PROCESO.............:  AYUDA
// DESCRIPCION.........:  Muestra ventana interactiva de ayuda.
// ENTRADAS............:  ca :    Numero de texto a mostrar en ventana
// PROCESOS QUE LLAMA..:  Raton, boton
//------------------------------------------------------------------------

process ayuda(ca)

private

id_b;                     // Identificador de proceso boton
t1,t2,t3,t4,t5,t6,t7,t8;  // Identificadores de textos
tiem;                     // Variable para almacenar el tiempo que queda
d;                        // Variable para for

begin

    tiem=timer[4];         // para que no se pierda tiempo en la ayuda
    x=320;
    y=230;
    z=-10;
    graph=500;
    size=109;

    if (ca==3)

        delete_text(all_text);   // borrar todos los textos, sin se quedan por encima de la ventana

        // Volver a sacar los textos de puntuacion, aciertos, vidas y tiempo

        write(fuente,180,420,0,'VIDAS:');
        write(fuente,300,420,0,'PUNTOS:');
        write_int(fuente2,245,410,0,&vidas);
        write_int(fuente2,400,410,0,&ptos);
        write(fuente,445,15,0,'TIEMPO');
        write_int(fuente2,475,52,4,&tiempo);
        write(fuente,322,15,0,'ACIERTOS');
        write_int(fuente2,355,52,4,&pas);
        write(fuente4,90,450,1,"F1 - AYUDA");

    end;

    raton();

    id_b=boton(0);

    if (ca==5)

        t1=write (fuente4,320,90,1,"CREDITOS");

    else

        t1=write (fuente4,320,90,1,"AYUDA");

    end;

    switch (ca)

        case 0:         // menu

            signal(type eleccion, s_sleep);
            t2=write (fuente4,320,180,1,"Pulsa el espacio o bot¢n izquierdo");
            t3=write (fuente4,320,220,1,"del rat¢n para seleccionar juego");

        end;

        case 1:         // juego1

            signal(type keko, s_sleep);
            t2=write (fuente4,320,135,1,"Realiza 3 buenas fotos al corredor");
            t3=write (fuente4,320,165,1,"para pasar el Juego");
            t4=write (fuente4,320,210,1,"Espacio o bot¢n izquierdo de rat¢n");
            t5=write (fuente4,320,240,1,"para hacer la foto");

        end;

        case 2:         // juego2

            signal(type recuadro, s_sleep);
            t2=write (fuente4,320,125,1,"Encontrar la combinaci¢n requerida (en");
            t3=write (fuente4,320,155,1,"el panel superior) en cualquier sentido");
            t4=write (fuente4,320,185,1,"no diagonal, dentro del panel inferior.");
            t5=write (fuente4,320,215,1,"Usa el rat¢n o los cursores para moverte");
            t6=write (fuente4,320,245,1,"y el espacio o bot¢n izq. para seleccio-");
            t7=write (fuente4,320,275,1,"nar. Hay que acertar 3 veces para pasar.");
            stop_sound(id_sn);  // Por si esta en los ultimos segundos

        end;

        case 3:         // juego3

            signal(type movimiento, s_sleep);
            signal(type elec, s_sleep);
            t2=write (fuente4,320,125,1,"Buscar en el panel de n£meros de la");
            t3=write (fuente4,320,155,1,"izquierda, la cifra que va apareciendo");
            t4=write (fuente4,320,185,1,"n£mero a n£mero a su derecha.");
            t5=write (fuente4,320,215,1,"Usa los botones del rat¢n o los cur-");
            t6=write (fuente4,320,245,1,"sores para moverte y el espacio para");
            t7=write (fuente4,320,275,1,"seleccionar. Hay que acertar 3 veces.");
            stop_sound(id_sn);  // Por si esta en los ultimos segundos

        end;

        case 4:         // juego4

            signal(type sombreros, s_sleep);
            signal(type barajar, s_sleep);
            signal(type ele, s_sleep);
            signal(type subir_sombreros ,s_sleep);

            t2=write (fuente4,320,125,1,"Fijar la vista en el sombrero con el");
            t3=write (fuente4,320,155,1,"peluche, para despues de barajados ");
            t4=write (fuente4,320,185,1,"adivinar donde se encuentra. Usa los");
            t5=write (fuente4,320,215,1,"botones del rat¢n o los cursores para");
            t6=write (fuente4,320,245,1,"moverte y el espacio para selecionar.");
            t7=write (fuente4,320,275,1,"Hay que acertar 3 veces para pasar.");

        end;

        case 5:         // Creditos

            signal(type eleccion, s_sleep);
            t2=write (fuente4,80,120,0,"PROGRAMADOR:");
            t3=write (fuente4,320,155,1,"Javier P‚rez Garc¡a (F.Perez2@cgac.es)");
            t4=write (fuente4,80,190,0,"PRUEBAS:");
            t5=write (fuente4,320,220,1,"Benjamin P‚rez");
            t6=write (fuente4,320,250,1,"Rodrigo Jimeno");
            t7=write (fuente4,320,280,1,"Cursos de Cit");

        end;

    end;

    t8=write (fuente4,320,340,4,"ACEPTAR");

    repeat

        frame;

    until (key(_enter) or key(_a) or key(_A) or (mouse.x>243 and mouse.x<397 and mouse.y>316 and mouse.y<364 and mouse.left))

    repeat

        frame;

    until (not mouse.left);              // Que no vuelva con raton apretado

    signal(type raton,s_kill);           // Eliminar el puntero

    // Borrar textos

    delete_text(t1);
    delete_text(t2);
    delete_text(t3);
    delete_text(t8);
    signal(id_b, s_kill);
    timer[4]=tiem;                        // Poner el tiempo de nuevo

    // Devolver el control a sus respectivos procesos

    switch (ca)

        case 0:                          // Menu

            signal(type eleccion, s_wakeup);

        end;

        case 1:                          // Juego1

            delete_text(t4);
            delete_text(t5);

            id_sn=sound(sn12,20,477);    // Volver a activar sonido
            signal(type keko, s_wakeup); // Devolver el control al proceso

        end;

        case 2:                          // Juego2

            delete_text(t4);
            delete_text(t5);
            delete_text(t6);
            delete_text(t7);

            sw=0;                            // Por si esta en los ultimos segundos
            signal(type recuadro, s_wakeup); // Devolver el control al proceso

        end;

        case 3:         // Juego3

            delete_text(t4);
            delete_text(t5);
            delete_text(t6);
            delete_text(t7);

            // Sacar de nuevo textos

            for(f=0;f<=11;f++)      // Once filas

                for(d=0;d<=4;d++)   // Cinco columnas

                    write_int(fuente2,(45+(d*30)),(41+(f*32)),4,&tab[(f*5)+d]);
                end;

            end;

            sw=0;                                // Por si esta en los ultimos segundos
            signal(type elec, s_wakeup);         // Devolver el control al proceso
            signal(type movimiento, s_wakeup);   // Devolver el control al proceso

        end;

        case 4:         // Juego4

            delete_text(t4);
            delete_text(t5);
            delete_text(t6);
            delete_text(t7);

            signal(type sombreros ,s_wakeup);     // Devolver el control al proceso
            signal(type barajar, s_wakeup);       // Devolver el control al proceso
            signal(type ele, s_wakeup);           // Devolver el control al proceso
            signal(type subir_sombreros, s_wakeup);  // Devolver el control al proceso

        end;

        case 5:          // Creditos

            delete_text(t4);
            delete_text(t5);
            delete_text(t6);
            delete_text(t7);
            signal(type eleccion, s_wakeup);

        end;

    end;

end;