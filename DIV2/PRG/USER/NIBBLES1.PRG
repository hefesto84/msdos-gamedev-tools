//----------------------------------------------------------
//TITULO:   GUSANOS_LOCOS
//AUTORES:  ENRIQUE Y CARLOS DE HUERTA MEZQUITA
//FECHA     20/02/98
//----------------------------------------------------------
// Programa para el Mini-Concurso Div
//                                   GameStudio
// Programadores Hermanos Huerta Mezquita
//
//      HallelujaH's Men  HHM

PROGRAM GUSANOS_LOCOS;

GLOBAL

    manzanasgeneral=14;
    volumengeneral=5;

    aprisa;
    aplauso;
    jon1;
    jon2;
    jon;
    jj1;
    j1;
    j2;
    RETORNO;
    ggg;
    dinero;
    corte1; //numero de cortes que ha sufrido el gusano
    corte2; //si hay mas de cinco se destruye
    cortemas1;
    cortemas2;
    cortes1;
    cortes2;
    teclasini[4]=44,45,75,77; //codigo de las teclas del juego
    busca_letra[100]=0,0,28,29,30,31,32,33,34,35,36,37,0,0,0,0,18,24,5,19,21,26,22,9,16,17,0,0,0,0,1,20,4,6,7,8,10,11,12,15,0,0,0,0,27,25,3,23,2,14,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,46,0,0,39,0,42,0,0,45;
    PARPA;

    fichero_grafico; //graficos del menu
    fichero_grafico2; //graficos para los records
    opcion=1; //opcion por el raton
    curop=1; //opcion por el teclado
    denuevo=0; //evita que una tecla se quede bloqueada
    omax; //numero maximo de opciones en el menu
    JUG=300; //numero de jugadores 300=1jugador 500=2jugadores

    estado_programa=1;

    fuentepuntos; //fuente para los puntos de los records
    alpha=0;
    ultimaletra;
    cuentaletra;
    nomb[40]; //contiene el nuevo record
    struct definitiva[10] //contiene la lista definitiva de records
        dni2[12];
        ptos2;
    end
    barco;
    struct listaentera[10] //contiene la lista antigua de records
        dni[12];
        ptos;
    end


    lapida []=16,80,91,90,89,-84,-85,-86,-87,88,87,86,85,84,83,82,81;
    nueva_paleta [255];
    pendulo [20]=0,-10000,-19000,-25000,-28000,-30000,-28000,-25000,-19000,-10000,0,10000,19000,25000,28000,30000,28000,25000,19000,10000;
    final1;
    final2;
    px1;
    px2;
    py1;
    py2;
    h [7]=0,-15,-25,-15,0,15,25,15;
    h1 [500];
    h2 [500];
    ppx1 [500];
    ppy1 [500];
    ppx2 [500];
    ppy2 [500];
    mmx [500];
    mmy [500];
    mmm [500];
    kkk [500];
    angulo1;
    angulo2;
    long1 = 10;
    long2 = 10;
    longmax; //longitud maxima
    avance = 65;
    avance2 = 65;
    alerta;
    music1;
    fuego;
    avion;
    bomba1;
    bomba2;
    choque;
    tragar;
    apples;
    stopmusic1;
    stopalert1;
    stopalert2;
    stopfuego1;
    stopfuego2;
    puntos1;
    puntos2;
    bomba=0;
    meteor=0;
    atila;
    atila2;
    radio;
    radio2;
    arrasa;
    app;
    fuente1;
    caso=0;
    arrasa3;
    ameteoro;
    ametx;
    amety;
    tiempo1;
    tiempo2;
    bomber;
    viento;
    theend;
    centauro;

    points;

BEGIN

    setup.master = volumengeneral;
    set_volume ();

    gun();
    repeat
        frame;
    until(theend==1)

    set_mode(m640x480);

    fichero_grafico=load_fpg ("USER\NIBBLES\menu.fpg");

    fade_on();
    put_screen(0,10);
    timer[2]=0;
    repeat
        frame;
    until(timer[2]>400)
    fade_off();

    priority=20;
    mouse.file=0;
    mouse.graph=2;
    mouse.x=10;
    mouse.y=10;

    fade_on();

    LOOP
        SWITCH (estado_programa);
            CASE 1:
                titulo();
            END
            CASE 2:
                redefinir();
            END
            CASE 3:
                points=-1;
                TIMER[9]=0;
                fama(0);
            END
            CASE 4:
                exit("",0);
            END
            CASE 5:
                volumen();
            END
            CASE 6:
                manzano();
            END
            CASE 7:
                load("USER\NIBBLES\records.dat",&listaentera);
                if (points>listaentera[9].ptos)
                    fama(1);
                    else
                    fama(2);
                end
            END
        END
        FRAME;
    END
END

PROCESS gun();

PRIVATE

conteo;
voz;
i1;
i2;

BEGIN

    set_mode (m640x480);

    //carga los graficos del inicio
    i1=load_fpg ("USER\NIBBLES\inicio.fpg");
    i2=load_fpg ("USER\NIBBLES\mon.fpg");
    voz=load_pcm ("USER\NIBBLES\voces.pcm",0);
    viento=load_pcm ("USER\NIBBLES\wind.pcm",0);

    WHILE (conteo<21)
        divfun (conteo,60+conteo*9,100,conteo,0);
        conteo++;
        frame(0);
    END
    sound (voz,256,225);
    sound (viento,256,256);
    REPEAT
        FRAME;
    UNTIL (TIMER[7]>500)
    TIMER[7]=0;
    conteo=0;
    signal(type divfun,s_kill);
    fade_off ();

    fade_on();
    WHILE (conteo<21)
        divfun (conteo,150,5+conteo*9,conteo,1);
        conteo++;
    END
    sound (voz,256,225);
    REPEAT
        FRAME;
    UNTIL (TIMER[7]>500)
    TIMER[7]=0;
    signal(type divfun,s_kill);
    fade_off ();

    stop_sound(all_sound);
    unload_pcm(voz);
    unload_pcm(viento);
    unload_fpg(i1);
    unload_fpg(i2);
    theend=1;
END

PROCESS divfun (graph,bx,by,ang,file);

PRIVATE

angulo;

BEGIN
    resolution=10;
    angulo=ang*pi/16;
    x=bx*10;
    y=by*10;
    LOOP
        FRAME;
        size=105+get_distx (angulo,14);
        angulo+=(pi/16);
    END
END

PROCESS titulo();

PRIVATE

rbasta=1;

