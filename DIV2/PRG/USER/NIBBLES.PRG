PROGRAM Warcraft;

GLOBAL
    lectura[10000];
    mx;
    my;
LOCAL
    xx;
    yy;
    plano;

BEGIN

        LOAD("USER\NIBBLES\mapa.dat",OFFSET lectura);
        //mapa creado por matriz
        //valor de cada figura en fpg

    load_fpg("USER\NIBBLES\pantalla.fpg");
    load_fpg("USER\NIBBLES\figuras.fpg");
    put_screen(0,1);
    mouse.file=0;
    mouse.graph=8;
    mouse.flags=4;
    ponmapa(0,0);

    WHILE (NOT key (_esc))
        IF (key(_left) AND mx>0)
            mx--;
            ponmapa(mx,my);
        END
        IF (key(_right) AND mx<25)
            mx++;
            ponmapa(mx,my);
        END
        IF (key(_down) AND my<14)
            my++;
            ponmapa(mx,my);
        END
        IF (key(_up) AND my>0)
            my--;
            ponmapa(mx,my);
        END

        FRAME(50);
    END

END

//dibuja el scroll para un jugador
PROCESS ponmapa(x,y)

BEGIN
    FROM xx=1 to 15;
        FROM yy=0 TO 10;
            plano=(yy+y)*40+xx+x-1;
            PUT(1,lectura[plano],xx*16+64,yy*16+20);
        END
    END
END