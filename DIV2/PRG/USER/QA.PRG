
//==============================================================================
// PROGRAMA QA
// VERSION 1.2
// AUTOR ROBERTO SELVA GOMIS           rselva@Tande.com
// FECHA DESDE 1/2/98 AL 30/3/98
//==============================================================================


program qa;

const
  impose=50;  //Im genes por segundo
  cols=14; //(una mas de las reales)
  altura=220; //comienzo de las filas (marca el centro de la primera)
  topesup=14; //altura<tam*topesup
  ancini=5;  //comienzo de las columnas (tam/2 del comienzo de la 1¦)
  tam=16; //Debe ser par(mejor mult. de 4)
  sigue=1;    //El Dibujo de la explosi¢n (de momento, es un c¡rculo)
  numfases=16; //n£mero de fases

  cayendo=0;detectando=1;imposible=83;  //Estados (no unidos, afortunadamente)
  nada=0;preparada=1;lanzada=2; //Estados de la bomba


global               //teclas de control
  izqda=_left;
  dcha=_right;
  abajo=_down;
  rotar=_up;
  bomba=_space;
  nivel_inicio=1;  //Nivel inicial (+1)
  struct fijos[cols]
     alt[17]; //topesup<13  //Almacena los procesos amontonados
  end;
  struct conexi[cols]
     alt[17]; //topesup<13  //Almacena los procesos de borrado
  end;
  struct fase_var[15]        //Descripci¢n de las fases
    col_ini;     //colores inicial y final
    col_fin;
    critico;     //n£mero de piezas a juntar
    mini_vel_y;  //velocidad m¡nima y m xima de ca¡da
    maxi_vel_y;
    puntosfase;  //puntos necesarios para pasar una fase;
    bombas;      //bombas permitidas
  end=2,8,3,100,120,2500,1,
    5,9,5,75,120,2000,1,
    10,14,6,100,150,1500,1,
    13,18,4,100,170,2500,2,
    2,6,6,120,180,2500,2,
    7,12,5,120,170,3000,2,
    10,18,3,130,150,2000,2,
    2,5,8,130,180,2500,1,
    6,13,4,140,175,2000,3,
    14,16,10,130,180,3000,1,
    12,18,5,140,180,3000,1,
    2,4,12,140,180,1500,1,
    5,13,4,140,180,1800,3,
    14,18,8,140,180,2500,1,
    5,12,5,140,180,1500,2,
    2,4,12,140,180,1500,1;

  arch_fondo[3]=
    "user\qa\fon1.fpg","user\qa\fon2.fpg","user\qa\fon3.fpg","user\qa\fon4.fpg";
  fase=0;     //Fase actual
  nivel=0;    //Nivel actual
  inc_ini_vel=0;  //Incremento de la velocidad en cada fase
  numcont; //cuenta cu ntos contactos se detectan
  contador;  //cuenta cu ntos objetos caen+cu ntos explotan
  ultimo[cols];  //¨Hasta qu‚ objeto llega cada columna?
  modo=m320x240;
  final=0;       // Fin de una fase.
  idfalso;       // identidad nula
  estado;  //0=cayendo pieza; 1=detectando;
  est_bomb; //estados de la bomba
  bomb_actu;  //bombas que quedan actualmente
  bomb_graf;  //archivo de gr ficos de la bomba
  expl_graf;   //archivo con los gr ficos de la explosi¢n (c¡rculos)
  roce;       //sonido del roce
  roce_suena;  //¨est  sonando el roce?
  canal_roce;   //canal por el que suena
  caida;      //sonido de la caida
  prepara_golpe; //que ha ca¡do
  logro;        //sonido de conseguir puntos ¨?
  mus_perder;  //sonido que se oye cuando pierdes
  rui_rota;    //ruido de rotar colores
  fondofase;   //guarda el id del archivo donde est n los fondos
  letra;       //tipo de letra a utilizar
  letinterf;    //letra entre fases
  letrpunt;    //tipo de letra de los puntos
  puntuacion;
  marc_punt;
  prepara[3]; //Contiene los colores de la pr¢xima ficha
  valor;     //valor de un cuadrado
  vel_y; //velocidad
  struct mejores[15]  //Tabla de records
    nombre[5];
    puntuacion;
  end;
  bomb_activa=true;  //Variables que determinan si la bomba
  expl_activa=true;  // y las explosiones est n activas

local
  color,baja=0;
  altu;

private
  i;j;dest;punt_fase;tatachan;mus_cambio;
  color_fase;

