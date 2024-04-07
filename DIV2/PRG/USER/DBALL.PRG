// Dragon ball Z
// Programado por Miguel Call¢n
// e-mail: neomax@jet.es

program DragonBall;
const
 parado=0;
 avanzando=1;
 retrocediendo=2;
 agachandose=3;
 saltando=4;
 corriendof=5;
 corriendor=6;
 pu¤etacear=7;
 patadear=8;
 superondear=9;
 volar=10;
 ondear=11;
 muerto=13;
 recargando=15;
 tocado=16;

 saltar=0;
 avanzar=1;
 retroceder=2;
 agacharse=3;
 golpear=4;
 correrf=5;
 correrr=6;
 patada=7;
 pu¤etazo=8;
 patadasalto=9;
 pu¤etazosalto=10;
 superonda=11;
 volando=12;
 onda=13;
 agachado=14;
 golpe_bajo=17;
 patada_bajo=18;
 cayendo=19;

 control_teclado1=1;
 control_teclado2=2;
global


 anim0[]=1,2;
 anim1[]=3,4,5,6;
 anim2[]=3,4,5,6;
 anim3[]=1,7,8,9,8,1;
 anim4[]=1,11,12,11,1;
 anim5[]=1,11,13,11,1;//10
 anim6[]=14,15,14;
 anim7[]=17,16,17,18,19,18;
 anim8[]=18,19,18;
 anim9[]=20,21,20;

 //superonda
 anim14[]=28;
 anim24[]=59,60,61,62,63,64,65,66,67,68,69,71;
 //onda
 anim18[]=47,46;
 //cabezaonda
 anim19[]=49,50;
 //aura
 anim15[]=32,33,34;

 //en salto//
 anim10[]=22,23,22;
 anim11[]=24,25,24;
 anim12[]=26;
 anim13[]=27;

 //en vuelo//
 anim16[]=38,39,40;
 //parado//
 anim17[]=40,41,42,42,41,40;

 //agachado
 //pu¤etazo
 anim20[]=53,54,55,54,53;
 //patada
 anim21[]=56,57,57,56;

 //sangre
 anim22[]=20,21,22;
 //polvo
 anim23[]=24,25,26,27,28;
 id_luchador1,id_luchador2;

 anim25[]=30,31,32,33,34,35,36,37,38,39;

 volumen;
 KO;

 luchador1;
 luchador2;
 sonidos1;
 sonidos2;

 opcion=0;
 contador=1; //un contador
 inc_M7=10;//incremento de distancia;
 inc_y;
 Inc_yM7=350;//incremento de X en el modo7;

 mata;
 mataos;

 ssuperonda;
 ssuperonda1;
 srecargars;
 srecargarv;
 sfinal;
 skame;
 sbigvan;
 smusica;
 sondav;
 sondas;
 simpactov;
 simpactos;
 sgolpe;

 DIFUNTO1;
 DIFUNTO2;

 //Definen que fichero de graficos se va a utilizar
 persona1=0;
 persona2=0;
 fuente1;
 borrartexto;

 difunto;

local

 personajes;

 paso;
 estado;
 fichero1;
 fichero2;
 fichero3;
 fichero4;
 sub_paso;
 bloquear;
 salto;

 dolor;
 energia;
 enemigo;
 fuerza;

 contacto;
 angulo;
 deletetexto;
 recar;
 tipo_control;
 posiciony,posicionx;
 ysup;

 incy;
 final;
