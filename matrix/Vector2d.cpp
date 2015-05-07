#include <math.h>
#include "Vector2d.h"

#ifdef _WIN32

#include <math.h>

#else



#endif

void Vector2d::setZero() {
	x = 0;
	y = 0;
}

bool Vector2d::isZero() const { 
	return (x == 0 && y == 0); 
}

float Vector2d::getLength() const {
	return sqrt(x*x + y*y);
}


float Vector2d::getDistance(const Vector2d &v2) const {
	float ySeparation = v2.y - y;
	float xSeparation = v2.x - x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}


float Vector2d::getDotProduct(const Vector2d &v2) const {
	return x*v2.x + y*v2.y;
}

void Vector2d::Truncate(float max) {
	if (this->getLength() > max)
	{
		this->Normalize();

		*this *= max;
	}
}

void Vector2d::Normalize() {
	float vector_length = this->getLength();

	if (vector_length > 0)
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}

double Vector2d::Dot(const Vector2d &v2) const {
	return x*v2.x + y*v2.y;
}

Vector2d Vector2d::Perp() const{
	return Vector2d(-y, x);
}

Vector2d Vector2d::GetReverse()const
{
	return Vector2d(-this->x, -this->y);
}

void Vector2d::Reflect(const Vector2d& norm) {
	*this += 2.0 * this->Dot(norm) * norm.GetReverse();
}