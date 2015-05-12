#ifdef _WIN32

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <direct.h>
#define GetCurrentDir _getcwd

#else

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_image/SDL_image.h>
#include <unistd.h>

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


SDL_Window   *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

using namespace std;

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

	gWindow = SDL_CreateWindow("Ded gaem!"
		, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SIZEX + 250, SIZEY, SDL_WINDOW_SHOWN);

	if (gWindow == NULL) return false;
	if (TTF_Init() == -1)
	{
		std::cout << TTF_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
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
	gWindow = NULL;

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

	char currentPath[FILENAME_MAX];
	int colorG = 0;
	bool colorGNeg = false;
	int contadorGeral = 0;

#ifdef _WIN32
	if (!GetCurrentDir(currentPath, sizeof(currentPath)))
	{
		return errno;
	}
	currentPath[sizeof(currentPath) - 1] = '\0'; /* not really required */
#else
	if (getcwd((char *) &currentPath, sizeof(currentPath)) == NULL)
	{
		cout << "Error while getting current dir" << endl;
	}
#endif
	printf("The current working directory is %s\n", currentPath);

	int a = 0;
	int i, j;
	int currentLevel = 0;
	bool wasInThePortal = false;

	SDL_Event event;
	const Uint8 *keyboard_state;

	if (!init()) exit(1);

	Nivel gNivel(gRenderer);
	Sprite gPlayer(gRenderer);
	Sprite dullEnemy(gRenderer);

	srand(time(NULL));
	if (!gPlayer.loadFromFile("media/ship.png", 1)) {
		cout << "image could not be loaded.";
		getchar();
		exit(1);
	}
	if (!dullEnemy.loadFromFile("media/dinosaur.png", 1)) {
		cout << "image could not be loaded.";
		getchar();
		exit(1);
	}

	dullEnemy._Height = 50;
	dullEnemy._Width = 50;

	gPlayer.setStep(1);
	dullEnemy.setStep(1);

	if (!gNivel.carregar("media/map1.txt")) {
		cout << "map1 could not be loaded.";
		getchar();
		exit(1);
	}

	gPlayer.x = 0;
	gPlayer.y = 0;
	dullEnemy.x = 10;
	dullEnemy.y = 11;
	dullEnemy.previousX = dullEnemy.x;
	dullEnemy.previousY = dullEnemy.y;

	int ifx, ify;

	ifx = 0;
	ify = 0;

	while (true) {
		if (currentLevel == 0 && wasInThePortal) {
			if (!gNivel.carregar("media/map2.txt")) {
				cout << "map2 could not be loaded.";
				getchar();
				exit(1);
			}
			currentLevel = 1;
		}
		gPlayer.previousX = gPlayer.x;
		gPlayer.previousY = gPlayer.y;

		while (SDL_PollEvent(&event)){
			if (event.type == SDL_KEYDOWN)		{
				switch (event.key.keysym.sym){
				case SDLK_RIGHT:
					gPlayer.x++; break;
				case SDLK_LEFT:
					gPlayer.x--; break;
				case SDLK_DOWN:
					gPlayer.y++; break;
				case SDLK_UP:
					gPlayer.y--; break;
				case SDLK_ESCAPE:   exit(0);
					//case SDLK_SPACE: gPlayer.jump(); da' mais trabalho que isso.
				default: break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION) {
				;//sem mouse nesse jogo (ou com?)
			}
		}

		dullEnemy.moveRandom();

		while (!dullEnemy.isValidMove(gNivel.matriz)) {
			dullEnemy.x = dullEnemy.previousX;
			dullEnemy.y = dullEnemy.previousY;
			dullEnemy.moveRandom();
		}
		dullEnemy.previousX = dullEnemy.x;
		dullEnemy.previousY = dullEnemy.y;

		
		if (gPlayer.y == LIMX) {
			++ify;
			gPlayer.y = LIMX - 1;
		}
		if (gPlayer.y < 0) {
			--ify;
			gPlayer.y = 0;
		}
		if (gPlayer.x == LIMX) {
			++ifx;
			gPlayer.x = LIMX - 1;
		}

		if (gPlayer.x < 0) {
			ifx = 0;
			gPlayer.x = 0;
		}

		// onde fomos parar?
		switch (gNivel.matriz[gPlayer.y + ify][gPlayer.x + ifx]) {
		case PORTAL: gPlayer.x = 0; gPlayer.y = 0; wasInThePortal = true; break;
		case PAREDE: gPlayer.x = gPlayer.previousX; gPlayer.y = gPlayer.previousY; break;
		case RATOEIRA:
			gPlayer.lifes--;
			gNivel.matriz[gPlayer.y + ifx][gPlayer.x + ifx] = VAZIO;
			if (gPlayer.lifes == 0) exit(0);
		case CENOURA:
			gPlayer.lifes++;
			gNivel.matriz[gPlayer.y + ifx][gPlayer.x + ifx] = VAZIO;
		case POISON:
			gPlayer.intoxication += 0.35;
			if (gPlayer.intoxication >= 1) {
				gPlayer.lifes--;
				gPlayer.intoxication = 0;
				if (gPlayer.lifes == 0) exit(0);
			}
			gNivel.matriz[gPlayer.y + ifx][gPlayer.x + ifx] = VAZIO;
		}

		contadorGeral++;
		//		std::cout << "frames "<< contadorGeral << endl;
		// inimigos.Exec_Maquina_Estados ();


		//------------------------------
		// Renderizacao

		if (colorG == 255) {
			colorGNeg = true;
		}

		if (colorGNeg) {
			colorG--;
			if (colorG == 0) {
				colorGNeg = false;
			}
		}
		else {
			colorG++;
		}


		SDL_SetRenderDrawColor(gRenderer, colorG, colorG, colorG, 0xFF);
		SDL_RenderClear(gRenderer);

		gNivel.render(ifx);
		gPlayer.render();
		dullEnemy.render();

		SDL_RenderPresent(gRenderer);
		//------------------------------
	}
}