begin

  load ("user\qa\mejores.dat",offset(mejores));
  estado=cayendo;
  set_fps(impose,3); //Pongo 3 para que funcione en ciertas
  set_mode(modo);    //m quinas, aunque sea a tirones.

  restore_type=complete_restore; //En realidad no es necesario...
  dump_type=complete_dump;
                                 //Vamos a cargar archivos
  load_fpg("user\qa\cuadr.fpg");   //¡ndice 0 (es el primero)
  bomb_graf=load_fpg("user\qa\bomb.fpg");
  expl_graf=load_fpg("user\qa\exp.fpg");
  roce=load_pcm("user\qa\roce.pcm",1);
  caida=load_pcm("user\qa\caida.pcm",0);
  logro=load_pcm("user\qa\fx34.pcm",0);
  tatachan=load_pcm("user\qa\start.pcm",0);
  mus_cambio=load_pcm("user\qa\muelle.pcm",0);
  mus_perder=load_pcm("user\qa\loss.pcm",0);
  rui_rota=load_pcm("user\qa\drop.pcm",0);

  letra=load_fnt("user\qa\nuevofn2.fnt");
  letrpunt=load_fnt("user\qa\nuevofnt.fnt");
  letinterf=load_fnt("user\qa\interf2.fnt");

  LOOP //BUCLE PRINCIPAL

  let_me_alone();                 //Borramos todo
  delete_text(all_text);
  clear_screen();
  FRAME;FRAME;FRAME;FRAME;
  estado=detectando;
  final=0;
  menu();
  while (estado==detectando)      //Esperamos que acabe el men£
   frame;
  end
  if (final==2)                   //Acabamos si se ha pulsado escape
    break;
  end
  fade_off();

  puntuacion=0;

  FRAME;
  delete_text(all_text);
  write(letinterf,160,60,4,"Prepara el primer nivel");
  fase=(nivel_inicio-1) mod numfases;
  inc_ini_vel=30*((nivel_inicio-1)/numfases);
  nivel=nivel_inicio;
  LOOP
    write(letinterf,160,90,4,"Total puntos");
    write(letinterf,80,130,4, "Puntos");
    write(letinterf,160,130,4, "Objetivo");
    write(letinterf,240,130,4, "Colores");
    write_int(letinterf,160,110,4,Offset(puntuacion));
    write_int(letinterf,80,150,4,Offset(fase_var[fase].puntosfase));
    write_int(letinterf,160,150,4,Offset(fase_var[fase].critico));
    color_fase=fase_var[fase].col_fin-fase_var[fase].col_ini+1;
    write_int(letinterf,240,150,4,Offset(color_fase));
    write(letinterf,160,180,4, "Lanza bomba para comenzar");
    clear_screen();
    fade_on();

    //write_int(letinterf,0,0,0,Offset(fps)); //para las pruebas

    estado=cayendo;                //Secuencia entre fases
    for (i=0;i<30;++i)
      test(rand(fase_var[fase].col_ini,fase_var[fase].col_fin));
    end;                           //Objetos voladores


    while (not (key(bomba)))       //Espera a que pulses la bomba
      frame;
    end
    estado=detectando;             //Al cambiar de estado, explotan los objetos
    sound(tatachan,200,256);

    timer=0; // uso timer[0] para esperar
    while (timer<300)
      frame;
    end                            //Espera 3 segundos y borra todo
    fade_off();


    delete_text(all_text);


    restore_type=complete_restore;
    dump_type=complete_dump;       //Prepara puntos y bombas
    punt_fase=fase_var[fase].puntosfase;
    bomb_actu=fase_var[fase].bombas;
    if (not(bomb_activa))          //(colores y nivel est n listos desde antes)
      bomb_actu=0;
    end
    let_me_alone();                //por si acaso
    frame;frame;frame;frame;
    clear_screen();
    fade_on();
                                   //pintamos el fondo
    fondofase=load_fpg(arch_fondo[fase mod 4]);
    put(fondofase,4,277,126);
    put(fondofase,1,6,altura/2+tam/2);
    put(fondofase,2,1+ancini+cols*tam-3,altura/2+tam/2); //(=mitad ancho col)
    put(fondofase,3,(ancini+cols*tam+4)/2,altura+14);
    FRAME;FRAME;FRAME;FRAME;       //asegura que se pinta ¨?
    restore_type=complete_restore; //a partir de aqu¡ dibuja s¢lo lo que se mueva
    dump_type=complete_dump;
    vel_y=fase_var[fase].mini_vel_y+inc_ini_vel; //Velocidad de ca¡da
    write(letra,280,80,4,"Puntos");              //Marcadores
    marc_punt=write_int(letra,280,95,4,OFFSET(puntuacion));
    write(letra,280,120,4,"Te quedan");
    write_int(letra,280,135,4,OFFSET(punt_fase));
    write(letra,265,160,4,"N");
    write_int(letra,265,175,4,OFFSET(nivel));
    write(letra,265,190,4,"O");
    write_int(letra,265,205,4,OFFSET(fase_var[fase].critico));
    write(letra,295,160,4,"C");
    write_int(letra,295,175,4,OFFSET(color_fase));
    write(letra,295,190,4,"B");
    write_int(letra,295,205,4,OFFSET(bomb_actu));

    //write_int(letinterf,0,0,0,Offset(fps));//Para las pruebas

    final=0;
    idfalso=falso();               //Proceso fantasma
    for (i=0;i<=cols;++i)          //Rellenamos con ‚l las casillas
      ultimo[i]=0;
      for(j=0;j<=17;++j)
        conexi[i].alt[j]=idfalso;
        fijos[i].alt[j]=idfalso;
      end
    end
    for(i=1;i<=3;++i)              //Prepara primera pieza
      prepara[i]=rand(fase_var[fase].col_ini,fase_var[fase].col_fin);
    end
    est_bomb=nada;
    LOOP
      estado=cayendo;               //Bucle del juego propiamente dicho
      if (est_bomb<>preparada)      //Lanzamos la pieza o la bomba
        cuadro();
      else
        obj_bomb();
      end
      while (estado==cayendo)       //Esperamos a que toque fondo
        FRAME;
        if (key(_esc))              //si pulsamos escape nos vamos
          final=2;
          break;
        end;
      end
      if (final<>0) break;          //Si se ha pulsado escape o hay demasiados
      end                           //cuadros, salimos del bucle.
      contador=0;
      valor=0;
      prepara_golpe=true;
      if (est_bomb==lanzada)        //Destrucci¢n de la bomba
        est_bomb=nada;
        sound(caida,300,256);
        prepara_golpe=false;
        for (i=0;i<=cols;++i)
          for(j=0;j<ultimo[i];++j)  //Localizamos la bomba
            if (fijos[i].alt[j].baja<0)
              fijos[i].alt[j].baja=0;
              numcont=0;
              if (j==0)             //Almacenamos color a destruir o uno impo-
                color=1;            //sible si toca el suelo.
              else
                color=fijos[i].alt[j-1].color;
              end                   //Destruimos la bomba
              compro(i,j,fijos[i].alt[j].color);
              FRAME(0);
              contador+=numcont;    //Apuntamos objetos a destruir (uno)
            end
          end
        end
        valor=10;                   //Ahora, cada cuadro vale 10.
        for (i=0;i<=cols;++i)
          for(j=0;j<ultimo[i];++j)  //Buscamos cuadros de ese color.
            if ((fijos[i].alt[j].color==color)&&(conexi[i].alt[j]==idfalso));
              numcont=0;            //Los destruimos
              compro(i,j,fijos[i].alt[j].color);
              FRAME(0);
              contador+=numcont;    //Apuntamos la cantidad y la puntuaci¢n
              puntuacion+=valor*numcont;
              punt_fase-=valor*numcont;
              if (punt_fase<0)
                punt_fase=0;
              end
            end
          end
        end
      end
      LOOP                          //Bucle de reacci¢n a la ca¡da de la pieza
        valor+=10;                  //o de la bomba
        while (contador>0)          //Incrementamos el valor por pieza
          FRAME;                    //Si contador es mayor que cero, no se han
          if (prepara_golpe)        //destru¡do todos los compro, o no han ba-
            sound(caida,300,256);   //do todos los cuadros.
            prepara_golpe=false;    //Si ha de sonar el golpe, suena.
          end
          if ((key(bomba))&&(est_bomb==nada)&&(bomb_actu>0))
            est_bomb=preparada;     //Detectamos la preparaci¢n de la bomba.
            signal(type otro,s_kill_tree);
            otro();
            --bomb_actu;
          end

        end
        if (prepara_golpe)          //Por si ha quedado alg£n golpe
          sound(caida,300,256);
          prepara_golpe=false;
        end

        for (i=0;i<=cols;++i)
          for(j=0;j<ultimo[i];++j)  //Localizamos los cuadrados reci‚n ca¡dos
            if (fijos[i].alt[j].baja<0)  //(por su "baja")
              fijos[i].alt[j].baja=0;
              numcont=0;
              dest=compro(i,j,fijos[i].alt[j].color);
              FRAME(0);             //Sembramos el detector
              if (numcont<fase_var[fase].critico)
                dest.baja=-1;       //Si no hay suficientes, destruimos s¢lo
                dest.priority=10;   //el detector.
              else
                contador+=numcont;  //Si s¡ hay, incrementamos los puntos y
                puntuacion+=valor*numcont;//el n£mero de objetos a destruir.
                punt_fase-=valor*numcont;
                if (punt_fase<0)
                  punt_fase=0;
                end
                //delete_text(marc_punt);
                //marc_punt=write_int(letra,280,95,4,OFFSET(puntuacion));
              end
            end
          end
        end
        if (contador==0) break;     //Si no hay que destruir ninguno, paramos
        else
          sound(logro,150,256);
        end

      end

      if (punt_fase<=0)             //Si hemos acabado la fase, pasamos de nivel
        sound(mus_cambio,200,256);
        fade_off();
        break;
      end

    END
    restore_type=complete_restore;  //Volvemos a pintar todo
    dump_type=complete_dump;
    if (final<>0) break;            //Si hay pulsaci¢n de ESC o hemos perdido
    end
    ++nivel;                        //Aumentamos el nivel
    ++fase;
    if (fase>=numfases)
      fase=0;
      inc_ini_vel+=30;
    end
    //debug;
    unload_fpg(fondofase);          //Descargamos el fondo
    fade_off();
    let_me_alone();
    FRAME;
    delete_text(all_text);          //Borramos e iniciamos escritura...
    write(letinterf,160,60,4,"Has pasado al siguiente nivel");
  END                               //FIN DE BUCLE DE JUEGO
  let_me_alone();
  if (final==2)                     //Si hemos pulsado ESC, acabamos partida
    break;
  end
  restore_type=complete_restore;
  dump_type=complete_dump;

  clear_screen();
  delete_text(all_text);
  frame;frame;frame;frame;
                                    //Escribimos puntuaci¢n
  write (letinterf,160,20,4,"Puntuacion");
  write_int(letinterf,160,50,4,offset(puntuacion));
  fade_on();

  //Aqui se almacena el c¢digo para actualizar tabla de marcas

  if (mejores[15].puntuacion<=puntuacion)
    for (i=15;i>1;--i) //bucle para resituar las marcas
      if (mejores[i-1].puntuacion>puntuacion)
        break;
      end
      for (j=0;j<=5;++j)
        mejores[i].nombre[j]=mejores[i-1].nombre[j];
      end
      mejores[i].puntuacion=mejores[i-1].puntuacion;
    end
    write (letinterf,160,90,4,"Posicion");
    write_int(letinterf,160,120,4,offset(i));
    mejores[i].puntuacion=puntuacion;


    write (letinterf,160,160,4,"Introduce tu nombre");

    dest=intro_record(i,190);       //Escribe el nombre
    j=0;
    punt_fase=marca(200,0);         //Sit£a la marca sobre la 1¦ letra

    while(not key(bomba))           //Acaba si pulsamos la bomba
      if (key(izqda))
        if (mejores[i].nombre[j]>0) //Si pulsamos izqda. rotamos la letra
          --mejores[i].nombre[j];   //que toca (j) hacia abajo
        else
          mejores[i].nombre[j]=37;
        end
        dest.baja=-1;               //borramos nombre y lo volvemos a poner
        dest=intro_record(i,190);
        for(color=0;color<5;++color);
          frame;                    //Esperamos un poco para que no cambie muy
        end                         //deprisa
      end
      if (key(dcha))
        if (mejores[i].nombre[j]<37)//Si dcha., aumentamos valor de la letra
          ++mejores[i].nombre[j];
        else
          mejores[i].nombre[j]=0;
        end

        dest.baja=-1;
        dest=intro_record(i,190);
        for(color=0;color<5;++color);
          frame;
        end
      end
      if (key(rotar))               //Cambiamos a siguiente letra
        j=(j+1) mod 6;
        punt_fase.baja=j;
        for(color=0;color<5;++color);
          frame;
        end
      end
      if (key(abajo))               //Cambiamos a anterior
        j=(j-1) mod 6;
        punt_fase.baja=j;
        for(color=0;color<5;++color);
          frame;
        end
      end
      frame;
    end                             //Almacenamos la tabla de records
    save ("user\qa\mejores.dat",offset(mejores),sizeof(mejores));
  end


  END //FIN DEL BUCLE PRINCIPAL
  exit("Espero que te haya gustado. (RSG)",0);
