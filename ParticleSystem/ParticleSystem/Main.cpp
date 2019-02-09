//std
#include <iostream>

//project
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//personal
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include "CollisionPlane.h"
#include "Particle.h"
#include "ParticleContainer.h"
#include "Quad.h"
#include "CollisionCube.h"

bool pause = true;

const int SCR_WIDTH = 800, SCR_HEIGHT = 800;
const unsigned int MAX_PARTICLES = 120000;

// function declarations
void ProcessInput(GLFWwindow * window);
GLFWwindow * InitializeWindow(int w, int h);
unsigned int GenerateTexture();
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
Particle RandomParticle(double timeElapsed);

// camera global variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), { 0, 1, 0 }, 270, 0);
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

int main() {
	// set up stuff
	camera.MovementSpeed = .1;
	GLFWwindow * window = InitializeWindow(SCR_WIDTH, SCR_HEIGHT);
	Shader shader("pvmCenterPositionVertex.fs", "centerPositionFragment.fs");
	Shader quadShader("quadVertex.fs", "colorFragment.fs");
	GLenum err;
	Quad::SetShader(&quadShader);
	unsigned int texture = GenerateTexture();
	float quad[] = {
		-.5, -.5, 0,
		.5, -.5, 0,
		-.5, .5, 0,
		.5, .5, 0
	};
	// enabling blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
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
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "error in initialize quad vao" << err << std::endl;
	}

	ParticleContainer particleContainer(MAX_PARTICLES, RandomParticle);
	double startTime, elapsedTime;
	double timeSinceLastBall = 0;
	int sphereSize;
	startTime = glfwGetTime();
	glm::mat4 proj(1.0);
	glm::mat4 view(1.0);
	glm::mat4 model(1.0);
	particleContainer.SetShader(&shader);
	particleContainer.InitializeVAO();
	while (!glfwWindowShouldClose(window)) {
		glClearColor(.1f, .1f, .1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		if (!pause) {
			elapsedTime = glfwGetTime() - startTime;
		}
		else {
			elapsedTime = 0;
		}
		startTime = glfwGetTime();
		if (!pause) {
			timeSinceLastBall += elapsedTime;
			while (timeSinceLastBall > .00002) {
				timeSinceLastBall -= .00002;
				particleContainer.AddParticle(0);
			}
		}
		std::cout << "elapsed: " << elapsedTime << " numParticles: " << particleContainer.GetNumParticles() << std::endl;
		particleContainer.UpdateTimestep(camera.Position, elapsedTime);
		particleContainer.UpdateBuffers();
		particleContainer.Draw(view, proj);
		glfwSwapBuffers(window);
		glfwPollEvents();
		ProcessInput(window);
	}
	return 0;
}
bool pNotHeld = false;
void ProcessInput(GLFWwindow * window)
{
	float deltaTime = .1;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && pNotHeld) {
		pause = !pause; pNotHeld = false;
	}
	if (!pNotHeld && glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
		pNotHeld = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		std::cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << std::endl;
	}
}

GLFWwindow * InitializeWindow(int w, int h) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow * window = glfwCreateWindow(w, h, "window", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

#define PI 3.14159f
Particle RandomParticle(double timeElapsed)
{
	float xVelocity = (float)rand() / (RAND_MAX / 5);
	float yVelocity = (float)rand() / (RAND_MAX / 5);
	float zVelocity = (float)rand() / (RAND_MAX / 5);
	if (rand() % 2 == 0) xVelocity *= -1;
	if (rand() % 2 == 0) zVelocity *= -1;
	float rVal = float(rand()) / RAND_MAX;
	float gVal = float(rand()) / RAND_MAX;
	float bVal = float(rand()) / RAND_MAX;
	float aVal = float(rand()) / RAND_MAX;
	return Particle({ 0, .2, 0 } , { xVelocity, yVelocity, zVelocity }, { rVal, gVal, bVal, aVal }, 2.1, 0, .01);
}

// Handle moving camera with mouse movement from learnopengl.com
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
