#pragma once
#include "Vector.h"
class Particle {
public:
	Particle();
	Particle(Vector p, Vector d, float s, Vector c);
	void UpdateTimeStep(float dt);
private:
	Vector position;
	Vector direction;
	float speed;
	Vector color;
	float lifeSpan;
};