end

//==============================================================================
// PROCESO Marca
// DATOS  Y Coordenada vertical
//        Baja Indice de la letra
// USO Marca la letra del nombre que se va a modificar
//==============================================================================

process marca(y,baja)
begin
  file=expl_graf;
  graph=2;
  loop
    x=150+baja*18;
    frame;
  end
end

//==============================================================================
// PROCESO Intro_Record
// DATOS  Indice Posici¢n en la tabla de marcas que se tiene
//        Y Coordenada vertical
// USO Escribe los datos que se van a grabar en la tabla
//==============================================================================

process intro_record(indice,y)
private
  miletra[5];
begin
  x=100;                            //Escribe la puntuaci¢n
  altu=write_int(letinterf,x,y,5,offset(mejores[indice].puntuacion));
  for (baja=0;baja<=5;++baja)       //Escribe el nombre actual
    miletra[baja]=letra_record(indice,baja);
  end
  baja=0;
  while (baja==0)                   //Esperamos que sea modificada baja
    frame;
  end
  for (baja=0;baja<=5;++baja)       //Elimina las letras
    delete_text(miletra[baja].altu);
    signal(miletra[baja],s_kill);
  end
  delete_text(altu);                //Elimina la puntuaci¢n
end

//==============================================================================
// PROCESO Otro
// USO Dibuja la siguiente pieza que va a caer
//==============================================================================

process otro()
private uno;dos;
begin
  x=280;
  y=20;
  if (est_bomb==preparada)    //Dibujamos el gr fico de la bomba
    file=bomb_graf;
    graph=1;
    y+=tam;
    loop
      frame;
      frame;                  //Espera 2 frames para actualizar el gr fico
      if (graph==24)          // (25 por seg)
        graph=1;
      else
        ++graph;
      end
    end
  else                              //Si no tiramos la bomba...
    color=prepara[1];
    graph=color;
    uno=simil(-tam,prepara[2]);     //Ponemos dibujos de la pr¢xima pieza
    dos=simil(-2*tam,prepara[3]);
  end;
  loop
    FRAME;
    signal(uno,s_freeze);           //Congelo (no se han de mover)
    signal(dos,s_freeze);
    signal(id,s_freeze);
  end
end

//==============================================================================
// PROCESO Obj_Bomb
// USO La bomba, cuando cae. Tambi‚n se queda esperando ser destruida por un
//     Compro
//==============================================================================

process obj_bomb()
private
  y_deci;                           //Coord. Vert. con dos decimales
  pos=cols/2;                       //Columna
  mov=0;                            //Hacia donde se mueve
  excep=0;                          //Detecta contacto con una pieza
  cont_bomb=0;                      //Control de la animaci¢n

