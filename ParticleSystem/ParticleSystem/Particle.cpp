#include "Particle.h"

Particle::Particle() : lifeSpan(-1)
{
}

Particle::Particle(glm::vec3 p, glm::vec3 vel, glm::vec4 c, float life, float s) :
	position(p),
	velocity(vel),
	color(c),
	lifeSpan(life),
	size(s)
{
	elasticity = .5;
}

Particle::Particle(glm::vec3 p, glm::vec3 vel, glm::vec4 c, float life, float e, float s) :
	position(p),
	velocity(vel),
	color(c),
	lifeSpan(life),
	elasticity(e),
	size(s)
{
}

bool Particle::operator<(Particle & rhs)
{
	return cameraDistance > rhs.cameraDistance;
}

void Particle::UpdateTimeStep(float dt)
{
	position = position + velocity * dt;
}
