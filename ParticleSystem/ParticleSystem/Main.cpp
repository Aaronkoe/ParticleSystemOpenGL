//std
#include <iostream>

//project
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//personal
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <learnopengl/shader.h>
#include "CollisionPlane.h"
#include "Particle.h"
#include "ParticleContainer.h"
#include "Quad.h"

GLFWwindow * InitializeWindow();
unsigned int GenerateTexture();

const unsigned int MAX_PARTICLES = 1000;

int main() {
	GLFWwindow * window = InitializeWindow();
	Shader shader("pVertex.fs", "Fragment.fs");
	unsigned int texture = GenerateTexture();
	float verts[] = {
		.5, -.5, 0,
		-.5, -.5, 0,
		0, .5, 0
	};
	unsigned int testVao, testVbo;
	glGenVertexArrays(1, &testVao);
	glGenBuffers(1, &testVbo);
	glBindVertexArray(testVao);
	glBindBuffer(GL_ARRAY_BUFFER, testVbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// create VAO and VBO for particles
	unsigned int particleVao, positionVbo;
	glGenVertexArrays(1, &particleVao);
	glGenBuffers(1, &positionVbo);
	glBindVertexArray(particleVao);
	glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
								//max particles     num per position
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle) * 3, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	ParticleContainer particleContainer(100);
	Particle circle({ .5, .5, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, 1000);
	particleContainer.AddParticle(circle);
	particleContainer.UpdateTimestep(.0001);
	while (!glfwWindowShouldClose(window)) {
		particleContainer.UpdateTimestep(.001);
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// update buffers
		glBindVertexArray(particleVao);
		glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle) * 3, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleContainer.GetNumParticles() * sizeof(float) * 9, particleContainer.GetPositionArray());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		shader.use();
		glDrawArrays(GL_TRIANGLES, 0, particleContainer.GetNumParticles() * 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

GLFWwindow * InitializeWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow * window = glfwCreateWindow(800, 800, "window", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

unsigned int GenerateTexture()
{
	int width, height, nrChannels;
	unsigned char *data = stbi_load("pixelCircle.png", &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return texture;
}
