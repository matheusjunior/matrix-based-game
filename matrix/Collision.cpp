//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//

#include <math.h>

#include "Collision.h"

bool Collision::AABBCollision(SDL_Rect *a, SDL_Rect *b)
{
	bool xCollision;
	bool yCollision;

	if (a->x > b->x + b->w || b->x > a->x + a->w) xCollision = false;
	else xCollision = true;

	if (a->y - a->h > b->y + b->h || b->y - b->h > a->y + a->h) yCollision = false;
	else yCollision = true;

	if (xCollision && yCollision) return true;
	return false;
}

bool Collision::CircleCollision(SDL_Rect c1, SDL_Rect c2) {
	double dx = c1.x - c2.x;
	double dy = c1.y - c2.y;

	int distance = sqrt(dx*dx + dy*dy);

	int bgst = 0;

	if (c1.h > c1.w) bgst = c1.h / 2;
	else bgst = c1.w / 2;

	int bgst2 = 0;
	if (c2.h > c2.w) bgst2 = c2.h / 2;
	else bgst2 = c2.w / 2;


	if (distance < bgst + bgst2) return true; return false;
}