begin
  y_deci=-tam*100/2;
  x=ancini+(cols/2)*tam;
  y=y_deci/100;
  baja=0;

  color=1;                          //Color de la bomba (para que no confunda
  file=bomb_graf;                   //a los "compro"
  graph=1;
  est_bomb=lanzada;
  signal(type otro,s_kill_tree);    //Eliminamos el "otro"
  otro();                           // Dibujamos la siguiente pieza
  FRAME;
  LOOP                              //Principio del bucle de ca¡da
    if (excep==0)                   //Control del movimiento izqda/dcha si no
      if ((key(dcha))&&(y<=altura-ultimo[pos+1]*tam))   //roza.
        if (not(key(izqda)))
          if (mov==-1) pos-=1; end
          mov=1;
        end
      END
      if ((key(izqda))&&(y<=altura-ultimo[pos-1]*tam))
        if (mov==1) pos+=1; end
        mov=-1;
      END
    end
    x+=mov*2;                       //Variaci¢n de la coordenada horizontal +o-
    if (x<ancini+tam)               //Control de bordes
      x=ancini+tam;
      mov=0;
      END
    if (x>ancini+cols*tam-tam)
      x=ancini+cols*tam-tam;
      mov=0;
      END
    if ((x-ancini) mod tam==0)      //Pasamos sobre una columna (podemos parar)
      pos+=mov;
      excep=0;
      mov=0;
      END
    if ((excep==0)&&(key(abajo)))   //tecla de bajar
      y_deci+=200;
    END
    y_deci+=vel_y;                  //Incremento de la Y
    y=y_deci/100;
                                    //Si tocamos una ficha...
    if ((y>altura-ultimo[pos]*tam) || (y>altura-ultimo[pos+mov]*tam))
      if (mov==0) break; end        //Posici¢n v lida:acabamos.
      y_deci=100*y-1;               //Corregimos posici¢n
      y=y_deci/100;
      if (excep==0)                 //Estamos rozando
        excep=1;
        if (not roce_suena)         //Ruido de roce
          canal_roce=sound(roce,200,256);
          roce_suena=true;
        end
      end
    else
      if (roce_suena)               //No debe sonar el roce
        roce_suena=false;
        stop_sound(canal_roce);
      end
    END                             //Si est  superpuesto, a£n hay que subirlo
    while ((y>altura-ultimo[pos]*tam) || (y>altura-ultimo[pos+mov]*tam))
      y_deci=100*y-1;
      y=y_deci/100;
    END
    FRAME;
    if (cont_bomb==1)               //¨Cambiamos el gr fico?
      cont_bomb=0;
      if (graph==24)
        graph=1;
      else
        ++graph;
      end
    else
      cont_bomb=1;
    end
  END                               //Fin bucle de ca¡da
  if (roce_suena)
    roce_suena=false;               //Paramos sonido roce
    stop_sound(canal_roce);
  end
  //x=ancini+tam*pos;
  y=altura-ultimo[pos]*tam;
  fijos[pos].alt[ultimo[pos]]=id;   //Lo situamos en el panel de objetos
  baja=-1;
  ultimo[pos]+=1;

  estado=detectando;                //Indicamos al programa principal que
  vel_y+=2;                         //inicie destrucci¢n
  if (vel_y>fase_var[fase].maxi_vel_y+inc_ini_vel)
    vel_y=fase_var[fase].mini_vel_y+inc_ini_vel;
  end                               //Incrementamos la velocidad de ca¡da (o no)
  //signal (id,s_freeze);
  LOOP
    FRAME;                          //Espera ser destruido (pronto)
    if (cont_bomb==1)
      cont_bomb=0;
      if (graph==24)
        graph=1;
      else
        ++graph;
      end
    else
      cont_bomb=1;
    end
  END
End


//==============================================================================
// PROCESO Cuadro
// USO Es la pieza mientras cae. Cuando acaba, se destruye.
//==============================================================================

process cuadro()
private
  y_deci;                           //Coord. Vert. con dos decimales
  pos=cols/2;                       //Columna
  mov=0;                            //Hacia donde se mueve
  excep=0;                          //Detecta contacto con una pieza
  hij1;                             //Id. del segundo color
  hij2;                             //Id. del tercer color
  i;

begin
  y_deci=-tam*100/2;
  x=ancini+(cols/2)*tam;
  y=y_deci/100;
  baja=0;

  color=prepara[3];                 //Toma colores del vector prepara
  graph=color;

  hij1=simil(tam,prepara[2]);
  hij2=simil(2*tam,prepara[1]);     //Dise¤a nuevo prepara
  prepara[1]=rand(fase_var[fase].col_ini,fase_var[fase].col_fin);
  prepara[2]=rand(fase_var[fase].col_ini,fase_var[fase].col_fin);
  prepara[3]=rand(fase_var[fase].col_ini,fase_var[fase].col_fin);
  signal(type otro,s_kill_tree);    //Elimina la muestra
  otro();                           //Y crea otra
  FRAME;
  LOOP                              //Principio bucle de ca¡da
    if (excep==0)                   //Control del movimiento izqda/dcha si no
      if ((key(dcha))&&(y<=altura-ultimo[pos+1]*tam))//si no roza
        if (not(key(izqda)))
          if (mov==-1) pos-=1; end
          mov=1;
        end
      END
      if ((key(izqda))&&(y<=altura-ultimo[pos-1]*tam))
        if (mov==1) pos+=1; end
        mov=-1;
      END
    end
    x+=mov*2;                       //Variaci¢n de la coordenada horizontal +o-
    if (x<ancini+tam)               //Control de bordes
      x=ancini+tam;
      mov=0;
      END
    if (x>ancini+cols*tam-tam)
      x=ancini+cols*tam-tam;
      mov=0;
      END
    if ((x-ancini) mod tam==0)      //Pasamos por encima de una columna
      pos+=mov;
      excep=0;
      mov=0;
      END
    if (timer>25)                   //Tecla de rotar colores (max: 4 por seg.)
      if (key(rotar))
        sound(rui_rota,150,256);
        i=color;
        graph=hij1.color;
        color=hij1.color;
        hij1.graph=hij2.color;
        hij1.color=hij2.color;
        hij2.graph=i;
        hij2.color=i;
        timer=0;           // inicio el contador de tiempo
      end
    END                             //Tecla de lanzar bomba
    if ((key(bomba))&&(est_bomb==nada)&&(bomb_actu>0))
      est_bomb=preparada;
      signal(type otro,s_kill_tree);
      otro();
      --bomb_actu;
    end                             //Tecla de bajar
    if ((excep==0)&&(key(abajo)))
      y_deci+=200;
    END
    y_deci+=vel_y;                  //Incremento de la Y
    y=y_deci/100;
                                    //Si tocamos una ficha...
    if ((y>altura-ultimo[pos]*tam) || (y>altura-ultimo[pos+mov]*tam))
      if (mov==0) break; end        //Posici¢n v lida: acabamos
      y_deci=100*y-1;               //Corregimos la Y
      y-=1;
      if (excep==0)                 //Estamos rozando
        excep=1;
        if (not roce_suena)
          canal_roce=sound(roce,200,256);
          roce_suena=true;          //Ruido de roce
        end
      end
    else
      if (roce_suena)               //Paramos roce
        roce_suena=false;
        stop_sound(canal_roce);
      end
    END                             //Si est  superpuesto, a£n hay que subirlo
    while ((y>altura-ultimo[pos]*tam) || (y>altura-ultimo[pos+mov]*tam))
      y_deci=100*y-1;
      y-=1;
    END
    FRAME;
  END                               //Fin bucle de ca¡da
  if (roce_suena)                   //Paramos sonido roce
    roce_suena=false;
    stop_sound(canal_roce);
  end                               //Generamos los procesos que nos van a
                                    //substituir
  fijos[pos].alt[ultimo[pos]]=estatic(pos,ultimo[pos],color);
  fijos[pos].alt[ultimo[pos]].baja=-1;
  ultimo[pos]+=1;                   //Incrementamos los objetos de la col.
  fijos[pos].alt[ultimo[pos]]=estatic(pos,ultimo[pos],hij1.color);
  fijos[pos].alt[ultimo[pos]].baja=-1;
  ultimo[pos]+=1;
  signal(hij1,s_kill);              //Eliminamos los colores 2§ y 3§
  fijos[pos].alt[ultimo[pos]]=estatic(pos,ultimo[pos],hij2.color);
  fijos[pos].alt[ultimo[pos]].baja=-1;
  ultimo[pos]+=1;
  signal(hij2,s_kill);

  if (ultimo[pos]>topesup)          //¨Nos hemos pasado?
    final=1;                        //Pues se acaba la partida
    sound(mus_perder,200,256);
    timer=0;
    while (timer<200)
      frame;
    end
  end
  estado=detectando;                //Dejamos que el proceso principal
  vel_y+=2;                         //detecte si hay que eliminar piezas
  if (vel_y>fase_var[fase].maxi_vel_y+inc_ini_vel)
    vel_y=fase_var[fase].mini_vel_y+inc_ini_vel;
  end                               //Aumentamos (o disminuimos) velocidad
