#include "CollisionPlane.h"

CollisionPlane::CollisionPlane(float p1, float p2, float p3, float n1, float n2, float n3)
	:
	point(p1, p2, p3),
	normal(n1, n2, n3)
{
}

bool CollisionPlane::isColliding(Particle & p)
{
	glm::vec3 inBetween(p.position.x - point.x, p.position.y - point.y, p.position.z - point.z);
	float check = glm::dot(inBetween, normal);
	return check < 0 && glm::dot(p.velocity, normal) < 0;
}
