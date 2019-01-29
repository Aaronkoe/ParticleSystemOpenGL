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

void ProcessInput(GLFWwindow * window);
GLFWwindow * InitializeWindow();
unsigned int GenerateTexture();

const unsigned int MAX_PARTICLES = 1000;

Particle RandomParticle(double timeElapsed);

int main() {
	GLFWwindow * window = InitializeWindow();
	Shader shader("centerPositionVertex.fs", "centerPositionFragment.fs");
	unsigned int texture = GenerateTexture();
	float quad[] = {
		-.5, -.5, 0,
		.5, -.5, 0,
		-.5, .5, 0,
		.5, .5, 0
	};
	// test VAO AND VBO
	unsigned int testVao, testVbo;
	glGenVertexArrays(1, &testVao);
	glGenBuffers(1, &testVbo);
	glBindVertexArray(testVao);
	glBindBuffer(GL_ARRAY_BUFFER, testVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// enabling blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// create VAO and VBO for particles
	unsigned int particleVao, positionVbo;
	glGenVertexArrays(1, &particleVao);
	glGenBuffers(1, &positionVbo);
	glBindVertexArray(particleVao);
	glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
								//max particles     num elements per position
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle) * 4, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// base mesh vbo
	unsigned int baseMeshVbo;
	glGenBuffers(1, &baseMeshVbo);
	glBindBuffer(GL_ARRAY_BUFFER, baseMeshVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glBindVertexArray(0);
	ParticleContainer particleContainer(100, RandomParticle);
	particleContainer.AddParticle(1);
	particleContainer.AddParticle(1);
	CollisionPlane floor(0.0f, -1.f, 0.0f, 0.0f, 1.0f, 0.0f);
	particleContainer.AddCollidable(floor);
	CollisionPlane rightWall(1.0f, -0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	particleContainer.AddCollidable(rightWall);
	CollisionPlane leftWall(-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	particleContainer.AddCollidable(leftWall);
	double startTime, elapsedTime;
	double timeSinceLastBall = 0;
	startTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		elapsedTime = glfwGetTime() - startTime;
		startTime = glfwGetTime();
		timeSinceLastBall += elapsedTime;
		if (timeSinceLastBall > .5) {
			particleContainer.AddParticle(1);
			timeSinceLastBall -= .5;
		}
		std::cout << "elapsed: " << elapsedTime << std::endl;
		particleContainer.UpdateTimestep(elapsedTime);
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// update buffers
		glBindVertexArray(particleVao);
		glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle) * 4, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleContainer.GetNumParticles() * sizeof(float) * 4, particleContainer.GetPositionArray());
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		shader.use();
		//glDrawArrays(GL_TRIANGLES, 0, particleContainer.GetNumParticles() * 3);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleContainer.GetNumParticles());
		glfwSwapBuffers(window);
		glfwPollEvents();
		ProcessInput(window);
	}
	return 0;
}

void ProcessInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
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

Particle RandomParticle(double timeElapsed)
{
	float xVelocity = (float)rand() / RAND_MAX;
	return Particle({ 0, .5, 0 }, { xVelocity, 1, 0 }, { 0, 0, 0 }, 100);
}
