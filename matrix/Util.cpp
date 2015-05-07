//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//

#include <stdlib.h>
#include "Util.h"

#ifdef _WIN32

#include <time.h>
#include <Windows.h>
#include <SDL.h>

#else

#endif

#include "Vector2d.h"
#include <math.h>

int Util::GenerateRandom(int l, int u)
{
    double r = rand() % ((u - l) + 1);
    r = l + r;

	return (int) r;
}

double Util::CosDeg(double angle) {
	
	if (angle == 90) return 0;
	double angleradians = angle * M_PI / 180.0f;
	return cos(angleradians);
}

double Util::SinDeg(double angle) {
	double angleradians = angle * M_PI / 180.0f;
	return sin(angleradians);
}

Vector2d Util::getDistance(Vector2d r1, Vector2d r2) {

	Vector2d temp;

	double x = r1.x - r2.x;
	double y = r1.y - r2.y;

	temp.x = sqrt(x*x);
	temp.y = sqrt(y*y);
	

	return temp;
}


// git reset --hard <sha1-commit-id>