begin

 priority=100;
 set_mode(m640x480);
 set_fps(200,0);       //90
 fichero1=load_fpg("user\dball\fondosdb.fpg");
 fuente1=load_fnt("tutorial\tutor6.fnt");

 put_screen(0,5);

 nube(90,195,25,0);
 nube(500,192,25,1);
 inicio(320,200);

 start_mode7(0,fichero1,3,3,0,200);
 m7.camera=id;
 m7.distance=32;
 m7.color=262;
 m7.height=75;


 repeat
   final=final+1;
   advance(10);
   if (final>30);
    fade(200,200,200,5);
   end;
  frame;
 until (key(_q) or (final==40));
 final=0;

 signal(type nube,s_kill);
 signal(type inicio,s_kill);

 stop_mode7(0);
 clear_screen();
 fade(100,100,100,3);

 loop

 fade_on();

 logotipo(320,200,0);
 start_scroll(0,fichero1,14,13,0,12);

 scroll.camera=sombra2(0);

 menu();
 minisongoku(170,250,fichero1);

 repeat
   if (key(_enter))
    opcion=contador;
   end;
   frame;
 until (opcion<>0)


   if (opcion==1)
    fade(200,200,200,5);
    while (fading) frame; end;


    clear_screen();
    signal(type logotipo,s_kill);
    signal(type minisongoku,s_kill);
    stop_scroll(0);
    delete_text(all_text);

    fade(100,100,100,5);

    SUB_MENU();



    define_region(1,200,240,75,90);
    define_region(2,350,240,75,90); //350,250

    start_scroll(2,fichero1,19,19,0,0);
    scroll[2].camera=sombra2(1);

    start_scroll(0,fichero1,18,0,1,0);
    scroll[0].camera=seleccion();


    start_scroll(1,fichero1,18,0,2,0);
    scroll[1].camera=seleccion2();

    repeat
    frame;
    UNTIL (key(_enter));


    stop_scroll(0);
    stop_scroll(1);
    stop_scroll(2);
    delete_text(all_text);

    let_me_alone();

    PUT_SCREEN(0,1);
    start_mode7(0,fichero1,12,12,0,300);
    m7.camera=procesooculto(320,350);
    m7.height=inc_m7;
    m7.distance=inc_m7;


  switch (persona1);
   case 0;
    fichero3=load_fpg("user\dball\fsongoku.fpg");
    sonidos1=0;
   end;
   case 1;
    fichero3=load_fpg("user\dball\fvegeta.fpg");
    sonidos1=1;
   end;
  end;

  switch (persona2);
   case 0;
    fichero2=load_fpg("user\dball\fsongoku.fpg");
    sonidos2=0;
   end;
   case 1;
    fichero2=load_fpg("user\dball\fvegeta.fpg");
    sonidos2=1;
   end;
  end;
  eligesonidos();




  id_luchador1=protagonista(150,350,fichero3,0,150,0,control_teclado1,sonidos1);
  id_luchador2=protagonista(450,350,fichero2,1,150,20,control_teclado2,sonidos2);

  id_luchador1.enemigo=id_luchador2;
  id_luchador2.enemigo=id_luchador1;


  define_region(3,25,35,259,8);
  define_region(4,354,35,259,8);

  start_scroll(3,fichero1,29,0,3,0);
  start_scroll(4,fichero1,29,0,4,0);


  repeat
  scroll[3].x0+=id_luchador1.energia;
  scroll[4].x0+=id_luchador2.energia;

    if (borrartexto<>0);
        deletetexto=1;
    end;
    gppersonajes(fichero3,fichero2,fichero1,deletetexto);

   M7.height=inc_m7+100;
   m7.distance=inc_m7+100;

   frame;
  until ((key(_q)) or (difunto==1));

  IF (difunto1==1);
   write(fuente1,320,200,0,"SONGOKU LOSE");
  end;
  if (difunto2==2);
   WRITE (fuente1,320,200,0,"VEGETA LOSE");
  end;

  DELETE_TEXT(all_text);
  stop_mode7(0);
  LET_ME_ALONE();
  OPCION=0;
  difunto=0;
  borrartexto=0;
  deletetexto=0;
  persona1=0;
  persona2=0;

  fade(200,200,200,5);
  while (fading) frame; end;
   end;

  if (opcion==2)
   exit("GRACIAS POR ELEGIR ESTE JUEGO, PRUEBA CON LA OPCION HISTORIA",0);

  end;
  if (opcion==3)
   exit("GRACIAS POR ELEGIR ESTE JUEGO, PRUEBA CON LA OPCION HISTORIA",0);
  end;
  IF (KEY(_ESC));
   EXIT("GRACIAS POR JUGAR",0);
  END;
 frame;
 end;
end;
process control(accion);
 begin


 Switch(father.tipo_control);
 case control_teclado1:

  switch(accion);
   case avanzar:
    return (key(_c));
    end;
   case retroceder:
    return (key(_z));
   end;
   case saltar:
    return (key(_d));
   end;
   case correrf:
    return (key(_f));
   end;
   case correrr:
    return (key(_s));
   end;
   case patada:
    return (key(_m));
   end;
   case pu¤etazo:
    return (key(_n));
   end;
   case superonda:
    return (key(_l));
   end;
   case onda:
    return (key(_k));
   end;
   case volar:
    return (key(_j));
   end;
   case agacharse:
    return (key(_x));
   end;
  end;
 end;
CASE control_teclado2:
  switch(accion);
   case avanzar:
    return (key(77));
   end;
   case retroceder:
    return (key(75));
   end;
   case agacharse:
    return (key(_down));
   end;
   case saltar:
    return (key(72));
   end;
   case pu¤etazo:
    return (key(82));
   end;
   case patada:
    return (key(83));
   end;
   case correrf:
    return (key(81));
   end;
   case correrr:
    return (key(79));
   end;
   case superonda:
    return (key(78));
   end;
   case onda:
    return (key(28));
   end;
   case volar:
    return (key(_c_center));
   end;
  end;
 end;
 end;

 return(0);
 end;
process protagonista(x,y,file,flags,tama¤o,Stama¤o,tipo_control,sonidos);
private

 nuevo_estado;
 nuevo_paso;
 xup;
 xsig;
 contador0=0;
 contador1=0;
 contador2=0;
 contador3=0; //retroceder
 contador4=0; //avanzar
 contador5=0;
 contador6=0;
 contador7=0;

 cuentopolvo=0;
 convinacion=0;

 incremento;
 suby;
 Cont=0;
 cont1=0;
 golpex,golpey;
 incx;
 otropaso=20;
 espera;


 yavuela;
 vuela;
 hecho;
 cae;
 altura;

 yasonidos;
 //bloquear;

