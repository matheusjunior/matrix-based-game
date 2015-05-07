//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//


#ifndef __Collision_H_
#define __Collision_H_



#ifdef _WIN32

#include <SDL_rect.h>

#else

#include <SDL2/SDL_rect.h>

#endif



/** Implements collision algorithms
* */
class Collision
{
public:
    /** AABB-AABB intersection test. Algorithm from http://goanna.cs.rmit.edu.au
    * \param a, b SDL_Rect objects to detect collision
    * \return true if a collided with b, false otherwise
    *
    * Check segment intersections
    * if (a_min_x > b_max_x || b_min_x > a_max_x)
    *   a and b are disjoint
    * else
    *   a and b overlaps
    * */
    bool static AABBCollision(SDL_Rect *a, SDL_Rect *b);

	bool static CircleCollision(SDL_Rect c1, SDL_Rect c2);
};

#endif //__Collision_H_
