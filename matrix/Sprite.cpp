#ifdef _WIN32

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#else

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_image/SDL_image.h>

#endif

#include <string>
#include <stdio.h>
#include <iostream>
#include "Util.h"
#include "Sprite.h"
#include "Consts.h"


void Sprite::jump()
{

	// inicio de rascunho
	// muita coisa precisa ser modificada para implementar saltos
	// sugestao, so' se preocupe com isto se realmente
	///

	phi.vy = -1;
	phi.fy = 0.1;

};


bool Sprite::loadFromFile(std::string path, int n)
{
	_nframes = n;
	free();
	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	//Color key image
	Uint8 r, g, b;
	SDL_GetRGB(((Uint32 *)loadedSurface->pixels)[0], loadedSurface->format, &r, &g, &b);

	//printf ("cores = %d %d %d\n", r, g, b);

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, r, g, b));

	//	SDL_SetColorKey (loadedSurface, SDL_TRUE, SDL_MapRGB (loadedSurface->format, 0, 0xFF, 0xFF));

	newTexture = SDL_CreateTextureFromSurface(_gRenderer, loadedSurface);

	if (newTexture == NULL) {
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}

	//Get image dimensions
	_Width = loadedSurface->w / n;
	_Height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	//Return success
	_Texture = newTexture;
	return _Texture != NULL;
}

void Sprite::free()
{

	if (_Texture != NULL) {
		SDL_DestroyTexture(_Texture);
		_Texture = NULL;
		_Height = 0;
		_Width = 0;
	}
}

void Sprite::render()
{
	SDL_Rect renderQuad = { x * PASSO, y * PASSO, _Width, _Height };
	++_frame;

	if ((_frame / _step) == _nframes) _frame = 0;
	if (_gRenderer == NULL) {
		std::cout << "erro";
	}
	SDL_RenderCopyEx(_gRenderer, _Texture, NULL, &renderQuad, _alpha * 57, NULL, SDL_FLIP_NONE);
}

void Sprite::moveRandom()
{
	auto direction = Util::GenerateRandom(0, 100);

	if (direction < 25) {
		y--;
	}
	if (direction >= 25 && direction < 50) {
		x--;
	}
	if (direction >= 50 && direction < 75) {
		y++;
	}
	if (direction >= 75) {
		x++;
	}

}

bool Sprite::isValidMove(int matrix[DIMY][DIMX]) {

	if (x >= DIMX) {
		return false;
	}

	if (y >= DIMY) {
		return false;
	}

	if (x < 0) {
		return false;
	}

	if (y < 0) {
		return false;
	}

	if (matrix[y][x] != 0) {
		return false;
	}
	/*	switch (matrix[x][y]) {
	case PAREDE: return false; break;
	case PORTAL: return false; break;
	case CENOURA: return false; break;
	case POISON: return false; break;
	default: return true;
	}
	*/

	return true;
}
