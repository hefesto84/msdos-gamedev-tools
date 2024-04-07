//-----------------------------------------------------------------------
// Programa : The Shag Attack -------------------------------------------
// Autor: Eduardo Marquez     -------------------------------------------
// Direccion: Paseo de Illetas N§70-72, 07015, Palma de Mallorca --------
// E-mail: eddie@ocea.es ------------------------------------------------
// WWW: http://www.ocea.es/personales/eddie -----------------------------
//-----------------------------------------------------------------------

program The_Shag_Attack;

global
digital;
puntos=0;
vidas=0;
energianave=5;
estadodisparo=0;
disparospeed=0;
estadonave=0;
boss1life=400;
selectplayer=0;
selecttrue=0;
specials=0;
firepower=0;
pass=0;
win=0;
info;

sonido_explode2;
sonido_attention;
sonido_disparo;
sonido_disparo2;
sonido_disparo3;
sonido_disparo4;
sonido_explosion;
sonido_rythm;
sonido_emergencia;
sonido_metralla;
sonido_transmision;
sonido_faser:
sonido_buiu;
sonido_boss1laser;
sonido_bomba;
sonido_explode;
sonido_looseenergy;
sonido_aaah;

english;
spanish;
powercount=0;
countselection=0;
escudorepeat=0;
escudorepeat2=0;
bar256;
choose=0;
estadoprograma=0;

Begin
 FragLord();frame;
 INTRO();frame;
 repeat
 if (key(_esc));estadoprograma=0;end
 if (estadoprograma==0);let_me_alone();unload_fpg(0);load_fpg("user\shag\select.fpg");load_pal("user\shag\select.fpg");powercount=0;countselection=0;escudorepeat=0;choose=0;delete_text(all_text);
     puntos=0;vidas=0;escudorepeat2=0;win=0;energianave=5;estadodisparo=0;disparospeed=0;estadonave=0;boss1life=400;selectplayer=0;selecttrue=0;specials=0;firepower=0;vidas=0;Main();frame;end
 if (estadoprograma==1);Selection();frame;end
 if (estadoprograma==2);loadnivel1();estadoprograma=3;end
 if (estadoprograma==3);nivel1();frame;estadoprograma=4;end
 if (vidas<0);stop_scroll(bar256);let_me_alone();delete_text(all_text);animation();frame;estadoprograma=0;end
 if (win==1);let_me_alone();delete_text(all_text);victoria();frame;estadoprograma=0;end
 frame;
until (estadoprograma==10);

let_me_alone();
credits();frame;
end


//**********************************************************************
// Process Load Nivel 1*************************************************
//**********************************************************************
process loadnivel1();

private
count=0;

begin

mouse.x=320;
mouse.y=300;
set_mode(m640x480);
set_fps(20,0);
load_pal("user\shag\shag.fpg");

nave();

    bar256=load_fpg("user\shag\shag.fpg"); // Carga los gr ficos
    put(bar256,103,320,45);               // Barra superior
    define_region(1,0,95,640,398);      // Define coordenadas de Barra Superior
    start_scroll(0,bar256,3,1,1,10);   // Inicializa el movimiento de pantalla
    animator(1);
    animator(2);

write_int(0,440,43,1,&puntos);
write_int(0,250,57,1,&vidas);
write_int(0,250,43,1,&energianave);
write_int(0,255,28,1,&specials);
write_int(digital,320,35,1,&firepower);
write_int(0,440,56,1,&boss1life);
 repeat

    if (rand(0,100)<4);
    scroll.x0=scroll.x0-1;
    scroll.y0=scroll.y0-1;
    end
    scroll.x1=scroll.x1-5;
    scroll.y1=scroll.y1-5;

 frame;
    if (count==0 AND puntos=>20000);vidas=vidas+1;count=1;end
    if (count==1 AND puntos=>40000);vidas=vidas+1;count=2;end
    if (count==2 AND puntos=>60000);vidas=vidas+1;count=3;end
 until (vidas<0);
end


//**************************************************************************
// process animator ********************************************************
//**************************************************************************

process animator(tipo);

private
tipo:
count=0;

Begin

if (tipo==1);
x=320;
y=77;
  repeat
   from graph=180 to 214;frame;end
  until (vidas<0);
 end

if (tipo==2);
x=478;
y=35;
  repeat
   from graph=20 to 23;frame(1000);end
  until (vidas<0);
 end

if (tipo==3);
x=48;
y=47;
 repeat
  from graph=25 to 30;frame(500);count=count+1;end
 until (count=>24)
end

if (tipo==4);
x=590;
y=47;
 repeat
  from graph=25 to 30;frame(500);count=count+1;end
 until (count=>24)
end


if (tipo==5);
x=100;
y=300;
 repeat
  x=x+20;
  from graph=52 to 53;
 frame;
 end
 until (x=>450)
 repeat
  from graph=52 to 53;frame;count=count+1;end
  if (count==10);write(0,320,460,1,"No hay nada que hacer. Vamos a por un bocata....");end
 until (count=>90)
 repeat
  x=x+20;
  from graph=52 to 53;
 frame;
 end
 until (x=>640)
end
end

//*************************************************************************
// Process Nivel1 *********************************************************
//*************************************************************************
process nivel1();

private
count=0;
count2=0;

