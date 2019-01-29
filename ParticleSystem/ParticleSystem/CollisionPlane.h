#pragma once
#include "Particle.h"
#include "Vector.h"
class CollisionPlane {
public:
	CollisionPlane(float p1, float p2, float p3, float n1, float n2, float n3);
	bool isColliding(Particle & p);
	Vector GetNormal() const { return normal; }
private:
	Vector point;
	Vector normal;
};