begin

 energia=0;

 fuerza=100;
 prosombra(x,y,fichero1,size);


 loop

 espera+=1;

 xup=get_dist(enemigo);


 if (xup>100);

   size=tama¤o-((xup+100)/5);
   inc_m7=10+((xup+100)/5);

 end;

 nuevo_estado=estado;


  switch (estado);
   case parado:
    salto=0;
    contador6=0;
    contador4=0;

    graph=anim0[paso];

    contador7+=1;
    bloquear=0;
    if (contador7=>4);
    contador7=0;
    paso+=1;
    end;

    if (paso==sizeof (anim0));
     Paso=0;
    end;


   if (espera=>3);
   if (flags);
    if (enemigo.x>x); flags=0; end;
   else
    if (enemigo.x<x); flags=1; end;
   end;

   end;

   if (control(avanzar));
    nuevo_estado=avanzando;
    if (flags==1);
     bloquear=control(avanzar);
    end;
   end;
   if (control(retroceder));
    nuevo_estado=retrocediendo;
    if (flags==0);
     bloquear=control(retroceder);
    end;
   end;
   if (control(saltar));
    nuevo_estado=saltando;
   end;
   if (control(correrf));
    nuevo_estado=corriendof;
   end;
   if (control(correrr));
    nuevo_estado=corriendor;
   end;
   if (control(pu¤etazo))
    nuevo_estado=pu¤etacear;

   end;
   if (control(patada));
    nuevo_estado=patadear;
   end;
   if (control(superonda));
    nuevo_estado=superondear;
   end;
   if (control(onda));
    nuevo_estado=ondear;
   end;
   if (control(volar));
    nuevo_estado=volando;
   end;
   if (control(agacharse));
    nuevo_estado=agachado;
   end;
   if ((control(pu¤etazo)) and (control(patada)));
    nuevo_estado=recargando;
    paso=0;
    if (sonidos==0);
     sound(srecargars,10,256);
    end;
    if (sonidos==1);
     sound(srecargarv,10,256);
    end;
   end;
  end;

  case recargando:

    graph=58;



     paso+=1;
     cuentopolvo+=1;


    if (yavuela<>2);

     if (cuentopolvo>=2);
      get_real_point(1,&posicionx,&posiciony);

      polvo(posicionx,y,2,4);

      get_real_point(2,&posicionx,&posiciony);

      polvo(posicionx,y,1,4);
      cuentopolvo=0;
     end;
    end;

    if (paso<2);
     recar=recarga(x,y,file,size,0);

    end;
    if ((control(pu¤etazo)) and (control(patada)));
     nuevo_estado=recargando;

    else
     nuevo_estado=parado;
     signal(recar,s_kill);
     paso=0;

     if (yavuela==2);
      nuevo_estado=volando;
      signal(type polvo,s_kill);
     end;
    end;
  end;
  case muerto:
     graph=51;


      angle+=15000;

      if (flags==0)
       x=x-5;
      end;
      if (flags==1);
       x=x+5;
      end;;

    if (angle=>60000);
     angle=0;
     graph=70;

     repeat
      if (flags==0)
       x=x-5;
      end;
      if (flags==1);
       x=x+5;
      end;

       if (y<350);
       y+=15;
       else
       y=350;
       contador5+=1;
       end;

      frame;

     until (contador5==5);
     contador5=0;
     loop
      frame;
      contador5+=1;
       if (sonidos==0);
        sonidos1=1;

       end;
       if (sonidos==1);
        sonidos2=1;

       end;

      if (contador5==10);
       difunto=1;
      end;
     end;
    end;
  end;


  case tocado:

    signal(recar,s_kill);

    if (not bloquear);

     if (ko==0);
     sound(sgolpe,5,256);
    graph=51;
    incx=1;
    get_real_point(2,&golpex,&golpey);
    sangre(golpex,golpey,fichero1);

    if (flags==0);
     x-=2;
    end;
    if (flags==1);
     x+=2;
    end;

     frame;

    if (salto==0);
    nuevo_estado=parado;

    if (yavuela==2);
     nuevo_estado=volando;
    end;

    else

    repeat;

    y+=15;

    until (y>350);
    y=350;
    nuevo_estado=parado;
    end;

     else

    graph=51;

      sound(sgolpe,5,256);
      angle+=15000;

      if (flags==0)
       x=x-5;
      end;
      if (flags==1);
       x=x+5;
      end;;

    if (angle=>60000);
     angle=0;
     graph=70;

     repeat
      if (flags==0)
       x=x-5;
      end;
      if (flags==1);
       x=x+5;
      end;

       if (y<350);
       y+=15;
       else
       y=350;
       contador5+=1;
       end;

      frame;

     until (contador5==5);



     nuevo_estado=parado;
     if (yavuela==2);
      nuevo_estado=volando;
     end;
     ko=0;
     contador5=0;
     end;

    end;
    else
     otropaso=enemigo.fuerza/20;
    repeat
    graph=37;
    energia-=0;
    dolor-=0;

    if (energia<>0);
     energia=0;
    end;



    xup=get_dist(enemigo);

     if (xup>100);
      size=tama¤o-((xup+100)/5);

     end;

     polvo(x+30,y,0,4);

     if (flags==1);
      x+=otropaso;
      otropaso-=1;
     end;
     if (flags==0);
      x-=otropaso;
      otropaso-=1;
     end;
     frame;

     until(otropaso=<0)

     nuevo_estado=parado;
     otropaso=20;



   end;
  end;

  case avanzando:


     graph=anim1[paso];
     contador1+=1;

     if (contador1=>3);
      contador1=0;
      paso+=1;


     if ((paso==1) or (paso==3));
      if (x<600);
       if (flags==1):
        x+=(30*size)/100;
        bloquear=control(avanzar);

       end;
       if (flags==0);
        x+=(30*size)/100;
        bloquear=control(retroceder);
       end;

      end;
     end;
     end;

     /*xup=get_dist(enemigo);
     if (xup>100);
     size=tama¤o-((xup+100)/5);
     end;*/

     if (paso==1)
       if (not control(avanzar));
       paso=0;
       nuevo_estado=parado;
       contador1=0;
       else
       nuevo_estado=avanzar;
       end;
     end;
     if (paso==sizeof (anim1));
       if (not control(avanzar));
       paso=0;
       nuevo_estado=parado;
       contador1=0;
       else
       nuevo_estado=avanzar;
       paso=0;
       end;
     end;
   end;


  case retrocediendo:


     graph=anim2[paso];
     contador1+=1;

     if (contador1=>3);
      contador1=0;
      paso+=1;


     if ((paso==1) or (paso==3));
      if (x>40);
       if (flags==1);
        x-=(30*size)/100;
        bloquear=control(avanzar);
       end;
       if (flags==0);
        x-=(30*size)/100;
        bloquear=control(retroceder);
       end;
      end;
     end;
     end;

    xup=get_dist(enemigo);

    if (xup>100);
    size=tama¤o-((xup+100)/5);
    end;

    if (paso==1);
     if (not control(retroceder));
      paso=0;
      nuevo_estado=parado;
      contador1=0;
     else
      nuevo_estado=retroceder;
     end;
    end;

    if (paso==sizeof (anim1));
     if (not control(retroceder));
      paso=0;
      nuevo_estado=parado;
      contador1=0;
      else
      nuevo_estado=retroceder;
      paso=0;
      end;
    end;

 end;

  case saltando;

   if (paso<2);
    incy=5;
    graph=anim3[paso++];
    contador5=0;
   end;

   if (paso==2);

    graph=anim3[paso];



    if ((control(avanzar)))
     x=x+10;
    end;
    if ((control(retroceder)));
     x=x-10;
    end;

    if ((control(patada)) or (control(pu¤etazo)) or (contador5==1));

     graph=anim10[contador6];
     contador5=1;
     contador4+=1;
     if (contador6==1);
       if (collision(type protagonista));
        get_real_point(1,&golpex,&golpey);
        golpe(golpex,golpey,7,0);
       end;
     end;

     if (contador4=>4);
       contador6+=1;
       contador4=0;
     end;

     if (contador6==sizeof (anim10));
      contador6=0;
      contador5=0;
     end;

    end;

    if (control(saltar) and (salto==0));        //control(saltar))
     if (y>20);
       suby=-1;
       y-=15;
       altura+=1;
      else
       salto=1;
     end;
     else
       salto=1;
     end;

    if (salto==1);
     y-=incy;
     /*inc_y*/incy-=1;
     if (incy=<-5);
      salto=2;
     end;
    end;
    if (salto==2);
     y+=1;
     suby=1;
     if (y=>350);
     nuevo_estado=parado;
     contador6=0;
     salto=0;
     paso=0;
     y=350;
     end;
    end;
     if (control(volar));
      contador6=0;
      nuevo_estado=volando;
      vuela=1;
      hecho=3;
      salto=0;
     end;
   end;
 end;

 case corriendof:

    if ((paso<3) and (control(correrf)));
      from contador0=0 to 1;
       graph=anim4[contador0];
       frame;
      end;
    end;
   if ((paso=3) and (control(correrf)));

    if (flags==0);
    graph=anim4[2];
    end;
    if (flags==1);
    graph=anim5[2];
    end;


    if (x<600);
     if (flags==1):
        x+=(40*size)/100;
     end;
     if (flags==0);
        x+=(40*size)/100;
     end;

    if (yavuela<>2);
     if (x>590);
      polvo(x,y,2,4);
     else
      get_real_point(1,&golpex,&golpey);
      polvo(golpex,golpey,0,4);
     end;
    end;
    end;

   end;
    if ((paso=3) AND (Not control(correrf)));
       nuevo_estado=parado;

      if (yavuela==2);
       nuevo_estado=volando;
       contador0=0;
       vuela=1;

       paso=0;
      end;
    end;
   end;

    case corriendor:
     if ((paso<3) and (control(correrr)));

      from contador0=0 to 1;
       graph=anim5[contador0];
       frame;
      end;
     end;

    if ((paso=3) and (control(correrr)));

     if (flags==0);
      graph=anim5[2];
     end;
     if (flags==1);
      graph=anim4[2];
     end;

    if (x=<40);

      polvo(x,y,1,4);
      paso+=10;
    end;

    if (x>40);

     if (yavuela<>2);

      if (x<50);
       polvo(x,y,1,4);
      else
       get_real_point(1,&golpex,&golpey);
       polvo(golpex,golpey,0,4);
      end;
     end



     if (flags==1):
        x-=(40*size)/100;
     end;
     if (flags==0);
        x-=(40*size)/100;
     end;
    end;
    end;
     if ((paso=3) and (Not control(correrr)));

      nuevo_estado=parado;
      if (yavuela==2);
       nuevo_estado=volando;
       vuela=1;

       contador0=0;
      end;
     end;
     end;

   case agachado;
    graph=53;
    paso=0;
   if (control(pu¤etacear));
    nuevo_estado=golpe_bajo;
   end;

   if (control(patadear));
    nuevo_estado=patada_bajo;
   end;

   if (not control(agacharse));
    nuevo_estado=parado;
   end;

   end;

   case golpe_bajo;

    graph=anim21[paso++];
     if (paso==2);
      get_real_point(1,&golpex,&golpey);

      golpe(golpex,golpey,40,1);
     end;
     if (paso==sizeof(anim21))
      paso=0;
      nuevo_estado=agachado;
     end;

   end;

   case patada_bajo;

    graph=anim20[paso++];
     if (paso==3);
      get_real_point(1,&golpex,&golpey);
      ko=1;
      golpe(golpex,golpey,40,1);
     end;
     if (paso==sizeof(anim20))

      nuevo_estado=agachado;
     end;

   end;

   case volando;

    if (vuela<>1);

     if (hecho<2);

      graph=anim3[hecho++];
      contador0=0;

     end;

     if (hecho==2);
      if (contador0<10);

      contador0+=1;
      incy=contador0;
      y-=incy*3;
      inc_m7+=incy*3;

      if (contador0==10);
       contador0=0;
       hecho=3;
       incy=5;
      end;
      end;
     end;
    end;

        graph=anim17[0];
        yavuela=2;
         if (flags);
           if (enemigo.x>x); flags=0; end;
            else
           if (enemigo.x<x); flags=1; end;
         end;




     if (hecho==3);
      if (not (control(saltar) or control(retroceder) or
         control(avanzar) or control(agacharse) or control(patada)
         or control(pu¤etazo)));

        if (contador0==0);
         y-=incy;
         incy-=1;
         if (incy==-5);
          contador0=1;
         end;
     end;

        if (contador0==1);
         y-=incy;
         incy+=1;
         if (incy==5)
          contador0=0;
         end;
        end;
      end;
     end;

     if (hecho==3);

      if (control(volar));
       nuevo_estado=cayendo;
      end;



      if (control(avanzar));
       if (flags==0);
       graph=anim16[0];
       end;
       if (flags==1);
       graph=anim16[1];
       end;
         x=x+10;

      end;

      if (control(retroceder)):
       if (flags==0);
       graph=anim16[1];
       end;
       if (flags==1);
       graph=anim16[0];
       end;
         x=x-10;

      end;

      if (control(pu¤etazo));
       nuevo_estado=pu¤etacear;
       paso=0;
      end;

      if (control(patada));
       nuevo_estado=patadear;
       paso=0;
      end;

      if (control(pu¤etazo) and control(patada));
       nuevo_estado=recargando;
       if (sonidos==0);
        sound(srecargars,5,256);
       end;
       if (sonidos==1);
        sound(srecargarv,5,256);
       end;
      end;

      if (control(correrf));
       nuevo_estado=corriendof;
      end;
      if (control(correrr));
       nuevo_estado=corriendor;
      end;

      if (control(onda));
       nuevo_estado=ondear;
      end;
      if (control(superonda));
       nuevo_estado=superondear;
      end;

      if (control(saltar));
       graph=anim17[0];
       if (y>50);
       y-=10;
       end;

      END;

      if (control(agacharse));:
       graph=anim17[0];
       y+=10;
      END;

    end;
   if (hecho>2);
    if ((cae==1) or (y>200));
     hecho=4;

     y=y+20;
     graph=anim3[3];

     if (y>350);
      y=350;
      graph=anim3[1];
      nuevo_estado=parado;

      yavuela=0;
      vuela=0;
      hecho=0;
      cae=0;
    end;
   end;
  end;
 end;


   CASE cayendo;
      graph=anim17[0];
       y=y+20;
      if (y>200)
       cae=1;
       nuevo_estado=volando;
      end;
   END;


   CASE patadear;
      graph=anim7[paso++];
      if ((paso==1) or (paso==3));
       get_real_point(1,&golpex,&golpey);
       golpe(golpex,golpey,6,0);
      end;
      if (paso==2);
       if (not control(patada));
        paso=0;
        nuevo_estado=parado;
        if (yavuela==2);
         nuevo_estado=volando;
         paso=3;
        end;

       else;
        nuevo_estado=patadear;
       end;
      end;
      if (paso==sizeof (anim7));
       if (not control(patada));
        paso=0;
        nuevo_estado=parado;
        if (yavuela==2);
         nuevo_estado=volando;
         vuela=1;
         paso=3;
        end;

       else
        paso=0;
        nuevo_estado=patadear;
       end;
      end;

   END;

   CASE pu¤etacear;
     graph=anim6[paso++];
     if (paso==2);
      get_real_point(1,&golpex,&golpey);
      golpe(golpex,golpey,7,0);
     end;
     if (paso==sizeof (anim6));
      paso=0;
      nuevo_estado=parado;
      if (yavuela==2);
       nuevo_estado=volando;
       vuela=1;
      end;
     end;

    if (control(pu¤etazo));
    nuevo_estado=pu¤etacear;
    end;
   END;


   CASE ondear;

     graph=anim18[contador3];   //[0];

      if (contador3==1);
       if (yasonidos<>1);
        if (sonidos==0);
         sound(sondas,5,256);
         yasonidos=1;
        end;
        if (sonidos==1);
         sound(sondav,5,256);
         yasonidos=1;
        end;
        get_real_point(1,&posicionx,&posiciony);
        ondanormal(posicionx,y,file,size,flags);
       end;
      end;


      contador2+=1;
      if (contador2=>5);
       contador3+=1;
       contador2=0;
      end;


     if (contador3==sizeof (anim18));
      nuevo_estado=parado;
      if (yavuela==2);
       nuevo_estado=volando;

      end;
      contador3=0;
      yasonidos=0;
     end;
   end;

   case superondear;
     graph=anim14[0];
     recarga (x,y,file,size,1);
    while (control(superonda));

     fuerza+=5;

     xup=get_dist(enemigo);

      if (xup>100);
      size=150-((xup+100)/5);
      end;


       frame;
    end;
    signal(type recarga,S_kill);

     graph=29;
     if (enemigo.y<350);
     graph=30;
     end;
     if (350>y);
     graph=31;
     end;
     if (enemigo.y<350 and y<350);
     graph=29;
     end;

     energia+=fuerza/40;
     dolor+=fuerza/40;


    if (sonidos==1);

     sound(sfinal,volumen,256);
    end;
    if (sonidos==0);
     sound(skame,volumen,256);
    end;

    get_real_point(1,&golpex,&golpey);
     casisuperonda(golpex,golpey);

    from cont=0 to 15 step 1;


     xup=get_dist(enemigo);

     if (xup>100);
      size=150-((xup+100)/5);
     end;

    frame;
    end;


    nuevo_estado=parado;

    if (yavuela==2);
     nuevo_estado=volando;
    end;

    fuerza=100;
   end;

 end;
 if (energia<>0); //dolor);
    energia=0; //dolor;
 end;
 if (estado<>nuevo_estado)
    estado=nuevo_estado;
    paso=nuevo_paso;
    nuevo_paso=0;
 end;
 if (x<40);
    x=45;
    if (enemigo.x<600)
    enemigo.x=enemigo.x+((30*size)/100);
    inc_ym7=inc_ym7-((30*size)/100);
    end;

 end;
 if (x>600);
    x=595;
    if (enemigo.x>40);
    enemigo.x=enemigo.x-((30*size)/100);
    inc_ym7=inc_ym7+((30*size)/100);
    end;

 end;


    frame;
 end;
