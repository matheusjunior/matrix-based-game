#ifndef SPRITE
#define SPRITE


#ifdef _WIN32

#include <time.h>
#include <SDL.h>
#include <math.h>

#else

#endif

#include "Vector2d.h"
#include "Physics.h"

class Sprite {
public:
    uint16_t lifes;
    float intoxication;
	Physics phi;
	SDL_Renderer* _gRenderer;
	int x, y, _alpha, _sx, _sy, _cx, _cy;
	float ax0, ay0, ax1, ay1, ax2, ay2, ax3, ay3;
	SDL_Texture *_Texture;
	int _Width, _Height, _frame, _nframes;
	float _step = 1;
	void setStep(int n) { _step = n; };

	Sprite() { _Texture = NULL; _Width = 0; _Height = 0; lifes = 1;};

	Sprite(SDL_Renderer *rend) { 
		_Texture = NULL; 
		_Width = 0; 
		_Height = 0; 
	    _gRenderer = rend;
        lifes = 1;
	};
	
	~Sprite() { free(); };

	bool loadFromFile(std::string, int);
	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue){
		SDL_SetTextureColorMod(_Texture, red, green, blue);
	};
	void setBlendMode(SDL_BlendMode blending) {
		SDL_SetTextureBlendMode(_Texture, blending);
	}
	void setAlpha(Uint8 alpha) {
		SDL_SetTextureAlphaMod(_Texture, alpha);
	}

	void render();

    void moveRandom();

	void jump();
};


#endif