BEGIN
    signal (type opciones,s_kill);
    signal (type pon,s_kill);
    signal (type divfun,s_kill);
    signal (type indicador2,s_kill);

    omax=4; //numero de opciones
    curop=1;
    opcion=1; //activa la primera opcion del menu
    denuevo=1;
    indicador(200,0);
    estado_programa=-1;     // Cambia el estado del programa a -1 para

    fade_on();

    put_screen(0,4);

    // Opciones del men£ principal
    pon(35,350,1,pi/16,-1,0);
    pon(320,50,5,0,1,0);
    pon(320,150,6,0,2,1);
    pon(320,250,8,0,3,0);
    pon(320,350,9,0,4,0);

    WHILE (estado_programa==-1) // Repite hasta que se cambie de estado

        // Usa las teclas del cursor

        IF (denuevo==0)
            if(teclascursor(0)==0) return; end
        END

        IF (key(_up)==0 and key(_down)==0 and key(_enter)==0)
            denuevo=0;
        END

        IF (key(_ESC))          // Salir del programa
            estado_programa=4;
        END

        // Limites de los botones

        IF ((mouse.x>175 and mouse.x<465) and ((mouse.y<385 and mouse.y>315) or (mouse.y<285 and mouse.y>215) or (mouse.y<185 and mouse.y>115) or (mouse.y<85 and mouse.y>15)))
            curop=opcion;
            IF (mouse.left and rbasta==0)
                rbasta=1;
                if(eleccion_hecha(opcion)==0) return; end
            END
            IF (mouse.left==0)
                rbasta=0;
            END
        END
        IF ((mouse.x>175 and mouse.y>315 and mouse.x<465 and mouse.y<385 and opcion!=4) or (curop==4 and opcion!=4))
            opcion=4;curop=4;
            indicador(600,3);
        END
        IF ((mouse.x>175 and mouse.y>215 and mouse.x<465 and mouse.y<285 and opcion!=3) or (curop==3 and opcion!=3))
            opcion=3;curop=3;
            indicador(400,2);
        END
        IF ((mouse.x>175 and mouse.y>115 and mouse.x<465 and mouse.y<185 and opcion!=2) or (curop==2 and opcion!=2))
            opcion=2;curop=2;
            indicador(JUG,1);
        END
        IF ((mouse.x>175 and mouse.y>15 and mouse.x<465 and mouse.y<85 and opcion!=1) or (curop==1 and opcion!=1))
            opcion=1;curop=1;
            indicador(200,0);
        END

        FRAME;

    END

    fade_off();                     // Apaga la pantalla

    unload_fpg(fichero_grafico);    // Descarga los gr ficos
END

PROCESS juego ();

BEGIN
END

PROCESS creditos ();

BEGIN
END

PROCESS pon(x,y,graf,ang,num,esp)

BEGIN
    LOOP
        IF (opcion==num)
            graph=0;
            ELSE
            graph=graf+(JUG/500)*esp;
        END
        FRAME;
        angle+=ang;
    END
END

PROCESS indicador(op,seleccion);

PRIVATE

cont=0;

BEGIN
    signal(type divfun,s_kill_tree);
    WHILE (cont<29)
        divfun (cont+op,180+cont*10,50+seleccion*100,cont,0);
        cont++;
    END
END

PROCESS indicador2(op,seleccion);

PRIVATE

cont=0;

BEGIN
    signal(type divfun,s_kill_tree);
    WHILE (cont<29)
        divfun (cont+op,180+cont*10,40+seleccion*80,cont,0);
        cont++;
    END
END

PROCESS teclascursor(member);

BEGIN
        priority=15;
        IF (key(_UP))
            curop--;
            IF (curop<1)
                curop=omax;
            END
        END
        IF (key(_DOWN))
            curop++;
            IF (curop>omax)
                curop=1;
            END
        END
        denuevo=1;
        IF (key(_ENTER))
            switch(member);
                case 0:
                    if (eleccion_hecha(curop)==1)
                        return(1);
                        else
                        return(0);
                    end
                end
                case 1:
                    if (eleccion_hecha2(curop)==1)
                        return(1);
                        else
                        return(0);
                    end
                end
            end
        END
END

PROCESS eleccion_hecha(choice);

BEGIN
    SWITCH (choice);
        CASE 1:
            fade_off();
            signal (type titulo,s_kill);
            signal (type pon,s_kill);
            signal (type divfun,s_kill);
            signal (type indicador,s_kill);
            unload_fpg(fichero_grafico);
            jugar_ya();
        END
        CASE 2:
            IF (JUG==300)
                JUG=500;
                ELSE
                JUG=300;
            END
            opcion=2;curop=2;
            indicador(JUG,1);
            return(1);
        END
        CASE 3:
            fade_off();
            opciones();
        END
        CASE 4:
            fade_off();
            exit("",0);
        END
    END
    return(0);
END




PROCESS opciones ();

PRIVATE

rbasta=1;

BEGIN
    signal (type titulo,s_kill);
    signal (type pon,s_kill);
    signal (type divfun,s_kill);
    signal (type indicador,s_kill);

    fade_on();

    omax=5;             //numero de opciones
    curop=1;            //reestablece valores
    denuevo=1;
    opcion=1;           //activa la primera opcion del menu
    indicador2 (800,0);

    put_screen (0,700);

    // Opciones de configuracion
    pon (35,350,1,pi/16,-1,0);
    pon (320,40,701,0,1,0);
    pon (320,120,702,0,2,0);
    pon (320,200,703,0,3,0);
    pon (320,280,704,0,4,0);
    pon (320,360,705,0,5,0);

    WHILE (estado_programa==-1)

        // Usa las teclas del cursor

        IF (denuevo==0)
            if(teclascursor(1)==0) return; end
        END

        IF (key(_up)==0 and key(_down)==0 and key(_enter)==0)
            denuevo=0;
        END

        IF (key(_ESC))          // Salir del programa
            estado_programa=4;
        END

        // Limites de los botones

        IF ((mouse.x>175 and mouse.x<465) and ((mouse.y>325 and mouse.y<395) or (mouse.y>245 and mouse.y<315) or (mouse.y>165 and mouse.y<235) or (mouse.y>85 and mouse.y<155) or (mouse.y>5 and mouse.y<75)))
            curop=opcion;
            IF (mouse.left and rbasta==0)
                rbasta=1;
                if(eleccion_hecha2(opcion)==0) return; end
            END
            IF (mouse.left==0)
                rbasta=0;
            END
        END
        IF ((mouse.x>175 and mouse.y>325 and mouse.x<465 and mouse.y<395 and opcion!=5) or (curop==5 and opcion!=5))
            opcion=5;curop=5;
            indicador2(920,4);
        END
        IF ((mouse.x>175 and mouse.y>245 and mouse.x<465 and mouse.y<315 and opcion!=4) or (curop==4 and opcion!=4))
            opcion=4;curop=4;
            indicador2(890,3);
        END
        IF ((mouse.x>175 and mouse.y>165 and mouse.x<465 and mouse.y<235 and opcion!=3) or (curop==3 and opcion!=3))
            opcion=3;curop=3;
            indicador2(860,2);
        END
        IF ((mouse.x>175 and mouse.y>85 and mouse.x<465 and mouse.y<155 and opcion!=2) or (curop==2 and opcion!=2))
            opcion=2;curop=2;
            indicador2(830,1);
        END
        IF ((mouse.x>175 and mouse.y>5 and mouse.x<465 and mouse.y<75 and opcion!=1) or (curop==1 and opcion!=1))
            opcion=1;curop=1;
            indicador2(800,0);
        END

        FRAME;

    END

    fade_off();
    unload_fpg (fichero_grafico);