end;
process eligesonidos();
begin

   skame=load_pcm("user\dball\kame.pcm",0);
   sbigvan=load_pcm("user\dball\Big_van.pcm",0);
   sfinal=load_pcm("user\dball\final_f.pcm",0);
   srecargarv=load_pcm("user\dball\vej_shaa.pcm",0);
   srecargars=load_pcm("user\dball\gok_ahh.pcm",0);

   sondas=load_pcm("user\dball\gok_sha.pcm",0);
   sondav=load_pcm("user\dball\vej_sha.pcm",0);
   simpactos=load_pcm("user\dball\gok_ah.pcm",0);
   simpactov=load_pcm("user\dball\vej_ahh.pcm",0);
   sgolpe=load_pcm("user\dball\hit.pcm",0);

   volumen=5;
end;
process casisuperonda(x,y);
private;
 cuen;
 id_mu¤ecos;
 tipoavance;
 begin
  file=father.file;


  if (350>father.y);
   if (father.enemigo.x>father.x);
    angle=angle-25000;
    tipoavance=0;
   else
    tipoavance=1;
    angle=angle+25000;
   end;

   end;

  if (father.enemigo.y<350);
   if (father.enemigo.x>father.x);
     angle=angle+25000;
   else
     tipoavance=1;
     angle=angle-25000;
   end;
  end;

  flags=father.flags;

  repeat
  graph=anim24[11];

  if(contacto=>1)break;end;

    if (flags==0);
     advance(+30);
     end;
    if (flags==1);
     advance(-30);
     end;



  cuen+=1;

  fuerza=father.fuerza;
  size=father.size;

  granonda (x,y,file,size+fuerza,flags,angle);

  /*if (y=>350);
   explosion(x,y,fichero1);
  end;*/

  frame;
  until ((x=>640) or (x=<0) or (y=>350) or (y=<0));

 end;

