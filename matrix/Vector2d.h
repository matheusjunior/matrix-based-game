#ifndef vector2d
#define vector2d

struct Vector2d {
	double x;
	double y;

	Vector2d():x(0.0), y(0.0){}

	Vector2d(float a, float b) :x(a), y(b){}

	void setZero();

	bool isZero() const;

	float getLength() const;

	float getDistance(const Vector2d &v2) const;

	float getDotProduct(const Vector2d &v2) const;

	void Truncate(float max);

	void Normalize();
	 
	double Dot(const Vector2d &v2) const;
	
	Vector2d Perp() const;

	Vector2d GetReverse() const;
	
	void Reflect(const Vector2d& norm);


	/********************************************/

	const Vector2d& operator+=(const Vector2d &rhs) {
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2d& operator-=(const Vector2d &rhs) {
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2d& operator*=(const double& rhs) {
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2d& operator/=(const double& rhs) {
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const Vector2d& rhs) const {
		return ((x == rhs.x) && (y == rhs.y));
	}

	bool operator!=(const Vector2d& rhs) const {
		return (x != rhs.x) || (y != rhs.y);
	}

};

inline Vector2d operator*(const Vector2d &lhs, double rhs);
inline Vector2d operator*(double lhs, const Vector2d &rhs);
inline Vector2d operator-(const Vector2d &lhs, const Vector2d &rhs);
inline Vector2d operator+(const Vector2d &lhs, const Vector2d &rhs);
inline Vector2d operator/(const Vector2d &lhs, double val);

inline Vector2d operator*(const Vector2d &lhs, double rhs)
{
	Vector2d result(lhs);
	result *= rhs;
	return result;
}

inline Vector2d operator*(double lhs, const Vector2d &rhs)
{
	Vector2d result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Vector2d operator-(const Vector2d &lhs, const Vector2d &rhs)
{
	Vector2d result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline Vector2d operator+(const Vector2d &lhs, const Vector2d &rhs)
{
	Vector2d result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline Vector2d operator/(const Vector2d &lhs, double val)
{
	Vector2d result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}

#endif