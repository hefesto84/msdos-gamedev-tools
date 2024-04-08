#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int WIDTH = 20;
const int HEIGHT = 20;
const int RENDER_WIDTH = 20;
const int RENDER_HEIGHT = 20;

struct tile
{
    int id;
};

typedef struct tile Tile;


struct map
{
    Tile tiles[20][20];
};

typedef struct map Map;


void loadMap(Map *mapa);
void renderTile(int id, int x, int y, BITMAP *tiles);
void renderMap(Map mapa, BITMAP* tiles);
void updateInput();

BITMAP *buffer;
int offsetX = 0;
int offsetY = 0;
clock_t t_current;
clock_t t_last;

int main(void)
{
   BITMAP *tiles;
   PALETTE paleta;
   Map mapa;
   
   t_last = clock();

   loadMap(&mapa);

   allegro_init();
   install_keyboard();
   install_timer();
   set_gfx_mode(GFX_AUTODETECT,800,600,0,0);

   buffer = create_bitmap(800,600);

   tiles = load_bitmap("tiles.pcx",paleta);
   
   set_palette(paleta);

    while(!key[KEY_ESC])
    {
       
        updateInput();
   
        renderMap(mapa, tiles);
        blit(buffer, screen, 0, 0, 0, 0, 800, 600);
        clear_bitmap(buffer);
    }

   allegro_exit();

   return 0;
  }

void renderTile(int id, int x, int y, BITMAP *tiles)
{
    if(id>2) return;

    blit(tiles, buffer, id*32, 0, x, y, 32, 32);
}

void renderMap(Map mapa, BITMAP* tiles)
{
    int i,j = 0;

    for(i = 0; i<RENDER_HEIGHT; i++)
    {
        for(j = 0; j<RENDER_WIDTH; j++)
        {
            renderTile(mapa.tiles[i+offsetY][j+offsetX].id, i*32, j*32, tiles);
        }
    }
    
}

void loadMap(Map *mapa)
{
    FILE *ftrp;
    srand(time(NULL));

    int i,j = 0;
    int t = 0;
 
    ftrp = fopen("map.bin","wb");
    //ftrp = fopen("map.bin","rb");

    for(i = 0; i<WIDTH; i++)
    {
        for(j = 0; j<HEIGHT; j++)
        {
            //if(i>10) mapa->tiles[i][j].id = 0; continue;

            if(i%2 == 0){
                mapa->tiles[i][j].id = 1;
            }else{
                mapa->tiles[i][j].id = 2;
            }
            
            //mapa->tiles[i][j].id = rand()%4; //rand()%3;
            //fprintf(ftrp,"%d",mapa->tiles[i][j].id);
            fwrite(&mapa->tiles[i][j].id,sizeof(int),1,ftrp);
            //fread(&mapa->tiles[i][j].id, sizeof(int), 1, ftrp);
        }
        //fprintf(ftrp, "\n");
    }

    fclose(ftrp);
}

void updateInput()
{
     t_current = clock();
printf("CLOCK : %f\n", (t_current-t_last));


     //if(t_current - t_last < 1) return; 

     

t_last = t_current;

        if(key[KEY_RIGHT])
        {
            offsetX++;

            if(offsetX==WIDTH){
                offsetX = offsetX--;
            }
        }

        if(key[KEY_LEFT])
        {
            offsetX--;
            if(offsetX==-1){
                offsetX = 0;
            }
        }

        if(key[KEY_DOWN])
        {
            offsetY++;

            if(offsetY==HEIGHT){
                offsetY = HEIGHT;
            }
        }

        if(key[KEY_UP])
        {
            offsetY--;
            if(offsetY==-1){
                offsetY = 0;
            }
        }

}