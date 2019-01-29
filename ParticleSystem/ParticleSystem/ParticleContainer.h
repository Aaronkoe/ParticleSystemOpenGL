#pragma once
#include <vector>
#include "CollisionPlane.h"
#include "Particle.h"

class ParticleContainer {
public:
	ParticleContainer(unsigned int maxPart);
	ParticleContainer(unsigned int maxPart, Particle(*randomParticleFunc)(double));
	void UpdateTimestep(double dt);
	float * GetPositionArray() { return particlePositionArray; }

	unsigned int GetNumParticles() { return particlesCount; }
	void AddParticle(double timeElapsed);
	void AddParticle(Particle p);
	void AddCollidable(CollisionPlane p);
private:
	std::vector<CollisionPlane> collidables;

	unsigned int maxParticles;
	unsigned int lastUsedParticle = 0;
	unsigned int particlesCount = 0;
	Particle * particleArray;
	float * particlePositionArray;

	void CheckCollisions(Particle & particle);
	Particle(*randomParticle)(double) = NULL;
};