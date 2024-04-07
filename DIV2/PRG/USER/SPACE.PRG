COMPILER_OPTIONS _Extended_conditions;
// ***************************************************************
// **                                                           **
// **   SPACE REVENGE v1.0     (c) Vicente J. S nchez S nchez   **
// **                                                           **
// ***************************************************************

// La humanidad se ha extendido por el universo, creando colonias
// que formaban inmensas ciudades, en las que se vivia tranquilamente...

// Pero no duro mucho esa tranquilidad... Un raza alienigena destruyo
// con sus poderosas armas a la mayoria de las colonias y con ellas
// hasta casi el exterminio de la humanidad...

// Los pocos cientificos que sobrevivieron a la gran batalla, unieron
// sus conocimientos para crear una nave rapida y destructora con la
// que poder hacer pagar tan horrenda masacre...

// Tu has sido elegido para pilotarla, y ha llegado el momento...
// de la VENGANZA.....

// ***************************************************************

// El funcionamiento del juego es simple... avanzar destruyendo todas
// las naves enemigas, asi como los Destructores alienigenas, durante
// veinte areas, hasta llegar al ultimo nivel donde tendras que vertelas
// con la nave Imperial Alienigena, y ver cumplida tan esperada venganza.
// Solo se tiene una vida... pero dispones de energia a modo de escudo...

// ***************************************************************

Program Space_Revenge;

    Global
        g_dir;
        g_vel;
        g_x;
        g_y;
        nivel;
        ingame;
        puntos;
        energia;
        Cancion_CD;
        fondo;
        laser1;
        laser2;
        explota;

    Local
        energi;

    Private;
        aux;
        id1;

    Begin

        Set_mode(m640x480);     // Definimos el modo de pantalla, asi
        Set_fps(50,1);          // como los fotogramas/segundo del juego...

        Load_fpg('user\spacer\space.fpg');  // Cargamos los graficos, fonts...
        Load_map('user\spacer\fondo1.map');
        Load_map('user\spacer\stars.map');
        fondo=Load_map('user\spacer\pan1.map');
        Load_fnt('user\spacer\space.fnt');

        laser1=Load_pcm('user\spacer\laser1.pcm',0);     // Cargamos los sonidos...
        laser2=Load_pcm('user\spacer\laser2.pcm',0);
        explota=Load_pcm('user\spacer\explota.pcm',0);

        Loop
            clear_screen();                       // Aqui empieza el bucle del juego...
            Load_pal('user\spacer\pan1.map');       // Y ponemos la pantalla de inicio...
            put_screen(0,1002);
            Fade_on();
            Timer=0;
            Repeat
                If (Timer>50) delete_text(all_text);
                    write(1,320,50,4,'(c) Vicente J. S nchez S nchez');
                End    // Y hacemos que parpadee
                If (Timer>100)                              // las letras de comienzo del juego...
                    Write(1,320,390,4,'Press Fire Button');
                    Write(1,320,430,4,'To Start');
                    Timer=0;
                End

                Frame;
            Until (key(_space));    // Hasta que se pulse el boton de disparo...

            nivel=1;
            ingame=0;  // Esta variable evita (0) que se compruebe(1) el paso a otro nivel...
            puntos=0;
            energia=20;  // Energia inicial de la nave protagonista...
            Cancion_CD=2;
            Fade_off();
            clear_screen();
            Load_pal('user\spacer\stars.map');

            Start_Scroll(0,0,1001,1000,0,15) ;  // Inicializamos el Scroll...
            Scroll[0].ratio=1200;

            mision(nivel);      // Carga el siguiente Nivel...

                Repeat

                    If (ingame==1)   // Si estamos jugando, comprobamos que no quede
                                     // nave viva en pantalla...

                        If ((Get_id(type Nave_Enemiga)==0) and (Get_id(type Jefe_Enemigo))==0 and (energia>0))
                            ingame=0;
                            id1=Get_id(type Jugador);
                            Scroll[0].camera=pasa_nivel(id1.x,id1.y,id1.angle,g_dir,g_vel);
                            Signal(type Jugador,s_Kill);
                        End
                    End

                    If (energia<=0 and ingame==1)     // Si estamos jugando, y
                        ingame=0;                     // nos quedamos sin energia
                        id1=Get_id(type Jugador);     // pues morimos y termina la partida...
                        Sound(explota,128,128);
                        explosion(id1.x,id1.y,300);
                        Scroll[0].camera=0;
                        Signal(id1,s_Kill);
                        Frame(1500);
                        Stop_CD();
                        Write(1,320,240,4,'GAME OVER');
                        Timer[2]=0;
                        Repeat
                            Frame;
                        Until (Timer[2]>500);
                        Fade_off();
                        Stop_Scroll(0);
                        ingame=9;
                        delete_text(all_text);
                    End

                    Frame;
                Until (ingame==9);

        End
    End

