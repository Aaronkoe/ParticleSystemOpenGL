#include "Particle.h"

Particle::Particle() : lifeSpan(-1)
{
}

Particle::Particle(Vector p, Vector d, Vector c, float life) :
	position(p),
	direction(d),
	color(c),
	lifeSpan(life)
{
	elasticity = .5;
}

Particle::Particle(Vector p, Vector d, Vector c, float life, float e) :
	position(p),
	direction(d),
	color(c),
	lifeSpan(life),
	elasticity(e)
{
}

void Particle::UpdateTimeStep(float dt)
{
	position = position + direction.Scale(dt);
}