begin
 textonivel1(320,240,41);frame(5000);
 screens(1,66,1);frame(20000);

 screens(4,5,0);frame; //enemigo 1

 count=0;count2=0;
 repeat
 count2=count2+1;
 count=count+1;
 if (count>=50);enemigo(1,x,y,0,0,0,5);count=0;END;
 Frame;
 UNTIL (count2>200 OR vidas<0);

 screens(5,19,0);frame; //enemigo 2

 enemigo(12,0,140,10,0,0,154);

 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(2,x,y,0,0,0,19);count=0;END;
 frame;
 Until (count2>150 OR vidas<0);

 screens(4,14,0);frame; //enemigo 3

 frame(5000);
 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(3,x,y,0,0,0,14);count=0;END
 frame;
 Until (count2>120 or vidas<0);

 enemigo(12,635,140,-10,0,1,154);
 count2=0;count=0;
 frame(12000);

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(33,x,y,0,0,0,14);count=0;END
 frame;
 Until (count2>120 OR vidas<0);

 count2=0;count=0;
 screens(2,66,1);frame;
 frame(3000);

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=50);enemigo(1,x,y,0,0,0,5);count=0;END;
 Frame;
 UNTIL (count2>200 OR vidas<0);

 count2=0;count=0;

 screens(5,152,0);frame; //enemigo 6


 repeat
 count2=count2+1;
 count=count+1;
 if (count==100);enemigo(6,0,240,0,0,0,152);END;
 if (count==200);enemigo(6,640,240,0,0,0,152);count=0;end
 Frame;
 UNTIL (count2>400 OR vidas<0);

 count2=0;count=0;

 screens(4,153,0);frame; //enemigo 3


 repeat
 count2=count2+1;
 count=count+1;
 if (count==25);enemigo(7,0,240,20,0,0,153);END
 if (count==50);enemigo(7,640,240,-20,0,1,153);end
 if (count==75);enemigo(7,0,125,20,0,0,153);end
 if (count==100);enemigo(7,640,400,-20,0,1,153);end
 if (count==125);enemigo(7,0,475,20,-10,0,153);end
 if (count==150);enemigo(7,635,475,-20,-10,1,153);end
 if (count==175);enemigo(7,0,240,20,0,0,153);END;
 if (count==200);enemigo(7,640,240,-20,0,1,153);end
 if (count==225);enemigo(7,0,125,20,0,0,153);end;
 if (count==250);enemigo(7,640,400,-20,0,1,153);end
 if (count==275);enemigo(7,0,475,20,-10,0,153);end
 if (count==300);enemigo(7,635,475,-20,-10,1,153);end
 if (count==400);enemigo(7,0,240,20,0,0,153);

     enemigo(12,0,140,10,0,0,154);

     enemigo(7,640,240,-20,0,1,153);
     enemigo(7,0,125,20,0,0,153);
     enemigo(7,640,400,-20,0,1,153);
     enemigo(7,0,475,20,-10,0,153);
     enemigo(7,635,475,-20,-10,1,153);
 end
 frame;
 UNTIL (count2>400 OR vidas<0);

 count2=0;count=0;
 screens(3,66,1);frame(5000);

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(2,x,y,0,0,0,19);count=0;END;
 frame;
 Until (count2>150 OR vidas<0);


 screens(5,159,0);frame; //enemigo 22

 count2=0;count=0;
 frame(5000);
 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(2,x,y,0,0,0,159);count=0;END;
 frame;
 Until (count2>150 OR vidas<0);

 enemigo(12,0,340,10,0,0,154);
 enemigo(12,0,240,8,0,0,154);
 enemigo(12,0,200,5,0,0,154);

 screens(4,151,0);frame; //enemigo 3

 repeat
 count2=count2+1;
 count=count+1;
 if (count==25);enemigo(5,1,115,0,0,0,151);
                enemigo(5,100,115,0,0,0,151);
                enemigo(5,200,115,0,0,0,151);
                enemigo(5,300,115,0,0,0,151);
                enemigo(5,400,115,0,0,0,151);
                enemigo(5,500,115,0,0,0,151);
                enemigo(5,600,115,0,0,0,151);end
 if (count==100);enemigo(5,1,115,0,0,0,151);
                enemigo(5,100,115,0,0,0,151);
                enemigo(5,200,115,0,0,0,151);
                enemigo(5,300,115,0,0,0,151);
                enemigo(5,400,115,0,0,0,151);
                enemigo(5,500,115,0,0,0,151);
                enemigo(5,600,115,0,0,0,151);end
 if (count==200);enemigo(5,1,115,0,0,0,151);
                enemigo(5,100,115,0,0,0,151);
                enemigo(5,200,115,0,0,0,151);
                enemigo(5,300,115,0,0,0,151);
                enemigo(5,400,115,0,0,0,151);
                enemigo(5,500,115,0,0,0,151);
                enemigo(5,600,115,0,0,0,151);end
 Frame;
 UNTIL (count2>400 OR vidas<0);

 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=50);
 enemigo(16,0,140,10,0,0,0);
 count=0;END
 frame;
 Until (count2>140 OR vidas<0);

 enemigo(12,638,240,10,0,1,154);

 count=0;count2=0;

 screens(5,162,0);frame; //enemigo 22

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=15);enemigo(15,rand(0,640),478,rand(-3,3),rand(-15,-3),2,162);count=0;end
 frame;
 Until (count2>500 OR vidas<0);

 count2=0;count=0;

 screens(5,166,0);frame; //enemigo 22

 repeat
  if (count==0);enemigo(10,0,0,0,0,0,165);end
  frame;
  count=1;
 until (pass==1)

 pass=0;
 count2=0;count=0;


 repeat
 count2=count2+1;
 count=count+1;
 if (count>=50);enemigo(11,rand(5,635),120,0,0,0,166);count=0;END
 frame;
 Until (count2>200 OR vidas<0);

 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count==30);enemigo(17,320,130,0,0,0,157);END;
 frame;
 Until (count2>100 OR vidas<0);

 count=0;count2=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=50);enemigo(1,x,y,0,0,0,5);count=0;END;
 Frame;
 UNTIL (count2>200 OR vidas<0);

 enemigo(12,0,140,10,0,0,154);

 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(2,x,y,0,0,0,19);count=0;END;
 frame;
 Until (count2>150 OR vidas<0);

 frame(5000);
 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count==100);enemigo(6,0,240,0,0,0,152);END;
 if (count==200);enemigo(6,640,240,0,0,0,152);count=0;end
 Frame;
 UNTIL (count2>400 OR vidas<0);

 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=50);
 enemigo(16,0,140,10,0,0,0);
 count=0;END
 frame;
 Until (count2>140 OR vidas<0);

 count2=0;count=0;

 enemigo(17,100,130,0,2,0,157);
 enemigo(17,540,130,0,2,0,157);

 repeat
 count2=count2+1;
 count=count+1;
 if (count==50);enemigo(6,100,110,0,0,0,152);end
 if (count==100);enemigo(6,200,110,0,0,0,152);end
 if (count==150);enemigo(6,300,110,0,0,0,152);end
 if (count==200);enemigo(6,400,110,0,0,0,152);end
 if (count==250);enemigo(6,500,110,0,0,0,152);end
 if (count==300);enemigo(6,600,110,0,0,0,152);end
 Frame;
 UNTIL (count2>500 OR vidas<0);

 count2=0;count=0;


 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(33,x,y,0,0,0,14);count=0;END
 frame;
 Until (count2>120 OR vidas<0);

 count2=0;count=0;

 screens(4,163,0);frame; //enemigo 22

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=10);enemigo(15,200,120,0,20,0,163);enemigo(15,400,120,0,20,0,163);count=0;end
 frame;
 Until (count2>100 OR vidas<0);

 count=0;count2=0;
 enemigo(16,0,140,10,0,0,0);

 screens(5,158,0);frame; //enemigo 22

 count=0;count2=0;
  repeat
  if (rand(0,100)<2);enemigo(18,rand(10,630),475,0,rand(-2,-5),0,0);frame(1500);count=count+1;end
  until (count=>40);
 count=0;count2=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count==25);enemigo(7,0,240,20,0,0,153);
 enemigo(2,x,y,0,0,0,19);end
 if (count==50);enemigo(7,640,240,-20,0,1,153);
 enemigo(2,x,y,0,0,0,19);end
 if (count==75);enemigo(7,0,125,20,0,0,153);
 enemigo(2,x,y,0,0,0,19);end
 if (count==100);enemigo(7,640,400,-20,0,1,153);
 enemigo(2,x,y,0,0,0,19);end
 if (count==125);enemigo(7,0,475,20,-10,0,153);
 enemigo(2,x,y,0,0,0,19);end
 if (count==150);enemigo(7,635,475,-20,-10,1,153);
 enemigo(2,x,y,0,0,0,19);end
 if (count==175);enemigo(7,0,240,20,0,0,153);
 enemigo(2,x,y,0,0,0,19);END;
 if (count==200);enemigo(7,640,240,-20,0,1,153);
 enemigo(2,x,y,0,0,0,19);END;
 if (count==225);enemigo(7,0,125,20,0,0,153);
 enemigo(2,x,y,0,0,0,19);END;
 if (count==250);enemigo(7,640,400,-20,0,1,153);
 enemigo(2,x,y,0,0,0,19);END;
 if (count==275);enemigo(7,0,475,20,-10,0,153);
 enemigo(2,x,y,0,0,0,19);END;
 if (count==300);enemigo(7,635,475,-20,-10,1,153);
 enemigo(2,x,y,0,0,0,19);END;
 if (count==400);enemigo(7,0,240,20,0,0,153);
 enemigo(2,x,y,0,0,0,19);

     enemigo(12,0,140,10,0,0,154);

     enemigo(7,640,240,-20,0,1,153);
     enemigo(7,0,125,20,0,0,153);
     enemigo(7,640,400,-20,0,1,153);
     enemigo(7,0,475,20,-10,0,153);
     enemigo(7,635,475,-20,-10,1,153);
 end
 frame;
 UNTIL (count2>400 OR vidas<0);

 count2=0;count=0;

 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(3,x,y,0,0,0,14);count=0;enemigo(33,x,y,0,0,0,14);count=0;end
 frame;
 Until (count2>200 or vidas<0);

 count=0;count2=0;
 repeat
 count2=count2+1;
 count=count+1;
 if (count>=30);enemigo(2,x,y,0,0,0,159);enemigo(6,320,240,0,0,0,152);count=0;END;
 frame;
 Until (count2>200 OR vidas<0);

 count=0;count2=0;

 screens(4,163,0);frame; //enemigo 22

  repeat
  if (rand(0,100)<4);enemigo(19,rand(10,630),475,0,0,0,0);frame(500);count=count+1;end
  until (count=>40);
 count=0;count2=0;


 enemigo(11,rand(5,635),120,0,0,0,166);frame;
 enemigo(11,rand(5,635),120,0,0,0,166);frame;
 enemigo(11,rand(5,635),120,0,0,0,166);frame;
 enemigo(11,rand(5,635),120,0,0,0,166);frame;


 count=0;count2=0;

 screens(4,160,0);frame; //enemigo 22

 repeat
 count2=count2+1;
 count=count+1;
 if (count==100);enemigo(14,320,120,0,0,0,0);END;
 if (count==200);enemigo(14,100,120,0,0,0,0);END;
 if (count==300);enemigo(14,500,120,0,0,0,0);END;
 Frame;
 UNTIL (count2>500 OR vidas<0);
 screens(6,66,1);frame(20000);
 frame(9000);

 enemigo(13,0,0,0,0,0,0);
END


//************************************************************************
//Process texto Nivel 1***************************************************
//************************************************************************

process textonivel1(x,y,graph);

private
count=0;

begin
x=x:
y=y;
size=0;
graph=graph; //41
if (graph==41);
 repeat
   from graph=40 to 41;
  if (size<200);size=size+10;angle=angle+10000;end
  if (size=>200);count=count+1;angle=angle+10000;end
  if (count>52)angle=angle-10000;
  end
  if (count>70)size=size+20;end
 frame;
 end
 until (size>500);
end

 if (graph==2 and x==320);
  write(0,320,300,1,"Thanks for Playing the Shareware Version of The Shag Attack.");
  write(0,320,320,1,"Programmed with DIV Games Studio by Eduardo Marquez. (FragLord)");
  write(0,320,340,1,"(c) 1998 FragLord");
 repeat
  if (size<100);size=size+1;end
  frame;
 until (estadoprograma==0);
 end

 if (graph==51);
  repeat
  if (size<150);size=size+5;end
  frame;
 until (estadoprograma==0);
 end

if (graph==45);
x=320;
y=240;
graph=45;
size=300;
 repeat
  size=size-10;
  frame;
 until (size=<0);
end

if (graph==1 or Graph==2 or graph==3 or graph==4);
size=0;
 repeat
  size=size+2;
  frame;
 until(size=>100);
while(selecttrue==0);frame;end
end
end


//*************************************************************************
//Process Screens *********************************************************
//*************************************************************************

process screens(tipo,grafico,pre);

private;
count=0;
count2=0;
count3=0;
ventext1;
ventext2;
ventext3;
ventext4;
ventext5;
ventext6;

Begin

if (pre==1);
  x=48;
  y=47;
  size=0;
  repeat
  graph=65;
  if (size<95)size=size+5;end
  frame;
  until (size==95 OR vidas<0 OR count3==1);
  count3=1;
end
  count3=1;
  if (count3==1);
  count=1;

   if (tipo==1); //66
   x=48;
   y=47;
    sound(sonido_transmision,256,256);
    graph=grafico;
    if (count==1);ventext1=write(0,556,20,3,"Ok lets kick");frame(500);count=2;end
    if (count==2);ventext2=write(0,556,30,3,"some alien");frame(500);count=3;end
    if (count==3);ventext3=write(0,556,40,3,"Ass!! Ill be");frame(500);count=4;end
    if (count==4);ventext4=write(0,556,50,3,"with you");frame(500);count=5;end
    if (count==5);ventext5=write(0,556,60,3,"this level.");frame(500);count=6;end
    if (count==6);ventext6=write(0,556,70,3,"Lets go.");frame(500);count=7;end
    if (count==7);frame(10000);
       delete_text(ventext1);
       delete_text(ventext2);
       delete_text(ventext3);
       delete_text(ventext4);
       delete_text(ventext5);
       delete_text(ventext6);
       count=8;
    end
    end
   end

   if (tipo==2);
   x=48;
   y=47;
    sound(sonido_transmision,256,256);
    graph=grafico; //66
    if (count==1);ventext1=write(0,556,20,3,"Remember: ");frame(500);count=2;end
    if (count==2);ventext2=write(0,556,30,3,"you loose");frame(500);count=3;end
    if (count==3);ventext3=write(0,556,40,3,"fire power");frame(500);count=4;end
    if (count==4);ventext4=write(0,556,50,3,"and energy");frame(500);count=5;end
    if (count==5);ventext5=write(0,556,60,3,"when hit.");frame(500);count=6;end
    if (count==6);frame(10000);
       delete_text(ventext1);
       delete_text(ventext2);
       delete_text(ventext3);
       delete_text(ventext4);
       delete_text(ventext5);
       count=8;
    end
    end

   if (tipo==3);
   x=48;
   y=47;
    sound(sonido_transmision,256,256);
    graph=grafico;
    if (count==1);ventext1=write(0,556,20,3,"Watch out");frame(500);count=2;end
    if (count==2);ventext2=write(0,556,30,3,"for blue");frame(500);count=3;end
    if (count==3);ventext3=write(0,556,40,3,"enemy shoots");frame(500);count=4;end
    if (count==4);ventext4=write(0,556,50,3,"they freeze");frame(500);count=5;end
    if (count==5);ventext5=write(0,556,60,3,"you stiff...");frame(500);count=6;end
    if (count==6);ventext6=write(0,556,70,3,"");frame(500);count=7;end
    if (count==7);frame(10000);
       delete_text(ventext1);
       delete_text(ventext2);
       delete_text(ventext3);
       delete_text(ventext4);
       delete_text(ventext5);
       delete_text(ventext6);
       count=8;
    end
    end

   if (tipo==6);
   x=48;
   y=47;
    sound(sonido_transmision,256,256);
    graph=grafico;
    if (count==1);ventext1=write(0,556,20,3,"Boss 1 is");frame(500);count=2;end
    if (count==2);ventext2=write(0,556,30,3,"just ahead!");frame(500);count=3;end
    if (count==3);ventext3=write(0,556,40,3,"I hope you");frame(500);count=4;end
    if (count==4);ventext4=write(0,556,50,3,"have a High");frame(500);count=5;end
    if (count==5);ventext5=write(0,556,60,3,"Fire Power.");frame(500);count=6;end
    if (count==6);ventext6=write(0,556,70,3,"Good Luck.");frame(500);count=7;end
    if (count==7);frame(10000);
       delete_text(ventext1);
       delete_text(ventext2);
       delete_text(ventext3);
       delete_text(ventext4);
       delete_text(ventext5);
       delete_text(ventext6);
       count=8;
    end
    end

   if (tipo==4);
    x=48;
    y=47;
    graph=grafico;
    repeat;angle=angle+20000;frame;until(angle=>360000 or vidas<0);
    animator(3);
    frame(12200);
    end

   if (tipo==5);
    x=590;
    y=47;
    graph=grafico;
    repeat;angle=angle+20000;frame;until(angle=>360000 or vidas<0);
    animator(4);
    frame(12200);
    end