End

//==============================================================================
// PROCESO Falso
// USO No hace nada, existe para llenar espacios vac¡os con color imposible.
//==============================================================================

process falso()
begin
  color=0;                          //Color imposible (distinto de la bomba)
  signal(id,s_freeze);              //Se congela
  LOOP
    FRAME;
  END
end

//==============================================================================
// PROCESO Simil
// DATOS  Disty Distancia vertical desde su padre
//        Color Color, gr fico asociado.
// USO Acompa¤a a su padre, sea cuadro() u otro().
//==============================================================================

process simil(disty,color)

begin
  graph=color;                      //Me pongo del color ordenado
  priority=-1;                      //Entro en acci¢n despu‚s que los dem s
  LOOP
    x=father.x;                     //Me sit£o por encima de mi pap  cada imagen
    y=father.y-disty;               //(Eso, si no me congelan)
    FRAME;
  end
end

//==============================================================================
// PROCESO Estatic
// DATOS  Colu columna que ocupa
//        Altu altura en la columna
//        Color color y gr fico usado
// USO Permanece en su posici¢n hasta ser destruido por compro, o hasta que le
//     modifican la variable Baja, para que baje a otra posici¢n.
//==============================================================================

process estatic(colu,altu,color)
begin
  graph=color;                      //Gr fico adecuado
  x=ancini+tam*colu;                //Posici¢n adecuada
  y=altura-altu*tam;
  signal (id,s_freeze);             //...Y me congelo
  LOOP
    if (baja>0)                     //Si alguien me descongela, ser  para
      baja-=2;                      //que baje
      y+=2;
      if (baja<=0)                  //Si he acabado de bajar
        baja=-1;                    //pongo baja a -1 para que el me detecte
        contador-=1;                //el proceso principal, preparo el sonido
        prepara_golpe=true;         //(por si varios a la vez) y me congelo
        signal (id,s_freeze);       // -qu‚ fr¡o-
      end
    end                             //Si quieres que vaya m s r pido el juego
    FRAME;                          //en vez de congelarlos, deber¡as buscar
  END                               //otra forma de almacenar colores en fijos
end                                 //y eliminar los procesos estatic, pintan-
                                    //do su imagen en el fondo de la pantalla

//==============================================================================
// PROCESO Puntos
// DATOS  X, Y Coordenadas iniciales
//        Altu Valor que muestra
// USO Muestra los puntos obtenidos por el cuadrado que desaparece. Va subiendo.
//==============================================================================

process puntos(x,y,altu)
begin
baja=0;
while(baja==0)                      //Espero a que me modifiquen baja
  FRAME;
end
if (altu>0)                         //Altu contiene el valor que he de mostrar
  color=write_int(letrpunt,x,y,4,OFFSET(altu));
  while (y>-tam/2)                  //Mientras se vea, voy subiendo el numerito
    y-=2;
    move_text(color,x,y);
    FRAME;
  end;
  delete_text(color);               //Hay que borrarlo.
end
end

//==============================================================================
// PROCESO Compro
// DATOS  Colu, Altu Columna y Altura en la que se sit£a
//        Color Color asociado.
// USO Se reproduce en los cuadros vecinos si tienen el mismo color, anotandose
//     en Conexi. Si no recibe una contraorden se destruye acabando con el
//     estatic correspondiente.
//==============================================================================

process compro(colu,altu,color)
private
proceso;colu;point;i;

