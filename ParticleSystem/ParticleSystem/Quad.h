#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "learnopengl/shader.h"

class Quad {
public:
	void Draw(glm::mat4 view, glm::mat4 proj);
	void SetRotation(glm::vec3 v) { rotation = v; }
	void SetTranslation(glm::vec3 v) { translation = v; }
	void SetScale(glm::vec3 v) { scale = v; }
	static void SetShader(Shader * s) { shader = s; }
	static void SetDefaultTexture(GLuint t) { defaultTexture = t; }
	void SetTexture(int t) { texture = t; }

	glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
private:
	static Shader * shader;
	static GLuint VAO;
	static const GLfloat vertices[18];
	static const GLfloat normals[18];
	static GLuint defaultTexture;
	static bool initialized;

	int texture{ -1 };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

	void InitializeVao();
};