if (pre==1);
  if (count==8);
   graph=65;
   repeat
    size=size-5;
   frame;
   until (size=<0);
  end
 frame;
end

pre=0;
end

//**********************************************************************
// Process NAVE ********************************************************
//**********************************************************************

PROCESS nave();

PRIVATE
espera=0;
espera1=0;
espera2=0;
count=0;

BEGIN
mouse.x=320;
mouse.y=400;

repeat
graph=selectplayer;
x=mouse.x;
y=mouse.y;
z=-512;
 if (key(_right));x=x+12;mouse.x=x;end
 if (key(_left));x=x-12;mouse.x=x;end
 if (key(_up));y=y-12;mouse.y=y;end
 if (key(_down));y=y+12;mouse.y=y;end
 if (y<=100);y=100;end
 frame;

    if (specials=>3);specials=3;end

    IF (mouse.left OR key (_space) OR key(_control));
     if (disparospeed==0);espera=espera+1;
      if (espera==5);disparomanager(x,y);espera=0;end
     end

     if (disparospeed==1);disparomanager(x,y);end
    end

    IF (specials==1);
     if (mouse.right OR key(_enter));
      specials=0;
      if (selectplayer==141);
       specialattack(1,320,500,95);
      end
      if (selectplayer==142);
       specialattack(1,320,500,144);
      end
      if (selectplayer==143);
       specialattack(1,320,500,146);
      end
      if (selectplayer==140);
       specialattack(1,320,500,145);
      end
     end
    end


    IF (specials==2);
     if (mouse.right OR key(_enter));
     specials=0;
     sound(sonido_attention,60,256);
     helicopter(320,500);
     helicopter(160,500);
     helicopter(480,500);
     end
    end


    IF (specials=>3);
     if (mouse.right OR key(_enter));
     specials=0;
     sound(sonido_attention,60,256);
     frame(2000);
    specialattack(3,mouse.x,mouse.y,0);
    end

    count=0;
    end

   if (estadonave==0);
    if (collision(type energyeater));frame(2000);end
    if (collision(type disparoenemigo)
    OR (collision(type enemigo))
    OR (collision(type bolafuego))
    OR (collision(type heatseeker))
    OR (collision(type minas)))

    sound(sonido_explosion,50,100);
    from graph=42 to 53;frame;end;
    energianave=energianave-1;
    estadodisparo=estadodisparo-1;

        if (energianave==1);lowenergy();frame;end

        if (energianave==0);vidas=vidas-1;energianave=5;estadodisparo=3;end

        mouse.x=320;
        mouse.y=400;

        if (escudorepeat==0);escudo(1,x,y);escudorepeat=1;end
    end;
   end;
  until (vidas<0);
end


//*******************************************************************
//Process Bonus *****************************************************
//*******************************************************************

process bonus(x,y,graph);

begin
graph=graph;
 repeat
  y=y+2;
  angle=angle+10000;
  frame;

     if (collision(type nave));sound(sonido_buiu,90,256);
      if (graph==169 OR graph==171 or Graph==177);escudo(1,x,y);end
      if (graph==101);vidas=vidas+1;end
      if (graph==168);puntos=puntos+5000;angle=0;graph=24;frame(3000);end
      if (graph==170);energianave=energianave+1;end
      if (graph==173 or graph==174 or graph==175 or graph==176);estadodisparo=estadodisparo+1;end
      if (graph==172 or graph==178);specials=specials+1;end
        from graph=42 to 53;frame;end
     break;end

 until (x>640 OR y>480 OR x<0 OR vidas<0)
end


//*********************************************************************
//process Disparo Manager *********************************************
//*********************************************************************

process disparomanager(x,y);

Private
espera=0;

begin
 if (estadodisparo=<0);disparospeed=0;escudorepeat2=0;disparo(0,x,y,0,0,0);estadodisparo=0;firepower=0;end
 if (estadodisparo==1);disparospeed=0;escudorepeat2=0;disparo(1,x,y-20,0,0,0);firepower=1;end
 if (estadodisparo==2);disparospeed=0;escudorepeat2=0;disparo(2,x,y,0,0,0);firepower=2;end
 if (estadodisparo==3);disparospeed=0;escudorepeat2=0;disparo(4,x,y,0,+30,0);disparo(4,x,y,0,-30,0);firepower=3;end
 if (estadodisparo==4);disparospeed=1;escudorepeat2=0;disparo(3,x,y,0,0,0);firepower=4;end
 if (estadodisparo==5);disparospeed=0;escudorepeat2=0;disparo(5,x,y,-4,0,0);disparo(5,x,y,-2,0,0);disparo(5,x,y,0,0,0);disparo(5,x,y,+2,0,0);disparo(5,x,y,+4,0,0);firepower=5;end
 if (estadodisparo==6);disparospeed=0;escudorepeat2=0;disparo(5,x,y,-4,0,0);disparo(5,x,y,-2,0,0);disparo(5,x,y,0,0,0);disparo(5,x,y,+2,0,0);disparo(5,x,y,+4,0,0);disparomisil(x,y,+4,0);disparomisil(x,y,-4,0);firepower=6;end
 if (estadodisparo==7);disparospeed=0;escudorepeat2=0;disparomisil(x,y,+4,0);disparomisil(x,y,+8,0);disparomisil(x,y,-4,0);disparomisil(x,y,-8,0);disparomisil(x,y,0,0);firepower=7;end
 if (estadodisparo==8);disparospeed=1;escudorepeat2=0;disparo(8,rand(x-5,x+5),y,0,-35,0);disparo(8,rand(x-5,x+5),y,-6,-35,0);disparo(8,rand(x-5,x+5),y,+6,-35,0);firepower=8;end
 if (estadodisparo==9);disparospeed=1;escudorepeat2=0;disparo(9,x,y,0,0,0);firepower=9;end
 if (estadodisparo=>10);disparospeed=0;estadodisparo=10;disparo(10,x,y,0,-35,0);disparo(10,x,y,8,-35,0);disparo(10,x,y,4,-35,0);disparo(10,x,y,-4,-35,0);disparo(10,x,y,-8,-35,0);disparomisil(x,y,+4,+4);disparomisil(x,y,+4,-4);disparomisil(x,y,-4,+4);disparomisil(x,y,-4,-4);firepower=10;if (escudorepeat2==0);escudo(2,x,y);end;end
end



//*******************************************************************
// Process Escudo ***************************************************
//*******************************************************************

process escudo(tipo,x,y);

Private
count=0;

Begin

if (tipo==1);
graph=71;
size=0;
 repeat
  if (y<=100);y=100;end
  estadonave=1;
  if (size<100);size=size+10;end
  x=mouse.x;
  y=mouse.y;
  angle=angle+10000;
 frame;
 until (angle=>1440000 OR vidas<0);
 while (size>=0);size=size-10;
  x=mouse.x;
  y=mouse.y;
  frame;end
 escudorepeat=0;
 estadonave=0;
end

if (tipo==2);
if (y<=100);y=100;end
graph=76;
size=0;
 repeat
 if (size<75);size=size+10;end
 x=mouse.x;
 y=mouse.y;
 angle=angle+20000;

frame;
escudorepeat2=1;
until (estadodisparo=<9 OR vidas<0)
end
end
//*******************************************************************
// Process DISPARO **************************************************
//*******************************************************************

process disparo(disp,x,y,incx,incy,flags)

Private
canal;
numhumos=0;
count=0;
count2=0;
navelocate;
naveangle;


Begin

if (disp=<0);
 graph=4;
 sound(sonido_disparo,20,200);
  repeat
  y-=16;
  frame;

    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end
  until(y=<105 OR vidas<0);
end


if (disp==1);

angle=angle-90000;
sound(sonido_disparo2,50,256);
graph=56;
frame(500);

  repeat
   y-=20;
   frame;
    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end

  until (x>640 OR y>480 OR x<0 OR y<130 OR vidas<0)
end



if (disp==2);

  graph=77;
  sound(sonido_faser,60,512);
  angle=angle+90000;
  repeat
  y-=25;
  frame;
    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end

  until(y=<115 OR vidas<0);
end


if (disp==3);
 graph=93;
  sound(sonido_disparo3,30,256);
  angle=angle+90000;
  repeat
  y-=25;
  frame;

    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end
  until(y=<115 OR vidas<0);
end

if (disp==4);
 graph=94;
 size=150;
  sound(sonido_disparo4,50,256);
  repeat
  x+=incx;
  y+=incy;
  frame;

    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end
  until(y=<115 OR y=>480 OR vidas<0);
end


if (disp==5);
 graph=98;
  sound(sonido_disparo,10,512);
  angle=angle-90000;
  repeat
  y-=25;
  x+=incx;
  frame;

    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end
  until(y=<115 OR x>640 OR x<0 OR vidas<0);
end


