#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <learnopengl/shader.h>

class Cube {
public:
	void Draw(glm::mat4 view, glm::mat4 proj);
	void InitializeVao();
	glm::vec3 translation{ 0, 0, 0 };
	glm::vec3 rotation{ 0, 0, 0 };
	glm::vec3 scaling{ 1, 1, 1 };
	static bool initialized;
	static unsigned int vao;
	static float vertices[180];
	static Shader * shader;
};
