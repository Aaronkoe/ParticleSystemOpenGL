#pragma once
#include "Vector.h"
class Particle {
public:
	Particle();
	Particle(Vector p, Vector d, Vector c, float life);
	void UpdateTimeStep(float dt);
	float lifeSpan;
	Vector position;
	Vector direction;
	Vector color;
private:
};