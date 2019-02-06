#pragma once
#include "Particle.h"
class Collidable {
public:
	virtual bool DetectCollision(const Particle & particle) = 0;
	virtual void ReactToCollision(Particle & particle) = 0;
};