if (disp==6);
 graph=98;
  sound(sonido_disparo,10,512);
  angle=angle-90000;
  repeat
  y-=25;
  x+=incx;
  frame;

    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end
  until(y=<115 OR x>640 OR x<0 OR vidas<0);
end


if (disp==8);

graph=16;
angle=-90000;
sound(sonido_disparo2,20,256);
  repeat
  y+=incy;
  x+=incx;
  frame;

    if (collision(type enemigo)
     OR (collision(type heatseeker)))
    break;end
  until(x>640 OR x<0 OR y>480 OR y<115 OR vidas<0);
end


if (disp==9);

graph=100;
angle=90000;

     repeat
    navelocate=get_id(type enemigo);
    if (navelocate==0);
       navelocate=get_id(type heatseeker);
       if (navelocate==0);return;end

    end
      canal=sound(sonido_metralla,5,512);
       naveangle=get_angle(navelocate);
       angle=near_angle(angle,naveangle,360000);
       advance(35);

    frame;
    until (collision(type enemigo)
    OR (collision(type heatseeker)))
end

if (disp==10);

graph=148;
sound(sonido_faser,10,256);
  repeat
  y+=incy;
  x+=incx;
  frame;
  until(x>640 OR x<0 OR y>480 OR y<115 OR vidas<0);
end

end

//**************************************************************************
// Process Disparo Misil ******************************************************
//**************************************************************************

process disparomisil(x,y,incx,incy)

private
navelocate;
naveangle;
count=0;
count2=0;

begin

if (estadodisparo=>8);
 graph=147;
 repeat
  angle=+90000;
   if (count==0);x+=incx;y+=incy;count2=count2+1;end
    if (count2==8);count=1;end
   frame;
  until (count==1)

   repeat
    navelocate=get_id(type enemigo);
    if (navelocate==0);
           navelocate=get_id(type heatseeker);
           if (navelocate==0);return;end
           end;
       naveangle=get_angle(navelocate);
       angle=near_angle(angle,naveangle,360000);
       advance(25);
    frame;
    until (collision(type enemigo) OR (collision(type heatseeker)))
end

if (estadodisparo==6 OR estadodisparo==7);

graph=99;
 repeat
  if (count==0);x+=incx;count2=count2+1;end
   if (count2==8);count=1;end
  frame;
 until (count==1)
 repeat
 y-=20;
 frame;
 if (collision(type heatseeker));from graph=8 to 13;frame;end;break;end

 until (vidas<0 OR y<=110)
end
end
//******************************************************************
// Process low Energy **********************************************
//******************************************************************

Process lowenergy();

Private
lowenergytext;

begin
  repeat
   sound(sonido_emergencia,50,200);
   lowenergytext=write(0,414,28,1,"Low Energy");frame(500);
   delete_text(lowenergytext);frame(500);
  until(vidas<0 OR energianave>1);
end


//******************************************************************
// Process Main ****************************************************
//******************************************************************

PROCESS Main();

PRIVATE
    select;

BEGIN
    choose=0;
    signal(father,s_sleep);

    set_fps(50,0);
    set_mode(m640x480);
    fade(200,200,200,8);        // Pone la pantalla en blanco
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla

    put(0,15,320,240);

    fade_on();                  // Hace que se vea la pantalla (la enciende)
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
    mouse.x=320;
    mouse.y=300;
    mouse.graph=7;

    repeat
      if (key(_right));x=x+12;mouse.x=x;end
      if (key(_left));x=x-12;mouse.x=x;end
      if (key(_up));y=y-12;mouse.y=y;end
      if (key(_down));y=y+12;mouse.y=y;end

     if (mouse.left or key(_enter))
      if (mouse.x>260 AND mouse.x<375 AND mouse.y>210 AND mouse.y<255);choose=1;end
      if (mouse.x>160 AND mouse.x<480 AND mouse.y>275 AND mouse.y<328);choose=2;end
      if (mouse.x>260 AND mouse.x<375 AND mouse.y>348 AND mouse.y<395);choose=3;end
     end

   frame;
  until (choose==1 OR choose==2 OR choose==3);

  repeat
   if (choose==2);
    clear_screen();
    delete_text(all_text);
    mouse.graph=0;
    put(0,14,320,240);
    write(english,0,180,3,"The object of the game is to lead one of the Shag Squad Members to Victory, by killing the");
    write(english,0,195,3,"End-Boss of level 1. You start your quest by selecting one of the Shag Squad Members. Each"):
    write(english,0,210,3,"one of them has different Fire power, Energy, and a number of *Specials*, ranging from");
    write(english,0,225,3,"Yaga -level beginner, to FragLord -level Pro. The missin itself is not so easy. There are");
    write(english,0,240,3,"many enemies out there. Some, just kill you. Others go as far as freezing you stiff, or"):
    write(english,0,255,3,"even sucking your fire power away. You will need to keep your Fire Power high if you want"):
    write(english,0,270,3,"to survive. You start your quest with ZERO lives, and a certain amount of energy. If your");
    write(english,0,285,3,"energy reaches Zero, you loose a life. If your lives and energy are both equal to zero, the");
    write(english,0,300,3,"game finishes, leaving you stranded in the desert, with failure as a result."):

    write(spanish,0,330,3,"El objetivo del juego es conseguir que unos de los miembros del equipo Shag logre la"):
    write(spanish,0,345,3,"victoria, derrotando al enemigo final de el nivel 1. Empiezas la aventura seleccionando"):
    write(spanish,0,360,3,"a un jugador de tu elecci¢n. Cada uno de ellos tiene diferentes niveles de energia,"):
    write(spanish,0,375,3,"potencia de disparos, y numero de *Especiales*, desde Yaga -nivel principiante, a"):
    write(spanish,0,390,3,"FragLord -nivel Pro. Existen muchos tipos de enemigos. Desde los que simplemente te"):
    write(spanish,0,405,3,"matan a los que te congelan y le roban energia a tu disparo. Se empieza con 0 vidas, y"):
    write(spanish,0,420,3,"un numero determinado de energia. Si tu energia llega a Cero, perderas una vida. Si no"):
    write(spanish,0,435,3,"te quedan vidas, preparate para comer tierra en el desierto."):
    frame;
    while (NOT mouse.left AND NOT key(_esc) AND NOT key(_enter) AND NOT key(_space));frame;end
     delete_text(all_text);
    write(english,0,180,3,"Each time your ship is destroyed, you will loose 1 point of Fire Power, and 1 point of");
    write(english,0,195,3,"energy. You will then restart with a shield, and you will be invulnerable for about 4"):
    write(english,0,210,3,"seconds. After then, you can be killed again. Your ship can be controlled with either");
    write(english,0,225,3,"the mouse, or the keyboard. Use the left mouse buton to shoot, and the right mouse");
    write(english,0,240,3,"button to fire your special attack (if you have any). With the keyboard, use the cursor"):
    write(english,0,255,3,"keys to move around, Space Bar or Control to Shoot, and Enter to Fire your Special."):
    write(english,0,270,3,"The mouse will give you more speed to move, but the keyboard will make you move and"):
    write(english,0,285,3,"shoot more accurately."):

    write(spanish,0,315,3,"Cada vez que la nave del jugador sea destruida, perdera 1 punto de energia, y 1 punto"):
    write(spanish,0,330,3,"de potencia de disparo. Despues, si aun queda energia o vidas, se volvera a empezar"):
    write(spanish,0,345,3,"con un escudo alrededor de la nave. Mientras dure el escudo, el jugador sera"):
    write(spanish,0,360,3,"invulnerable. La nave puede ser controlada con el raton, o con el teclado."):
    write(spanish,0,375,3,"Con el raton: Para disparar, se usa el boton izquierdo. Para usar los *Especiales*,"):
    write(spanish,0,390,3,"el derecho (si es que se dispone de especiales). Con el teclado: las teclas de cursor"):
    write(spanish,0,405,3,"son para moverse. Para disparar, se usa Espacio, o Control. Para usar los Especiales,"):
    write(spanish,0,420,3,"Enter. El raton da mas velocidad a la hora de moverse, pero con los cursores se puede ");
    write(spanish,0,435,3,"disparar con mas precisi¢n.");
     frame(1000);
    while (NOT mouse.left AND NOT key(_esc) AND NOT key(_enter) AND NOT key(_space));frame;end
     put(0,16,320,240);
     delete_text(all_text);
     frame(1000);
    while (NOT mouse.left AND NOT key(_esc) AND NOT key(_enter) AND NOT key(_space));frame;end
     clear_screen();
     put(0,40,320,240);
     delete_text(all_text);
     frame(1000);
    while (NOT mouse.left AND NOT key(_esc) AND NOT key(_enter) AND NOT key(_space));frame;end
     clear_screen();
     delete_text(all_text);
     mouse.graph=0;
     put(0,14,320,240);

     write(english,320,180,1,"Important Notes from the Programmer:");
     write(english,320,195,1,"------------------------------------"):
     write(english,0,225,3,"1.) The background Scroll can only be seen with no reflection on the monitor, or");
     write(english,0,240,3,"    in a room with no sun light.");
     write(english,0,255,3,"2.) This is the Shareware Version of THE SHAG ATTACK, which means that there is");
     write(english,0,270,3,"    only 1 level.");
     write(english,0,285,3,"3.) Vendetta appears during the whole game (even if you pick Yaga or FragLord as");
     write(english,0,300,3,"    your player because Vendetta is in charge of level 1. On the registered"):
     write(english,0,315,3,"    version, there will be 4 levels, with each character on a different level."):
     write(english,0,330,3,"4.) When your Fire Power reaches 10, an Anti-Missle Shield will appear."):
     write(english,0,345,3,"5.) Every 20,000 points you will receive an extra life automatically."):
     write(english,320,375,1,"** The future release of the Shag Attack will include: **");
     write(english,320,390,1,"------------------------------------------------------");
     write(english,320,405,1,"* 20 Levels of Fire Power *  /  * 2 Player Action *  /  * Tons of New Enemies *");
     write(english,320,420,1,"* 4 New Episodes *  /  * More Special Attacks *  /  * More music & New SFX *");
     write(english,320,435,1,"* New Bonuses *  /  * More Animations *  / * Etc etc! *");


     frame(1000);
    while (NOT mouse.left AND NOT key(_esc) AND NOT key(_enter) AND NOT key(_space));frame;end
     clear_screen();
     delete_text(all_text);
     mouse.graph=0;
     put(0,14,320,240);

     write(spanish,320,180,1,"Notas importantes del Programador:");
     write(spanish,320,195,1,"------------------------------------"):
     write(spanish,0,225,3,"1.) El scroll de pantalla aveces no se puede ver si hay demasiada luz sobre");
     write(spanish,0,240,3,"    el monitor. Es recomendable jugar sin reflejos solares en pantalla.");
     write(spanish,0,255,3,"2.) Esta es la version shareware de The Shag Attack, lo que significa que");
     write(spanish,0,270,3,"    solo hay 1 nivel.");
     write(spanish,0,285,3,"3.) Vendetta aparece durante todo el juego porque ‚l es el encargado del nivel");
     write(spanish,0,300,3,"    1. En la version registrada apareceran los 4 personajes, en 4 niveles distintos."):
     write(spanish,0,315,3,"4.) Cuando la potencia de disparo llegue a 10, aparece  un escudo anti-misil."):
     write(spanish,0,330,3,"5.) Cada 20,000 puntos el jugador recibira una vida extra."):
     write(spanish,320,375,1,"** La version registrada de The Shag Attack incluir : **");
     write(spanish,320,390,1,"------------------------------------------------------");
     write(spanish,320,405,1,"* 20 niveles de Potencia de Disparo *  /  * 2 jugadores simultaneos *  /  * muchos enemigos *");
     write(spanish,320,420,1,"* 4 Episodios *  /  * Mas Ataques especiales *  /  * Nuevos efectos de musica y sonido *");
     write(spanish,320,435,1,"* Nuevos Bonuses *  /  * Mas Animaciones *  / * Etc etc! *");


     frame(1000);
    while (NOT mouse.left AND NOT key(_esc) AND NOT key(_enter) AND NOT key(_space));frame;end


    signal(father,s_wakeup);
    estadoprograma=0;
    break;
   end

   if (choose==3);
    clear_screen();
    delete_text(all_text);
    mouse.graph=0;
    signal(father,s_wakeup);
    estadoprograma=10;
   end

   if (choose==1);
    clear_screen();
    delete_text(all_text);
    signal(father,s_wakeup);
    mouse.graph=0;
    estadoprograma=1;
   end

  until (estadoprograma==1 OR estadoprograma==10 OR estadoprograma==0)