END

PROCESS eleccion_hecha2(choice);

BEGIN
    SWITCH (choice);
        CASE 1:
            fade_off();
            signal(type opciones,s_kill);
            signal (type pon,s_kill);
            signal (type divfun,s_kill);
            signal (type indicador2,s_kill);
            estado_programa=3;
        END
        CASE 2:
            fade_off();
            signal(type opciones,s_kill);
            signal (type pon,s_kill);
            signal (type divfun,s_kill);
            signal (type indicador2,s_kill);
            estado_programa=2;
        END
        CASE 3:
            fade_off();
            signal(type opciones,s_kill);
            signal (type pon,s_kill);
            signal (type divfun,s_kill);
            signal (type indicador2,s_kill);
            estado_programa=5;
        END
        CASE 4:
            fade_off();
            signal(type opciones,s_kill);
            signal (type pon,s_kill);
            signal (type divfun,s_kill);
            signal (type indicador2,s_kill);
            estado_programa=6;
        END
        CASE 5:
            fade_off();
            titulo();
        END
    END
    return(0);
END




PROCESS fama(member);

PRIVATE

captura;
enterletra=1;
angle2;
ty;
CBA;

BEGIN

    delete_text(all_text);
    alpha=0;
    ultimaletra=0;
    cuentaletra=0;
    from ty=0 to 39;
        nomb[ty]=0;
    end
    barco=0;

    mouse.graph=0;

    unload_fpg(fichero_grafico);

    fichero_grafico2=load_fpg("USER\NIBBLES\fama.fpg");
    fuentepuntos=load_fnt("USER\NIBBLES\puntos.fnt");
    estado_programa=-1;

    fade_on();

    put_screen(0,41);

    IF (member==0 or member==2)
        lista(member);
        else
        x=310;
        angle2=pi/40;
        FROM CBA=1 TO 40;
            size+=get_disty(angle2,5);
            x+=get_distx(angle2,40);
            y=(size*2)-25;
            angle2+=pi/20;
            letra(CBA,x,y,size);
            FRAME(0);
        END

        LOOP
            if (key(_right))
                alpha++;
            end
            if (key(_left))
                alpha--;
            end
            if (key(_enter) and enterletra==0)
                if (ultimaletra==40)
                    TIMER[9]=0;
                    lista(1);
                    return;
                    else
                    if (ultimaletra==39)
                        if (cuentaletra>0)
                            nomb[cuentaletra]=0;
                            cuentaletra--;
                        end
                        else
                        if (cuentaletra<12)
                            nomb[cuentaletra++]=ultimaletra;
                            letramas(ultimaletra,cuentaletra);
                        end
                    end
                end
                enterletra=1;
            end
            if (not key(_enter))
                enterletra=0;
            end
            frame;
        END
    END
END

PROCESS letra(beta,x,y,s);

PRIVATE

gamma;

BEGIN
    size=s;
    LOOP
        if (beta==40)
            size=200;
            else
            size=s;
        end
        graph=beta+alpha+gamma;
        if (graph>40)
            gamma-=40;
            graph=beta+alpha+gamma;
        end
        if (graph<1)
            gamma+=40;
            graph=beta+alpha+gamma;
        end

        ultimaletra=graph;

        z=-y;
        FRAME;
    END
END

PROCESS letramas(ultlet,borroletra)

BEGIN
    graph=ultlet;
    size=100;
    y=300;
    repeat
        x=borroletra*50+(590-cuentaletra*50)/2;
        frame;
    until (cuentaletra<borroletra)
END

PROCESS lista(member);

PRIVATE

kilo;
kilo2=0;
kiloo=0;
point;
killoo;

BEGIN
    estado_programa=-1;

    signal(type letra,s_kill);
    signal(type letramas,s_kill);
    signal(type ola,s_kill);
    signal(type marea,s_kill);

    load("USER\NIBBLES\records.dat",&listaentera);
    repeat
        if (points<listaentera[kilo2].ptos)
            from kilo=0 to 11;
            definitiva[kilo2].dni2[kilo]=listaentera[kilo2-kiloo].dni[kilo];
            definitiva[kilo2].ptos2=listaentera[kilo2-kiloo].ptos;
            end
            kilo2++;
        end
        if (points=>listaentera[kilo2].ptos)
            killoo=kilo2;
            kilo2=situo(kilo2);
            kilo2++;
            kiloo=1;
            point=points;
            points=-1;
        end
    until (listaentera[kilo2-kiloo].dni[0]==0 or kilo2>9);

    if (member==1)
        definitiva[(killoo)].ptos2=point;
        save("c:\utilidad\dos\div\divgames\concurso\gusano\records.dat",&definitiva,sizeof(definitiva));
    end

    ola(member);
END

PROCESS ola(member);

PRIVATE

contax;
contay;

BEGIN
    from contay=0 to 9;
        write_int(fuentepuntos,520,contay*35+20,0,&definitiva[contay].ptos2);
        from contax=0 to 11;
            if (definitiva[contay].dni2[contax]==0)
                marea(contax*40+45,contay*35+45,38);
                else
                marea(contax*40+45,contay*35+45,definitiva[contay].dni2[contax]);
            end
            frame(0);
        end
    end
    LOOP
        if (TIMER[9]>500)
            unload_fpg(fichero_grafico2);
            unload_fnt(fuentepuntos);
            signal(type marea,s_kill);
            fichero_grafico=load_fpg ("USER\NIBBLES\menu.fpg");
            mouse.graph=2;
            if (member==0)
                delete_text(all_text);
                eleccion_hecha(3);
                else
                delete_text(all_text);
                estado_programa=1;
                fade_off();
            end
            return;
        end
        FRAME;
    END
END

PROCESS marea(x,y,graph);

BEGIN
    LOOP
        size+=get_distx(barco,10);
        barco+=pi/32;
        FRAME;
    END
END

PROCESS situo(kilo3);

PRIVATE

kilos;

