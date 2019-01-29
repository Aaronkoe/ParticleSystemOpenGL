#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(unsigned int maxPart) :
	maxParticles(maxPart)
{
	particleArray = new Particle[maxParticles];
	particlePositionArray = new float[4 * maxParticles];
}

ParticleContainer::ParticleContainer(unsigned int maxPart, Particle(*randomParticleFunc)(double)) :
	maxParticles(maxPart)
{
	particleArray = new Particle[maxParticles];
	particlePositionArray = new float[4 * maxParticles];
	randomParticle = randomParticleFunc;
}

void ParticleContainer::UpdateTimestep(double dt)
{
	Vector GRAVITY_VECTOR(0, -9.81, 0);
	GRAVITY_VECTOR = GRAVITY_VECTOR.Scale(.5);
	unsigned int numParticlesToDraw = 0;
	for (int i = 0; i < maxParticles; ++i) {
		// create reference for easy reading
		Particle & particle = particleArray[i];
		if (particle.lifeSpan > 0) {
			// update direction then update position
			particle.direction = (particle.direction + GRAVITY_VECTOR.Scale(dt));
			CheckCollisions(particle);
			particle.position = particle.position + particle.direction.Scale(dt);
			particle.lifeSpan -= dt;
			// update position array
			particlePositionArray[4 * numParticlesToDraw + 0] = particle.position.x;
			particlePositionArray[4 * numParticlesToDraw + 1] = particle.position.y;
			particlePositionArray[4 * numParticlesToDraw + 2] = particle.position.z;
			particlePositionArray[4 * numParticlesToDraw + 3] = .1f;
			// check for collisions
			// increment numParticles
			++numParticlesToDraw;
		}
		else {
			//todo
		}
	}
	particlesCount = numParticlesToDraw;
}

void ParticleContainer::AddParticle(double timeElapsed)
{
	Particle newParticle = randomParticle(timeElapsed);
	AddParticle(newParticle);
}

void ParticleContainer::AddParticle(Particle p)
{
	for (int i = lastUsedParticle; i < maxParticles; ++i) {
		if (particleArray[i].lifeSpan < 0) {
			lastUsedParticle = i;
			particleArray[i] = p;
			return;
		}
	}
	for (int i = 0; i < lastUsedParticle; ++i) {
		if (particleArray[i].lifeSpan < 0) {
			lastUsedParticle = i;
			particleArray[i] = p;
			return;
		}
	}
}

void ParticleContainer::AddCollidable(CollisionPlane p)
{
	collidables.push_back(p);
}

void ParticleContainer::CheckCollisions(Particle & particle)
{
	for (CollisionPlane & collidable : collidables) {
		if (collidable.isColliding(particle)) {
			particle.direction = particle.direction - collidable.GetNormal().Scale(2 * (particle.direction.Dot(collidable.GetNormal())));
			//particle.direction = particle.direction.Scale(particle.elasticity);
		}
	}
}