begin
  numcont+=1;                       //Aumento el contador de conexiones
  conexi[colu].alt[altu]=id;        //Me apunto en las conexiones
  x=ancini+tam*colu;                //Me sit£o
  y=altura-altu*tam;
  z=-1;                             //Me aseguro que me pintan por delante
  point=puntos(x,y,valor);          //Pongo el indicador de puntos (aunque no
                                    //lo activo
  file=expl_graf;                   //Gr ficos animaci¢n
  graph=0;                          //De momento, no pinto nada
  if (colu>=1)                      //Intento reproducirme
    if((conexi[colu-1].alt[altu]==idfalso)&&
      (fijos[colu-1].alt[altu].color==color))
      compro(colu-1,altu,color);
      fijos[colu-1].alt[altu].baja=0;
    end
  end
  if (colu<=cols-2)
    if((conexi[colu+1].alt[altu]==idfalso)&&
      (fijos[colu+1].alt[altu].color==color))
      fijos[colu+1].alt[altu].baja=0;
      compro(colu+1,altu,color);
    end
  end
  if (altu>=1)
    if((conexi[colu].alt[altu-1]==idfalso)&&
      (fijos[colu].alt[altu-1].color==color))
      fijos[colu].alt[altu-1].baja=0;
      compro(colu,altu-1,color);
    end
  end
  if (altu<=topesup+2)
    if((conexi[colu].alt[altu+1]==idfalso)&&
      (fijos[colu].alt[altu+1].color==color))
      fijos[colu].alt[altu+1].baja=0;
      compro(colu,altu+1,color);
    end
  end
  //debug;
  FRAME;                            //Ahora, que decida el proc. principal
                                    //si me elimina o no
  if (baja<0)                       //Si me elimina, pone baja a -1
    signal(point,s_kill);           //Elimino los puntos
    if (colu>=1)                    //Busco mi descendencia y la elimino
      if((conexi[colu-1].alt[altu]<>idfalso)&&
        (fijos[colu-1].alt[altu].color==color))
        conexi[colu-1].alt[altu].baja=-1;
        conexi[colu-1].alt[altu].priority=10;
      end                           //(Hay que ponerles baja a -1)
    end
    if (colu<=cols-2)
      if((conexi[colu+1].alt[altu]<>idfalso)&&
        (fijos[colu+1].alt[altu].color==color))
        conexi[colu+1].alt[altu].baja=-1;
        conexi[colu+1].alt[altu].priority=10;
      end
    end
    if (altu>=1)
      if((conexi[colu].alt[altu-1]<>idfalso)&&
        (fijos[colu].alt[altu-1].color==color))
        conexi[colu].alt[altu-1].baja=-1;
        conexi[colu].alt[altu-1].priority=10;
      end
    end
    if (altu<=topesup+2)
      if((conexi[colu].alt[altu+1]<>idfalso)&&
        (fijos[colu].alt[altu+1].color==color))
        conexi[colu].alt[altu+1].baja=-1;
        conexi[colu].alt[altu+1].priority=10;
      end
    end
    conexi[colu].alt[altu]=idfalso; //Me borro de la tabla de conexiones
    return;
  end
  For (graph=1;graph<10;graph++)    //Ahora s¡ dibujo el gr fico
    FRAME;                          //durante un ratito
    if (baja>0)                     //No creo que me quiten el de abajo, pero
      y+=2;baja-=2;                 //por si acaso...
    end
  end
  if (expl_activa)                  //Expulso restos, si est n activas las exp.
    for (i=0;i<4;++i)
      restos(x,y);
    end
  end
  For (graph=10;graph>1;graph--)    //Contin£o la animaci¢n al rev‚s
    FRAME;
    if (baja>0)
      y+=2;baja-=2;
    end
  end
  conexi[colu].alt[altu]=idfalso;   //Me borro de la lista
  contador-=1;                      //Y decremento el contador

  if (fijos[colu].alt[altu]==idfalso) debug;
  end                               //Esto jam s deber¡a ocurrir
  if (fijos[colu].alt[altu].baja>0) //Puede que elimine uno que caiga
    contador-=1;                    // y si es as¡ hay que decrementar contador
  end
  signal(fijos[colu].alt[altu],s_kill);
  ultimo[colu]-=1;                  //Bueno, pues lo elimino.
  loop                              //¨Tiene que bajar alguno?
    if (altu>=ultimo[colu])         //Si es as¡, ya hemos acabado
      break;
    end                             //Cambia posiciones
    fijos[colu].alt[altu]=fijos[colu].alt[altu+1];
    conexi[colu].alt[altu]=conexi[colu].alt[altu+1];
    if(conexi[colu].alt[altu]<>idfalso) //si hay conexi¢n, b jala
      conexi[colu].alt[altu].baja+=tam;
      conexi[colu].alt[altu].altu-=1;
    end
    if (fijos[colu].alt[altu]==idfalso)
      debug;                        //No debe ocurrir
    end
    if (fijos[colu].alt[altu].baja==0)
      contador+=1;                  //¨Estaba quieto?
    end;
    fijos[colu].alt[altu].baja+=tam;//Lo que he de bajar, y despertalo
    signal(fijos[colu].alt[altu],s_wakeup);
    ++altu;                         //Pasamos al de arriba...
  end
  fijos[colu].alt[altu]=idfalso;    //"Borramos" al £ltimo de la columna
  conexi[colu].alt[altu]=idfalso;
  point.baja=1;                     //Y (casi se me olvida) activamos los puntos
end

//==============================================================================
// PROCESO Test
// DATOS  Graph Gr fico asociado
// USO Aparece desplaz ndose en l¡nea recta por la pantalla. Adorno.
//==============================================================================

process test(graph)
private
x_deci;y_deci;velo;direc; //Coordenadas en decimales, velocidad y direcci¢n
begin
  x_deci=rand(0,32000);   //Todo es aleatorio
  y_deci=rand(0,24000);
  direc=rand(0,360000);
  velo=rand(20,100);
  while (estado==cayendo)
    x=x_deci/100;         //Calcula la posici¢n real
    y=y_deci/100;
    FRAME;                //Incrementa la posici¢n
    x_deci+=get_distx(direc,velo);
    y_deci+=get_disty(direc,velo);
    if (x_deci<-tam*100)  //Si te sales mucho, apareces por el otro lado
      x_deci=32000+tam*100;
    end
    if (y_deci<-tam*100)
      y_deci=24000+tam*100;
    end
    if (x_deci>32000+tam*100)
      x_deci=-tam*100;
    end
    if (y_deci>24000+tam*100)
      y_deci=-tam*100;
    end
  end                     //Si est n activas las explosiones, activamos restos
  if (expl_activa) for (direc=0;direc<3;++direc) restos(x,y); end end
end

//==============================================================================
// PROCESO Restos
// DATOS  X,Y Coordenadas originales
// USO Empezando en estas coordenadas, selecciona una direcci¢n aleatoria y se
//     desplaza haciendo caso de una especie de gravedad.
//==============================================================================

process restos(x,y)
private
x_deci;y_deci;velo_x;velo_y;direc;
begin
  y+=rand(-tam/2,tam/2);       //Para que no salga todo del mismo punto
  x+=rand(-tam/2,tam/2);       //vario un poco las coordenadas
  file=expl_graf;
  graph=1;
  x_deci=x*100;y_deci=y*100;
  direc=rand(0,360000);        //Direcci¢ aleatoria
  velo_x=get_distx(direc,150); //Componentes de la velocidad
  velo_y=get_disty(direc,150);
  while (y<=240)               //Hasta que sale por debajo de la pantalla
    frame;
    x_deci+=velo_x;
    x=x_deci/100;
    y_deci+=velo_y;
    y=y_deci/100;
    velo_y+=4;                 //Act£a la aceleraci¢n de la "gravedad"
  end
end

//==============================================================================
// PROCESO Logo
// USO Presenta el logotipo del juego.
//==============================================================================

process logo()
begin
  x=160;
  graph=load_map("user\qa\qa.map");
  for (y=-140;y<120;y+=2)
    frame;
  end
  loop
    frame;
  end
end

//==============================================================================
// PROCESO Menu
// USO Interacciona con el usuario para que seleccione las distintas opciones.
//==============================================================================

