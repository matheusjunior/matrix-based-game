//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//


#ifndef __Text_H_
#define __Text_H_


#ifdef _WIN32

#include <SDL_render.h>
#include <SDL_ttf.h>

#else

#include <SDL2/SDL_render.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <string>

#endif


#include <string>

/* \todo Create interface for applying currTexture
* \todo Document class
* */
class Text
{
public:
    SDL_Surface *surface;
    SDL_Rect rect;
    TTF_Font *font;
    SDL_Color color;
    SDL_Texture *texture;
    std::string displayText;

    Text();
};


#endif //__Text_H_
