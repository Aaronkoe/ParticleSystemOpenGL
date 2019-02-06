#pragma once
#include <glm/glm.hpp>
class Drawable {
public:
	virtual void Draw(glm::mat4 view, glm::mat4 projection) = 0;
};