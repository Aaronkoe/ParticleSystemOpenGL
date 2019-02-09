#include "CollisionPlane.h"

CollisionPlane::CollisionPlane(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3)
{
	point.x = c1.x; point.y = c1.y; point.z = c1.z;
	normal = glm::cross(c2 - c1, c3 - c1);
	normal = glm::normalize(normal);
}

CollisionPlane::CollisionPlane(float p1, float p2, float p3, float n1, float n2, float n3)
	:
	point(p1, p2, p3),
	normal(n1, n2, n3)
{
	normal = glm::normalize(normal);
}

bool CollisionPlane::isColliding(Particle & p)
{
	glm::vec3 inBetween(p.position - point);
	if (glm::dot(inBetween, normal) <= 0) {
		p.position -= glm::dot(inBetween, normal) / (glm::length(inBetween) * glm::length(inBetween)) * inBetween;
		return true;
	}
}

void CollisionPlane::HandleCollision(Particle & p)
{
	glm::vec3 inBetween = p.position - point;
	//glm::vec3 inBetween = point - p.position;
	if (glm::dot(inBetween, normal) < 0) {
		p.position -= normal * glm::dot(inBetween, normal) / (glm::length(inBetween) * glm::length(inBetween));
		p.velocity = p.velocity - normal * (1 + p.elasticity) * glm::dot(p.velocity, normal);
	}
}
