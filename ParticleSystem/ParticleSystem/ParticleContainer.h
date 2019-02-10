#pragma once
#include <algorithm>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <learnopengl/shader.h>
#include "CollisionPlane.h"
#include "Particle.h"

class ParticleContainer {
public:
	unsigned int VAO, baseVbo, posVbo, colorVbo;
	Shader * shader = NULL;

	ParticleContainer(unsigned int maxPart);
	ParticleContainer(unsigned int maxPart, Particle(*randomParticleFunc)(double));
	ParticleContainer(unsigned int maxPart, Particle(*randomParticleFunc)(glm::vec3));
	void InitializeVAO();
	void UpdateBuffers();
	void Draw(glm::mat4 view, glm::mat4 proj);
	void SetShader(Shader * s) { shader = s; }
	void UpdateTimestep(glm::vec3 cameraPosition, float dt);
	void UpdateTimestepSpecial(glm::vec3 cameraPosition, float dt);
	float * GetPositionArray() { return particlePositionArray; }
	float * GetColorArray() { return particleColorArray; }
	unsigned int GetNumParticles() { return particlesCount; }
	void AddParticle(double timeElapsed);
	void AddParticle(glm::vec3 origin);
	void AddParticle(Particle p);
	void AddCollidable(CollisionPlane p);
private:
	std::vector<CollisionPlane> collidables;

	unsigned int maxParticles;
	unsigned int lastUsedParticle = 0;
	unsigned int particlesCount = 0;
	Particle * particleArray;
	float * particlePositionArray;
	float * particleColorArray;

	void SortParticles();
	int FindUnusedParticle();
	void CheckCollisions(Particle & particle);
	Particle(*randomParticle)(double) = NULL;
	Particle(*randomParticleOrigin)(glm::vec3) = NULL;

	static float quadVerts[12];
};

