// Programa creado por Jose Luis Arasa con Div.
// Este programa en si no es realmente un juego, sino simplemente una
// demostracion de aventura gr fica (estan las rutinas necesarias para
// este cometido), es como un ejemplo muy b sico.
// Correo electronico:   jara3130@alu-etsetb.upc.es
// Instrucciones: Boton derecho:   Ver
//                Boton izquierdo: Coger,ir,abrir...
//                Tecla Esc.:      Salir del juego/ejemplo

PROGRAM prueba2;
  CONST
    //NUMEROS MAXIMOS
    num_game=9;
    num_objeto=10;
    num_escenario=1;
    num_cosa=4;
    num_coger=7;             //numero maximo de objetos que puede coger

    //acciones posible con un objeto
    leer=1;
    escena=2;
    coger=3;

    //Coordenadas necesarias para los textos
    write_x=319;
    write_y=361;


    //Graficos del fichero_fpg
    cursor_normal=1;
    graph_marcador=2;
    graph_yo=3;
    graph_escenario1=4;
    graph_fondo1=5;
    graph_iglu=6;
    graph_entrada=7;
    graph_alcantilado=8;
    graph_interior_iglu=9;
    graph_caja=10;
    graph_caja_abierta=11;
    graph_llave=12;
    graph_moneda=13;
    graph_cajita=14;
    graph_salida=15;
    graph_fin=16;
    graph_ini=17;


    //COORDENADAS DE LOS OBJETOS
    x_iglu=500;                     y_iglu=250;
    x_ent_iglu=x_iglu+68;           y_ent_iglu=y_iglu+1;
    x_alcantilado=0;                y_alcantilado=350;
    x_caja=47;                       y_caja=285;
    x_cajita=950;                   y_cajita=300;
    x_salida=280;                   y_salida=250;

  GLOBAL

    struct game[num_game]
      yo_xinicio;
      yo_yinicio;
      fase;                   //Escenario o fase donde empiezo
      cogidos[7];             //Objetos que llevo
      estado[9];             //Estado o mutacion en que se halla el objeto
    end=0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;

    struct escenario[num_escenario]
      tipo_escenario;               //c_screen 0 o c_scroll 1
      fondo1;
      fondo2;
      y_max;
      yo_xinicio;
      yo_yinicio;
      tama¤o;
      num_cosas;           //contando el 0
      lista_objetos[4];
    end=
         //ESCENARIO 1
      c_scroll,graph_escenario1,graph_fondo1,125,200,300,100,3,0,1,2,5,0,

         //ESCENARIO 2
      c_screen,graph_interior_iglu,0,290,370,320,200,1,3,7,0,0,0;

    struct objetos[num_objeto]
      x;
      y;
      z;
      graph;
      accion;
      cursor;
      text_right;
      text_left;
      opc;  //coger,respuesta o ir
      animacion; //booleano
      mutacion;
      tipo;      //ctype -> c_scroll,c_m7 o c_screen
    end=
         //IGLU 0
    x_iglu,y_iglu,0,graph_iglu,leer,cursor_normal,"IGLU","¨Vivira alguien dentro?"
       ,"Quizas si.",0,0,c_scroll,

        //entrada IGLU 1
    x_ent_iglu,y_ent_iglu,0,graph_entrada,escena,cursor_normal,"ENTRADA AL IGLU"
       ,"HOLA!!!",1,30,0,c_scroll,

        //ALCANTILADO 2
    x_alcantilado,y_alcantilado,-1,graph_alcantilado,escena,graph_moneda,"ALCANTILADO"
     ,"Necesito la moneda",2,0,0,c_scroll,

       //CAJA CERRADA 3
    x_caja,y_caja,0,graph_caja,coger,graph_llave,"CAJA CERRADA","LA MONEDA",graph_moneda,
    50,1,c_screen,
       //CAJA ABIERTA 4
    x_caja,y_caja,0,graph_caja_abierta,leer,cursor_normal,"CAJA ABIERTA","Esta vacia.",
    "",0,0,c_screen,

       //CAJITA 5

    x_cajita,y_cajita,0,graph_cajita,coger,cursor_normal,"CAJITA","¨Que hay dentro?",graph_llave,
    -30,1,c_scroll,

      //CAJITA VACIA 6
    x_cajita,y_cajita,0,graph_cajita,leer,cursor_normal,"CAJITA","No queda nada dentro","",
    0,0,c_scroll,

      //SALIDA IGLU  7
    x_salida,y_salida,0,graph_salida,escena,cursor_normal,"SALIDA DEL IGLU",
    "QUE FRIO HACE.",0,40,0,c_screen;

    //FICHEROS DEL JUEGO
    fichero_fpg;
    fichero_fnt;

    //identificador del protagonista y sus variables de posicion final
    id2;
    x_meta;
    y_meta;

    //indica nueva fase
    fase;     //variable booleana
    fin;      //final de juego
    partida;  //partida cargada o nueva que se juega
    obj;      //variable booleana que indica si he pulsado sobre un objeto


    //Para escribir

    fuente;

  LOCAL
    num;
    wide;
    y_abajo;
  PRIVATE
    anterior=-1;
  BEGIN
     set_fps(60,1);
     set_mode(m640x480);
     fichero_fpg=load_fpg("user\prueba2\prueba2.fpg");
     fuente=load_fnt("user\prueba2\prueba2.fnt");
     mouse.graph=cursor_normal;
     priority=1000;

     put_screen(fichero_fpg,graph_ini);

    REPEAT
     FRAME;
    UNTIL (mouse.left or mouse.right)
    mouse.left=0;
    mouse.right=0;

    marcador();
    REPEAT
     if(mouse.left or mouse.right)
       delete_text(all_text);
     end
     if(game[partida].fase!=anterior)
      anterior=game[partida].fase;
      nivel(game[partida].fase);
     end

     FRAME;
    UNTIL(key(_esc) or game[partida].fase==2)
    stop_scroll(0);
    let_me_alone();
    if(game[partida].fase==2)
      clear_screen();
      put_screen(fichero_fpg,graph_fin);
      timer=0;
      REPEAT
        FRAME;
      UNTIL ((mouse.left or mouse.right) && timer>300)
    end
    fade_off();
    exit("Gracias por ""jugar"".",0);
  END