end


//******************************************************************
// Process Selection ***********************************************
//******************************************************************

PROCESS selection();

PRIVATE
    select;
BEGIN
    //sonido_rythm=load_pcm("c:\shag\rythm.pcm",1);
    signal(father,s_sleep);
    set_fps(50,0);
    fade(200,200,200,8);        // Pone la pantalla en blanco
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla

    textonivel1(100,100,1);
    textonivel1(100,400,2);
    textonivel1(530,100,3);
    textonivel1(520,400,4);

    selectionship(100,210,10);
    selectionship(100,300,11);
    selectionship(520,210,9);
    selectionship(520,300,8);

    //sound(sonido_rythm,70,256);


    fade_on();                  // Hace que se vea la pantalla (la enciende)
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
    selectiontext();frame;
    mouse.x=320;
    mouse.y=300;
    mouse.graph=7;

    repeat
     if (countselection==0);
      if (key(_right));x=x+12;mouse.x=x;end
      if (key(_left));x=x-12;mouse.x=x;end
      if (key(_up));y=y-12;mouse.y=y;end
      if (key(_down));y=y+12;mouse.y=y;end

      if (mouse.x>50 AND mouse.x<150 AND mouse.y>50 AND mouse.y<150);
         delete_text(all_text);
         write(0,320,45,1,"FragLord");
         write(0,320,60,1,"--------");
         write(0,320,75,1,"Level: Pro");
         write(0,320,90,1,"Inital Energy: 2");
         write(0,320,105,1,"Initial Fire Power: 0");
         write(0,320,120,1,"N§ of Specials: 0");
         countselection=1;

      end
     end

     countselection=0;

     if (countselection==0);
      if (mouse.x>500 AND mouse.x<600 AND mouse.y>50 AND mouse.y<150);
         delete_text(all_text);
         write(0,320,45,1,"Yaga");
         write(0,320,60,1,"----");
         write(0,320,75,1,"Level: Beginner");
         write(0,320,90,1,"Inital Energy: 6");
         write(0,320,105,1,"Initial Fire Power: 4");
         write(0,320,120,1,"N§ of Specials: 1");
         countselection=1;

      end
     end

     countselection=0;

     if (countselection==0);
      if (mouse.x>50 AND mouse.x<150 AND mouse.y>350 AND mouse.y<450);
         delete_text(all_text);
         write(0,320,350,1,"Vendetta");
         write(0,320,365,1,"--------");
         write(0,320,380,1,"Level: Advanced");
         write(0,320,395,1,"Inital Energy: 3");
         write(0,320,410,1,"Initial Fire Power: 2");
         write(0,320,425,1,"N§ of Specials: 0");
         countselection=1;
      end
     end

     countselection=0;

     if (countselection==0);
      if (mouse.x>500 AND mouse.x<600 AND mouse.y>340 AND mouse.y<440);
         delete_text(all_text);
         write(0,320,350,1,"Nemireon");
         write(0,320,365,1,"--------");
         write(0,320,380,1,"Level: Medium");
         write(0,320,395,1,"Inital Energy: 5");
         write(0,320,410,1,"Initial Fire Power: 3");
         write(0,320,425,1,"N§ of Specials: 1");
         countselection=1;
      end
     end

     if (mouse.left or key(_enter))
      if (mouse.x>50 AND mouse.x<150 AND mouse.y>50 AND mouse.y<150);energianave=2;estadodisparo=0;specials=0;firepower=0;selectplayer=142;end     // Eddie
      if (mouse.x>500 AND mouse.x<600 AND mouse.y>50 AND mouse.y<150);energianave=6;estadodisparo=4;specials=1;firepower=4;selectplayer=141;end    // Daniel
      if (mouse.x>50 AND mouse.x<150 AND mouse.y>350 AND mouse.y<450);energianave=3;estadodisparo=2;specials=0;firepower=2;selectplayer=143;end    // Steven
      if (mouse.x>500 AND mouse.x<600 AND mouse.y>340 AND mouse.y<440);energianave=5;estadodisparo=3;specials=1;firepower=4;selectplayer=140;end   // Nemir
     end

   frame;
   until (selectplayer==143 OR selectplayer==142 OR selectplayer==141 or selectplayer==140 OR vidas<0);

   clear_screen();
   delete_text(all_text);
   unload_fpg(select);
   signal(father,s_wakeup);
   mouse.graph=0;
   selecttrue=1;
   estadoprograma=2;
end

//******************************************************************
// Process Selectiontext *******************************************
//******************************************************************

process selectiontext();

begin
x=320;
y=240;
 repeat
  from graph=5 to 6;
  frame;
  end
 until (selecttrue==1);
end


//******************************************************************
// Process Selectionship *******************************************
//******************************************************************

process selectionship(x,y,graph)

begin
x=x;
y=y;
graph=graph;

 repeat
  angle=angle+5000;
  frame;
 until (selecttrue==1);
end


//******************************************************************
// Process FragLord ************************************************
//******************************************************************

PROCESS fraglord();
PRIVATE
    contador0;
    intromapa;
    fragmap;
    fragfpg;
BEGIN
    signal(father,s_sleep);
    set_fps(20,0);
    set_mode(m640x480);

    load_pcm("user\shag\explosio.pcm",0);
    load_pal("user\shag\fraglord.fpg");
    fragfpg=load_fpg("user\shag\fraglord.fpg");

    x=320;
    y=240;
    graph=1;
    size=0;

     repeat
      angle=angle-30000;
      size=size+1;
      frame;
     until (angle=<-1440000);
    sound(sonido_explosion,70,100);

    fade(200,200,200,8);        // Pone la pantalla en blanco
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
    unload_map(fragmap);    // Descarga el grafico
    fade_on();                  // Hace que se vea la pantalla (la enciende)
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla

    frame(2000);

    write(0,320,300,1,"Presents....");

    WHILE (NOT key(_esc) AND NOT key(_space) AND NOT key(_enter) AND contador0<70)
    FRAME;
    contador0++;

    END
    unload_fpg(fragfpg);
    delete_text(all_text);
    signal(father,s_wakeup);
end

//******************************************************************
// Process Intro ***************************************************
//******************************************************************

PROCESS intro();
PRIVATE
    contador0;
    intromapa;
    fx_intro;
BEGIN
    signal(father,s_sleep);
    set_fps(25,0);
    //set_mode(m640x480);
    english=load_fnt("user\shag\english.fnt");
    spanish=load_fnt("user\shag\spanish.fnt");

digital=load_fnt("user\shag\fire.fnt");
info=load_fnt("user\shag\info.fnt");
sonido_disparo=load_pcm("user\shag\disparo.pcm",0);
sonido_disparo2=load_pcm("user\shag\disparo2.pcm",0);
sonido_disparo3=load_pcm("user\shag\disparo3.pcm",0);
sonido_disparo4=load_pcm("user\shag\disparo4.pcm",0);
sonido_explosion=load_pcm("user\shag\explosion.pcm",0);
sonido_emergencia=load_pcm("user\shag\emergenc.pcm",0);
sonido_metralla=load_pcm("user\shag\metralla.pcm",0);
sonido_transmision=load_pcm("user\shag\trans.pcm",0);
sonido_faser=load_pcm("user\shag\faser.pcm",0);
sonido_buiu=load_pcm("user\shag\buiu.pcm",0);
sonido_attention=load_pcm("user\shag\attenti.pcm",0);
sonido_bomba=load_pcm("user\shag\bomba.pcm",0);
sonido_explode=load_pcm("user\shag\explode.pcm",0);
sonido_looseenergy=load_pcm("user\shag\loose.pcm",0);
sonido_aaah=load_pcm("user\shag\aaah.pcm",0);
sonido_explode2=load_pcm("user\shag\explode2.pcm",0);

    load_fpg("user\shag\select.fpg");
    load_pal("user\shag\select.fpg");
    fx_intro=load_pcm("user\shag\intromus.pcm",0);

    fade(200,200,200,8);        // Pone la pantalla en blanco
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
    put_screen(0,13);  // Pone el fondo de pantalla
    fade_on();                  // Hace que se vea la pantalla (la enciende)
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla




    graph=12;  //Shag Attack Graphic
     x=320;
     y=50;
     size=0;
     repeat
      size=size+2;
      frame;
     until (size=>100);
     sound(fx_intro,100,256);       // Hace el sonido


     intronaves(0,200,10);
     frame(1000);
     intronaves(0,300,9);
     frame(1000);
     intronaves(0,250,8);
     frame(1000);
     intronaves(0,350,11);

    WHILE (contador0<120);
    FRAME;
    contador0++;
    end
    signal(father,s_wakeup);
