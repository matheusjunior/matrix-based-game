//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//


#ifndef Util_H_
#define Util_H_

#ifdef _WIN32

#include <time.h>
#include <SDL.h>
#include <math.h>

#else

#endif

#include "Vector2d.h"

class Util
{
public:    
    int static GenerateRandom(int l, int u);

	double static CosDeg(double angle);

	double static SinDeg(double angle);

	Vector2d static getDistance(Vector2d r1, Vector2d r2);
};

#endif
