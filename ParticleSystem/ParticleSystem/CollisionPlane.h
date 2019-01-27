#pragma once
#include "Vector.h"
class CollisionPlane {
public:
	CollisionPlane(float p1, float p2, float p3, float n1, float n2, float n3, float t);
	bool isColliding(float x, float y, float z);
private:
	Vector point;
	Vector normal;
	float tolerance;
};