// **********************************************************************

// Este proceso es tu nave. Necesita valores iniciales X, Y, ANGLE...
// El control se realiza con las teclas del cursor de Izquierda, Derecha,
// Arriba y la barra espaciadora para disparar...
// La nave se comporta con inercia, tanto para el giro como para la aceleracion
// y frenada...


Process Jugador(x,y,angle);

    Private
        vel;
        dir;
        gatillo=0;
        id1;
        aux=0;

    Begin

        ctype=c_Scroll;       // Todos los procesos que quieran moverse por el Scroll, deben definirse con este tipo...
        graph=1;

        Loop
            If (key(_left)) angle+=10000; End     // Gira la nave...
            If (key(_right)) angle-=10000; End

            If (angle>360000) angle-=360000;End   // Para que el angulo de la nave siempre
            If (angle<0) angle+=360000;End        // este entre 0 y 360000...

            If (key(_up))                         // Impulsa la nave...
                if (aux==0) dir=angle; aux=1; end // Si antes no estaba pulsada, pues le quitamos la inercia...
                vel+=3;
                dir=near_angle(dir,angle,5000);   // Pero le damos inercia si gira...
            End

            if (not(key(_up))) aux=0; end      // Quitamos la inercia de la direccion, ya que no se impulsa la nave...

            If (vel>64) vel=64; End    // Limita la velocidad Maxima...

            If (key(_space) and gatillo==0)                    // Si el Gatillo estaba sin pulsar y se pulsa ahora
               Laser_Jugador(x,y,angle,(vel/8)+16);gatillo=1;  // pues lanza un disparo... Pero la velocidad varia dependiendo la de la nave...
            End

            If (gatillo==1 and not key(_space))  // Espera que suelte el gatillo para cargar el Laser de nuevo...
               gatillo=0;
            End

            If (id1=collision(type Nave_Enemiga))  // Si colisionamos con una nave enemiga,
                advance(-32);vel=-64;              // pues revotamos...
                Signal(id1,s_Kill);
                Sound(explota,128,256);
                explosion(id1.x,id1.y,50);
                energia-=2;marcador();
            End

            If (id1=collision(type Jefe_Enemigo))
                explosion(x,y,25);advance(-32);vel=-64;  // Lo mismo con los jefes Enemigos...
                energia-=2;marcador();
            End

            If (id1=collision(type Explosion_Nuclear))
                explosion(x,y,25);advance(32);vel=64;  // Si te alcanza una explosion Nuclear...
                energia--;marcador();                  // sales revotado...
            End


            x+=Get_distx(dir,vel/8);   // Esto hace la misma funcion que ADVANCE()
            y+=Get_disty(dir,vel/8);   // pero solo avanza en direccion DIR y no ANGLE

            If (vel>0) vel--; End   // Vamos frenando la nave...
            If (vel<0) vel=0; End   // Y evitamos la marcha atras...


            g_vel=vel;
            g_dir=dir;

            radar();   // Imprimimos la rejilla del radar en pantalla...

            Frame;
            g_x=x;
            g_y=y;
        End

    End;

// **********************************************************************

// Este proceso produce un laser con valores iniciales de X,Y, un angulo (ANGLE)
// y una velocidad (VEL).
// El proceso tiene una vida limitada a un Bucle (ALCANCE) o hasta que haga
// colision con otros procesos, donde sera finalizado...

