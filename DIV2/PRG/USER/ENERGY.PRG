COMPILER_OPTIONS _EXTENDED_CONDITIONS;
PROGRAM ENERGY;

// POR   : PABLO ARAGON GARCIA
// EMAIL : poliag@mx2.redestb.es
// Si te interesa hacer algo interesante con DIV
// y te consideras buen grafista ponte en contacto
// conmigo.

// Esto es solo un peque¤o ejemplo de lo que se puede
// hacer en DIV en un par de horas (4) . No he intentado
// hacer nada ni muy depurado , ni demasiado bonito .
// Solo algo simple para contribuir con un peque¤o
// ejemplo para aquellos que le guste el DIV y no sepan muy
// bien por donde van los tiros.


GLOBAL
    N_B=0;VIDAS[2]=3,3,3;
BEGIN
    SET_MODE(M800X600);
    LOAD_FPG('USER\ENERGY\ENERGY.FPG');
    PORTADA();
END

PROCESS PORTADA();
BEGIN
    GRAPH=015;
    REPEAT
        FRAME;
    UNTIL(KEY(_SPACE));
    FADE_OFF();
    PAREDES();
    JUGADOR(1,100,300,0     ,_RIGHT,_LEFT,_UP,_CONTROL);
    JUGADOR(2,700,300,0     ,_P    ,_O   ,_Q ,_SPACE  );
    FADE_ON();
    WRITE(0,10,10,0,'VIDAS    : ');
    WRITE(0,10,20,0,'PUNTOS   : ');
    WRITE(0,10,30,0,'MUNICION : ');
    WRITE(0,660,10,0,'VIDAS    : ');
    WRITE(0,660,20,0,'PUNTOS   : ');
    WRITE(0,660,30,0,'MUNICION : ');
END;

PROCESS JUGADOR(IDJ,IX,IY,ANGLE,T1,T2,T3,T4);
PRIVATE
    IDB;PUNTOS=0;IDT1,IDT2,IDT3;
    ENER=100;VELOCIDAD=10;MUNICION=20;E=0;V=0;
BEGIN
    GRAPH=001;
    X=IX;
    Y=IY;
    IDT1=WRITE_INT(0,(650*(IDJ-1))+80 ,10,0,OFFSET VIDAS[IDJ]);
    IDT2=WRITE_INT(0,(650*(IDJ-1))+80 ,20,0,OFFSET PUNTOS);
    IDT3=WRITE_INT(0,(650*(IDJ-1))+80 ,30,0,OFFSET MUNICION);
    ESCUDO();
    LOOP
        IF (KEY(_ESC))
            EXIT(0,'ADIOS');
        END
        IF (KEY(T1))
            ANGLE-=20000;
        END
        IF (KEY(T2))
            ANGLE+=20000;
        END
        IF (KEY(T3))
            ADVANCE(VELOCIDAD);
        END
        IF (KEY(T4) AND (MUNICION>0))
            LASER(ENER);
            MUNICION-=1;
        END
        IF (COLLISION(TYPE PAREDES))
            ADVANCE(-VELOCIDAD);
        END

        IF (IDB=COLLISION(TYPE B_ENERGY))
            ENER+=10;
            E+=1;
            N_B-=1;
            M_ENERGY(((20*(E-1))+15)+(650*(IDJ-1)));
            SIGNAL(IDB,S_KILL);
        END

        IF (IDB=COLLISION(TYPE B_VELOCIDAD))
            VELOCIDAD+=2;
            V+=1;
            N_B-=1;
            M_VELOCIDAD(((20*(V-1))+15)+(650*(IDJ-1)));
            SIGNAL(IDB,S_KILL);
        END
        IF (IDB=COLLISION(TYPE B_PUNTOS))
            PUNTOS+=10;
            N_B-=1;
            IF (PUNTOS==100)
                VIDAS+=1;
                PUNTOS=0;
            END;
            SIGNAL(IDB,S_KILL);
        END
        IF (IDB=COLLISION(TYPE B_MUNICION))
            MUNICION+=10;
            N_B-=1;
            SIGNAL(IDB,S_KILL);
        END

        IF (IDB=COLLISION(TYPE LASER))
            SIGNAL(IDB,S_KILL);
            VIDAS[IDJ]-=1;
            IF (VIDAS[IDJ]==0)
                IF (IDJ==1)
                    WRITE(0,300,250,0,'!!!!!! EL JUGADOR 2 HA GANADO !!!!!!');
                END;
                IF (IDJ==2)
                    WRITE(0,300,250,0,'!!!!!! EL JUGADOR 1 HA GANADO !!!!!!');
                END;
                REPEAT
                    FRAME;
                UNTIL (KEY(_SPACE));
                EXIT(0,'ESPERO QUE TE GUSTE');
            END
            BREAK;
        END;
        FRAME;
    END
    SIGNAL((TYPE M_VELOCIDAD),S_KILL);
    SIGNAL((TYPE M_ENERGY),S_KILL);
    DELETE_TEXT(IDT1);DELETE_TEXT(IDT2);DELETE_TEXT(IDT3);
    FROM GRAPH=6 TO 11;
        FRAME;
    END;
    FADE_OFF();
    FADE_ON();
    JUGADOR(IDJ,IX,IY,ANGLE,T1,T2,T3,T4);