PROCESS yo(ctype,y_max,size)
 PRIVATE
  x_tmp;
  y_tmp;
  x_final;
  y_final;
  choque;
  sombra=0;
 BEGIN
   priority=-100;
   id2=id;
   graph=graph_yo;
   if(game[partida].yo_xinicio==0  &&  game[partida].yo_yinicio==0)
     x=escenario[game[partida].fase].yo_xinicio;
     y=escenario[game[partida].fase].yo_yinicio;
   else
     x=game[partida].yo_xinicio;
     y=game[partida].yo_yinicio;
   end
   x_final=x_meta=x;
   y_final=y_meta=y;
   z=y_max-y;

   wide=graphic_info(fichero_fpg,graph,g_wide)>>1;
  LOOP
   if(mouse.left && !obj)
    x_final=mouse.x;
    y_final=mouse.y;
    x_final=x_final+scroll.x0*ctype;
    if(y_final>y_max);
     choque=get_id(type marcador);
     FRAME(0);
     if(y_final>choque.y)
       x_final=x_meta;
       y_final=y_meta;
     else
      x_tmp=x;
      y_tmp=y;
      x=x_final;
      y=y_final;
      choque=collision(type cosa);
      x=x_tmp;
      y=y_tmp;
      if(choque)
       if(choque.y<y_final && choque.y_abajo>y_final)
          if(choque.x>=x_final && choque.x-wide-1<=x_final )
             x_final=choque.x-wide-1;
             x_meta=x_final;
             y_meta=y_final;
          else
            if(choque.wide<x_final && choque.wide+wide+1>x_final)
              x_final=choque.wide+wide+1;
              x_meta=x_final;
              y_meta=y_final;
            else
              if(!obj)
                x_final=x_meta;
                y_final=y_meta;
              end
            end
          end
       else
          if(!(choque.y<=y_final && y_final<=choque.y_abajo &&
              x_final>=choque.x && x_final<=choque.wide))
             x_meta=x_final;
             y_meta=y_final;
          else
            x_final=x_meta;
            y_final=y_meta;
          end
       end
      else
          x_meta=x_final;
          y_meta=y_final;
      end
     end
    else
     x_final=x_meta;
     y_final=y_meta;
    end
   end

   FRAME(0);
   if(x>x_final)
       x--;
       choque=collision(type cosa);
       if(choque)
         if(!(choque.y>=y or y>=choque.y_abajo))
           x++;
           sombra++;
         else
           sombra--;
         end
       else
           sombra--;
       end
   else
     if(x<x_final)
       x++;
       choque=collision(type cosa);
       if(choque)
         if(!(choque.y>=y or y>=choque.y_abajo))
            x--;
            sombra++;
         else
           sombra--;
         end
       else
           sombra--;
       end
     else
       sombra=sombra+2;
     end
   end
   FRAME(0);
   if(y>y_final)
       y--;
       z++;
       choque=collision(type cosa);
       if(choque)
         if(!(choque.y>=y or y>=choque.y_abajo))
             y++;
             z--;
             sombra++;
         else
             sombra--;
         end
       else
         sombra--;
       end
   else
     if(y<y_final)
       y++;
       z--;
       choque=collision(type cosa);
       if(choque)
         if(!(choque.y>=y or y>=choque.y_abajo))
            sombra=1;
            y--;
            z++;
            sombra++;
         else
           sombra--;
         end
       else
           sombra--;
       end
     else
      sombra=sombra+2;
     end
   end

   if(sombra==2 or sombra==3)
     switch(x)
      case choque.x-wide:
                    x_final=x;
                    if(y>y_final)
                       y_final=choque.y;
                    else
                       if(y<y_final)
                         y_final=choque.y_abajo;
                       else
                         if(y-choque.y<choque.y_abajo-y)
                           y_final=choque.y;
                         else
                           y_final=choque.y_abajo;
                         end
                       end
                    end
      end
      case choque.wide+wide:
                    x_final=x;
                    if(y>y_final)
                      y_final=choque.y;
                    else
                      if(y<y_final)
                        y_final=choque.y_abajo;
                      else
                        if(y-choque.y<choque.y_abajo-y)
                           y_final=choque.y;
                         else
                           y_final=choque.y_abajo;
                        end
                      end
                    end
      end
    end
    switch(y)
        case choque.y:
                    y_final=y;
                    if(x>x_final)
                      x_final=choque.x-wide;
                    else
                      if(x<x_final)
                        x_final=choque.wide+wide;
                      else
                        if(x-choque.x<choque.wide-x)
                          x_final=choque.x-wide;
                        else
                          x_final=choque.wide+wide;
                        end
                      end
                    end
        end
        case choque.y_abajo:
                    y_final=y;
                    if(x>x_final)
                      x_final=choque.x-wide;
                    else
                      if(x<x_final)
                        x_final=choque.wide+wide;
                      else
                        if(x-choque.x<choque.wide-x)
                          x_final=choque.x-wide;
                        else
                          x_final=choque.wide+wide;
                        end
                      end
                    end
        end
    end
   end



   if(x==x_final && y==y_final && (x_final!=x_meta or y_final!=y_meta))
     if(choque==0)
       x+=wide;
       y--;
       FRAME(0);
       choque=collision(type cosa);
       x-=wide;
       y++;
     end
     if(choque==0)
       x-=wide;
       y--;
       FRAME(0);
       choque=collision(type cosa);
       x+=wide;
       y++;
     end
     if(choque==0)
       x+=wide;
       y++;
       FRAME(0);
       choque=collision(type cosa);
       x-=wide;
       y--;
     end
     if(choque==0)
       x-=wide;
       y++;
       FRAME(0);
       choque=collision(type cosa);
       x+=wide;
       y--;
     end
     if(choque)
      if(x==choque.x-wide && y==choque.y)
       if(x_meta>choque.x-wide && y_meta>choque.y_abajo)
         y_final=choque.y_abajo;
       else
         if(x_meta>choque.wide+wide && y_meta>choque.y)
           x_final=choque.wide+wide;
         else
           y_final=y_meta;
           x_final=x_meta;
         end
       end
      else
       if(x==choque.wide+wide && y==choque.y)
         if(x_meta<choque.wide && y_meta>choque.y_abajo)
           y_final=choque.y_abajo;
         else
           if(x_meta<choque.x && y_meta>choque.y)
             x_final =choque.x-wide;
           else
             y_final=y_meta;
             x_final=x_meta;
           end
         end
       else
         if(x==choque.x-wide && y==choque.y_abajo)
           if(x_meta>choque.x && y_meta<choque.y)
             y_final=choque.y;
           else
             if(x_meta>choque.wide && y_meta<choque.y_abajo)
               x_final=choque.wide+wide;
             else
               y_final=y_meta;
               x_final=x_meta;
             end
           end
         else
           if(x==choque.wide+wide && y==choque.y_abajo)
             if(x_meta<choque.wide && y_meta<choque.y)
               y_final=choque.y;
             else
               if(x_meta<choque.x && y_meta<choque.y_abajo)
                 x_final=choque.x-wide;
               else
                 y_final=y_meta;
                 x_final=x_meta;
               end
             end
           end
         end
       end
      end
     else
       y_final=y_meta;
       x_final=x_meta;
     end
   end

   FRAME;

   sombra=0;

  END
 END

