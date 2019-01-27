#include "Particle.h"

Particle::Particle(Vector p, Vector d, float s, Vector c) :
	position(p),
	direction(d),
	color(c)
{
	speed = s;
}

void Particle::UpdateTimeStep(float dt)
{
	position = position + direction.Scale(dt * speed);
}