process menu()
private
i;j;alter;music;redibujar;
begin

  music=load_pcm("user\qa\music.pcm",0);
  alter=logo();
  sound(music,200,256);
  textomenu();
                                    //Bucle del men£
  loop                              //Si pulsas "Alt+b" borras las marcas
    if (key(_b)&&((shift_status AND 8)==8))
      debug;
      for(i=0;i<=15;++i)
        mejores[i].puntuacion=0;
        for (j=0;j<=5;++j)
          mejores[i].nombre[j]=0;
        end
      end
      save ("prg\cuadr\mejores.rec",offset(mejores),sizeof(mejores));
    end
    if (key(_f2))                   //con F2 salimos (y vamos a jugar)
      break;
    end
    if (key(_esc))                  //con ESC tambi‚n, pero para acabar.
      final=2;
      break;
    end
    if (key(_f1))                   //F1-> Presenta las pantallas de ayuda
      delete_text(all_text);
      ayuda1();
      while (scan_code<>0)
        frame;
      end;
      while (scan_code==0)
        frame;
      end;
      delete_text(all_text);
      ayuda2();
      while (scan_code<>0)
        frame;
      end;
      while (scan_code==0)
        frame;
      end;
      delete_text(all_text);
      ayuda3();
      while (scan_code<>0)
        frame;
      end;
      while (scan_code==0)
        frame;
      end;
      delete_text(all_text);
      ayuda4();
      while (scan_code<>0)
        frame;
      end;
      while (scan_code==0)
        frame;
      end;
      delete_text(all_text);
      textomenu();
    end
    if (key(_f3))                   //Opciones de juego
      while (key(_f3))
        frame;
      end
      redibujar=true;               //Esta variable controla si hay que
                                    //escribir otra vez el texto o no
      loop                          //Bucle de las opciones del juego
        if (redibujar)
          delete_text(all_text);
          write(letinterf,60,50,3,"F1");
          write(letinterf,120,50,3,"Volver al Men£");
          write(letinterf,60,80,3,"F2");
          write(letinterf,120,80,3,"Cambiar Teclas");
          write(letinterf,60,110,3,"F3");
          write(letinterf,120,110,3,"Bombas (S¡/No)");
          write(letinterf,60,140,3,"F4");
          write(letinterf,120,140,3,"Explosiones (S¡/No)");
          redibujar=false;
        end
        frame;
        if ((key(_esc)) OR (key(_f1)))//As¡ salimos.
          break;
        end
        if (key(_f2))               //Selecci¢n de teclado
          delete_text(all_text);
          while (key(_f2))
            frame;
          end
          write(letinterf,160,120,4,"Pulsa tecla para izquierda");
          while (scan_code==0)
            frame;
          end;
          izqda=scan_code;
          while (scan_code<>0)
            frame;
          end
          delete_text(all_text);
          write(letinterf,160,120,4,"Pulsa tecla para derecha");
          while (scan_code==0)
            frame;
          end;
          dcha=scan_code;
          while (scan_code<>0)
            frame;
          end
          delete_text(all_text);
          write(letinterf,160,120,4,"Pulsa tecla para bajar");
          while (scan_code==0)
            frame;
          end;
          abajo=scan_code;
          while (scan_code<>0)
            frame;
          end
          delete_text(all_text);
          write(letinterf,160,120,4,"Pulsa tecla para rotar colores");
          while (scan_code==0)
            frame;
          end;
          rotar=scan_code;
          while (scan_code<>0)
            frame;
          end
          delete_text(all_text);
          write(letinterf,160,120,4,"Pulsa tecla para lanzar bomba");
          while (scan_code==0)
            frame;
          end;
          bomba=scan_code;
          redibujar=true;
        end
        if (key(_f3))               //Activaci¢n de bombas
          delete_text(all_text);
          if (bomb_activa)
            i=write (letinterf,160,80,4,"Bombas activadas");
          else
            i=write (letinterf,160,80,4,"Bombas desactivadas");
          end
          write(letinterf,60,110,3,"F1");
          write(letinterf,120,110,3,"Volver men£ anterior");
          write(letinterf,60,140,3,"F2");
          write(letinterf,120,140,3,"Cambiar estado");
          while (scan_code<>0)
            frame;
          end
          loop
            frame;
            if ((key(_f1)) or (key(_esc)))
              break;
            end
            if (key(_f2))
              bomb_activa=not(bomb_activa);
              delete_text(i);
              if (bomb_activa)
                i=write (letinterf,160,80,4,"Bombas activadas");
              else
                i=write (letinterf,160,80,4,"Bombas desactivadas");
              end
              while (key(_f2))
                frame;
              end
            end
          end
          while(key(_f1))           //Hay que asegurarse de que suelta la tecla
            frame;
          end
          redibujar=true;
        end
        if (key(_f4))
          delete_text(all_text);
          if (expl_activa)
            i=write (letinterf,160,80,4,"Explosiones activadas");
          else
            i=write (letinterf,160,80,4,"Explosiones desactivadas");
          end
          write(letinterf,60,110,3,"F1");
          write(letinterf,120,110,3,"Volver men£ anterior");
          write(letinterf,60,140,3,"F2");
          write(letinterf,120,140,3,"Cambiar estado");
          while (scan_code<>0)
            frame;
          end
          loop
            frame;
            if ((key(_f1)) or (key(_esc)))
              break;
            end
            if (key(_f2))
              expl_activa=not(expl_activa);
              delete_text(i);
              if (expl_activa)
                i=write (letinterf,160,80,4,"Explosiones activadas");
              else
                i=write (letinterf,160,80,4,"Explosiones desactivadas");
              end
              while (key(_f2))
                frame;
              end
            end
          end
          while (key(_f1))
            frame;
          end
          redibujar=true;
        end
      end
      delete_text(all_text);
      while (key(_f1))
        frame;
      end
      textomenu();
    end
    if (key(_f4))                   //Selecci¢n de nivel
      while (scan_code<>0)
        frame;
      end
      delete_text(all_text);
      write(letinterf,160,90,4,"Nivel");
      write_int(letinterf,160,120,4,offset(nivel_inicio));
      while (scan_code==0)
        frame;
      end;
      loop
        switch (scan_code)
          case (rotar):
            break;
          end
          case (izqda):
            if (nivel_inicio>1)
              --nivel_inicio;
            end
          end
          case (dcha):
            ++nivel_inicio;
          end
        end
        frame;
      end
      delete_text(all_text);
      textomenu();
    end
    if (key(_f5))           //Presentar las mejores marcas
      while (scan_code<>0)
        frame;
      end
      delete_text(all_text);
      record();
      loop
        switch (scan_code)
          case (rotar):
            break;
          end
          case (izqda):
            if (son.y<20)
              son.y+=15;
            end
          end
          case (dcha):
            if (son.y>220-15*30)
              son.y-=15;
            end
          end
        end
        frame;
      end
      signal(son,s_kill_tree);
      delete_text(all_text);
      textomenu();
    end
    frame;
  end

  estado=cayendo;                   //Vamos a empezar juego
  unload_pcm(music);
  unload_map(alter.graph);          //Eliminamos el logotipo
  signal(alter,s_kill);

end

//==============================================================================
// PROCESO Record
// USO Proceso que se encarga de poner la tabla de records
//==============================================================================

process record ()
begin
  x=160;y=20;
  altu=write(letinterf,x,y,4,"Las mejores marcas");
  for (baja=1;baja<=15;++baja)
    linea_record(baja);
  end;
  loop
    move_text(altu,x,y);            //Puede que me las cambien desde men£
    frame;
  end
end

//==============================================================================
// PROCESO Linea_Record
// DATOS  Indice Posici¢n en la tabla.
// USO Dibuja la puntuaci¢n y controla la aparici¢n de las letras de una l¡nea
//     de la tabla de records.
//==============================================================================

process linea_record(indice)
begin
  priority=-1;
  x=100;y=father.y+indice*30;
  altu=write_int(letinterf,x,y,5,offset(mejores[indice].puntuacion));
  for (baja=0;baja<=5;++baja)
    letra_record(indice,baja);
  end
  loop
    y=father.y+indice*30;           //sigo a pap  (tengo menor prioridad)
    move_text(altu,x,y);
    frame;
  end
end

