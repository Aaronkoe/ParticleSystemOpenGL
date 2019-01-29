#pragma once
#include "Vector.h"
class Particle {
public:
	Particle();
	Particle(Vector p, Vector d, Vector c, float life);
	Particle(Vector p, Vector d, Vector c, float life, float elasticity);
	void UpdateTimeStep(float dt);
	float lifeSpan;
	Vector position;
	Vector direction;
	Vector color;
	float elasticity;
private:
};