BEGIN
    from kilos=0 to 11;
        if (nomb[kilos]==0)
            nomb[kilos]=38;
        end
        definitiva[kilo3].dni2[kilos]=nomb[kilos];
    end
    return(kilo3);
END




PROCESS redefinir();

BEGIN
    PARPA=0;

    mouse.graph=0;

    unload_fpg(fichero_grafico);

    fichero_grafico2=load_fpg ("USER\NIBBLES\fama.fpg");
    estado_programa=-1;

    fade_on();

    info(300,200,44);
    tec();

    repeat
        frame;
    until(not key(_enter) and scan_code==0)

    LOOP
        IF (scan_code!=0)
            teclasini[PARPA]=scan_code;
            PARPA++;
            signal(type pontecla,s_kill);
            tec();
            if (parpa>3)
                luz();
                return;
            end
        END
        FRAME;
    END
END

PROCESS tec();

PRIVATE

tecn;
tecn2;

BEGIN
    from tecn=0 to 1;
        from tecn2=0 to 1;
            if (busca_letra[teclasini[tecn*2+tecn2]]!=0)
                pontecla(460,y+tecn2*40+170+tecn*145,busca_letra[teclasini[tecn*2+tecn2]],tecn*2+tecn2);
                else
                pontecla(460,y+tecn2*40+170+tecn*145,38,tecn*2+tecn2);
            end
        end
    end
END
PROCESS pontecla(x,y,graphn,ngp);

PRIVATE

GOP;

BEGIN
    LOOP
        IF (PARPA==ngp)
            IF (GOP==1)
                graph=graphn;
                GOP=0;
                ELSE
                graph=43;
                GOP++;
            END
            ELSE
            graph=graphn;
        END
        FRAME;
    END
END

PROCESS luz();

BEGIN
    timer[8]=0;
    REPEAT
        FRAME;
    UNTIL (timer[8]>50);
    unload_fpg(fichero_grafico2);
    fichero_grafico=load_fpg ("USER\NIBBLES\menu.fpg");
    signal(type pontecla,s_kill);
    signal(type info,s_kill);
    eleccion_hecha(3);
    mouse.graph=2;
END

PROCESS info(x,y,graph);

BEGIN
    LOOP
        FRAME;
    END
END




PROCESS manzano();

PRIVATE

rbasta=1;

BEGIN

    opcion=-1;

    denuevo=1;
    indicador3(60,0,8);
    indicador3(920,1,29);

    estado_programa=-1;     // Cambia el estado del programa a -1 para
                            // llamarse solo una vez
    fade_on();

    put_screen(0,700);

    // Opciones del men£ principal
    pon(35,350,1,pi/16,-1,0);
    info(320,150,21);
    manzanaactual();

    WHILE (estado_programa==-1) // Repite hasta que se cambie de estado

        // Usa las teclas del cursor

        IF (denuevo==0)
            if(aumenta()==1) return; end
        END

        IF (key(_enter)==0)
            denuevo=0;
        END

        // Limites de los botones

        IF (mouse.x>175 and mouse.y>215 and mouse.x<465 and mouse.y<285)
            IF (mouse.left and rbasta==0)
                rbasta=1;
                signal(type info,s_kill);
                signal(type manzanaactual,s_kill);
                signal(type manzano,s_kill);
                eleccion_hecha(3);
                return;
            END
            IF (mouse.left==0)
                rbasta=0;
            END
        END
        IF (mouse.x>246 and mouse.y>134 and mouse.x<475 and mouse.y<165)
            IF (mouse.left)
                manzanasgeneral=(mouse.x-256)/12;
            END
        END

        FRAME;

    END

    fade_off();                     // Apaga la pantalla

END

PROCESS indicador3(op,seleccion,centrado);

PRIVATE

cont=0;

BEGIN
    WHILE (cont<centrado)
        divfun (cont+op,180+cont*10,150+seleccion*100,cont,0);
        cont++;
    END
END

PROCESS aumenta();

BEGIN
    priority=15;
    IF (key(_right))
        manzanasgeneral++;
    END
    IF (key(_left))
        manzanasgeneral--;
    END
    IF (key(_enter))
        denuevo=1;
        signal(type info,s_kill);
        signal(type manzanaactual,s_kill);
        signal(type manzano,s_kill);
        eleccion_hecha(3);
        return(1);
    END
    return(0);
END

PROCESS manzanaactual();

BEGIN
    graph=22;
    y=150;
    LOOP
        if (manzanasgeneral>16)
            manzanasgeneral=16;
        end
        if (manzanasgeneral<0)
            manzanasgeneral=0;
        end
        x=manzanasgeneral*12+265;
        FRAME;
    END
END




PROCESS volumen();

PRIVATE

rbasta=1;

BEGIN

    opcion=-1;

    denuevo=1;
    indicador3(50,0,8);
    indicador3(920,1,29);

    estado_programa=-1;     // Cambia el estado del programa a -1 para
                            // llamarse solo una vez
    fade_on();

    put_screen(0,700);

    // Opciones del men£ principal
    pon(35,350,1,pi/16,-1,0);
    info(320,150,21);
    volumenactual();

    WHILE (estado_programa==-1) // Repite hasta que se cambie de estado

        // Usa las teclas del cursor

        IF (denuevo==0)
            if(aumenta2()==1) return; end
        END

        IF (key(_enter)==0)
            denuevo=0;
        END

        // Limites de los botones

        IF (mouse.x>175 and mouse.y>215 and mouse.x<465 and mouse.y<285)
            IF (mouse.left and rbasta==0)
                rbasta=1;
                signal(type info,s_kill);
                signal(type volumenactual,s_kill);
                signal(type volumen,s_kill);
                eleccion_hecha(3);
                return;
            END
            IF (mouse.left==0)
                rbasta=0;
            END
        END
        IF (mouse.x>246 and mouse.y>134 and mouse.x<475 and mouse.y<165)
            IF (mouse.left)
                volumengeneral=(mouse.x-256)/12;
            END
        END

        FRAME;

    END

    fade_off();                     // Apaga la pantalla

END

PROCESS aumenta2();

BEGIN
    priority=15;
    IF (key(_right))
        volumengeneral++;
    END
    IF (key(_left))
        volumengeneral--;
    END
    setup.master=volumengeneral;
    set_volume();
    IF (key(_enter))
        denuevo=1;
        signal(type info,s_kill);
        signal(type volumenactual,s_kill);
        signal(type volumen,s_kill);
        eleccion_hecha(3);
        return(1);
    END
    return(0);
END

PROCESS volumenactual();

BEGIN
    graph=22;
    y=150;
    LOOP
        if (volumengeneral>16)
            volumengeneral=16;
        end
        if (volumengeneral<0)
            volumengeneral=0;
        end
        x=volumengeneral*12+265;
        FRAME;
    END