//==============================================================================
// PROCESO Letra_Record
// DATOS  Indice Posici¢n en la tabla.
//        N_Letra N£mero de la letra.
// USO Coloca la letra que corresponde a estos datos.
//==============================================================================

process letra_record(indice,n_letra)
begin
  priority=-1;
  x=150+18*n_letra;y=father.y;      //Ser¡a m s elegante con una lista
                                    //pero cuando me di cuenta, ya estaba
  switch (mejores[indice].nombre[n_letra])
    case 0:
      altu=write(letinterf,x,y,4,'-');
    end
    case 1:
      altu=write(letinterf,x,y,4,'A');
    end
    case 2:
      altu=write(letinterf,x,y,4,'B');
    end
    case 3:
      altu=write(letinterf,x,y,4,'C');
    end
    case 4:
      altu=write(letinterf,x,y,4,'D');
    end
    case 5:
      altu=write(letinterf,x,y,4,'E');
    end
    case 6:
      altu=write(letinterf,x,y,4,'F');
    end
    case 7:
      altu=write(letinterf,x,y,4,'G');
    end
    case 8:
      altu=write(letinterf,x,y,4,'H');
    end
    case 9:
      altu=write(letinterf,x,y,4,'I');
    end
    case 10:
      altu=write(letinterf,x,y,4,'J');
    end
    case 11:
      altu=write(letinterf,x,y,4,'K');
    end
    case 12:
      altu=write(letinterf,x,y,4,'L');
    end
    case 13:
      altu=write(letinterf,x,y,4,'M');
    end
    case 14:
      altu=write(letinterf,x,y,4,'N');
    end
    case 15:
      altu=write(letinterf,x,y,4,'¥');
    end
    case 16:
      altu=write(letinterf,x,y,4,'O');
    end
    case 17:
      altu=write(letinterf,x,y,4,'P');
    end
    case 18:
      altu=write(letinterf,x,y,4,'Q');
    end
    case 19:
      altu=write(letinterf,x,y,4,'R');
    end
    case 20:
      altu=write(letinterf,x,y,4,'S');
    end
    case 21:
      altu=write(letinterf,x,y,4,'T');
    end
    case 22:
      altu=write(letinterf,x,y,4,'U');
    end
    case 23:
      altu=write(letinterf,x,y,4,'V');
    end
    case 24:
      altu=write(letinterf,x,y,4,'W');
    end
    case 25:
      altu=write(letinterf,x,y,4,'X');
    end
    case 26:
      altu=write(letinterf,x,y,4,'Y');
    end
    case 27:
      altu=write(letinterf,x,y,4,'Z');
    end
    case 28:
      altu=write(letinterf,x,y,4,'0');
    end
    case 29:
      altu=write(letinterf,x,y,4,'1');
    end
    case 30:
      altu=write(letinterf,x,y,4,'2');
    end
    case 31:
      altu=write(letinterf,x,y,4,'3');
    end
    case 32:
      altu=write(letinterf,x,y,4,'4');
    end
    case 33:
      altu=write(letinterf,x,y,4,'5');
    end
    case 34:
      altu=write(letinterf,x,y,4,'6');
    end
    case 35:
      altu=write(letinterf,x,y,4,'7');
    end
    case 36:
      altu=write(letinterf,x,y,4,'8');
    end
    case 37:
      altu=write(letinterf,x,y,4,'9');
    end
  end
  loop
    y=father.y;
    move_text(altu,x,y);
    frame;
  end
end

//==============================================================================
// PROCESO Textomenu
// USO Escribe los textos del men£ principal.
//==============================================================================

process textomenu();
begin
  write(letinterf,60,50,3,"F1");
  write(letinterf,120,50,3,"Instrucciones");
  write(letinterf,60,80,3,"F2");
  write(letinterf,120,80,3,"Empezar el Juego");
  write(letinterf,60,110,3,"F3");
  write(letinterf,120,110,3,"Opciones del Juego");
  write(letinterf,60,140,3,"F4");
  write(letinterf,120,140,3,"Seleccionar Nivel");
  write(letinterf,60,170,3,"F5");
  write(letinterf,120,170,3,"Ver Marcas");
end;

//==============================================================================
// PROCESO Ayuda1
// USO Escribe los textos de la ayuda. P gina 1.
//==============================================================================

process ayuda1()
begin
  write(letinterf,10,15,3,"El objetivo del juego");
  write(letinterf,25,40,3,"es juntar piezas de un mismo");
  write(letinterf,25,65,3,"color, hasta lograr acumular");
  write(letinterf,25,90,3,"una cierta cantidad.");
  write(letinterf,10,115,3,"Puedes mover las que caen,");
  write(letinterf,25,140,3,"incluso rotar sus colores.");
  write(letinterf,10,165,3,"Cuando consigues una cierta");
  write(letinterf,25,190,3,"cantidad de puntos, cambia");
  write(letinterf,25,215,3,"el nivel.");
end

//==============================================================================
// PROCESO Ayuda2
// USO Escribe los textos de la ayuda. P gina 2.
//==============================================================================

process ayuda2()
begin
  write(letinterf,10,15,3,"Puedes lanzar una bomba si");
  write(letinterf,25,40,3,"amontonas demasiadas piezas,");
  write(letinterf,25,65,3,"pero dispones de un n£mero");
  write(letinterf,25,90,3,"limitado.");
  write(letinterf,10,115,3,"Al comienzo, puedes optar");
  write(letinterf,25,140,3,"entre varias opciones, como");
  write(letinterf,25,165,3,"jugar sin bombas o sin ex-");
  write(letinterf,25,190,3,"plosiones, con lo que el ");
  write(letinterf,25,215,3,"juego ir  m s r pido.");
end

//==============================================================================
// PROCESO Ayuda3
// USO Escribe los textos de la ayuda. P gina 3.
//==============================================================================

process ayuda3()
begin
  write(letinterf,10,15,3,"En el panel derecho se te");
  write(letinterf,25,40,3,"informa del nivel actual, del");
  write(letinterf,25,65,3,"objetivo (n£mero de piezas que");
  write(letinterf,25,90,3,"debes juntar), de la cantidad de");
  write(letinterf,25,115,3,"colores disponibles y del n£mero");
  write(letinterf,25,140,3,"de bombas, as¡ como de tu pun-");
  write(letinterf,25,165,3,"tuaci¢n y de los puntos que te");
  write(letinterf,25,190,3,"faltan para pasar a la siguiente");
  write(letinterf,25,215,3,"fase.");
end

//==============================================================================
// PROCESO Ayuda4
// USO Escribe los textos de la ayuda. P gina 4.
//==============================================================================

process ayuda4()
begin
  write(letinterf,10,15,3,"Las teclas iniciales son los");
  write(letinterf,25,40,3,"cursores para el movimiento");
  write(letinterf,25,65,3,"y rotar los colores y la tecla");
  write(letinterf,25,90,3,"de espacio para la bomba.");
  write(letinterf,160,140,4,"QA v. 1.2");
  write(letinterf,160,175,4,"Roberto Selva Gomis");
  write(letinterf,160,200,4,"rselva@Tande.com");
  write(letinterf,160,225,4,"(espero tu opini¢n)");
end
