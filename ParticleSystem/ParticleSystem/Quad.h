#pragma once
#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Quad {
public:
	Quad(float p1, float p2, float p3, float p4);
	unsigned int VAO, VBO;
	float verts[48] = {
	-.5f, -.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	.5f, -.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	.5f, .5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-.5f, -.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-.5f, .5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, .5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};
};