END

PROCESS jugar_ya();

PRIVATE

contador;
boomx;
boomy;

BEGIN

    jon=0;
    jon1=0;
    jon2=0;
    j1=10;
    j2=10;
    TIMER[5]=0; //tiempo consumido por jugadores
    TIMER[6]=0;
    ggg=0; //reestablece todas las variables globales de jugar_ya a cero
    dinero=0;
    corte1=0;
    corte2=0;
    cortemas1=0;
    cortemas2=0;
    cortes1=0;
    cortes2=0;
    final1=0;
    final2=0;
    long1=10;
    long2=10;
    angulo1=0;
    angulo2=0;
    apples=0;
    meteor=0;
    caso=0;
    bomba=0;
    px1=0;
    px2=0;
    py1=0;
    py2=0;
    avance = 65;
    avance2 = 65;
    puntos1=0;
    puntos2=0;
    atila=0;
    atila2=0;
    radio=0;
    radio2=0;
    arrasa=0;
    app=0;
    fuente1=0;
    caso=0;
    arrasa3=0;
    ameteoro=0;
    ametx=0;
    amety=0;
    bomber=0;

    RETORNO=0;


    mouse.graph=0;

    estado_programa=-1;


    set_mode(m640x480);

    FROM contador=0 TO 255;
        nueva_paleta[contador]=0;
    END

    set_fps (18,9);

    fuente1=load_fnt ("USER\NIBBLES\ptos.fnt");
    ggg=load_fpg ("USER\NIBBLES\gusano.fpg");
    music1=load_pcm ("USER\NIBBLES\musica1.pcm",1);
    alerta=load_pcm ("USER\NIBBLES\alarma.pcm",1);
    fuego=load_pcm ("USER\NIBBLES\fuego.pcm",1);
    choque=load_pcm ("USER\NIBBLES\choque.pcm",0);
    tragar=load_pcm ("USER\NIBBLES\tragar.pcm",0);
    bomba1=load_pcm ("USER\NIBBLES\bomba1.pcm",0);
    bomba2=load_pcm ("USER\NIBBLES\bomba2.pcm",0);
    avion=load_pcm ("USER\NIBBLES\avion.pcm",0);
    dinero=load_pcm ("USER\NIBBLES\dinero.pcm",0);
    aplauso=load_pcm ("USER\NIBBLES\aplauso.pcm",0);
    aprisa=load_pcm ("USER\NIBBLES\aprisa.pcm",0);

    //imprime el primer y segundo gusano

    TIMER[4]=0; //espera a que termine de cargar los datos
    REPEAT
        FRAME;
    UNTIL (TIMER[4]>50);

    stopmusic1=sound (music1,256,256);

    if (JUG==500)
        puntos1=300; //tiempo disponible
        puntos2=300; //tendran que coger una manzana para aumentarlo
        define_region (1,20,0,280,99);
        define_region (2,20,101,280,99);
        gusano2 ();
        //muestra la direccion del otro jugador
        brujula1 ();
        brujula2 ();
        centauro=20;
        longmax=30;
        else
        centauro=24;
        define_region (1,20,40,280,119);
        longmax=120;
    end

    gusano1 ();
    cut(); //visualiza el numero de cortes que quedan
    priority = 10;
    LOOP
        app=0;
        //las manzanas aparecen aleatoriamente al cabo de 250 centesimas, como mucho habra 5 manzanas
        IF (timer[0]>250 AND apples<5)
            manzanas (rand(32,480),rand(32,480),(rand (0,(1+16-manzanasgeneral))/(1+16-manzanasgeneral))*18+5); //doy una probabilidad para la manzana amarilla
            timer[0]=0;
        END
        //lanza meteoritos aleatoriamente
        IF (meteor>1 and timer[1]>200)
            boomx=rand(320,4800);
            boomy=rand(320,4800);
            meteoritos(rand(320,4800),rand(320,4800),boomx,boomy);
            meteor--;
            timer[1]=0;
        END
        //los meteoritos estallan
        IF (ameteoro==1)
            ameteoro=0;
            activa(ametx,amety); //activa la explosion del meteorito
        END
        //se inicia la lista de procesos para la bomba
        IF (bomba>0)
            boomx=rand(32,480);
            boomy=rand(32,480);
            bombas(boomx,boomy);
            bomba=-1; //sale de la condicion
        END
        IF (bomba==-2)
            explosion(boomx,boomy);
        END

        IF (key (_p)) pausa();END //tecla comun de pausa

        IF (RETORNO==1)
            stop_sound(all_sound);
                fade_off();
                delete_text(all_text);
                signal(type cut,s_kill);
                signal(type gusano1,s_kill);
                signal(type gusano2,s_kill);
                signal(type brujula1,s_kill);
                signal(type brujula2,s_kill);
                signal(type manzanas,s_kill);
                signal(type tumba,s_kill);
                signal(type ganador,s_kill);

                unload_fnt(fuente1);
                unload_fpg(ggg);
                unload_pcm(music1);
                unload_pcm(alerta);
                unload_pcm(fuego);
                unload_pcm(choque);
                unload_pcm(tragar);
                unload_pcm(bomba1);
                unload_pcm(bomba2);
                unload_pcm(avion);
                unload_pcm(dinero);
                unload_pcm(aplauso);
                delete_text(all_text);
                fichero_grafico=load_fpg ("USER\NIBBLES\menu.fpg");
                set_mode(m640x480);
                if (centauro!=20)
                    points=puntos1;
                    estado_programa=7;
                    else
                    points=-1;
                    estado_programa=1;
                end
                mouse.graph=2;
                return;
        END

        FRAME;
    END
END

PROCESS gusano1 ();

PRIVATE
val1;
val2;
ya=0;

