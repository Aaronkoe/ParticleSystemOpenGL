#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(unsigned int maxPart) :
	maxParticles(maxPart)
{
	particleArray = new Particle[maxParticles];
	particlePositionArray = new float[9 * maxParticles];
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
			particle.position = particle.position + particle.direction.Scale(dt);
			particle.lifeSpan -= dt;
			// update position array
			particlePositionArray[9 * numParticlesToDraw + 0] = particle.position.x;
			particlePositionArray[9 * numParticlesToDraw + 1] = particle.position.y;
			particlePositionArray[9 * numParticlesToDraw + 2] = particle.position.z;
			particlePositionArray[9 * numParticlesToDraw + 3] = particle.position.x;
			particlePositionArray[9 * numParticlesToDraw + 4] = particle.position.y - .1;
			particlePositionArray[9 * numParticlesToDraw + 5] = particle.position.z;
			particlePositionArray[9 * numParticlesToDraw + 6] = particle.position.x + .1;
			particlePositionArray[9 * numParticlesToDraw + 7] = particle.position.y;
			particlePositionArray[9 * numParticlesToDraw + 8] = particle.position.z;
			// check for collisions
			CheckCollisions(particle);
			// increment numParticles
			++numParticlesToDraw;
		}
		else {
			//todo
		}
	}
	particlesCount = numParticlesToDraw;
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
		if (collidable.isColliding(particle.position.x, particle.position.y, particle.position.z)) {
			particle.direction = particle.direction.Scale(-1);
		}
	}
}