PROCESS cosa(num,z,priority)
 PRIVATE
   inicio_anima=0;
 BEGIN
  x=objetos[num].x;
  y=objetos[num].y;
  graph=objetos[num].graph;
  ctype=objetos[num].tipo;
  if(objetos[num].z==0);
    z=z-y;
  else
    z=objetos[num].z;
  end
  wide=x+graphic_info(fichero_fpg,graph,g_wide);
  y_abajo=y+graphic_info(fichero_fpg,graph,g_height)
            -graphic_info(fichero_fpg,graph,g_y_center);
  LOOP
    if(id==collision(type mouse) or (objetos[num].animacion!=0
                        && x_meta==id2.x && y_meta==id2.y && inicio_anima))
      if(mouse.right)
        delete_text(all_text);
        write(fuente,write_x,write_y,1,objetos[num].text_right);
      end
      if(mouse.left or (objetos[num].animacion!=0 && x_meta==id2.x
            && y_meta==id2.y && inicio_anima))
        if(objetos[num].animacion!=0)
           delete_text(all_text);
           write(fuente,write_x,write_y,4,"Vamos hacia all¡");
           y_meta=y+objetos[num].animacion;
           x_meta=x;
           inicio_anima=1;
        end
        if(objetos[num].animacion==0
            or (objetos[num].animacion!=0 && x_meta==id2.x && y_meta==id2.y))
         delete_text(all_text);
         obj=id;
         switch(objetos[num].accion)
          case 1:   //leer
             write(fuente,write_x,write_y,4,objetos[num].text_left);
             write(fuente,write_x,write_y+25,4,objetos[num].opc);
          end
          case 2:   //ir
             write(fuente,write_x,write_y,4,objetos[num].text_left);
             if(mouse.graph==objetos[num].cursor or objetos[num].cursor==cursor_normal)
                write(fuente,write_x,write_y,4,objetos[num].text_left);
                game[partida].fase=objetos[num].opc;
             end
          end
          case 3:    //coger
             if(mouse.graph==cursor_normal)
               write(fuente,write_x,write_y,1,objetos[num].text_right);
             end
             if(mouse.graph==objetos[num].cursor)
                delete_text(all_text);
                mouse.graph=objetos[num].opc;
                write(fuente,write_x,write_y,1,objetos[num].text_left);
                if(objetos[num].mutacion!=0)
                   obj=0;
                   cosa(num+objetos[num].mutacion,escenario[father.num].y_max,priority);
                   game.estado[num]=objetos[num].mutacion;
                   signal(id,s_sleep);
                end
             end
          end
         end
        end
      end
    else
      if(mouse.left or mouse.right)
        inicio_anima=0;
      end
      if(mouse.left && obj==id)
        obj=0;
      end
    end
    FRAME;
  END
 END