process sangre(x,y,file);
private
 cuenta;
 begin
  if (father.flags==1);
   flags=0;
  end;
  if (father.flags==0);
   flags=1;
  end;
  size=200;
  from cuenta=0 to (sizeof (anim22)-1);
  size=200+father.size;
   graph=anim22[cuenta];
   frame;
  end;
  cuenta=0;
 end;
process polvo(x,y,direccion,flags);
 private
 cuenta;
 otracuenta=5;
 begin


 from cuenta=0 to (sizeof(anim23)-1);



  if (direccion==1);
   x+=(30*size)/100;
  end;
  if (direccion==2);
   x-=(30*size)/100;
  end;


 size=father.size+father.size;
 graph=anim23[cuenta];
 frame;
 end;
end;

process sub_menu();
 private

 id_escrito;
 begin


  write (fuente1,310,265,1,"VS");
  id_escrito=write (fuente1,320,50,1,"ESCOGE TU LUCHADOR");
 LOOP
  FRAME;
 END;
 end;
process gppersonajes(ficha1,ficha2,ficha3,texto);
private

 begin

    z=-2;

    x=40;
    y=75;
    file=ficha1;
    graph=52;
    frame;
     if (texto==0);
       write (fuente1,320,20,1,"VS");
     switch (persona1);
      case 0;
       write (fuente1,90,75,3,"GOKU");
      END;
      case 1;
       write (fuente1,90,75,3,"VEGETA");
      END;
     end;
     switch (persona2);
      case 0;
       write (fuente1,550,75,5,"GOKU");
      END;
      case 1;
       write (fuente1,550,75,5,"VEGETA");
      END;
     END;
    FRAME;
    end;

    x=600;
    y=75;
    file=ficha2;
    graph=52;
    frame;

    x=320;
    y=40;
    file=ficha3;
    graph=16;
    frame;


  borrartexto=1; //hace que no se pueda volver a escribir
 end;

