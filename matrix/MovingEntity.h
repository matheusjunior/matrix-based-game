<<<<<<< HEAD
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
#include "Consts.h"

class MovingEntity : public Sprite {
public:
	int world[]

	MovingEntity() { _Texture = NULL; _Width = 0; _Height = 0; lifes = 1;};

	MovingEntity(SDL_Renderer *rend) { 
		_Texture = NULL; 
		_Width = 0; 
		_Height = 0; 
	    _gRenderer = rend;
        lifes = 1;
		previousX = 0;
		previousY = 0;

	};
	
	~MovingEntity() { free(); };

};


=======
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
#include "Consts.h"
#include "Sprite.h"


class MovingEntity : public Sprite {
public:
	

	MovingEntity() { _Texture = NULL; _Width = 0; _Height = 0; lifes = 1;};

	MovingEntity(SDL_Renderer *rend) { 
		_Texture = NULL; 
		_Width = 0; 
		_Height = 0; 
	    _gRenderer = rend;
        lifes = 1;
		previousX = 0;
		previousY = 0;

	};
	
	~MovingEntity() { free(); };

};


>>>>>>> 14961fccf3f669cb5ebc5868b37660b82a5db73c
#endif