#pragma once
#include <glm/glm.hpp>
#include "ParticleContainer.h"
#include "Particle.h"
#include "Cube.h"
class FireWork {
public:
	FireWork(float propRate, unsigned int max1, unsigned int max2, Particle(*prop)(glm::vec3), Particle(*expl)(glm::vec3), Shader * propShader, Shader * explShader);
	void Draw(glm::mat4 view, glm::mat4 proj);
	void Update(glm::vec3 camPos, float dt);
	void DrawParticles(glm::mat4 view, glm::mat4 proj);
	float propellantRate;
	float explosionRate;
	ParticleContainer propellant;
	ParticleContainer explosion;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	Cube toDraw;
	float timeSinceProp = 0;
	float timeToExplode;
	float timeSinceExplosion;
};