process seleccion();
private

 personaje=0;
 Id_texto1;
 Id_texto2;
 xsub,ysub;

 begin
  ctype=c_scroll;
  x=0;

  id_texto2=write (fuente1,20,400,0,"SONGOKU");

  loop
  xsub=x;
  ysub=y:
  if (key(_c));
   if (x=<0);
   from x=0 to 150 step 10;
   frame;
   end;
   personaje+=1;
   persona1+=1;
   end;
  end;
   if(key(_z));

   if (x=>150)
    from x=150 to 0 step -10;
     frame;
    end;
   personaje-=1;
   persona1-=1;
   end;
  end;
  switch (personaje);
   case 0:
    delete_text(id_texto2);
    id_texto1=Write (fuente1,20,400,0,"SONGOKU");
    personas(0,110,250);
   end;
   case 1:
    delete_text(id_texto1);
    id_texto2=write (fuente1,20,400,0,"VEGETA");
    personas(1,110,250);
   end;
  END;
  frame;
  end;
 end;
process personas(imagen,x,y);
 begin
  file=fichero1;
  size=60;
  switch (imagen);
   case 0;
    graph=14;
   end;
   case 1;
    graph=13;
   end;
  end;
  frame;
end;

process seleccion2();
private
 personaje=0;
 id_texto1;
 id_texto2;

 begin
  ctype=c_scroll;

  x=0;
  id_texto2=write(fuente1,400,400,1,"SONGOKU");
  loop
  if (key(_right));
   if (x=<0);
    from x=0 to 150 step 10;
     frame;
    end;
    personaje+=1;
    persona2+=1;
   end;
  end;
  if (key(_left));
   if (x=>150);
    from x=150 to 0 step -10;
     frame;
    end;
    personaje-=1;
    persona2-=1;
   end;
  end;
   switch (personaje);
    case 0:
     delete_text(id_texto2);
     id_texto1=write(fuente1,510,400,1,"SONGOKU");
     personas(0,510,250);
    end;
    CASE 1:
     delete_text(id_texto1);
     id_texto2=write(fuente1,510,400,1,"VEGETA");
     personas(1,510,250);
    end;
   end;
  frame;
  end;
 end;