BEGIN
    resolution=10;
    x = 500;
    y = 500;
    angle = 7*pi/4;
    angulo1=angle;

    m7.height = 256;
    m7.focus = 128;
    m7.camera = id;
    m7.distance = 105;

    start_mode7 (0,0,22,2,1,centauro);

    m7 [2].height = -64;
    m7 [2].focus = 128;
    m7 [2].camera = id;
    m7 [2].distance = 105;

    start_mode7 (2,0,600,600,1,centauro+1);

    LOOP
        val1=final2;
        WHILE (val1<long2 and centauro==20)
            IF (fget_dist(x/10,y/10,ppx2[val1],ppy2[val1])<(20-val1*100/8/(long2-1)) and final1==0)
                stop_sound (all_sound);
                sound(choque,512,256);
                stopfuego1=sound(fuego,512,256);
                m7.distance = 150;
                final1=1;
            END
            val1++;
        END
        val2=10;
        WHILE (val2<long1)
            IF (fget_dist(x/10,y/10,ppx1[val2],ppy1[val2])<(20-val2*100/8/(long1-1)) and final1==0)
                stop_sound (all_sound);
                sound(choque,512,256);
                stopfuego1=sound(fuego,512,256);
                m7.distance = 150;
                final1=1;
            END
            val2++;
        END
        IF ((cortes1>5 or j1<=0) and final1==0)
            stop_sound (all_sound);
            sound(choque,512,256);
            stopfuego1=sound(fuego,512,256);
            m7.distance = 150;
            final1=1;
        END
        val2=0;
        WHILE (val2<apples and final1==0)
            IF (fget_dist(x/10,y/10,mmx[val2],mmy[val2])<15)
                mmx[val2]=-100;mmy[val2]=-100;
                sound(tragar,128,256);
                if (kkk [val2]>22)
                    if (rand(0,4)==0)
                        puntos1+=50;
                        if (centauro!=20)
                            bonus();
                            else
                            bonus2(0);
                        end
                    end
                    if  (bomba==0 and meteor==0)
                        puntos1+=20;
                        if (rand(0,1)==0)
                            bomba=5;
                            else
                            meteor=11;
                        end
                    end
                end
                puntos1+=10;
                signal (mmm [val2],s_kill);
                if (long1<longmax)
                    long1++;
                end
            END
            val2++;
        END
        val2=0;
        IF ((y<100 or y>5020 or x<100 or x>5020) and final1==0)
            stop_sound (all_sound);
            sound(choque,512,256);
            stopfuego1=sound(fuego,512,256);
            m7.distance = 150;
            final1=1;
        END
        IF ((y<420 or y>4700 or x<420 or x>4700) and final1==0)
            IF (ya==0)
                stopalert1=sound (alerta,128,256);
            END
            ya=1;
            ELSE
            ya=0;stop_sound (stopalert1);
        END
        obj1 ();
        // captura teclas (la tecla esc es de ejecucion inmediata)
        IF (key (teclasini[0]) and final1==0) angle += pi/16;angulo1=angle;END
        IF (key (teclasini[1]) and final1==0) angle -= pi/16;angulo1=angle;END
        advance (avance);
        px1=x/10;
        py1=y/10;
        FRAME;
    END
END

PROCESS gusano2();

PRIVATE
val1;
val2;
ya=0;

BEGIN
    resolution=10;
    x = 4620;
    y = 4620;
    angle = 3*pi/4;
    angulo2=angle;

    m7 [1].height = 256;
    m7 [1].focus = 128;
    m7 [1].camera = id;
    m7 [1].distance = 105;

    start_mode7 (1,0,22,2,2,20);

    m7 [3].height = -64;
    m7 [3].focus = 128;
    m7 [3].camera = id;
    m7 [3].distance = 105;

    start_mode7 (3,0,600,600,2,21);

    LOOP
        val1=final1;
        WHILE (val1<long1)
            IF (fget_dist(x/10,y/10,ppx1[val1],ppy1[val1])<(20-val1*100/8/(long1-1)) and final2==0)
                stop_sound (all_sound);
                sound(choque,512,128);
                stopfuego2=sound(fuego,512,128);
                m7 [1].distance = 150;
                final2=1;
            END
            val1++;
        END
        val2=10;
        WHILE (val2<long2)
            IF (fget_dist(x/10,y/10,ppx2[val2],ppy2[val2])<(20-val2*100/8/(long2-1)) and final2==0)
                stop_sound (all_sound);
                sound(choque,512,128);
                stopfuego2=sound(fuego,512,128);
                m7 [1].distance = 150;
                final2=1;
            END
            val2++;
        END
        IF ((cortes2>5 or j2<=0) and final2==0)
            stop_sound (all_sound);
            sound(choque,512,128);
            stopfuego2=sound(fuego,512,128);
            m7 [1].distance = 150;
            final2=1;
        END
        val2=0;
        WHILE (val2<apples and final2==0)
            IF (fget_dist(x/10,y/10,mmx[val2],mmy[val2])<15)
                mmx[val2]=-100;mmy[val2]=-100;
                sound(tragar,128,128);
                if (kkk [val2]>22)
                    if (rand(0,4)==0)
                        puntos2+=50;
                        if (centauro!=20)
                            bonus();
                            else
                            bonus2(1);
                        end
                    end
                    if  (bomba==0 and meteor==0)
                        puntos2+=20;
                        if (rand(0,1)==0)
                            bomba=5;
                            else
                            meteor=11;
                        end
                    end
                end
                puntos2+=10;
                signal (mmm [val2],s_kill);
                if (long2<longmax)
                    long2++;
                end
            END
            val2++;
        END
        val2=0;
        IF ((y<100 or y>5020 or x<100 or x>5020) and final2==0)
            stop_sound (all_sound);
            sound(choque,512,128);
            stopfuego2=sound(fuego,512,128);
            m7 [1].distance = 150;
            final2=1;
        END
        IF ((y<420 or y>4700 or x<420 or x>4700) and final2==0)
            IF (ya==0)
                stopalert2=sound (alerta,128,128);
            END
            ya=1;
            ELSE
            ya=0;stop_sound (stopalert2);
        END
        obj2();
        // captura teclas (la tecla esc es de ejecucion inmediata)
        IF (key (teclasini[2]) and final2==0) angle += pi/16;angulo2=angle;END
        IF (key (teclasini[3]) and final2==0) angle -= pi/16;angulo2=angle;END
        advance (avance2);
        px2=x/10;
        py2=y/10;
        FRAME;
    END
END

PROCESS tumba (xgraph,x,y)

BEGIN
    ctype=c_m7;
    cnumber=c_0+c_1;
    size=200;
    LOOP
        FRAME;
    END
END

PROCESS obj1();

PRIVATE

val=0;

BEGIN
    resolution=10;
    x=father.x;
    y=father.y;
    ctype=c_m7;
    cnumber=c_0+c_1;
    flags=4;
    IF (final1==0)
        graph=3;
        else
        avance=0;
        graph=99+final1;
        final1++;
        father.angle+=pi/16;
        if (final1>long1 and jon1==0)
            if (centauro!=20)
                stop_sound(all_sound);
                jon1=1;
            end
            if (jon>5 and centauro==20)
                stop_sound(all_sound);
                ganador(1);
                jon1=1;
            end
        end
        if (final1>long1+100)
            RETORNO=1;
            return;
        end
        if (final1>4)
            return;
        end
        if (final1==2)
            polvo(x/10,y/10,800);
            map_xput(0,22,96,x/10,y/10,0,80,0);
            tumba(&lapida,x/10,y/10);
        end
    END
    IF ((key (teclasini[0])==0 and key(teclasini[1])==0) or (key (teclasini[0])>0 and key(teclasini[1])>0))
        y+=get_disty(angulo1+90000,h[h1[val]]);
        x+=get_distx(angulo1+90000,h[h1[val]]);
        h1[val]++;
        IF (h1[val]>7)
            h1[val]=0;
        END
    END
    WHILE (val<long1)
        size=400-(val*250/(long1-1));
        ppx1[val]=x/10;
        ppy1[val]=y/10;
        FRAME;
        val++;
    END
