#ifndef NIVEL
#define NIVEL

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <stdio.h>

class Nivel {
private:
	int		colorG = 150;
	bool	colorGNeg = false;
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
			puts("sem arquivo"); exit(0);
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
			std::cout << count++ << " " << s << std::endl;

			char *p = s;
			idx = 0;
			while (*p) {
				switch (*p) {
					case '~': matriz[idy][idx] = VAZIO; break;
					case 'X': matriz[idy][idx] = PAREDE; break;
					case '*': matriz[idy][idx] = PORTAL; break;
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

	void render(int ifx) {

		int x, y;

		if ((ifx < 0) || (ifx + LIMX >= DIMX)) {
			puts("algo errado em sua logica");
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


		for (x = 0 + ifx; x < LIMX + ifx; x++) {
			for (y = 0; y < DIMY; y++) {
				switch (matriz[y][x]) {
					case VAZIO: SDL_SetRenderDrawColor(_gRenderer, 60, 60, 60, 255); break;
					case PORTAL: SDL_SetRenderDrawColor(_gRenderer, 0, colorG, 0, 255); break;
					case 2: SDL_SetRenderDrawColor(_gRenderer, 50, 250, 60, 255); break;
					case PAREDE: SDL_SetRenderDrawColor(_gRenderer, 0, 0, 0, 255); break;
				}
				rect.x = (x - ifx)*PASSO;
				rect.y = y*PASSO;
				SDL_RenderFillRect(_gRenderer, &rect);

			}
		}
	};
};

#endif