end

//******************************************************************
// Process Intronaves **********************************************
//******************************************************************

process intronaves(x,y,graph)

Begin
graph=graph;
x=x;
y=y;
angle=-90000;

 repeat
  x+=4;
  frame;
 until (x>320);

 repeat
  x=x+15;
  frame;
 until (x>640);
end


//******************************************************************
// Process Credits *************************************************
//******************************************************************

PROCESS credits();
PRIVATE
    fx_end;
BEGIN
    signal(father,s_sleep);

    set_fps(25,0);

    load_pal("user\shag\select.fpg");
    //codigo_mapa=load_map("user\shag\credits.map");


    fade(200,200,200,8);        // Pone la pantalla en blanco
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
    put(0,17,320,240);          // Pone el fondo de pantalla
    fade_on();                  // Hace que se vea la pantalla (la enciende)
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla


    WHILE (NOT key(_esc) AND NOT key(_space) AND NOT key(_enter))
    FRAME;

    END
    signal(father,s_wakeup);
    exit("Thanks for Shagging Today!",0);
end

//*******************************************************************
// Process Enemigo *************************************************
//*******************************************************************

process enemigo(enemigotype,x,y,incx,incy,flags,graph)

Private
navelocate;
naveangle;
count=0;
count2=0;
count4=0;
espera=0;
espera1=0;
espera2=0;
espera3=0;
espera4=0;
enemigo1life=4;
enemigo2life=7;
enemigo3life=10;
enemigo8life=15;
enemigo4life=30;

Begin

if (enemigotype==1);
x=20;
y=116;
graph=graph; //=5

  repeat
   if (y<100);y=y+5;end
    if (y=>100 and x=<600);x=x+5;
     if (rand(0,100)<25);espera1=espera1+1;
      if (espera1==5);bolafuego(0,x,y+10,10,55);espera1=0;end
     end
    end
   if (x>600);y=y+7;end

   if (collision(type disparo)
   OR (collision(type disparomisil)))
   enemigo1life=enemigo1life-1;end

   if (collision(type specialattack));enemigo1life=enemigo1life-20;end

   if (enemigo1life=<0 OR vidas<0);sound(sonido_explosion,30,256);
    from graph=8 to 13;frame;end
    puntos=puntos+50;
    break;
   end
   frame;
   until (y>=480 OR x>=640 OR y<=115 OR x<=0 OR vidas<0 OR enemigo1life=<0);
end

if (enemigotype==2);
x=320;
y=120;

angle=+90000;
graph=graph;  //=19
if (graph==19);
angle=+90000;
else angle=-90000;
end

    repeat
      if (y==120);x=x-5;end
      if (x=<20);y=y+5;end
      if (y>200);x=x+5;end
       if (rand(0,100)<25);espera2=espera2+1;
        if (graph==19);
        if (espera2==5);disparoenemigo(1,56,x,y,0,20);espera2=0;end
        end

        if (graph==159);
        if (espera2==5);energyeater(x,y,20,164);espera2=0;end
        end

       end

        if (collision(type disparo)
        OR (collision(type disparomisil)))
        enemigo2life=enemigo2life-1;end

         if (collision(type specialattack));enemigo2life=enemigo2life-20;end

         if (enemigo2life=<0);sound(sonido_explosion,30,100);
            from graph=8 to 13;frame;end;
            puntos=puntos+50;
            break;
         end
   frame;
   until (y>480 OR y<0 OR x>640 OR x<0 OR vidas<0);
end