process golpe(x,y,da¤o,kaos);
private
 id_mu¤ecos;
 contador0;
 cuent;
 begin

  size=father.size;
  z=10000;
  graph=17;
  id_mu¤ecos=collision(type protagonista);
  if (id_mu¤ecos==father);
     (id_mu¤ecos=collision(type protagonista));
  end;
  if (id_mu¤ecos);
   if (dolor=>300);
      id_mu¤ecos.estado=volando;
   end;

   id_mu¤ecos.paso=0;
   father.enemigo.estado=tocado;
   if (kaos==1);
    ko=1;
   end;

    if (not father.bloquear)      //bloquear;
     id_mu¤ecos.energia+=da¤o/2;
     father.enemigo.dolor+=da¤o/2;

     if (father.enemigo.dolor=>260);
      father.enemigo.estado=muerto;
    end;
    else
     id_mu¤ecos.energia=0;
    end;
  end;
  frame;

 end;

process inicio(x,y);
private
 contador0;
begin
 graph=2;
  from contador0=0 to 10;
   size=size+4;
  frame;
  end;
  from contador0=0 to 10;
  frame;
  end;
  from contador0=0 to 11;
  angle=angle+30000;
  frame;
  end;
  from contador0=0 to 5;
  size=size+20;
  flags=4;
  frame;
  end;
end;
process prosombra(x,y,file,size);
 begin
  graph=15;
  loop
  z=2;
  flags=4;
  x=father.x;
  size=father.size;
  /*if (get_dist(enemigo)>100)
   size=father.size-((get_dist(enemigo)+100)/5);
  end;*/
  frame;
  end;
 end;

/*process presentacion();
private
 contador0;
 contador2;
begin
 fade_on();
 graph=10;
 size=10;
  x=320;
  y=100;
  from contador0=0 to 20;
   size=size+5;
   y=y+2;
   if (y=>3);
    angle=angle+15000;
    end;
   if (size=>40)
    fade(0,0,0,8);
   end;
  frame;
  end;
 angle=0;
 fade_on();

 graph=9;
 size=10;
  x=600;
  y=50;
 from contador0=0 to 30;
  size=size+15;
   x=x-50;
   y=y+50;
  if (size=>90);
   fade(0,0,0,1);
  end;
   frame;
  end;



  fade_on();

  graph=6;
  size=50;
  x=320;
  y=200;
 from contador0=0 to 30;
  size=size+1;
  if (size=>65)
   fade(0,0,0,3);
  end;
 frame;
 end;

 fade_on();

 graph=1;
 size=100;
 x=200;
 y=350;
 from contador0=0 to 40;
 size=size+1;
  if (size=>120);
   fade(0,0,0,3);
  end;
 frame;
 end;


end;*/
process ondanormal (x,y,file,size,flags);
 private
 id2;
 contador0;
 begin
 z=-2;
 if (flags==1);
  flags=4;
 end;
 if (flags==0);
  flags=5;
 end;
 graph=45;

 repeat

  size=father.size;


 id2=collision(type protagonista);
 if (id2==father);
  id2=collision(type protagonista);
 end;

 if (id2)
  id2.estado=tocado;
  salto=1;
  get_real_point(1,&posicionx,&posiciony);

   explosion(posicionx,posiciony,fichero1);
 end;

 if (flags==4);
  advance(-20);
 end;
 if (flags==5);
  advance(+20);
 end;
   frame;

 until ((x=>645) or (X=<-5) or id2);
end;

process trozoonda(x,y,flags,file,angle);
private
 id2;
 variacion;
 begin
 if (father.flags==1);
  flags=1;
 end;
 if (father.flags==0);
  flags=7;
 end;
  graph=48;
 x=father.x;
 loop

