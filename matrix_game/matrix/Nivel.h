#ifndef NIVEL
#define NIVEL

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

class Nivel {

  public:

    SDL_Rect  rect;


   int matriz[DIMY][DIMX] = {
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {3,3,3,3,3,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   };

   Nivel () {
       rect.w = rect.h = PASSO - 1;
   };

   ~Nivel () {;};


   void render (int ifx) {

      int 
           x
          ,y
          ;

      if ((ifx < 0) || (ifx + LIMX >= DIMX)) {
         puts ("algo errado em sua logica");
         exit (0);      
      }

      for (x = 0+ifx; x < LIMX+ifx; x++) {
         for (y = 0; y < DIMY; y++) {
            switch (matriz[y][x]) {
             case 0: SDL_SetRenderDrawColor (gRenderer, 60, 60, 60, 255); break;
             case 1: SDL_SetRenderDrawColor (gRenderer, 250, 60, 60, 255); break;
             case 2: SDL_SetRenderDrawColor (gRenderer, 50, 250, 60, 255); break;
             case 3: SDL_SetRenderDrawColor (gRenderer, 50,  50, 0, 255); break;
            }
            rect.x = (x-ifx)*PASSO;
            rect.y = y*PASSO;
            SDL_RenderFillRect (gRenderer, &rect);
         }
      }
   };
};

#endif