END

PROCESS obj2();

PRIVATE

val=0;

BEGIN
    resolution=10;
    x=father.x;
    y=father.y;
    ctype=c_m7;
    cnumber=c_0+c_1;
    flags=4;
    IF (final2==0)
        graph=11;
        else
        avance2=0;
        graph=99+final2;
        final2++;
        father.angle+=pi/16;
        if (final2>long2 and jon2==0)
            if (centauro!=20)
                stop_sound(all_sound);
                jon2=1;
            end
            if (jon>5 and centauro==20)
                stop_sound(all_sound);
                ganador(0);
                jon2=1;
            end
        end
        if (final2>long2+100)
            RETORNO=1;
            return;
        end
        if (final2>4)
            return;
        end
        if (final2==2)
            polvo(x/10,y/10,800);
            map_xput(0,22,96,x/10,y/10,0,80,0);
            tumba(&lapida,x/10,y/10);
        end
    END
    IF ((key (teclasini[2])==0 and key(teclasini[3])==0) or (key (teclasini[2])>0 and key(teclasini[3])>0))
        y+=get_disty (angulo2+90000,-h[h2[val]]);
        x+=get_distx (angulo2+90000,-h[h2[val]]);
        h2[val]++;
        IF (h2 [val]>7)
            h2 [val]=0;
        END
    END
    WHILE (val<long2)
        size=400-(val*250/(long2-1));
        ppx2[val]=x/10;
        ppy2[val]=y/10;
        FRAME;
        val++;
    END
END

PROCESS brujula1 ();

BEGIN
    x=20;y=20;
    flags=4;
    graph=16;
    LOOP
        angle=fget_angle(px1,py1,px2,py2)-angulo1;
        FRAME;
    END
END

PROCESS brujula2 ();

BEGIN
    x=20;y=120;
    flags=4;
    graph=16;
    LOOP
        angle=fget_angle(px2,py2,px1,py1)-angulo2;
        FRAME;
    END
END

PROCESS manzanas (x,y,kind);

PRIVATE

appear;

BEGIN
    priority=15;
    appear=kind;
    ctype=c_m7;
    cnumber=c_0+c_1;
    size=400;
    LOOP
        graph=appear;
        if (appear<kind+5)
            appear++;
        end
        kkk [app]=kind;
        mmm [app]=id;
        mmx [app]=x;
        mmy [app]=y;
        app++;
        apples=app;
        FRAME;
    END
END

PROCESS bombas (x,y);

private

bby=300;
val;

BEGIN
    ctype=c_m7;
    cnumber=c_0+c_1;
    size=125;
    graph=30;
    if (bomber==0) sound(avion,256,256);end
    loop
    convert_palette(0,30,offset nueva_paleta);
    map_xput(0,30,31,125,0,pendulo[val],100,0);
    height=bby;
    val++;
    if (val>19)
        val=0;
    end
    bby-=5;
    if (bby<-30)
        sound(bomba1,1024,256);
        signal(id,s_kill);
        bomba=-2;//inicia el proceso explosion
    end
    frame;
    end
END

PROCESS explosion (x,y);

PRIVATE

val=0;
val1=0;
val2=0;

BEGIN
    bomba=-3; //no deja que volvamos a iniciar el proceso;
    atila=0;
    radio=50;

    //si hay alguna manzana, se destruye
    val2=0;
    WHILE (val2<apples)
        IF (fget_dist(x,y,mmx[val2],mmy[val2])<95)
            polvo(mmx[val2],mmy[val2],400);
            mmx[val2]=-100;mmy[val2]=-100;
            eraser(mmm[val2]);
        END
        val2++;
    END
    while (val<20)
        detona(x,y);
        val++;
        frame(0);
    end
    val=0;
    polvo(x,y,800);
    map_xput(0,22,95,x,y,0,90,4);
    loop
        atila+=4;
        radio+=16;

        //el gusano muere o se segmenta por la explosion
        val1=0;
        WHILE (val1<long1)
            IF (fget_dist(x,y,ppx1[val1],ppy1[val1])<atila+val1*100/8/(long1-1) and final1==0)
                if (val1<10)
                    stop_sound (all_sound);
                    sound(choque,512,256);
                    stopfuego1=sound(fuego,512,256);
                    m7.distance = 150;
                    final1=1;
                    else
                    long1=val1;
                    corte1++;
                end
            END
            val1++;
        END
        val1=0;
        WHILE (val1<long2 and centauro==20)
            IF (fget_dist(x,y,ppx2[val1],ppy2[val1])<atila+val1*100/8/(long2-1) and final2==0)
                if (val1<10)
                    stop_sound (all_sound);
                    sound(choque,512,128);
                    stopfuego2=sound(fuego,512,128);
                    m7 [1].distance = 150;
                    final2=1;
                    else
                    long2=val1;
                    corte2++;
                end
            END
            val1++;
        END

        if (val>92)
            bomber++;
            if (bomber>2)
                bomba=0; //explosion finalizada
                bomber=0;
                else
                bomba=5; //siguiente bomba
            end
            if (corte1!=cortemas1)
                cortes1++;
            end
            if (corte2!=cortemas2)
                cortes2++;
            end
            cortemas1=corte1;
            cortemas2=corte2;
            signal(type explosion,s_kill_tree);
        end
        val+=4;
        frame;
    end
END

PROCESS detona (bax,bay);

BEGIN
    ctype=c_m7;
    cnumber=c_0+c_1;
    loop
    size=radio;
    graph=100;
    x=bax+get_distx(arrasa,atila);
    y=bay+get_disty(arrasa,atila);
    arrasa+=pi/10;
    frame;
    end
END

PROCESS polvo (x,y,size);

PRIVATE

val=0;

BEGIN
    ctype=c_m7;
    cnumber=c_0+c_1;
    loop
    graph=50+val;
    val++;
    if (val>21)
        signal(type polvo,s_kill);
    end
    frame;
    end
END

PROCESS meteoritos(x,y,mex,mey);

PRIVATE

ang;
ang2;
trecho;
val=0;

