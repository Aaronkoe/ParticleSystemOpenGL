#pragma once
#include <glm/glm.hpp>
#include "Collidable.h"
#include "Drawable.h"
#include <learnopengl/shader.h>
#include "Quad.h"
class CollisionCube : public Collidable, public Drawable {
public:
	CollisionCube();

	virtual bool DetectCollision(const Particle & particle) override { return false; };
	virtual void ReactToCollision(Particle & particle) override { } ;
	virtual void Draw(glm::mat4 view, glm::mat4 proj) override;
private:
	static unsigned int VAO;
	static float vertices[180];
};