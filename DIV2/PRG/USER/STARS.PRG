//-----------------------------------------------------------------------
//
// Programa: Efecto de estrellas Scrolando
// Autor: Antonio Soler Asenci - morgalad@gestinet.es
// fecha: 21/5/98
//
//-----------------------------------------------------------------------


program stars;

const
    max_x=320;
    max_y=200;

global
    azar;
    cont;
    fichero1;
    semilla1;
    semilla2;

begin
    azar=rand(1,555);
    control_sal();
    fichero1=load_fpg("user\stars\stars.fpg");
    semilla2= rand(1,99999999);
    semilla1= rand(1,99999999);

    rand_seed(semilla1);
    genera(1);


    rand_seed(semilla2);
    genera(2);


    fondo();


end

//-----------------------------------------------------------------------

process control_sal();
begin
    repeat
    frame;
    until (scan_code==_esc)
    exit("   Morgalad Software Creations 1998 - morgalad@gestinet.es ",0);
end

//-----------------------------------------------------------------------

process fondo()
begin
    start_scroll (0,0,1,2,0,5);
    scroll.camera=id;
    ctype=C_scroll;
    graph=0;
    scroll.ratio=200;
    loop

        azar+=1;
        x+=4;

        if (x mod (max_x * 3) == 0) refresca2(azar); end
        if (x mod (max_x * 2) == 0) refresca1(azar); end
        frame;
    end
end


//-----------------------------------------------------------------------

process genera(grafico);


begin

    for (cont=1; cont<max_y; cont=cont+1)
        map_put_pixel(0,grafico,rand(1,max_x),rand(1,max_y),234);
     end

end

//-----------------------------------------------------------------------


process refresca1(extra);

begin
    if ( azar > 21000000) azar=1;end
    rand_seed(semilla1);
    for (cont=1; cont<max_y; cont=cont+1)
        map_put_pixel(0,1,rand(1,max_x),rand(1,max_y),0);
     end
    semilla1=semilla1+extra;

    rand_seed(semilla1);
    for (cont=1; cont<max_y; cont=cont+1)
        map_put_pixel(0,1,rand(1,max_x),rand(1,max_y),234);
     end

end




//-----------------------------------------------------------------------

process refresca2(extra);

begin
    rand_seed(semilla2);
    for (cont=1; cont<max_y; cont=cont+1)
        map_put_pixel(0,2,rand(1,max_x),rand(1,max_y),0);
     end
    semilla2=semilla2+extra;

    rand_seed(semilla2);
    for (cont=1; cont<max_y; cont=cont+1)
        map_put_pixel(0,2,rand(1,max_x),rand(1,max_y),234);
     end

end



