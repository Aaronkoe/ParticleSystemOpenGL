#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(unsigned int maxPart) :
	maxParticles(maxPart)
{
	particleArray = new Particle[maxParticles];
	particlePositionArray = new float[9 * maxParticles];
}

void ParticleContainer::UpdateTimestep(double dt)
{
	float GRAVITY_CONSTANT = .5f;
	unsigned int numParticlesToDraw = 0;
	for (int i = 0; i < maxParticles; ++i) {
		// create reference for easy reading
		Particle & particle = particleArray[i];
		if (particle.lifeSpan > 0) {
			// update direction then update position
			particle.direction = (particle.direction + Vector(0, -9.81, 0)).Scale(GRAVITY_CONSTANT * dt);
			particle.position = particle.position + particle.direction.Scale(dt);
			particle.lifeSpan -= dt;
			// update position array
			particlePositionArray[3 * numParticlesToDraw + 0] = particle.position.x;
			particlePositionArray[3 * numParticlesToDraw + 1] = particle.position.y;
			particlePositionArray[3 * numParticlesToDraw + 2] = particle.position.z;
			particlePositionArray[3 * numParticlesToDraw + 3] = particle.position.x;
			particlePositionArray[3 * numParticlesToDraw + 4] = particle.position.y - .1;
			particlePositionArray[3 * numParticlesToDraw + 5] = particle.position.z;
			particlePositionArray[3 * numParticlesToDraw + 6] = particle.position.x + .1;
			particlePositionArray[3 * numParticlesToDraw + 7] = particle.position.y;
			particlePositionArray[3 * numParticlesToDraw + 8] = particle.position.z;
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
