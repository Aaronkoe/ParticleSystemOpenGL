#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "learnopengl/shader.h"

class Quad {
public:
	Quad() = default;
	Quad(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);
	void Draw(glm::mat4 view, glm::mat4 proj);
	void SetRotation(glm::vec3 v) { rotation = v; }
	void SetTranslation(glm::vec3 v) { translation = v; }
	void SetScale(glm::vec3 v) { scale = v; }
	static void SetShader(Shader * s) { shader = s; }
	static void SetDefaultTexture(GLuint t) { defaultTexture = t; }
	void SetTexture(int t) { texture = t; }
	void SetColor(glm::vec3 c) { color = c; }

	glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
private:
	static Shader * shader;
	static GLuint VAO;
	static const GLfloat vertices[18];
	static const GLfloat normals[18];
	static GLuint defaultTexture;
	static bool initialized;
	bool usesCorners = false;
	unsigned int privateVao, privateVbo;
	bool privateInit = false;

	int texture{ -1 };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 color{ 1, 1, 1 };
	glm::vec3 corner1, corner2, corner3, corner4;

	void DrawWithCorners(glm::mat4 view, glm::mat4 proj);
	void InitializeVao();
	void PrivateInit();
};