#include "Vector.h"

Vector::Vector() : x(0), y(0), z(0)
{
}

Vector::Vector(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

Vector Vector::Dot(const Vector & rhs) const
{
	return Vector(x * rhs.x, y * rhs.y, z * rhs.z);
}

Vector Vector::Cross(const Vector & rhs) const
{
	return Vector(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

float Vector::Magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector Vector::Scale(float s) const
{
	return Vector(s * x, s * y, s * z);
}

Vector Vector::operator+(const Vector & rhs) const
{
	return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector Vector::operator-(const Vector & rhs) const
{
	return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
}
