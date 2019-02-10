#include "FireWork.h"

FireWork::FireWork(float propRate, unsigned int max1, unsigned int max2, Particle(*prop)(glm::vec3), Particle(*expl)(glm::vec3), Shader * propShader, Shader * explShader)
	:
	propellantRate(propRate),
	propellant(max1, prop),
	explosion(max1, expl),
	position(0, 0, 0),
	velocity(0, 1, 0),
	acceleration(0, .2, 0)
{
	propellant.SetShader(propShader);
	explosion.SetShader(explShader);
	propellant.InitializeVAO();
	explosion.InitializeVAO();
	toDraw.translation = position;
	toDraw.scaling = glm::vec3(.02, .1, .02);
	timeToExplode = 3;
	explosionRate = .00002;
	timeSinceExplosion = 0;
}

void FireWork::Draw(glm::mat4 view, glm::mat4 proj)
{
	if (timeToExplode > 0) {
		toDraw.Draw(view, proj);
		return;
	}
}

void FireWork::Update(glm::vec3 camPos, float dt)
{
	timeToExplode -= dt;
	toDraw.translation = position;
	propellant.UpdateTimestep(camPos, dt);
	explosion.UpdateTimestepSpecial(camPos, dt);
	if (timeToExplode > 0) {
		timeSinceProp += dt;
		while (propellantRate < timeSinceProp) {
			timeSinceProp -= propellantRate;
			propellant.AddParticle(position);
		}
		position += velocity * dt;
		velocity += acceleration * dt;
	}
	else if (timeToExplode > -.3) {
		timeSinceExplosion += dt;
		while (explosionRate < timeSinceExplosion) {
			timeSinceExplosion -= explosionRate;
			explosion.AddParticle(position);
		}
		explosion.AddParticle(position);
	}
}

void FireWork::DrawParticles(glm::mat4 view, glm::mat4 proj)
{
	propellant.UpdateBuffers();
	propellant.Draw(view, proj);
	explosion.UpdateBuffers();
	explosion.Draw(view, proj);
}
