#include "ParticleContainer.h"
ParticleContainer::ParticleContainer(unsigned int maxPart) :
	maxParticles(maxPart)
{
	particleArray = new Particle[maxParticles];
	particlePositionArray = new float[4 * maxParticles];
	particleColorArray = new float[4 * maxParticles];
}

ParticleContainer::ParticleContainer(unsigned int maxPart, Particle(*randomParticleFunc)(double)) :
	maxParticles(maxPart)
{
	particleArray = new Particle[maxParticles];
	particlePositionArray = new float[4 * maxParticles];
	particleColorArray = new float[4 * maxParticles];
	randomParticle = randomParticleFunc;
}

void ParticleContainer::InitializeVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &baseVbo);
	glBindBuffer(GL_ARRAY_BUFFER, baseVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glBindVertexArray(0);
}

void ParticleContainer::UpdateBuffers()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(float) * 4, particlePositionArray);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(float) * 4, particleColorArray);
}
void ParticleContainer::Draw(glm::mat4 view, glm::mat4 proj)
{
	glm::mat4 model(1.0);
	shader->use();
	shader->setMat4("projection", proj);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);
}
void ParticleContainer::UpdateTimestep(glm::vec3 cameraPosition, float dt)
{
	SortParticles();
	glm::vec3 GRAVITY_VECTOR(0, -9.81, 0);
	GRAVITY_VECTOR = GRAVITY_VECTOR * .2f;
	unsigned int numParticlesToDraw = 0;
	for (int i = 0; i < maxParticles; ++i) {
		// create reference for easy reading
		Particle & particle = particleArray[i];
		if (particle.lifeSpan > 0) {
			// update direction then update position
			particle.lifeSpan -= dt;
			particle.velocity = particle.velocity + GRAVITY_VECTOR * dt;
			CheckCollisions(particle);
			particle.position = particle.position + particle.velocity * dt;

			particle.cameraDistance = glm::length(particle.position - cameraPosition);
			// update position array
			particlePositionArray[4 * numParticlesToDraw + 0] = particle.position.x;
			particlePositionArray[4 * numParticlesToDraw + 1] = particle.position.y;
			particlePositionArray[4 * numParticlesToDraw + 2] = particle.position.z;
			particlePositionArray[4 * numParticlesToDraw + 3] = particle.size;

			particleColorArray[4 * numParticlesToDraw + 0] = particle.color.r;
			particleColorArray[4 * numParticlesToDraw + 1] = particle.color.g;
			particleColorArray[4 * numParticlesToDraw + 2] = particle.color.b;
			particleColorArray[4 * numParticlesToDraw + 3] = particle.color.a;
			// check for collisions
			// increment numParticles
			++numParticlesToDraw;
		}
		else {
			particle.cameraDistance = -1.0f;
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

void ParticleContainer::SortParticles()
{
	std::sort(&particleArray[0], &particleArray[maxParticles]);
}

int ParticleContainer::FindUnusedParticle()
{
	for (int i = lastUsedParticle; i < maxParticles; ++i) {
		if (particleArray[i].lifeSpan <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}
	for (int i = 0; i < lastUsedParticle; ++i) {
		if (particleArray[i].lifeSpan <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}
	return 0;
}

void ParticleContainer::CheckCollisions(Particle & particle)
{
	for (CollisionPlane & collidable : collidables) {
		if (collidable.isColliding(particle)) {
			particle.velocity = particle.velocity - collidable.GetNormal() * 2.0f * glm::dot(particle.velocity, collidable.GetNormal());
			//particle.velocity = particle.velocity.Scale(particle.elasticity);
			particle.velocity = particle.velocity * particle.elasticity;
		}
	}
}

float ParticleContainer::quadVerts[12] = {
		-.5, -.5, 0,
		.5, -.5, 0,
		-.5, .5, 0,
		.5, .5, 0
};