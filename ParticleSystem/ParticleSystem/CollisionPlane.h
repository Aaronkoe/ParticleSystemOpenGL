#pragma once
#include <glm/glm.hpp>
#include "Particle.h"
#include "Vector.h"
class CollisionPlane {
public:
	CollisionPlane(float p1, float p2, float p3, float n1, float n2, float n3);
	bool isColliding(Particle & p);
	glm::vec3 GetNormal() const { return normal; }
private:
	Vector point;
	glm::vec3 normal;
};