Process Laser_Jugador(x,y,angle,vel);

    Private
        alcance;
        tocado;

    Begin
        ctype=c_Scroll;
        graph=13;size=150;
        Sound(laser1,128,256);

        From alcance=1 to 40;        // Alcance limitado de la bala...
            advance(vel);
            If (tocado=collision(type Nave_Enemiga))    // Comprobamos si a impactado....
                Signal(tocado,S_Kill);
                explosion(tocado.x,tocado.y,50);
                Sound(explota,128,256);
                alcance=9999;
                puntos+=2500;
            End

            If (tocado=collision(type Jefe_Enemigo))
                tocado.energi--;
                explosion(x,y,25);
                Sound(explota,64,256);
                alcance=9999;
                puntos+=25;
            End

            If (tocado=collision(type Mina))
                Signal(tocado,s_Kill);
                explosion(x,y,50);
                Sound(explota,128,256);
                alcance=9999;
                puntos+=250;
            End

            Frame;

        End

    End

// ********************************************************************

// Este proceso produce una Mina que tiene dos graficos dIferentes, cada uno
// para cada tipo de comportamiento, uno en el que persigue al jugador y otro
// cuando esta activada, rotando sobre si misma... La velocidad de movimiento
// es lenta, pero tiene un radio de destruccion muy alto, igual que el misil,
// creando una explosion nuclear cuando explosiona. Se puede destruir, anulando
// la carga mortifera...

Process Mina(x,y);

    Private
        vel;
        id1;

    Begin

        ctype=c_Scroll;
        graph=10;
        id1=Get_id(type Jugador);

        Loop

            If (Get_dist(id1)>300)                           // Si el jugador esta a distancia,
                angle=near_angle(angle,Get_angle(id1),5000); // la mina lo buscara....
                graph=10;vel++;
                If (vel>32) vel=32; End
              Else
                vel--;                                    // Y una vez encontrada la nave,
                If (vel<6)                                // se activa...
                    vel=6;
                    graph=11;
                    angle+=25000;
                End
            End

            advance(vel/6);
            mapa(x,y,graph,angle);

            If (Get_dist(id1)<150)          // Vaya... te has acercado demasiado....
                Explosion_Nuclear(x,y);
                Break;
            End

            Frame;

        End

    End

// ********************************************************************

// Este proceso es el corazon del enemigo... De aqui salen todas las naves
// enemigas de peque¤o tama¤o... Dependiendo de grafico (GRAPH) con el que
// se le llame (al proceso) actuaran de una forma o de otra...
// Asi que si GRAPH=6 la nave sera lenta, y no disparara, en cambio si
// GRAPH=2 la nave sera rapida y mortal... (Ver comportamiento)...


Process Nave_Enemiga(x,y,graph);

    Private
        idnave;
        idlaser;
        angulo;
        aux;
        ant;
        vel;
        giro;
        disparo;
        esquiba;

    Begin

        ctype=c_Scroll;

        switch (graph);                      // Aqui se definen el comportamiento de las naves enemigas...

            Case 7;       // Caza Imperial
                vel=10;
                giro=10000;
                disparo=10;
                esquiba=1;
            End

            Case 6;      // Nave de carga
                vel=4;
                giro=5000;
                disparo=0;
                esquiba=0;
            End

            Case 5;      // Nave Tiburon
                vel=6;
                giro=5000;
                disparo=5;
                esquiba=1;
            End

            Case 4;     // Nave Standard de combate
                vel=8;
                giro=8000;
                disparo=8;
                esquiba=1;
            End

            Case 3;     // Nave Tridente
                vel=7;
                giro=1000;
                disparo=5;
                esquiba=0;
            End

            Case 2:      // Nave Asalto Rapido
                vel=8;
                giro=20000;
                disparo=15;
                esquiba=1;
            End
        End


        Loop

            idlaser=Get_id(type Laser_Jugador);
            idnave=Get_id(type Jugador);

            If (Get_dist(idlaser)<200)                      // Distancia a la que esta el disparo
                angle=near_angle(angle,idlaser.angle,giro);
            End;

            angulo=Get_angle(idnave);

                                           // La nave enemiga dara la vuelta para atacar...

            If (Get_dist(idnave)>400)
                angle=near_angle(angle,angulo,rand(100,giro));
            End

                                          // La nave enemiga cambiara el rumbo para no chocar con el jugador...

            If (Get_dist(idnave)<100 and esquiba)
                angle=near_angle(angle,angulo+180000,rand(100,giro));
            End

                                          // Capacidad y frecuencia de disparo enemigo...

            If (rand(1,1000)>1000-disparo) Laser_Enemigo(x,y,angle,vel+3); End

            advance(vel);

            Frame;
            mapa(x,y,graph,angle);  // Se encarga de poner la nave en el mini-mapa...
        End
    End

