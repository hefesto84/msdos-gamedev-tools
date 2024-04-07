PROGRAM pinta_cuadrado;

IMPORT "polys.dll";
PRIVATE
  contador1;

BEGIN
    set_mode(m640x480);

    LOOP
        FROM contador1=0 TO 9;
            poncuadrado(rand(0,320),rand(0,240),rand(0,320),rand(0,240),rand(0,255),contador1);
            poncirculo(rand(160,480),rand(120,360),rand(0,240),rand(0,255),contador1);
        END
        FROM contador1=0 TO 29;
            ponlinea(rand(0,640),rand(0,480),rand(0,640),rand(0,480),rand(0,255),contador1);
        END

        FRAME;
    END
END