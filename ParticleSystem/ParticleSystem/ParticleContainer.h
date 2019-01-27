#pragma once

#include "Particle.h"

class ParticleContainer {
public:
	ParticleContainer(unsigned int maxPart);
	void UpdateTimestep(double dt);
	float * GetPositionArray() { return particlePositionArray; }

	//debug functions
	unsigned int GetNumParticles() { return particlesCount; }
	void AddParticle(Particle p);
private:
	unsigned int maxParticles;
	unsigned int lastUsedParticle = 0;
	unsigned int particlesCount = 0;
	Particle * particleArray;
	float * particlePositionArray;
};