// **********************************************************************

// Este proceso es el mas peligroso... Se trata de un misil dirigido con
// cabeza nuclear... Es inmune a nuestros disparos... Asi que has de ser
// rapido para esquivarlo...
// Tiene dos comportamientos.... de pendiendo del valor de la variable (PERSIGUE)...
// Asi, pues, el misil es capaz de seguirte cuidadosamente... hasta el
// momento del ataque, donde se lanzara para impactar.... Cada vez que falle
// un ataque, se volvera mas peligroso... ya que sera capaz de explotar mas
// rapidamente y si necesidad de tocarte.... solo pasanto junto a la nave...

Process Misil_Nuclear(x,y);

    Private
        id1;
        vel;
        persigue;
        tocado;
        intentos=1;    // Intentos del misil en alcanzarte....

    Begin
        ctype=c_Scroll;
        graph=12;
        id1=Get_id(type Jugador);

        Loop

            If (persigue)             // busca tu nave...
                angle=near_angle(angle,Get_angle(id1),2000*intentos);

                If (vel<=0)                 // TRUCO: si consigues que el
                    Explosion_Nuclear(x,y); // misil se pare... Explotara...
                    Break;                  // siendo mas facil quitartelo de encima...
                End

                If (Get_dist(id1)>200)       // Hace el efecto de aceleracion/frenada...
                    vel++;
                    If (vel>64) vel=64; End
                  Else
                    vel--;
                End

                If (rand(1,500)>498) persigue=false;intentos++; End   // ATACA !!!!

              Else

                vel++;

                If (Get_dist(id1)>500) persigue=true; End    // Si el misil esta lejos, comienza otro ataque...
            End;

            advance(vel/6);

            mapa(x,y,graph,angle);

            If (Get_dist(id1)<5*intentos)     // Cuidado... que explota si esta cerca...
                Explosion_Nuclear(x,y);
                Break;
            End

            Frame;

        End
    End

// *********************************************************************

// Este proceso crea una explosion nuclear... mortal su contacto...


Process Explosion_Nuclear(x,y);

    Private
        contador;
        tocado;

    Begin
        ctype=c_Scroll;
        Sound (explota,128,200);
        graph=23;
        size=0;

        From contador=1 to 20;     // Vamos aumentando su tama¤o, al
            size+=30;              // tiempo que gira...
            angle+=5000;
            Frame;
        End;
    End;

// ********************************************************************

// Generamos una bala enemiga... De vida limitada...


Process Laser_Enemigo(x,y,angle,vel);

    Private
        tocado;
        alcance;

    Begin
        ctype=c_Scroll;
        graph=15;
        size=200;
        Sound(laser2,128,1000);

        From alcance=1 to 150;
            advance(vel);

            If (tocado=collision(type Jugador))      // Si toca al jugador le quitara energia...
                explosion(tocado.x,tocado.y,25);
                alcance=9999;
                energia--;marcador();
            End

            Frame;
        End
    End

// ********************************************************************

// Proceso que crea un Jefe Supremo a traves de datos como:
// Las posiciones X e Y, asi como su GIRO, la distancia (dist) de ataque,
// la velocidad (fact_vel), y la del disparo (vel_disp), la energia con
// la que comienza, el tama¤o e incluso el grafico que utilizara...


Process Jefe_Enemigo(x,y,giro,dist,fact_vel,vel_disp,energi,size,graph);

    Private
        id1;
        vel;
        aux;

    Begin
        ctype=c_Scroll;
        id1=Get_id(type Jugador);

        Loop
            angle=near_angle(angle,Get_angle(id1),giro);     // Busca la nave...

            If (vel<0) vel=0; End                // Y se acerca a la nave...
            If (Get_dist(id1)>dist)
                vel++;
                If (vel>64) vel=64; End
              Else                               // Parandose a una distancia (dist)
                vel--;
            End

            If (angle==Get_angle(id1) and Timer[0]>vel_disp*25)   // Dispara con pausas...(vel_disp)
                Laser_Jefe_Enemigo(x,y,angle);
                Timer[0]=0;
            End

            If (energi<0)                 // Si se queda sin energia... muere...
                Sound(explota,128,128);
                explosion(x,y,200);
                graph=0;
                energia+=4;marcador();
                Frame;
                Break;
            End

            advance(vel/fact_vel);
            mapa(x,y,graph,angle);      // La imprimimos en el mini_mapa...

            Frame;
        End
  End


