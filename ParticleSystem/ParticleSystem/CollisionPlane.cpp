#include "CollisionPlane.h"

CollisionPlane::CollisionPlane(float p1, float p2, float p3, float n1, float n2, float n3, float t)
	:
	point(p1, p2, p3),
	normal(n1, n2, n3)
{
	tolerance = t;
}

bool CollisionPlane::isColliding(float x, float y, float z)
{
	Vector inBetween(x - point.x, y - point.y, z - point.z);
	inBetween = inBetween.Dot(normal);
	return inBetween.Magnitude() < tolerance;
}
