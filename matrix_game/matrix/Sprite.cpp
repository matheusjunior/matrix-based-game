
#ifdef _WIN32

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


#else

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_image/SDL_image.h>

#endif

#include <string>

#include "Sprite.h"
#include "Consts.h"





void Sprite::jump () {

   // inicio de rascunho
   // muita coisa precisa ser modificada para implementar saltos
   // sugestao, so' se preocupe com isto se realmente
   ///  

   phi.vy = -1;
   phi.fy = 0.1;   

} ;



bool Sprite::loadFromFile (std::string path, int n) {

    _nframes = n;

	free();

	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load (path.c_str());
	if (loadedSurface == NULL) {
		printf ("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		exit (0);
	}

	    //Color key image
		Uint8 r,g,b;
		SDL_GetRGB      (((Uint32*)loadedSurface->pixels)[0],loadedSurface->format,&r,&g,&b);
		
		//printf ("cores = %d %d %d\n", r, g, b);
		
		SDL_SetColorKey (loadedSurface, SDL_TRUE, SDL_MapRGB (loadedSurface->format, r,g,b));
	
//	SDL_SetColorKey (loadedSurface, SDL_TRUE, SDL_MapRGB (loadedSurface->format, 0, 0xFF, 0xFF));

	newTexture = SDL_CreateTextureFromSurface (gRenderer, loadedSurface);

	if (newTexture == NULL) {
		printf ("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		exit (0);
	}

	//Get image dimensions
	_Width = loadedSurface->w / n;
	_Height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface (loadedSurface);

	//Return success
	_Texture = newTexture;
	return _Texture != NULL;
}

void Sprite::free() {

	if (_Texture != NULL) {
		SDL_DestroyTexture (_Texture);
		_Texture = NULL;
		_Height = 0;
		_Width = 0;
	}
}

void Sprite::render () {

	SDL_Rect renderQuad = { x*PASSO, y*PASSO, _Width, _Height};	

	++_frame;
	if ((_frame/_step) == _nframes) _frame = 0;
	
	SDL_RenderCopyEx (gRenderer, _Texture, NULL, &renderQuad, _alpha*57, NULL, SDL_FLIP_NONE);
	
}
