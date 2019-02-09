#pragma once
#include <glm/glm.hpp>
#include "Particle.h"
#include "Vector.h"
class CollisionPlane {
public:
	CollisionPlane(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3);
	CollisionPlane(float p1, float p2, float p3, float n1, float n2, float n3);
	bool isColliding(Particle & p);
	void HandleCollision(Particle & p);
	glm::vec3 GetNormal() const { return normal; }
private:
	glm::vec3 point;
	glm::vec3 normal;
};