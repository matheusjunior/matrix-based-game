<<<<<<< HEAD
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

using namespace std;

SDL_Window   *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

void gameOver(Text tGameOverMsg) {

	SDL_Color tGameOverColor;

	tGameOverColor.r = 255;
	tGameOverColor.g = 150;
	tGameOverColor.b = 0;

	if (tGameOverMsg.font == NULL) {
		tGameOverMsg.font = TTF_OpenFont("media/emulogic.ttf", 20);
		tGameOverMsg.displayText = "GAME OVER";
		tGameOverMsg.color = tGameOverColor;
		tGameOverMsg.rect.x = SIZEX;
		tGameOverMsg.rect.y = 200;
		tGameOverMsg.rect.w = 150;
		tGameOverMsg.rect.h = 25;
	}

	tGameOverMsg.surface = TTF_RenderText_Solid(tGameOverMsg.font, tGameOverMsg.displayText.c_str(), tGameOverColor);
	tGameOverMsg.texture = SDL_CreateTextureFromSurface(gRenderer, tGameOverMsg.surface);


	SDL_RenderClear(gRenderer);

	SDL_RenderCopy(gRenderer, tGameOverMsg.texture, NULL, &tGameOverMsg.rect);	
}


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

	int currentLevel = 0;
	bool wasInThePortal = false;

	SDL_Event event;
	const Uint8 *keyboard_state;

	Text tScore;
	Text tFrames1;
	Text tFrames2;
	Text tLifes;
	Text tGameOverMsg;

	int lastFrameTime = 0;
	int  currentFrameTime = 0;
	int fpsMili = 1000 / FPS;
	int currentSpeed = FPS;
	int totalFrames = 0;
	float deltaTime;

	if (!init()) exit(1);

	Nivel gNivel(gRenderer);
	Sprite gPlayer(gRenderer);
	Sprite dullEnemy(gRenderer);
	SDL_Color colorBlue;

#ifdef _WIN32
	if (!GetCurrentDir(currentPath, sizeof(currentPath)))
	{
		return errno;
	}
	currentPath[sizeof(currentPath) - 1] = '\0'; /* not really required */
#else
	if (getcwd((char *)&currentPath, sizeof(currentPath)) == NULL)
	{
		cout << "Error while getting current dir" << endl;
	}
