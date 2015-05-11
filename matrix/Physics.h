#ifndef PHYSICS
#define PHYSICS

#include <math.h>

class Physics {

public:
	float    x, y
		, vx, vy
		, ax, ay
		, fx, fy
		, m
		;

	Physics() { x = y = vx = vy = ax = ay = fx = fy = 0; m = 1; };

	void EulerStep(float dt) {

		ax = fx / m; ay = fy / m;

		vx += ax * dt; vy += ay * dt;
		x += vx * dt; y += vy * dt;
	};
};

#endif