END

PROCESS ESCUDO();
PRIVATE IDL;TOQUES=3;
BEGIN
    GRAPH=002;
    LOOP
        X=FATHER.X;
        Y=FATHER.Y;
        ANGLE=FATHER.ANGLE;
        IF (IDL=COLLISION(TYPE LASER))
            SIGNAL(IDL,S_KILL);
            TOQUES-=1;
            IF (TOQUES==0) BREAK; END;
        END;
        FRAME;
    END;
END

PROCESS LASER(SIZE);
BEGIN
    Z=-5;
    GRAPH=003;
    X=FATHER.X;
    Y=FATHER.Y;
    ANGLE=FATHER.ANGLE;
    ADVANCE(40);
    LOOP
        ADVANCE(10);
        IF ((X<0) OR (X>800) OR (Y<0) OR (Y>600)
        OR COLLISION(TYPE PAREDES))
            BREAK;
        END;
        FRAME;
    END
    FROM GRAPH=006 TO 011;
        FRAME;
    END
END

PROCESS PAREDES();
BEGIN
    GRAPH=004;
    Z=10;
    LOOP
        IF ((RAND(1,200)>=199) AND (N_B<4))
            B_PUNTOS();
            N_B+=1;
        END
        IF ((RAND(1,200)>=199) AND (N_B<4))
            B_VELOCIDAD();
            N_B+=1;
        END
        IF ((RAND(1,200)>=199) AND (N_B<4))
            B_ENERGY();
            N_B+=1;
        END
        IF ((RAND(1,200)>=199) AND (N_B<4))
            B_MUNICION();
            N_B+=1;
        END
        FRAME;
    END
END

PROCESS B_ENERGY();
PRIVATE IDL;
BEGIN
    Z=5;
    GRAPH=005;
    REPEAT
        X=RAND(1,800);
        Y=RAND(1,600);
    UNTIL(COLLISION(TYPE PAREDES)==0);
    LOOP
        ANGLE+=10000;
        IF (IDL=COLLISION(TYPE LASER))
            SIGNAL(IDL,S_KILL);
            BREAK;
        END;
        FRAME;
    END
    N_B-=1;
END

PROCESS B_VELOCIDAD();
PRIVATE IDL;
BEGIN
    Z=5;
    GRAPH=012;
    REPEAT
        X=RAND(1,800);
        Y=RAND(1,600);
    UNTIL(COLLISION(TYPE PAREDES)==0);
    LOOP
        ANGLE+=10000;
        IF (IDL=COLLISION(TYPE LASER))
            SIGNAL(IDL,S_KILL);
            BREAK;
        END;
        FRAME;
    END
    N_B-=1;
END

PROCESS B_PUNTOS();
PRIVATE IDL;
BEGIN
    Z=5;
    GRAPH=013;
    REPEAT
        X=RAND(1,800);
        Y=RAND(1,600);
    UNTIL(COLLISION(TYPE PAREDES)==0);
    LOOP
        ANGLE+=10000;
        IF (IDL=COLLISION(TYPE LASER))
            SIGNAL(IDL,S_KILL);
            BREAK;
        END;
        FRAME;
    END
    N_B-=1;
END

PROCESS B_MUNICION();
PRIVATE IDL;
BEGIN
    Z=5;
    GRAPH=014;
    REPEAT
        X=RAND(1,800);
        Y=RAND(1,600);
    UNTIL(COLLISION(TYPE PAREDES)==0);
    LOOP
        ANGLE+=10000;
        IF (IDL=COLLISION(TYPE LASER))
            SIGNAL(IDL,S_KILL);
            BREAK;
        END;
        FRAME;
    END
    N_B-=1;
END

PROCESS M_VELOCIDAD(X);
PRIVATE IDL;
BEGIN
    Z=-20;
    SIZE=50;
    Y=70;
    GRAPH=012;
    LOOP
        FRAME;
    END
END

PROCESS M_ENERGY(X);
PRIVATE IDL;
BEGIN
    Z=-20;
    SIZE=50;
    Y=50;
    GRAPH=005;
    LOOP
        FRAME;
    END
END

