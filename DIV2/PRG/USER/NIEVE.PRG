//-----------------------------------------------------------------------
//
// Programa: Efecto de nieve
// Autor: Antonio Soler Asenci - morgalad@gestinet.es
// fecha: 21/5/98
//
//-----------------------------------------------------------------------


program nieve;

const
    max_x=175;
    max_y=75;
    min_x=1;
    min_y=25;
    numcopos=5;

global
    cont;
    cx;
    cy;
    rumbo;
begin

   load_fpg("USER\NIEVE\nieve.fpg");
   map_block_copy (0,1,8,10,2,0,0,180,100);
   set_mode(m320x200);
    loop
      from cont=1 to numcopos;
        map_put_pixel (0,1,rand (min_x,max_x),min_y,234);
      end;
      put_screen(0,1);
      frame;
      from cy=max_y to min_y step -1;
        from cx=min_x to max_x;
             if (((map_get_pixel(0,1,cx,cy)==234) and  (cy<>max_y)) and (cx<>min_x))
                   rumbo=0;
                   if (map_get_pixel (0,1,cx-1,cy+1)==0) rumbo+=1; end;
                   if (map_get_pixel (0,1,cx,cy+1)==0) rumbo+=2; end;
                   if (map_get_pixel (0,1,cx+1,cy+1)==0) rumbo+=4; end;
                   if (rumbo>0)
                     map_put_pixel (0,1,cx,cy,0);
                     switch (rumbo)
                        case 1: map_put_pixel (0,1,cx- 1          ,cy+1,234); end
                        case 2: map_put_pixel (0,1,cx+ 0          ,cy+1,234); end
                        case 3: map_put_pixel (0,1,cx+(rand(-1,0)),cy+1,234); end
                        case 4: map_put_pixel (0,1,cx+ 1          ,cy+1,234); end
                        case 5: map_put_pixel (0,1,cx+pow(-1,rand(1,2)),cy+1,234); end
                        case 6: map_put_pixel (0,1,cx+(rand (0,1)),cy+1,234); end
                        case 7: map_put_pixel (0,1,cx+(rand(-1,1)),cy+1,234); end
                    end
                   else
                       map_put_pixel (0,1,cx,cy,rand(141,146));
                   end;
             end;
        end;
      end;

    end;
end

//-----------------------------------------------------------------------