PROCESS marcador()
  BEGIN
    graph=graph_marcador;
    x=0;
    y=graphic_info(fichero_fpg,graph,g_height);
    y=480-y;
    LOOP
      FRAME;
    END
  END

PROCESS nivel(num)
  PRIVATE
    cont=0;
  BEGIN
    if(escenario[num].tipo_escenario==c_scroll)
       start_scroll(0,fichero_fpg,escenario[num].fondo1,
                                   escenario[num].fondo2,0,0);
       scroll.camera=yo(c_scroll,escenario[num].y_max,escenario[num].tama¤o);
    else
       put_screen(fichero_fpg,escenario[num].fondo1);
       yo(c_screen,escenario[num].y_max,escenario[num].tama¤o);
    end
    for(cont=0;cont<=escenario[num].num_cosas;cont++)
      cosa(escenario[num].lista_objetos[cont]+game[partida].estado[cont],escenario[num].y_max,-cont);
    end
    REPEAT
      FRAME();
    UNTIL (game[partida].fase!=num);
    FRAME();
    escenario[num].yo_xinicio=x_meta;
    escenario[num].yo_yinicio=y_meta;
    obj=0;
    if(escenario[num].tipo_escenario==c_scroll)
      stop_scroll(0);
    else
       signal(id,s_wakeup_tree);
       signal(id,s_kill_tree);
    end
  end
