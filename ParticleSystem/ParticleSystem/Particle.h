#pragma once
#include <glm/glm.hpp>
#include "Vector.h"
class Particle {
public:
	Particle();
	Particle(glm::vec3 p, glm::vec3 d, glm::vec4 c, float life, float s);
	Particle(glm::vec3 p, glm::vec3 d, glm::vec4 c, float life, float elasticity, float s);
	bool operator<(Particle & rhs);
	void UpdateTimeStep(float dt);
	float lifeSpan;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 acceleration;
	glm::vec4 color;
	float cameraDistance = -1.0f;
	float elasticity;
	float size;
private:
};