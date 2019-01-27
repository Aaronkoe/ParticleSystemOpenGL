#pragma once
#include <cmath>
struct Vector {
	Vector(float x_, float y_, float z_);
	Vector Dot(const Vector & rhs) const;
	Vector Cross(const Vector & rhs) const;
	float Magnitude() const;
	Vector Scale(float s) const;
	Vector operator+(const Vector & rhs) const;
	Vector operator-(const Vector & rhs) const;
	float x, y, z;
};