// *********************************************************************

// Proceso en el que se lanza un laser enemigo... Igual que los otros, pero
// con la diferencia de que este tiene ACELERACION ...


Process Laser_Jefe_Enemigo(x,y,angle);

    Private
        accel;
        aux;
        tocado;

    Begin

        ctype=c_Scroll;
        accel=0;
        graph=14;

        Sound(laser2,128,256);

        From aux=1 to 100;            // Tambien... como no... con alcance limitado...
            advance(accel);
            If (tocado=collision(type Jugador))  // Y si toca la nave
                explosion(tocado.x,tocado.y,20);
                aux=9999;                        // pues quita energia...
                energia--;marcador();
            End

            accel++;                      // Aceleramos la velocidad...

            If (accel>16) accel=16; End   // pero sin pasar de un limite...

            Frame;
        End
    End


// ********************************************************************

// Imprime en un mini_mapa los objetos (Graph), con coordenadas X-Y, y con
// giro (g)...


Process mapa(x,y,g,angle);

    Private
        idnave;

    Begin

        If (idnave=Get_id(type Jugador));

            x=80+((x-idnave.x)/50);y=420+((y-idnave.y)/50);

            If (x>155) x=155; End    // Evitamos que las coordenas
            If (x<5) x=5; End
            If (y>475) y=475; End    // se salgan del mini_mapa...
            If (y<365) y=365; End

            graph=g;size=20;z=512;

        End

        Frame;

    End;


// ********************************************************************

// Este proceso crea la explosion generica...


Process Explosion(x,y,size);

    Private
        aux;

    Begin

        ctype=c_Scroll;
        angle=rand(-180000,180000);

        From aux=16 to 21;
            graph=aux;
            Frame(300);
        End
    End


// ********************************************************************

// El alma del juego... donde se definen todas las areas con sus enemigos...