BEGIN
    ctype=c_m7;
    cnumber=c_0+c_1;
    resolution=10;
    size=200;
    graph=300;
    height=300;
    trecho=fget_dist(0,0,fget_dist(x,y,mex,mey),height);
    repeat
        if (height>5) //el meteorito sigue el trayecto fijado: desde x,y hasta mex,mey
            ang=fget_angle(0,0,fget_dist(x,y,mex,mey),height);
            ang2=fget_angle(x,y,mex,mey);
        end
        x+=get_distx(ang2,trecho/50);
        y+=get_disty(ang2,trecho/50);
        height-=get_disty(ang,trecho/50);
        humo(x/10+rand(-8,8),y/10+rand(-8,8),height);
        frame;
    until(height<0)
    sound(bomba2,1024,256);
    ameteoro=1;
    ametx=x/10;
    amety=y/10;
    while (val<4) //destruye el meteorito
    graph++;
    val++;
    frame;
    end
END

PROCESS activa(x,y);

PRIVATE

val=0;
val1=0;
val2=0;

BEGIN
    atila2=0;
    radio2=50;

    //si hay alguna manzana, se destruye
    val2=0;
    WHILE (val2<apples)
        IF (fget_dist(x,y,mmx[val2],mmy[val2])<40)
            polvo(mmx[val2],mmy[val2],400);
            mmx[val2]=-100;mmy[val2]=-100;
            eraser(mmm[val2]);
        END
        val2++;
    END
    while (val<50)
        esfera(x,y,val/10*pi/10,arrasa3,val);
        arrasa3+=pi/5;
        val++;
        frame(0);
    end
    val=0;
    polvo(x,y,800);
    map_xput(0,22,95,x,y,0,40,4);
    loop
        atila2+=2;
        radio2+=10;

        //el gusano muere o se segmenta por la explosion
        val1=0;
        WHILE (val1<long1)
            IF (fget_dist(x,y,ppx1[val1],ppy1[val1])<atila2+val1*100/8/(long1-1) and final1==0)
                if (val1<10)
                    stop_sound (all_sound);
                    sound(choque,512,256);
                    stopfuego1=sound(fuego,512,256);
                    m7.distance = 150;
                    final1=1;
                    else
                    long1=val1;
                    corte1++;
                end
            END
            val1++;
        END
        val1=0;
        WHILE (val1<long2 and centauro==20)
            IF (fget_dist(x,y,ppx2[val1],ppy2[val1])<atila2+val1*100/8/(long2-1) and final2==0)
                if (val1<10)
                    stop_sound (all_sound);
                    sound(choque,512,128);
                    stopfuego2=sound(fuego,512,128);
                    m7 [1].distance = 150;
                    final2=1;
                    else
                    long2=val1;
                    corte2++;
                end
            END
            val1++;
        END

        if (val>32)
            signal(type activa,s_kill_tree);
            caso++; //explosion finalizada
            if (caso==10)
                caso=0;
                meteor=0;
            END
            if (corte1!=cortemas1)
                cortes1++;
            end
            if (corte2!=cortemas2)
                cortes2++;
            end
            cortemas1=corte1;
            cortemas2=corte2;
        END
        val+=2;
        FRAME;
    END
END

PROCESS eraser(fuera);

PRIVATE

seguro;

BEGIN
    from seguro=0 to 1;
    signal (fuera,s_kill);
    FRAME;
    END
END

PROCESS esfera(bax,bay,cierra,arrasa2,vales);

BEGIN
    ctype=c_m7;
    cnumber=c_0+c_1;
    arrasa2+=(vales/10)*pi/10;
    LOOP
        size=radio2;
        graph=100;
        height=-get_disty(cierra,atila2*4);
        x=bax+get_distx(arrasa2,get_distx(cierra,atila2));
        y=bay+get_disty(arrasa2,get_distx(cierra,atila2));
        FRAME;
    END
END

PROCESS humo(x,y,height);

BEGIN
    ctype=c_m7;
    cnumber=c_0+c_1;
    size=200;
    graph=101;
    flags=4;
    REPEAT
        height+=8;
        size-=20;
        FRAME;
    UNTIL (size<=100)
END

PROCESS pausa();

PRIVATE

contador=0;

BEGIN
    priority=50;
    tiempo1=timer[0]; //graba los tiempos
    tiempo2=timer[1];

    //se asegura de que ningun proceso salte la pausa

    signal (get_id(0),s_freeze_tree);
    signal (type pausa,s_wakeup_tree);

    WHILE (contador<31)
        divfun (200+contador,28+contador*9,100,contador,0);
        contador++;
    END
    REPEAT
        FRAME;
    UNTIL (scan_code!=0 and key(_p)==0 and scan_code!=25);
    timer[0]=tiempo1;timer[1]=tiempo2; //reestablece los tiempos perdidos

    signal (get_id(0),s_wakeup_tree);
    signal (type divfun,s_kill_tree);

    FRAME;
END

PROCESS cut()

PRIVATE

J3;
J4;

BEGIN
    priority=-2;
    loop
        delete_text(all_text);
        if (final1==0 and final2==0)
            if (centauro==20)
                j1=(puntos1/5-TIMER[5]/100);
                j2=(puntos2/5-TIMER[6]/100);
                write_int(fuente1,25,0,0,&j1); //imprime segundos
                write_int(fuente1,25,100,0,&j2);
                j3=5-cortes1;
                j4=5-cortes2;
                write_int(fuente1,285,14,4,&j3); //imprime cortes
                write_int(fuente1,285,114,4,&j4);
                if ((j1<5 or j2<5) and jj1!=j1)
                    sound(aprisa,256,256);
                end
                jj1=j1;
                else
                j3=5-cortes1;
                write_int(fuente1,285,48,4,&j3);
                write_int(fuente1,25,34,0,&puntos1);
            end
        end
        if ((final1!=0 or final2!=0) and jon1==0 and jon2==0)
            jon++;
            if (final1!=0 and final2!=0)
                jon=0;
            end
        end
        frame;
    end
END

PROCESS bonus();

PRIVATE

ssize=10;

BEGIN
    graph=1;
    x=160;
    y=100;
    size=20;
    sound(dinero,256,256);
    repeat
    size+=ssize;
        if (size=>100)
            ssize=-ssize;
        end
        frame;
    until (size<=20)
END

PROCESS ganador(ng);

BEGIN
    z=-500;
    graph=4;
    x=160;
    y=ng*100+25;
    size=20;
    sound(aplauso,256,256);
    repeat
        size+=10;
        frame;
    until (size=>100)
    loop
        frame;
    end
END

PROCESS bonus2(nnb);

PRIVATE

ssize=10;

BEGIN
    graph=1;
    x=160;
    y=nnb*100+50;
    size=20;
    sound(dinero,256,256);
    repeat
    size+=ssize;
        if (size=>100)
            ssize=-ssize;
        end
        frame;
    until (size<=20)
END
