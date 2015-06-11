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
#include "MovingObject.h"

using namespace std;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

void gameOver(Text tGameOverMsg)
{

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


void rotatePoint(float vx, float vy, float cx, float cy, float theta, float *xn, float *yn)
{
    *xn = cx + (vx - cx) * cos(theta) + (vy - cy) * sin(theta);
    *yn = cy + (vx - cx) * sin(theta) + (vy - cy) * cos(theta);
}


int distance(int x1, int y1, int x2, int y2)
{
    x1 = x2 - x1;
    y1 = y2 - y1;
    return sqrt(x1 * x1 + y1 * y1);
}


bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;

    gWindow = SDL_CreateWindow("Ded gaem!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SIZEX + 250, SIZEY, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) return false;
    if (TTF_Init() == -1) {
        std::cout << TTF_GetError() << endl;
        return false;
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
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

    Text menuScore;
    Text menuFPS;
    Text tFrames2;
    Text menuLifes;
    Text tGameOverMsg;

    int lastFrameTime = 0;
    int currentFrameTime = 0;
    int fpsMili = 1000 / FPS;
    int currentSpeed = FPS;
    int totalFrames = 0;
    float deltaTime;

    if (!init()) exit(1);

    Nivel gNivel(gRenderer);
    MovingObject player(gRenderer);
    MovingObject dullEnemy(gRenderer);
    SDL_Color colorBlue;

    menuScore.font = TTF_OpenFont("media/emulogic.ttf", 20);
    menuFPS.font = TTF_OpenFont("media/emulogic.ttf", 20);
    tFrames2.font = TTF_OpenFont("media/emulogic.ttf", 20);
    menuLifes.font = TTF_OpenFont("media/emulogic.ttf", 20);

    colorBlue.r = 255;
    colorBlue.g = 40;
    colorBlue.b = 255;

    menuScore.color = colorBlue;
    menuScore.rect.x = SIZEX - 60;
    menuScore.rect.y = 20;
    menuScore.rect.w = 80;
    menuScore.rect.h = 25;
    menuScore.displayText = "Test";

    cout << menuScore.font;

    if (menuScore.font == NULL || menuFPS.font == NULL) {
        std::cout << "Error: font " << TTF_GetError() << endl;
        return -1;
    }

    srand(time(NULL));
    if (!player.loadFromFile("media/ship.png", 1)) {
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

    player.setStep(1);
    dullEnemy.setStep(1);

    if (!gNivel.carregar("media/map1.txt")) {
        cout << "map1 could not be loaded.";
        getchar();
        exit(1);
    }
    // Copy world to objects
    player.copyWorld(gNivel.matriz);
    dullEnemy.copyWorld(gNivel.matriz);

    player.x = 0;
    player.y = 0;
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
        player.previousX = player.x;
        player.previousY = player.y;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        player.x++;
                        break;
                    case SDLK_LEFT:
                        player.x--;
                        break;
                    case SDLK_DOWN:
                        player.y++;
                        break;
                    case SDLK_UP:
                        player.y--;
                        break;
                    case SDLK_SPACE:
                        player.shooting = true;
                        player.firstShoot = true;
                        break;
                    case SDLK_ESCAPE:
                        exit(0);
                        //case SDLK_SPACE: player.jump(); da' mais trabalho que isso.
                    default:
                        break;
                }
            }
            else if (event.type == SDL_MOUSEMOTION) {;//sem mouse nesse jogo (ou com?)
            }
        }

        currentFrameTime = SDL_GetTicks();
        deltaTime = (float) (currentFrameTime - lastFrameTime) / 1000;
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


        if (player.y == LIMY) {
            ++ify;
            player.y = LIMY - 1;
        }
        if (player.y < 0) {
            ify = 0;
            player.y = 0;
        }
        if (player.x == LIMX) {
            ++ifx;
            player.x = LIMX - 1;
        }

        if (player.x < 0) {
            ifx = 0;
            player.x = 0;
        }

        // onde fomos parar?

        if (player.x + ifx >= DIMX - 1) {
            player.x = player.previousX;
            ifx--;
        }

        switch (gNivel.matriz[player.y + ify][player.x + ifx]) {
            case PORTAL:
                player.x = 0;
                player.y = 0;
                wasInThePortal = true;
                break;
            case PAREDE:
                player.x = player.previousX;
                player.y = player.previousY;
                break;
            case RATOEIRA:
                player.lifes--;
                gNivel.matriz[player.y + ifx][player.x + ifx] = VAZIO;
                if (player.lifes == 0) {
                    gameOver(tGameOverMsg);
                }
            case CENOURA:
                player.lifes++;
                gNivel.matriz[player.y + ifx][player.x + ifx] = VAZIO;
            case POISON:
                player.intoxication += 0.35;
                if (player.intoxication >= 1) {
                    player.lifes--;
                    player.intoxication = 0;
                    if (player.lifes == 0) exit(0);
                }
                gNivel.matriz[player.y + ifx][player.x + ifx] = VAZIO;
        }

        contadorGeral++;

        std::stringstream cont;
        cont << contadorGeral;

        menuFPS.displayText = "Frames: ";
        tFrames2.displayText = cont.str();

        std::stringstream lifesSTR;
        lifesSTR << player.lifes;

        menuLifes.displayText = "Lifes: " + lifesSTR.str();
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

        if (menuScore.font == NULL) {
            menuScore.font = TTF_OpenFont("media/emulogic.ttf", 20);
            menuScore.displayText = "hello";
            menuScore.color = colorBlue;
            menuScore.rect.x = SIZEX;
            menuScore.rect.y = 20;
            menuScore.rect.w = 80;
            menuScore.rect.h = 25;
        }

        if (menuFPS.font == NULL) {
            menuFPS.font = TTF_OpenFont("media/emulogic.ttf", 20);
            menuFPS.displayText = "Frames: ";
            menuFPS.color = colorBlue;
            menuFPS.rect.x = SIZEX;
            menuFPS.rect.y = 50;
            menuFPS.rect.w = 150;
            menuFPS.rect.h = 25;
        }

        if (tFrames2.font == NULL) {
            tFrames2.font = TTF_OpenFont("media/emulogic.ttf", 20);
            tFrames2.color = colorBlue;
            tFrames2.rect.x = SIZEX + 150;
            tFrames2.rect.y = 50;
            tFrames2.rect.w = 30;
            tFrames2.rect.h = 25;
        }

        if (menuLifes.font == NULL) {
            menuLifes.font = TTF_OpenFont("media/emulogic.ttf", 20);
            menuLifes.displayText = "Lifes: ";
            menuLifes.color = colorBlue;
            menuLifes.rect.x = SIZEX;
            menuLifes.rect.y = 80;
            menuLifes.rect.w = 150;
            menuLifes.rect.h = 25;
        }

//		menuScore.surface = TTF_RenderText_Solid(menuScore.font, menuScore.displayText.c_str(), colorBlue);
//		menuScore.texture = SDL_CreateTextureFromSurface(gRenderer, menuScore.surface);
//
//		menuFPS.surface = TTF_RenderText_Solid(menuFPS.font, menuFPS.displayText.c_str(), colorBlue);
//		menuFPS.texture = SDL_CreateTextureFromSurface(gRenderer, menuFPS.surface);
//
//        // \bug raising bad access exception
//		tFrames2.surface = TTF_RenderText_Solid(tFrames2.font, tFrames2.displayText.c_str(), colorBlue);
//		tFrames2.texture = SDL_CreateTextureFromSurface(gRenderer, tFrames2.surface);
//
//		menuLifes.surface = TTF_RenderText_Solid(menuLifes.font, menuLifes.displayText.c_str(), colorBlue);
//		menuLifes.texture = SDL_CreateTextureFromSurface(gRenderer, menuLifes.surface);
//
//		SDL_SetRenderDrawColor(gRenderer, colorG, colorG, colorG, 0xFF);
//		SDL_RenderClear(gRenderer);
//
//		SDL_RenderCopy(gRenderer, menuScore.texture, NULL, &menuScore.rect);
//		SDL_RenderCopy(gRenderer, menuFPS.texture, NULL, &menuFPS.rect);
//		SDL_RenderCopy(gRenderer, tFrames2.texture, NULL, &tFrames2.rect);
//		SDL_RenderCopy(gRenderer, menuLifes.texture, NULL, &menuLifes.rect);

        gNivel.render(ifx, ify);
        player.render();
        if (player.shooting) {
            player.shoot();
        }
//        player.seeStraightLine(3);
        dullEnemy.render();

        SDL_RenderPresent(gRenderer);

        fpsMili = 1000 / FPS;

        SDL_Delay(fpsMili - deltaTime);
        //------------------------------
    }
}