if (sub_paso==0);
 id2=collision(type protagonista);
 if (id2==father);
  id2=collision(type protagonista);
 end;

 if (id2);

   id2.energia+=father.father.fuerza;
   sub_paso=1;
 end;

end;
  if  ((x>600) or (x<0)); break; end;


 mataos=74;
 advance(mataos*size/124);

 size=father.size;

 frame; //200
 end;
 end;
Process granonda(x,y,file,size,flags,angle);
private
 sub_paso2=0;
 ID2;
 contador0;
 variacion;
 otracuenta=0;
 id_mu¤ecos;
 cuen;
 begin
 set_fps(60,0);
 z=-2;

 if (flags==0);
  flags=4;
 end;
 if (flags==1);
  flags=7;
 end;

 contador0=0;
 otracuenta=0;

 graph=anim24[10];

 repeat

  cuen+=1;
  size=size-10;

  contador0+=1;
  if (contador0=>5);
  otracuenta=1;
  contador0=0;
  end;
   id2=collision(type casisuperonda);
  if (id2==father);
    id2=collision(type casisuperonda);
  end;



   id_mu¤ecos=collision(type protagonista);
  if (id_mu¤ecos==father.father);
     (id_mu¤ecos=collision(type protagonista));
  end;
  if (id_mu¤ecos or id2 or (y=>350));
    if (id_mu¤ecos);

    id_mu¤ecos.estado=tocado;


    if (not id_mu¤ecos.bloquear);
    id_mu¤ecos.energia+=father.father.fuerza/4;
    id_mu¤ecos.dolor+=father.father.fuerza/4;



    if (id_mu¤ecos.y<350);
     ko=1;
    end;

    if (id_mu¤ecos.dolor>340);
     id_mu¤ecos.estado=muerto;
    end;

     father.contacto+=1;
     explosion(id_mu¤ecos.x,id_mu¤ecos.y,fichero1);

    else
     id_mu¤ecos.energia=0;
     father.contacto+=1;
     explosion(id_mu¤ecos.x,id_mu¤ecos.y,fichero1);
    end;
    end;

    if (id2);
     explosion (id2.x,id2.y,fichero1);
     father.contacto+=1;
    end;

    if (y=>350);
     explosion (x,y,fichero1);
     father.contacto+=1;
    end;



  end;
    if (father.contacto==1) break; end;

  frame;

 until ((otracuenta==(sizeof (anim24)-1)) or (size=<0));
end;
process explosion(x,y,file);
private
id2;
contador0=0;
contador1=0;
cuent;
cuen;
begin;
 signal(father.enemigo,s_freeze);
 z=-3;


 if (father.salto<>1);
 y=father.y-10;
 end;

 size=father.size+(2*(father.father.father.fuerza));
 sound((load_pcm("user\dball\boom.pcm",0)),10,256);

 repeat

   cuent+=1;
   if (cuent==2);
    if (cuen+1<>sizeof(anim25));
    cuen+=1;
    else
    break;
    end;
    cuent=0;
   end;
   graph=anim25[cuen];
   frame;
 until (cuen==sizeof(anim25));

end;

process recarga(x,y,file,size,controla);
private;
 sub_paso2;
begin
  flags=4;
  sub_paso=0;
  z=-2;
  loop
   x=father.x;
   y=father.y;

   if (get_dist(Father.enemigo)>100)
    size=150-((get_dist(Father.enemigo)+100)/5);
   end;
  graph=anim15[sub_paso2++];

  if (controla==0)
  father.energia-=1;
  father.dolor-=1;
  end;

   if (sub_paso2==sizeof(anim15))
   /*if (controla==0);
    father.energia-=1;
    father.dolor-=1;
   end;*/

   sub_paso2=0;
   end;
  frame;
  end;
 end;

process logotipo(x,y,flags);
private;
begin
 size=150;
 graph=2;

 loop
  if (y>120);
   y=y-1;
  end;

 frame;
 end;
end;


process nube(x,y,size,flags);
private
 contador0;
begin
 graph=4;
 from contador0=1 to 100;
  size=size+1;
  frame;
 end;
 loop
 frame;
 end;
end;

process sombra();
 begin
  set_fps(60,0);
  ctype=c_scroll;
  loop
  x=x+2;
  frame;
  end;
 end;
process sombra2(tipo);
 begin
  set_fps(60,0);
  ctype=c_scroll;
  loop
  switch (tipo)
  case 0;
   X=x+2;
  end;
  case 1;
   x=x+2;
   y=y+2;
  end;
  end;
  frame;
  end;
 end;

process menu()
private;

begin



 write(fuente1,320,250,4,"HISTORIA");
 write(fuente1,320,300,4,"VS");
 write(fuente1,320,350,4,"OPCIONES");

 loop
 frame;
 end;

end;
process sombramenu(y);
 begin
  flags=4;
  size=200;
  file=fichero1;
  x=400;
  graph=2;
   loop
     frame;
   end;

 end;
process minisongoku(x,arriba,file);
private
 cont_graficos=0;
begin
 size=30;
 y=arriba;
 graph=11;
 loop
  if (key(_down)and y<arriba+100);
   y+=50;
   contador+=1;
  end;
  if (key(_up)and y>arriba);
   y-=50;
   contador-=1;
  end;
  frame(200);
  end;
end;
process procesooculto(x,y);
 begin
  ctype=c_m7;
   loop
   y=inc_ym7;
   frame;
  end;
 end;