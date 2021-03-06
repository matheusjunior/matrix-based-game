#ifdef _WIN32

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <direct.h>
#define GetCurrentDir _getcwd

#else

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_image/SDL_image.h>

#endif

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <time.h>

#include "Text.h"
#include "Collision.h"
#include "Util.h"
#include "Consts.h"

#include "Nivel.h"
#include "Physics.h"
#include "Sprite.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
Mix_Music* music = NULL;
Nivel *gNivel;
Sprite *gPlayer;

using namespace std;

/** Initialize SDL components
* \return true if: video component initialized properly, window succesfully created,
* and SDL_Tff is properly initialized
* false otherwise
* */

void rotatePoint(float vx, float vy, float cx, float cy, float theta, float *xn, float *yn) {
	*xn = cx + (vx - cx)*cos(theta) + (vy - cy)*sin(theta);
	*yn = cy + (vx - cx)*sin(theta) + (vy - cy)*cos(theta);
}


int distance(int x1, int y1, int x2, int y2) {
	x1 = x2 - x1; y1 = y2 - y1;
	return sqrt(x1*x1 + y1*y1);
}


bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;

	gWindow = SDL_CreateWindow("terreno"
		, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SIZEX, SIZEY, SDL_WINDOW_SHOWN);

	if (gWindow == NULL) return false;
	if (TTF_Init() == -1)
	{
		std::cout << TTF_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG) == 0)
	{
		std::cout << IMG_GetError() << endl;
		return false;
	}


	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	return true;
}

/** Close SDL resources in use
* */
void close()
{
	SDL_DestroyWindow(gWindow);
	Mix_FreeMusic(music);
	gWindow = NULL;

	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

/** Game main function. All event handling is first processed here
* and delegated to classes if needed.
* \return 0 if succeeds, -1 otherwise
* */
int main(int argc, char *args[])
 {
	srand(time(NULL));

	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}

	cCurrentPath[sizeof(cCurrentPath)-1] = '\0'; /* not really required */

	printf("The current working directory is %s", cCurrentPath);

	int a = 0;
	int i, j;

	SDL_Event event;

	const Uint8 *keyboard_state;

	if (!init())  goto saida;

	gNivel = new Nivel(gRenderer);
	gPlayer = new Sprite(gRenderer);


	srand(time(NULL));
	if (!gPlayer->loadFromFile("ship.png", 1)) 
		goto saida;
	gPlayer->setStep(1);


	gPlayer->x = LIMX / 2;
	gPlayer->y = DIMY / 2;

	int
		oldx
		, oldy
		, ifx
		, ify
		;


	ifx = 0;
	ify = 0;

	while (true) {

		oldx = gPlayer->x;
		oldy = gPlayer->y;


		while (SDL_PollEvent(&event)){

			if (event.type == SDL_KEYDOWN)		{
				switch (event.key.keysym.sym){
				case SDLK_RIGHT: gPlayer->x++; break;
				case SDLK_LEFT:  gPlayer->x--; break;
				case SDLK_DOWN:  gPlayer->y++; break;
				case SDLK_UP:    gPlayer->y--; break;
				case SDLK_ESCAPE:   exit(0);
					//case SDLK_SPACE: gPlayer->jump(); da' mais trabalho que isso.
				default: break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION) {
				;//sem mouse nesse jogo (ou com?)
			}
		}


		if (gPlayer->x == LIMX) {
			++ifx;
			gPlayer->x = LIMX - 1;
		}

		if (gPlayer->x < 0) {
			--ifx;
			gPlayer->x = 0;
		}

		// onde fomos parar?
		switch (gNivel->matriz[gPlayer->y][gPlayer->x + ifx]) {
		case PORTAL: gPlayer->x = 0; gPlayer->y = 0; break;
		case PAREDE: gPlayer->x = oldx; gPlayer->y = oldy; break;
		}


		// inimigos.Exec_Maquina_Estados ();


		//------------------------------
		// Renderizacao
		SDL_SetRenderDrawColor(gRenderer, 255, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		gNivel->render(ifx);
		gPlayer->render();

		SDL_RenderPresent(gRenderer);
		//------------------------------
	}
	saida:

		close();

		return 0;

}