Process mision(fase);

    Private
        aux;

    Begin
        If (not(is_Playing_CD())) Play_CD(1,1); End     // Ponemos el AREA
        delete_text(all_text);
        Write(1,320,150,4,'AREA');
        Write_int(1,320,190,4,offSet (fase));
        Write(1,320,330,4,'Ready!!');
        Timer[0]=0;
        Fade_on();
                                                     // y movemos el scroll para dejarlo bonito..
        Repeat
            Scroll[0].y0-=32;
            Scroll[0].y1-=2;
            Frame;
        Until (Timer[0]>100 and key(_space))

        Fade_off();
        delete_text(all_text);
        marcador();
        Scroll[0].camera=Jugador (8000,8000,90000);

        switch (fase);                           // Y aqui, dependiendo de la (fase), pues ponemos los enemigos...

            Case 1;
                From aux=1 to 6;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),6);
                End
            End

            Case 2;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),6);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),5);
                End

            End

            Case 3;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),5);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),3);
                End
            End

            Case 4;
                Nave_Enemiga(9000,7000,2);
                Jefe_Enemigo(8000,5000,1000,350,8,4,10,300,5);
                Nave_Enemiga(7000,7000,2);
            End

            Case 5;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),3);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),4);
                    Mina(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 6;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),3);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),4);
                    Mina(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 7;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),4);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),7);
                    Mina(rand(5000,11000),rand(5000,11000));
                End
                Misil_Nuclear(rand(5000,11000),rand(5000,11000));
            End

            Case 8;
                Nave_Enemiga(9000,7000,2);
                Jefe_Enemigo(8000,5000,1000,350,8,4,10,300,3);
                Nave_Enemiga(7000,7000,2);
                Mina(rand(5000,11000),rand(5000,11000));
            End


            Case 9;
                From aux=1 to 2;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),7);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                    Mina(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 10;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),7);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                End
                Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                Mina(rand(5000,11000),rand(5000,11000));
            End

            Case 11;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),7);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                    Mina(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 12;
                Nave_Enemiga(9000,7000,2);
                Jefe_Enemigo(8000,5000,1000,350,8,4,15,300,4);
                Nave_Enemiga(7000,7000,2);
                Mina(rand(5000,11000),rand(5000,11000));
            End

            Case 13;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),7);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 14;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),7);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 15;
                From aux=1 to 3;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),7);
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                    Mina(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 16;
                Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                Jefe_Enemigo(8000,5000,1000,350,6,4,20,300,7);
                Misil_Nuclear(rand(5000,11000),rand(5000,11000));
            End

            Case 17;
                From aux=1 to 4;
                    Nave_Enemiga(rand(5000,11000),rand(5000,11000),2);
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                End
            End

            Case 18;
                From aux=1 to 8;
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                End
                Nave_Enemiga(8000,0,6);
            End

            Case 19;
                Jefe_Enemigo(8000,11000,1000,350,8,3,20,200,2);
                Jefe_Enemigo(8000,5000,1000,340,6,4,20,200,2);
                Misil_Nuclear(6000,8000);
                Misil_Nuclear(10000,8000);
            End

            Case 20;
                Jefe_Enemigo(8000,11000,2000,350,10,2,35,300,8);
                From aux=1 to 2;
                    Misil_Nuclear(rand(5000,11000),rand(5000,11000));
                    Mina(rand(5000,11000),rand(5000,11000));
                End
                Misil_Nuclear(8000,-10000);
                Misil_Nuclear(8000,15000);
            End

        End

        Fade_on();
        Play_CD(cancion_CD,1);
        If (is_Playing_CD()) cancion_CD++; Else cancion_CD=2;Play_CD(cancion_CD,1); End
        ingame=1;
    End


// **********************************************************************

// Este es el proceso de paso de un Area a otra....
// Creamos una animacion de la nave perdiendose en el espacio...


Process Pasa_Nivel(x,y,angle,dir,vel);

    Private
        vel;
        vel2;

    Begin
        ctype=c_Scroll;
        graph=1;
        Repeat                         // Creamos la transicion del proceso (Jugador) a este...
            x+=Get_distx(dir,vel/8);   // Esto hace la misma funcion que ADVANCE()
            y+=Get_disty(dir,vel/8);   // pero solo avanza en direccion DIR y no ANGLE
            vel--;If (vel<0) vel=0; End
            angle=near_angle(angle,90000,2000);

            Frame;

        Until (angle==90000 and vel==0);

        Play_CD(1,1);       // Y ponemos musica del CD si existe...

        Repeat
            vel++;          // Y comenzamos a acelerar...
            advance(vel/8);
            Frame;
        Until (vel>256);

        Scroll[0].camera=0;
        Scroll[0].y0=y-240;Scroll[0].x0=x-320;
        vel2=0;

        Repeat
            Scroll[0].y0-=32;    // Y lanzamos la nave al infinito...
            Scroll[0].y1-=2;
            y-=32+(vel2/10);
            Frame;
            vel2++;
            g_x=Scroll[0].y0;
            g_y=y;
        Until (vel2>100);

        graph=0;

        nivel++;                  // pasamos de AREA y ganamos energia...
        energia++;
        marcador();

        if (nivel==21)                                       // HEMOS LLEGADO AL FINAL....
          Write(1,320,100,4,'La Humanidad a sido Vengada');
          write(1,320,150,4,'La paz reina en el Universo');
          Write(1,320,200,4,'Pero quien sabe lo que nos');
          Write(1,320,250,4,'aguarda el futuro...');
          Write(1,320,350,4,'GAME OVER');
          timer=0;
          repeat
            frame;
          until (key(_space) and timer>500);
          exit('Gracias por jugar',0);
        end

        mision(nivel);
    End


// ********************************************************************

// Este proceso imprime/actuliza los marcadores en pantalla, tanto de
// vida como los de puntos...

Process marcador();

    Private
        aux;

    Begin

        delete_text(all_text);
        Write_int (1,30,0,0,offSet puntos);
        for (aux=0;aux<energia;aux++);
            Write(1,300+(aux*7),0,0,'|');
        End

        Frame;
    End


// ********************************************************************

// Este proceso pone la rejilla del Radar...

Process Radar();

    Begin

        x=80;y=420;
        graph=22;

        Frame;

    End