if (enemigotype==3);
x=0;
y=400;
graph=graph; //=14
angle=+90000;

   repeat
     if (x<620);x=x+4;end
     if (x=>620);y=y-3;end
     if (y=<120);x=x-10;
      if (rand(0,100)<15);espera3=espera3+1;
       if (espera3=>5);disparoenemigo(3,0,x,y+100,0,50);espera3=0;end
      end
     end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo3life=enemigo3life-1;end

         if (collision(type specialattack));enemigo3life=enemigo3life-20;end

         if (enemigo3life=<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end;
          puntos=puntos+50;
         break;
         end

    frame;
   until (x<0 or vidas<0);
end



if (enemigotype==33);
x=640;
y=120;
graph=graph;  //=14
angle=+90000;

   repeat
    if (x>25 AND y<121);x=x-4;
         if (rand(0,100)<15);espera3=espera3+1;
          if (espera3=>5);disparoenemigo(3,0,x,y+100,0,50);espera3=0;end
         end
    end

    if (x<=50 AND y<400);y=y+3;end
    if (y>=400);x=x+4;end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo3life=enemigo3life-1;end

         if (collision(type specialattack));enemigo3life=enemigo3life-20;end

         if (enemigo3life=<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end;
          puntos=puntos+50;
          break;
         end
   frame;
   until (x>640 or x<0 or y>480 or y<120 OR Vidas<0);
end


if (enemigotype==4);
graph=graph; //=82
flags=1;
x=0;
y=220;

  repeat
   if(rand(0,100)<10);disparoenemigo(5,63,x,y+20,20,0);frame;end
   if (count4==0);y=y+3;frame;end
   if (y=>480);count4=1;end
   if (count4==1);y=y-3;frame;end
   if (y=<225);count4=0;end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo4life=enemigo4life-1;end

         if (collision(type specialattack));enemigo4life=enemigo4life-20;end


   if (enemigo4life=<0);sound(sonido_explosion,30,100);
    from graph=8 to 13;frame;end;
    puntos=puntos+200;
    break;
   end
  until (enemigo4life=<0 OR vidas<0 OR boss1life=<100);

from graph=8 to 13;
frame;
end
sound(sonido_explosion,100,256);

end


if (enemigotype==44);
graph=graph; // =82
x=640;
y=480;

  repeat
   if(rand(0,100)<10);disparoenemigo(5,63,x,y+20,-20,0);frame;end
   if (count4==0);y=y+3;frame;end
   if (y=>480);count4=1;end
   if (count4==1);y=y-3;frame;end
   if (y=<225);count4=0;end

   if (collision(type disparo)
   OR (collision(type disparomisil)))
   enemigo4life=enemigo4life-1;end

   if (collision(type specialattack));enemigo4life=enemigo4life-20;end

   if (enemigo4life=<0);sound(sonido_explosion,30,100);
    from graph=8 to 13;frame;end;
    puntos=puntos+200;
    break;
   end

  until (enemigo4life=<0 OR vidas<0 OR boss1life=<100);

from graph=8 to 13;
frame;
end
sound(sonido_explosion,100,256);

end


if (enemigotype==5);
x=x;
y=y;
graph=graph; //=151
angle=+90000;

   repeat
    navelocate=get_id(type nave);
    if (navelocate==0);return;end
    naveangle=get_angle(navelocate);
    angle=near_angle(angle,naveangle,360000);
    advance(6);

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo3life=enemigo3life-1;end

         if (collision(type specialattack));enemigo3life=enemigo3life-20;end

         if (enemigo3life=<0 OR vidas<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end
          puntos=puntos+100;
         break;
         end
       frame;
   until (vidas<0 OR enemigo3life=<0);
end



if (enemigotype==6);
flags=flags;
graph=graph;  //=152
angle=-90000;

   repeat
    navelocate=get_id(type nave);
    if (navelocate==0);return;end

       naveangle=get_angle(navelocate);
       angle=near_angle(angle,naveangle,360000);
       advance(3);

       if (rand(0,100)<15);count=count+1;
        if (count=>5);heatseeker(x,y,15,155);count=0;end
       end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo2life=enemigo2life-1;end

         if (collision(type specialattack));enemigo2life=enemigo2life-20;end

         if (enemigo2life=<0 OR vidas<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end
          puntos=puntos+100;
         break;
         end

       frame;

   until (vidas<0 OR enemigo2life=<0);
end


if (enemigotype==7);
graph=graph;  //=153
flags=flags;
x=x;
y=y;
  repeat
       x+=incx;
       y+=incy;
        if (rand(0,100)<45);count=count+1;
         if (count=>3);minas(x,y);count=0;end
        end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo1life=enemigo1life-1;end

         if (collision(type specialattack));enemigo1life=enemigo1life-20;end

   frame;
   if (enemigo1life=<0);sound(sonido_explosion,30,100);
    from graph=8 to 13;frame;end;
    puntos=puntos+200;
    //if (rand(0,10)<5);l(x,y);end
    break;
   end

  until (enemigo1life=<0 OR vidas<0 OR x>640 or x<0 OR y>480 OR y<110);
end


if (enemigotype==8);
x=x;
y=y;
graph=graph; //=159

   repeat
    navelocate=get_id(type nave);
    if (navelocate==0);return;end

       naveangle=get_angle(navelocate);
       angle=near_angle(angle,naveangle,360000);
       advance(5);

       if (rand(0,100)<10);energyeater(x,y,25,164);end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo2life=enemigo2life-1;end

         if (collision(type specialattack));enemigo2life=enemigo2life-20;end

         if (enemigo2life=<0 OR vidas<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end
          //if (rand(0,10)<5);s(x,y);end
          puntos=puntos+100;
         break;
         end
       frame;
   until (vidas<0 OR enemigo2life=<0);
end


if (enemigotype==9);
x=x;
y=y;
graph=graph; //=157

   repeat
    navelocate=get_id(type nave);
    if (navelocate==0);return;end

       naveangle=get_angle(navelocate);
       angle=near_angle(angle,naveangle,90000);
       advance(6);

       if (rand(0,100)<25);bolafuego(0,x,y,25,55);end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo8life=enemigo8life-1;end

         if (collision(type specialattack));enemigo8life=enemigo8life-20;end

         if (enemigo8life=<0 OR vidas<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end
          if (rand(0,10)<5);bonus(x,y,rand(168,173));end
          puntos=puntos+100;
         break;
         end

       frame;

   until (vidas<0 OR enemigo8life=<0);
end


if (enemigotype==10);
x=-120;
y=150;
angle=-90000;
graph=graph; //=165

   repeat
         if (x<670 AND y==150)x=x+3;
          if (rand(0,100)<2);enemigo(11,x,y,0,0,0,166);end
         end
   frame;
   until (vidas<0 OR x=>670)
   x=750;y=420;
   repeat
         flags=1;
         x=x-3;
         if (rand(0,100)<25);bolafuego(0,x,y,25,55);end
   frame;
   until (vidas<0 OR x=<-120);
pass=1;
end



if (enemigotype==11);
x=x;
y=y;
graph=graph; //=166

   repeat
    navelocate=get_id(type nave);
    if (navelocate==0);return;end
    naveangle=get_angle(navelocate);
    angle=near_angle(angle,naveangle,360000);
    advance(3);

         if (rand(0,100)<8);bolafuego(1,x,y,20,167);end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo4life=enemigo4life-1;end

        if (collision(type specialattack));enemigo4life=enemigo4life-20;end

         if (enemigo4life=<0 OR vidas<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end
          //if (rand(0,10)<5);s(x,y);end
          puntos=puntos+100;
         break;
         end

       frame;

   until (vidas<0 OR enemigo4life=<0);
end


if (enemigotype==12);
graph=graph; //=154
flags=flags;
x=x;
y=y;
  repeat
       x+=incx;

         count=count+1;
         if (count=<10);y=y+3;end
         if (count>10);y=y-3;end
         if (count=>20);count=0;end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo1life=enemigo1life-1;end

         if (collision(type specialattack));enemigo1life=enemigo1life-20;end


   frame;
   if (enemigo1life=<0);sound(sonido_explosion,30,100);
    from graph=8 to 13;frame;end;
    puntos=puntos+300;
    bonus(x,y,rand(168,178));
    break;
   end
  until (enemigo1life=<0 OR vidas<0 OR x>640 or x<0 OR y>480 OR y<110);
end



if (enemigotype==13); // *************** BOSS LEVEL 1 ******************

fade(200,0,0,8);        // Pone la pantalla en blanco
WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
stop_scroll(0);
fastscroll();
fade_on();                  // Hace que se vea la pantalla (la enciende)
WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla

unload_pcm(sonido_rythm);

x=320;
y=110;
angle=-90000;
size=5;

    repeat
       graph=78;

        if (rand(0,100)<25);espera3=espera3+1;
         if (espera3==5);bolafuego(0,x,y+10,30,55);espera3=0;end
        end

       if (y<150);y=y+2;end
       if (y=>150 AND size<35);size=size+1;count=1;end
       if (size=>35 AND count==1);x=x+4;
        if (rand(0,100)<25);espera=espera+1;
         if (espera=>5);disparoenemigo(3,0,x-30,y+125,0,50);espera=0;end
        end
        if (rand(0,100)<25);espera=espera+1;
         if (espera=>5);disparoenemigo(3,0,x+30,y+125,0,50);espera=0;end
        end


       end
       if (x>630);count=2;end
       if (count==2);x=x-4;
        if (rand(0,100)<25);espera=espera+1;
         if (espera=>5);disparoenemigo(3,0,x+30,y+125,0,50);espera=0;end
        end
        if (rand(0,100)<25);espera=espera+1;
         if (espera=>5);disparoenemigo(3,0,x+30,y+125,0,50);espera=0;end
        end
       end
       if (x<10);count=1;end


         if (collision(type disparo)
         OR (collision(type disparomisil)))
         graph=79;
         boss1life=boss1life-1;end

         if (collision(type specialattack));boss1life=boss1life-2;end

       if (boss1life=<0);break;end
    frame;
    until (boss1life=<300);

    count=3;
    enemigo(16,0,140,10,0,0,0);

    repeat
     graph=78;
      if (count==3);angle=angle+10000;end
      if (angle=>360000);count=4;end
      if (count==4);x=x-25;size=size-1;end
      if (x=<0);size=0;count=5;end
      if (count==5);enemigo(4,x,y,0,0,0,82);enemigo(44,x,y,0,0,0,82);count=6;frame(5000);end

      if (count==6);x=320;y=150;angle=-90000;size=size+1;end
      if (count==6 AND size=>35);frame(3000);count=7;end
      if (count==7 AND size=>35);x=x-5;
        if (rand(0,100)<25);espera=espera+1;
         if (espera=>5);disparoenemigo(4,80,x,y+30,0,50);espera=0;end
        end
      end
      if (count==7 AND x=<0);count=8;end
      if (count==8)x=x+5;
        if (rand(0,100)<25);espera=espera+1;
         if (espera=>5);disparoenemigo(4,80,x,y+30,0,50);espera=0;end
        end
      end
      if (count==8 AND x=>640);count=7;end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         graph=79;
         boss1life=boss1life-1;end

        if (rand(0,100)<25);espera3=espera3+1;
         if (espera3==5);heatseeker(x,y,15,155);espera3=0;end
        end

         if (collision(type specialattack));boss1life=boss1life-2;end

     frame;

    until (boss1life=<200)
    repeat
     graph=78;
     navelocate=get_id(type nave);
     if (navelocate==0);return;end
     naveangle=get_angle(navelocate);
     angle=near_angle(angle,naveangle,360000);
     advance(8);
         if (collision(type disparo)
         OR (collision(type disparomisil)))
         graph=79;
         boss1life=boss1life-1;end
         if (rand(0,100)<10);heatseeker(x,y,20,155);end
         if (rand(0,100)<50);bolafuego(0,x,y,40,6);end
     frame;
     until (boss1life=<0)
   if (vidas>=0);
    from graph=105 to 120;size=180;frame;end;sound(sonido_explosion,100,300);count2=1;
    if (count2==1);from graph=105 to 120;frame;end;sound(sonido_explosion,100,200);count2=2;end
    if (count2==2);from graph=105 to 138;frame;end;sound(sonido_explosion,100,100);end
   end
win=1;
end

if (enemigotype==14);
graph=160;
x=x;  //320;
y=y;  //120;
   repeat
    if (y<140);y=y+1;end
     if (rand(0,50)<10);heatseeker(x,y,15,155);count2=count2+1;end

     if (count2=>45);y=y-2;end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo8life=enemigo8life-1;end

         if (collision(type specialattack));enemigo8life=enemigo8life-20;end

         if (enemigo8life=<0 OR vidas<0);sound(sonido_explosion,30,100);
          from graph=8 to 13;frame;end
          if (rand(0,10)<5);bonus(x,y,rand(168,173));end
          puntos=puntos+100;
         break;
         end

   frame;
   until (vidas<0 OR enemigo8life=<0 OR y<119);
end

if (enemigotype==15);
graph=graph;
x=x;
y=y;
repeat
x+=incx;
y+=incy;

        if (graph==162);
         if (rand(0,50)<10);disparoenemigo(1,56,x,y,0,20);end
        end

        if (collision(type disparo)
        OR (collision(type disparomisil)))
        enemigo2life=enemigo2life-1;end

         if (collision(type specialattack));enemigo2life=enemigo2life-20;end

         if (enemigo2life=<0);sound(sonido_explosion,30,100);
            from graph=8 to 13;frame;end;
            puntos=puntos+50;
            break;
         end
frame;
until(vidas<0 or x>640 or x<0 or y>480 or y<119 OR enemigo2life=<0)
end

if (enemigotype==16);
graph=179;
flags=flags;
x=x;
y=y;
  repeat
       x+=incx;
         if (rand(0,50)<25);navehumo(x,y,-10,0);end
         count=count+1;
         if (count=<10);y=y+3;end
         if (count>10);y=y-3;end
         if (count=>20);count=0;end

         if (collision(type disparo)
         OR (collision(type disparomisil)))
         enemigo1life=enemigo1life-1;end

         if (collision(type specialattack));enemigo1life=enemigo1life-20;end


   frame;
   if (enemigo1life=<0);sound(sonido_explosion,30,100);
    from graph=8 to 13;frame;end;
    puntos=puntos+300;
    bonus(x+50,y,rand(168,178));
    bonus(x-50,y,rand(168,178));
    bonus(x,y+50,rand(168,178));
    bonus(x,y-50,rand(168,178));
    bonus(x,y,rand(168,173));
    break;
   end
  until (enemigo1life=<0 OR vidas<0 OR x>640 OR y>480 OR y<110);
end

if (enemigotype==17);
graph=157;
angle=-90000;
x=x;
y=y;
repeat
x+=incx;
y+=incy;

    navelocate=get_id(type nave);
    if (navelocate==0);return;end
    naveangle=get_angle(navelocate);
    angle=near_angle(angle,naveangle,360000);


        heatseeker(x,y,30,139);

        if (collision(type disparo)
        OR (collision(type disparomisil)))
        enemigo2life=enemigo2life-1;end

         if (collision(type specialattack));enemigo2life=enemigo2life-20;end

         if (enemigo2life=<0);sound(sonido_explosion,30,100);
            from graph=8 to 13;frame;end;
            puntos=puntos+50;
            break;
         end
frame;
until(vidas<0 or x>640 or x<0 or y>480 or y<119 OR enemigo2life=<0)
end


if (enemigotype==18);
graph=158;
angle=+90000;
x=x;
y=y;
heatseeker(x,y,5,155);
repeat
x+=incx;
y+=incy;


        if (collision(type disparo)
        OR (collision(type disparomisil)))
        enemigo2life=enemigo2life-1;end

         if (collision(type specialattack));enemigo2life=enemigo2life-20;end

         if (enemigo2life=<0);sound(sonido_explosion,30,100);
            from graph=8 to 13;frame;end;
            puntos=puntos+50;
            break;
         end
frame;
until(vidas<0 or x>640 or x<0 or y>480 or y<119 OR enemigo2life=<0)
end



if (enemigotype==19);
graph=163;
flags=2;
x=x;
y=y;
repeat
        if (y=>400);y=y-4;end
        if (y<400);y=y-30:end


        if (collision(type disparo)
        OR (collision(type disparomisil)))
        enemigo2life=enemigo2life-1;end

         if (collision(type specialattack));enemigo2life=enemigo2life-20;end

         if (enemigo2life=<0);sound(sonido_explosion,30,100);
            from graph=8 to 13;frame;end;
            puntos=puntos+50;
            break;
         end
frame;
until(vidas<0 or x>640 or x<0 or y>480 or y<119 OR enemigo2life=<0)
end
end



//**********************************************************************
// Process Minas *******************************************************
//**********************************************************************

process minas(x,y);

Private
count=0;

Begin
graph=156;

  repeat
   count=count+1;
   y=y+5;
   frame;
  until (count=>20)
 count=0;
 sound(sonido_explosion,50,256);
 from graph=8 to 13;frame;end
end


//************************************************************
// Process Bolas de Fuego ************************************
//************************************************************

process bolafuego(tipo,x,y,speed,graph)

private
navelocate;
naveangle;

begin
tipo=tipo;
x=x;
y=y;
speed=speed;
    navelocate=get_id(type nave);
    if (navelocate==0);return;end;
    angle=-90000;
    sound(sonido_disparo,30,110);
    graph=graph;
    naveangle=get_angle(navelocate);
    angle=near_angle(angle,naveangle,360000);
     if (tipo==0);
      repeat
       advance(speed);
       frame;
      until (collision (type nave) or vidas<0 OR x>640 OR x<0 OR y>480 or y<100);
     end
     if (tipo==1);
      repeat
       advance(speed);
       frame;
      until (collision (type nave) or collision(type escudo) OR collision(type disparo) or collision(type disparomisil) OR vidas<0 OR x>640 OR x<0 OR y>480 or y<100);
      sound(sonido_explosion,50,256);from graph=8 to 13;frame;end
     end
end

//************************************************************
// Process Energy Eater ************************************
//************************************************************

process energyeater(x,y,speed,graph)

private
navelocate;
naveangle;

begin
    navelocate=get_id(type nave);
    if (navelocate==0);return;end;
    angle=-90000;
    sound(sonido_disparo,30,110);
    graph=graph; //164;
    naveangle=get_angle(navelocate);
    angle=near_angle(angle,naveangle,360000);
      repeat
       advance(speed);
       frame;



      if (collision(type nave));sound(sonido_looseenergy,70,512);end
     until (collision(type nave) or collision(type escudo) or vidas<0 OR x>640 OR x<0 OR y>480 or y<100);
end

//************************************************************
// Process Misiles Enemigo Heat Seeker ***********************
//************************************************************

process heatseeker(x,y,speed,graph)

private
navelocate;
naveangle;
count=0;

begin
graph=graph;
     if (graph==139);
     repeat
      navelocate=get_id(type disparo);
      if (navelocate==0);
       navelocate=get_id(type disparomisil);
       if (navelocate==0);return;end;
      end
      sound(sonido_disparo,5,512);
      naveangle=get_angle(navelocate);
      angle=near_angle(angle,naveangle,360000);
      advance(speed);
      frame;
     until (collision(type nave) OR collision(type escudo) or collision(type disparo) OR collision(type disparomisil) OR collision(type specialattack) OR vidas<0);
      sound(sonido_explosion,40,256);
      from graph=8 to 13;frame;end
     end

    if (graph==155);
    sound(sonido_disparo2,50,256);
    repeat
     navelocate=get_id(type nave);
     naveangle=get_angle(navelocate);
     angle=near_angle(angle,naveangle,360000);
     advance(speed);
     frame;
     count=count+1;
    until (count=>50 OR collision(type nave) OR collision(type escudo) or collision(type disparo) OR collision(type disparomisil) OR collision(type specialattack) OR vidas<0);

 sound(sonido_explosion,40,256);
 from graph=8 to 13;frame;end
end
end

//********************************************************************
//Process Disparo Enemigo (Disparos solo hacia abajo)*****************
//********************************************************************

Process disparoenemigo(tipo,graph,x,y,incx,incy);

Begin

if (tipo==1);
angle=angle+90000;
sound(sonido_disparo2,50,256);
graph=graph;
//graph=56;
frame(500);
  repeat
   y+=incy;  // =20
   frame;
   if (collision(type nave) OR collision(type escudo));break;end
  until (x>640 OR y>480 OR x<0 OR y<0 OR vidas<0)
end

if (tipo==2);
sound(sonido_disparo2,20,256);
graph=graph;
//graph=56;
x=x;
y=y;
  repeat
   x+=incx;
   y+=incy;
   frame;
   if (collision(type nave) OR collision(type escudo));break;end
  until (x>640 OR y>480 OR x<0 OR y<0 OR vidas<0)
end

if (tipo==3);
angle=angle+90000;
sound(sonido_faser,60,512);
  repeat
    y+=incy; // = 50
    from graph=61 to 62;
    frame;
    end
  until (y>560 OR vidas<0);
end


if (tipo==4);
  x=x;
  y=y;
  graph=graph; //=80
  size=35;
  sonido_boss1laser=load_pcm("user\shag\bosslase.pcm",0);
  angle=angle+90000;
  sound(sonido_faser,60,256);
  repeat
    y+=incy; // = 50
    frame;
  until (y>480 OR vidas<0);
end


if (tipo==5);
x=x;
y=y;
graph=graph; // = 63
sound(sonido_disparo,30,110);
 repeat
  x+=incx; // = 20
  frame;
 until (vidas<0 OR boss1life=<0 or x=>640 OR x<0);
end
end

//**************************************************************************
// Process Special Attack **************************************************
//**************************************************************************

process specialattack(tipo,x,y,graph)

private
navelocate;
naveangle;

begin
if (tipo==1);
specials=0;
sound(sonido_attention,60,256);
graph=graph;
x=x;
y=y;
 repeat
  navelocate=get_id(type enemigo);
  if (navelocate==0);
  navelocate=get_id(type heatseeker);
  if (navelocate==0);
  end
  end

  naveangle=get_angle(navelocate);
  angle=near_angle(angle,naveangle,360000);
  advance(8);

disparo(9,x,y,0,-20,0);
frame;
until (y=<90);
end

if (tipo==2);
x=x:
y=y;
specials=0;
 from graph=105 to 138;frame;end
end

if (tipo==3);
specials=0;
sound(sonido_explode2,200,256);
graph=31;
 repeat
  size=size+10;
  frame;
 until (size=>700);
end
end

//******************************************************************
// Process Animation ***********************************************
//******************************************************************

PROCESS animation();
PRIVATE
    contador0;
    intromapa;

BEGIN
    signal(father,s_sleep);
    set_fps(25,0);
    clear_screen();
    unload_fpg(bar256);
    load_fpg("user\shag\select.fpg");
    load_pal("user\shag\select.fpg");

    write(info,320,50,1,"Final Score:");
    write_int(info,320,60,1,&puntos);


    fade(200,200,200,8);        // Pone la pantalla en blanco
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
    put(0,18,320,360);
    put(0,19,320,120);
    fade_on();                  // Hace que se vea la pantalla (la enciende)
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla

    naveestrellar();

    WHILE (NOT key(_esc) AND NOT key(_space) AND NOT key(_enter)AND NOT mouse.left)
    FRAME;
    end
    delete_text(all_text);
    clear_screen();
    textonivel1(320,200,45);frame(3500);
   signal(father,s_wakeup);
end


//******************************************************************
// Process nave que se va a estrellar en el desierto ***************
//******************************************************************

process naveestrellar();

private
count=0;
count2=0;
pre=0;

Begin
 if (selectplayer==143);graph=11;end
 if (selectplayer==142);graph=10;end
 if (selectplayer==141);graph=9;end
 if (selectplayer==140);graph=8;end

 graph=graph;
 x=0:
 y=100;
 angle=-135000;
  repeat
   if (rand(0,100)<20);navehumo(x,y,-6,-6);end
   if (rand(0,100)<20);navehumo(x+10,y,-6,-6);end
   if (rand(0,100)<20);navehumo(x-10,y,-6,-6);end
   if (key(_enter) or (key(_space) or mouse.left));break;end


   if (count==0);angle=angle+5000;count2=count2+1;
    if (count2=>5);count=1;
    count2=0;
    end
   end

   if (count==1);angle=angle-5000;count2=count2+1;
    if (count2=>5);count=0;
    count2=0;
    end
   end

   x=x+2;
   y=y+1;
   frame;
   if (y=>320);pre=1;end
  until (y=>320)
 angle=0;
 sound(sonido_explosion,100,100);
 sound(sonido_aaah,30,256);
 size=200;
 from graph=20 to 39;frame;end
if (pre==1);
 if (selectplayer==143);put(0,50,450,320);end
 if (selectplayer==142);put(0,49,450,320);end
 if (selectplayer==141);put(0,48,450,320);end
 if (selectplayer==140);put(0,47,450,320);end
 textonivel1(320,200,51);
 animator(5);
end
pre=0;
end


//*************************************************************************
//Process navehumo ********************************************************
//*************************************************************************

process navehumo(x,y,incx,incy);

private
count=0;

begin
graph=46;
flags=4;
x=x;
y=y;

repeat
 count=count+1;
 x+=incx;
 y+=incy;
 size=size-5;
 frame;
until (count==20);
end


//*********************************************************************
// Helicopter *********************************************************
//*********************************************************************

process helicopter(x,y);

Private
count=0;

Begin
frame(2000);
x=x:
y=y;
sound(sonido_bomba,20,256);
repeat
 from graph=149 to 150;
 y=y-10;
 frame;
 end
until (y<=90);

sound(sonido_explode,100,256);
specialattack(2,160,380,0);specialattack(2,320,380,0);specialattack(2,480,380,0);
frame(1000);
sound(sonido_explode,100,256);
specialattack(2,160,260,0);specialattack(2,320,260,0);specialattack(2,480,260,0);
frame(1000);
sound(sonido_explode,100,256);
specialattack(2,160,140,0);specialattack(2,320,140,0);specialattack(2,480,140,0);
end

//******************************************************************
// Process Victoria ***********************************************
//******************************************************************

PROCESS victoria();
PRIVATE
    contador0;
    intromapa;
BEGIN
    signal(father,s_sleep);
    clear_screen();
    set_fps(25,0);
    set_mode(m640x480);

    fade(200,200,200,8);        // Pone la pantalla en blanco
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla
    fade_on();                  // Hace que se vea la pantalla (la enciende)
    WHILE (fading) FRAME; END   // Espera hasta que se encienda la pantalla

    victorynaves(140,150,800);
    victorynaves(141,250,1000);
    victorynaves(142,350,900);
    victorynaves(143,450,1100);
    textonivel1(320,240,2);

    start_scroll(0,0,1,1,0,10);   // Inicializa el movimiento de pantalla

    loop
     scroll.x0=scroll.x0-10;
     scroll.y0=scroll.y0-10;
     scroll.x1=scroll.x1-5;
     scroll.y1=scroll.y1-5;
     if (key(_esc) or key(_space) or key(_enter));break;end
    frame;
    end
   signal(father,s_wakeup);
end

//******************************************************************
// Process VictoryNaves *********************************************
//******************************************************************

process victorynaves(graph,x,y)

private
count=0;
count2=0;
count3=0;

Begin
graph=graph;
x=x;
y=y;
 repeat
  y=y-5;
 frame;
 until (y<=320);
 repeat
  y=y-25;
 frame;
 until (y<0);
end

//************************************************************************
// process fastscroll ****************************************************
//************************************************************************

process fastscroll();

Begin
start_scroll(0,bar256,0,1,1,10);

 loop
  scroll.x0=scroll.x0-10;
  scroll.y0=scroll.y0-10;
 frame;
 end
end
