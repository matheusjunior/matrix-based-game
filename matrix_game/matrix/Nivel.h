#ifndef NIVEL
#define NIVEL

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#endif
#include <iostream>
#include <stdio.h>

class Nivel {
private:
	int		     colorG = 150;
	bool	     colorGNeg = false;
	SDL_Surface *surf_ratoeira;
	SDL_Surface *surf_poison;
	SDL_Surface *surf_cenoura;
public:

	SDL_Rect	  rect;
	SDL_Renderer* _gRenderer;

	int matriz[DIMY][DIMX];

	Nivel() {
		rect.w = rect.h = PASSO - 1;
	};

	Nivel(SDL_Renderer *gRend) {
		_gRenderer = gRend;
		rect.w = rect.h = PASSO - 1;
	}

	~Nivel() { ; };

	bool carregar(const char *path) {

		puts(path);

		FILE *arq;
		char s[100];

		if (NULL == (arq = fopen(path, "rt"))) {
			puts("sem arquivo");
			return false;
		}

		int idx, idy;
		idy = 0;
		int count = 1;
		if (arq == NULL) {
			fclose(arq);
			return false;
		}
		if (arq == 0) {
			fclose(arq);
			return false;
		}

		while (!feof(arq)) {

			fgets(s, 100, arq);
			//			std::cout << count++ << " " << s << std::endl;

			char *p = s;
			idx = 0;
			while (*p) {
				switch (*p) {
				case '~': matriz[idy][idx] = VAZIO; break;
				case 'X': matriz[idy][idx] = PAREDE; break;
				case '*': matriz[idy][idx] = PORTAL; break;
				case 'R': matriz[idy][idx] = RATOEIRA; break;
				case 'P': matriz[idy][idx] = POISON; break;
				case 'C': matriz[idy][idx] = CENOURA; break;
				default: matriz[idy][idx] = VAZIO; break;
				}
				++idx;
				++p;
			}
			++idy;

			if (arq == NULL) {
				fclose(arq);
				return false;
			}
			if (arq == 0) {
				fclose(arq);
				return false;
			}
		}

		fclose(arq);
		return true;
	}

	bool surfacesNotLoaded() {
		if (surf_ratoeira == NULL) return true;
		if (surf_poison == NULL) return true;
		if (surf_cenoura == NULL) return true;
		return false;
	}

	void render(int ifx, int ify) {
		int x, y;

		if ((ifx < 0) || (ifx + LIMY >= DIMX)) {
			puts("algo errado em sua logicaY");
			exit(0);
		}

		if ((ifx < 0) || (ifx + LIMX >= DIMX)) {
			puts("algo errado em sua logicaX");
			exit(0);
		}

		if (colorG == 255) {
			colorGNeg = true;
		}
		if (colorGNeg) {
			colorG--;
			if (colorG == 150) {
				colorGNeg = false;
			}
		}
		else {
			colorG++;
		}


		SDL_Texture *texture = NULL;
		bool isSprite = false;

        /// \bug bad access exception if commented out
//		if (surfacesNotLoaded()) {
			surf_ratoeira = IMG_Load("media/ratoeira.jpg");
			surf_poison = IMG_Load("media/poison.png");
			surf_cenoura = IMG_Load("media/cenoura.png");
//		}



		for (x = 0 + ifx; x < LIMX + ifx; x++) {
			for (y = 0; y < DIMY; y++) {
				switch (matriz[y][x]) {
				case VAZIO: SDL_SetRenderDrawColor(_gRenderer, 60, 60, 60, 255); break;
				case PORTAL: SDL_SetRenderDrawColor(_gRenderer, 0, colorG, 0, 255); break;
				case 2: SDL_SetRenderDrawColor(_gRenderer, 50, 250, 60, 255); break;
				case PAREDE: SDL_SetRenderDrawColor(_gRenderer, 0, 0, 0, 255); break;
				case RATOEIRA:
					//surface = IMG_Load("media/ratoeira.jpg");
					texture = SDL_CreateTextureFromSurface(_gRenderer, surf_ratoeira);
					isSprite = true;
					break;
				case POISON:
					//surface = IMG_Load("media/poison.png");
					texture = SDL_CreateTextureFromSurface(_gRenderer, surf_poison);
					isSprite = true;
					break;
				case CENOURA:
					//surface = IMG_Load("media/cenoura.png");
					texture = SDL_CreateTextureFromSurface(_gRenderer, surf_cenoura);
					isSprite = true;
					break;
				}
				rect.x = (x - ifx)*PASSO;
				rect.y = y*PASSO;
				if (isSprite) SDL_RenderCopy(_gRenderer, texture, NULL, &rect);
				else SDL_RenderFillRect(_gRenderer, &rect);
				isSprite = false;
			}
		}
	}
};


#endif