#endif
	printf("The current working directory is %s\n", currentPath);

	tScore.font = TTF_OpenFont("media/emulogic.ttf", 20);
	tFrames1.font = TTF_OpenFont("media/emulogic.ttf", 20);
	tFrames2.font = TTF_OpenFont("media/emulogic.ttf", 20);
	tLifes.font = TTF_OpenFont("media/emulogic.ttf", 20);

	colorBlue.r = 255;
	colorBlue.g = 40;
	colorBlue.b = 255;

	tScore.color = colorBlue;
	tScore.rect.x = SIZEX - 60;
	tScore.rect.y = 20;
	tScore.rect.w = 80;
	tScore.rect.h = 25;
	tScore.displayText = "Test";

	cout << tScore.font;

	if (tScore.font == NULL || tFrames1.font == NULL)
	{
		std::cout << "Error: font " << TTF_GetError() << endl;
		return -1;
	}


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

		while (SDL_PollEvent(&event)) {
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

		currentFrameTime = SDL_GetTicks();
		deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;
		lastFrameTime = SDL_GetTicks();

		totalFrames++;
		SDL_RenderClear(gRenderer);

		dullEnemy.moveRandom();

		while (!dullEnemy.isValidMove(gNivel.matriz)) {
			dullEnemy.x = dullEnemy.previousX;
			dullEnemy.y = dullEnemy.previousY;
			dullEnemy.moveRandom();
		}
		dullEnemy.previousX = dullEnemy.x;
		dullEnemy.previousY = dullEnemy.y;


		if (gPlayer.y == LIMY) {
			++ify;
			gPlayer.y = LIMY - 1;
		}
		if (gPlayer.y < 0) {
			ify = 0;
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

		if (gPlayer.x + ifx >= DIMX - 1) {
			gPlayer.x = gPlayer.previousX;
			ifx--;
		}

		switch (gNivel.matriz[gPlayer.y + ify][gPlayer.x + ifx]) {
		case PORTAL: gPlayer.x = 0; gPlayer.y = 0; wasInThePortal = true; break;
		case PAREDE: gPlayer.x = gPlayer.previousX; gPlayer.y = gPlayer.previousY; break;
		case RATOEIRA:
			gPlayer.lifes--;
			gNivel.matriz[gPlayer.y + ifx][gPlayer.x + ifx] = VAZIO;
			if (gPlayer.lifes == 0) {
				gameOver(tGameOverMsg);
			}
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

		std::stringstream cont;
		cont << contadorGeral;

		tFrames1.displayText = "Frames: ";
		tFrames2.displayText = cont.str();

		std::stringstream lifesSTR;
		lifesSTR << gPlayer.lifes;

		tLifes.displayText = "Lifes: " + lifesSTR.str();
		//		std::cout << "tFrames "<< contadorGeral << endl;
		// inimigos.Exec_Maquina_Estados ();


		//------------------------------
		// Renderizacao

		if (colorG == 255) {
			colorGNeg = true;
		}

		if (colorGNeg) {
			colorG--;
			colorBlue.r++;
			if (colorG == 0) {
				colorGNeg = false;
			}
		}
		else {
			colorG++;
			colorBlue.r--;
		}

		if (tScore.font == NULL) {
			tScore.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tScore.displayText = "hello";
			tScore.color = colorBlue;
			tScore.rect.x = SIZEX;
			tScore.rect.y = 20;
			tScore.rect.w = 80;
			tScore.rect.h = 25;
		}

		if (tFrames1.font == NULL) {
			tFrames1.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tFrames1.displayText = "Frames: ";
			tFrames1.color = colorBlue;
			tFrames1.rect.x = SIZEX;
			tFrames1.rect.y = 50;
			tFrames1.rect.w = 150;
			tFrames1.rect.h = 25;
		}

		if (tFrames2.font == NULL) {
			tFrames2.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tFrames2.color = colorBlue;
			tFrames2.rect.x = SIZEX + 150;
			tFrames2.rect.y = 50;
			tFrames2.rect.w = 30;
			tFrames2.rect.h = 25;
		}

		if (tLifes.font == NULL) {
			tLifes.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tLifes.displayText = "Lifes: ";
			tLifes.color = colorBlue;
			tLifes.rect.x = SIZEX;
			tLifes.rect.y = 80;
			tLifes.rect.w = 150;
			tLifes.rect.h = 25;
		}

		tScore.surface = TTF_RenderText_Solid(tScore.font, tScore.displayText.c_str(), colorBlue);
		tScore.texture = SDL_CreateTextureFromSurface(gRenderer, tScore.surface);

		tFrames1.surface = TTF_RenderText_Solid(tFrames1.font, tFrames1.displayText.c_str(), colorBlue);
		tFrames1.texture = SDL_CreateTextureFromSurface(gRenderer, tFrames1.surface);

		tFrames2.surface = TTF_RenderText_Solid(tFrames2.font, tFrames2.displayText.c_str(), colorBlue);
		tFrames2.texture = SDL_CreateTextureFromSurface(gRenderer, tFrames2.surface);

		tLifes.surface = TTF_RenderText_Solid(tLifes.font, tLifes.displayText.c_str(), colorBlue);
		tLifes.texture = SDL_CreateTextureFromSurface(gRenderer, tLifes.surface);

		SDL_SetRenderDrawColor(gRenderer, colorG, colorG, colorG, 0xFF);
		SDL_RenderClear(gRenderer);

		SDL_RenderCopy(gRenderer, tScore.texture, NULL, &tScore.rect);
		SDL_RenderCopy(gRenderer, tFrames1.texture, NULL, &tFrames1.rect);
		SDL_RenderCopy(gRenderer, tFrames2.texture, NULL, &tFrames2.rect);
		SDL_RenderCopy(gRenderer, tLifes.texture, NULL, &tLifes.rect);
		
		gNivel.render(ifx, ify);
		gPlayer.render();
        gPlayer.shoot();
		dullEnemy.render();

		SDL_RenderPresent(gRenderer);

		fpsMili = 1000 / FPS;

		SDL_Delay(fpsMili - deltaTime);
		//------------------------------
	}
=======
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

using namespace std;

SDL_Window   *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

void gameOver(Text tGameOverMsg) {

	SDL_Color tGameOverColor;

	tGameOverColor.r = 255;
	tGameOverColor.g = 150;
	tGameOverColor.b = 0;

	if (tGameOverMsg.font == NULL) {
		tGameOverMsg.font = TTF_OpenFont("media/emulogic.ttf", 20);
		tGameOverMsg.displayText = "GAME OVER";
		tGameOverMsg.color = tGameOverColor;
		tGameOverMsg.rect.x = SIZEX;
		tGameOverMsg.rect.y = 200;
		tGameOverMsg.rect.w = 150;
		tGameOverMsg.rect.h = 25;
	}

	tGameOverMsg.surface = TTF_RenderText_Solid(tGameOverMsg.font, tGameOverMsg.displayText.c_str(), tGameOverColor);
	tGameOverMsg.texture = SDL_CreateTextureFromSurface(gRenderer, tGameOverMsg.surface);


	SDL_RenderClear(gRenderer);

	SDL_RenderCopy(gRenderer, tGameOverMsg.texture, NULL, &tGameOverMsg.rect);	
}


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

	int currentLevel = 0;
	bool wasInThePortal = false;

	SDL_Event event;
	const Uint8 *keyboard_state;

	Text tScore;
	Text tFrames1;
	Text tFrames2;
	Text tLifes;
	Text tGameOverMsg;

	int lastFrameTime = 0;
	int  currentFrameTime = 0;
	int fpsMili = 1000 / FPS;
	int currentSpeed = FPS;
	int totalFrames = 0;
	float deltaTime;

	if (!init()) exit(1);

	Nivel gNivel(gRenderer);
	Sprite gPlayer(gRenderer);
	Sprite dullEnemy(gRenderer);
	SDL_Color colorBlue;

#ifdef _WIN32
	if (!GetCurrentDir(currentPath, sizeof(currentPath)))
	{
		return errno;
	}
	currentPath[sizeof(currentPath) - 1] = '\0'; /* not really required */
#else
	if (getcwd((char *)&currentPath, sizeof(currentPath)) == NULL)
	{
		cout << "Error while getting current dir" << endl;
	}
#endif
	printf("The current working directory is %s\n", currentPath);

	tScore.font = TTF_OpenFont("media/emulogic.ttf", 20);
	tFrames1.font = TTF_OpenFont("media/emulogic.ttf", 20);
	tFrames2.font = TTF_OpenFont("media/emulogic.ttf", 20);
	tLifes.font = TTF_OpenFont("media/emulogic.ttf", 20);

	colorBlue.r = 255;
	colorBlue.g = 40;
	colorBlue.b = 255;

	tScore.color = colorBlue;
	tScore.rect.x = SIZEX - 60;
	tScore.rect.y = 20;
	tScore.rect.w = 80;
	tScore.rect.h = 25;
	tScore.displayText = "Test";

	cout << tScore.font;

	if (tScore.font == NULL || tFrames1.font == NULL)
	{
		std::cout << "Error: font " << TTF_GetError() << endl;
		return -1;
	}


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

		while (SDL_PollEvent(&event)) {
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

		currentFrameTime = SDL_GetTicks();
		deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;
		lastFrameTime = SDL_GetTicks();

		totalFrames++;
		SDL_RenderClear(gRenderer);

		dullEnemy.moveRandom();

		while (!dullEnemy.isValidMove(gNivel.matriz)) {
			dullEnemy.x = dullEnemy.previousX;
			dullEnemy.y = dullEnemy.previousY;
			dullEnemy.moveRandom();
		}
		dullEnemy.previousX = dullEnemy.x;
		dullEnemy.previousY = dullEnemy.y;


		if (gPlayer.y == LIMY) {
			++ify;
			gPlayer.y = LIMY - 1;
		}
		if (gPlayer.y < 0) {
			ify = 0;
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

		if (gPlayer.x + ifx >= DIMX - 1) {
			gPlayer.x = gPlayer.previousX;
			ifx--;
		}

		switch (gNivel.matriz[gPlayer.y + ify][gPlayer.x + ifx]) {
		case PORTAL: gPlayer.x = 0; gPlayer.y = 0; wasInThePortal = true; break;
		case PAREDE: gPlayer.x = gPlayer.previousX; gPlayer.y = gPlayer.previousY; break;
		case RATOEIRA:
			gPlayer.lifes--;
			gNivel.matriz[gPlayer.y + ifx][gPlayer.x + ifx] = VAZIO;
			if (gPlayer.lifes == 0) {
				gameOver(tGameOverMsg);
			}
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

		std::stringstream cont;
		cont << contadorGeral;

		tFrames1.displayText = "Frames: ";
		tFrames2.displayText = cont.str();

		std::stringstream lifesSTR;
		lifesSTR << gPlayer.lifes;

		tLifes.displayText = "Lifes: " + lifesSTR.str();
		//		std::cout << "tFrames "<< contadorGeral << endl;
		// inimigos.Exec_Maquina_Estados ();


		//------------------------------
		// Renderizacao

		if (colorG == 255) {
			colorGNeg = true;
		}

		if (colorGNeg) {
			colorG--;
			colorBlue.r++;
			if (colorG == 0) {
				colorGNeg = false;
			}
		}
		else {
			colorG++;
			colorBlue.r--;
		}

		if (tScore.font == NULL) {
			tScore.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tScore.displayText = "hello";
			tScore.color = colorBlue;
			tScore.rect.x = SIZEX;
			tScore.rect.y = 20;
			tScore.rect.w = 80;
			tScore.rect.h = 25;
		}

		if (tFrames1.font == NULL) {
			tFrames1.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tFrames1.displayText = "Frames: ";
			tFrames1.color = colorBlue;
			tFrames1.rect.x = SIZEX;
			tFrames1.rect.y = 50;
			tFrames1.rect.w = 150;
			tFrames1.rect.h = 25;
		}

		if (tFrames2.font == NULL) {
			tFrames2.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tFrames2.color = colorBlue;
			tFrames2.rect.x = SIZEX + 150;
			tFrames2.rect.y = 50;
			tFrames2.rect.w = 30;
			tFrames2.rect.h = 25;
		}

		if (tLifes.font == NULL) {
			tLifes.font = TTF_OpenFont("media/emulogic.ttf", 20);
			tLifes.displayText = "Lifes: ";
			tLifes.color = colorBlue;
			tLifes.rect.x = SIZEX;
			tLifes.rect.y = 80;
			tLifes.rect.w = 150;
			tLifes.rect.h = 25;
		}

		tScore.surface = TTF_RenderText_Solid(tScore.font, tScore.displayText.c_str(), colorBlue);
		tScore.texture = SDL_CreateTextureFromSurface(gRenderer, tScore.surface);

		tFrames1.surface = TTF_RenderText_Solid(tFrames1.font, tFrames1.displayText.c_str(), colorBlue);
		tFrames1.texture = SDL_CreateTextureFromSurface(gRenderer, tFrames1.surface);

		tFrames2.surface = TTF_RenderText_Solid(tFrames2.font, tFrames2.displayText.c_str(), colorBlue);
		tFrames2.texture = SDL_CreateTextureFromSurface(gRenderer, tFrames2.surface);

		tLifes.surface = TTF_RenderText_Solid(tLifes.font, tLifes.displayText.c_str(), colorBlue);
		tLifes.texture = SDL_CreateTextureFromSurface(gRenderer, tLifes.surface);

		SDL_SetRenderDrawColor(gRenderer, colorG, colorG, colorG, 0xFF);
		SDL_RenderClear(gRenderer);

		SDL_RenderCopy(gRenderer, tScore.texture, NULL, &tScore.rect);
		SDL_RenderCopy(gRenderer, tFrames1.texture, NULL, &tFrames1.rect);
		SDL_RenderCopy(gRenderer, tFrames2.texture, NULL, &tFrames2.rect);
		SDL_RenderCopy(gRenderer, tLifes.texture, NULL, &tLifes.rect);
		
		gNivel.render(ifx, ify);
		gPlayer.render();


		dullEnemy.render();

		SDL_RenderPresent(gRenderer);

		fpsMili = 1000 / FPS;

		SDL_Delay(fpsMili - deltaTime);
		//------------------------------
	}
>>>>>>> 14961fccf3f669cb5ebc5868b37660b82a5db73c
}