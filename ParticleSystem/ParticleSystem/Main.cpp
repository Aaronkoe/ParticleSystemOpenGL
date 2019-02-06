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

const int SCR_WIDTH = 800, SCR_HEIGHT = 800;

void ProcessInput(GLFWwindow * window);
GLFWwindow * InitializeWindow(int w, int h);
unsigned int GenerateTexture();
unsigned int GenerateSphereVao(float radius, int resolution, int & sphereVertSize);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
const unsigned int MAX_PARTICLES = 100000;

Particle RandomParticle(double timeElapsed);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), { 0, 1, 0 }, 270, 0);
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

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


int main() {
	camera.MovementSpeed = .1;
	GLFWwindow * window = InitializeWindow(SCR_WIDTH, SCR_HEIGHT);
	Shader shader("pvmCenterPositionVertex.fs", "centerPositionFragment.fs");
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
	//glBindVertexArray(testVao);
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
	ParticleContainer particleContainer(MAX_PARTICLES, RandomParticle);
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
	int sphereSize;
	unsigned int SphereVao = GenerateSphereVao(1, 10, sphereSize);
	startTime = glfwGetTime();
	glm::mat4 proj(1.0);
	glm::mat4 view(1.0);
	glm::mat4 model(1.0);
	while (!glfwWindowShouldClose(window)) {
		shader.use();
		proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		elapsedTime = glfwGetTime() - startTime;
		startTime = glfwGetTime();
		timeSinceLastBall += elapsedTime;
		while (timeSinceLastBall > .5) {
			timeSinceLastBall -= .5;
			particleContainer.AddParticle(0);
		}
		shader.setMat4("projection", proj);
		shader.setMat4("view", view);
		shader.setMat4("model", model);

		//std::cout << "elapsed: " << elapsedTime << " numParticles: " << particleContainer.GetNumParticles() << std::endl;
		particleContainer.UpdateTimestep(elapsedTime);
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// update buffers
		glBindVertexArray(particleVao);
		glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(float) * 4, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleContainer.GetNumParticles() * sizeof(float) * 4, particleContainer.GetPositionArray());
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		//glBindVertexArray(SphereVao);
		//glDrawArrays(GL_TRIANGLES, 0, sphereSize);
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
	float deltaTime = .1;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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

unsigned int GenerateSphereVao(float Radius, int Resolution, int & sphereVertSize)
{
	double PI = 3.14159;
	std::vector<float> v;
	// iniatiate the variable we are going to use
	float X1, Y1, X2, Y2, Z1, Z2;
	float inc1, inc2, inc3, inc4, inc5, Radius1, Radius2;

	for (int w = 0; w < Resolution; w++) {
		for (int h = (-Resolution / 2); h < (Resolution / 2); h++) {


			inc1 = (w / (float)Resolution) * 2 * PI;
			inc2 = ((w + 1) / (float)Resolution) * 2 * PI;

			inc3 = (h / (float)Resolution)*PI;
			inc4 = ((h + 1) / (float)Resolution)*PI;


			X1 = sin(inc1);
			Y1 = cos(inc1);
			X2 = sin(inc2);
			Y2 = cos(inc2);

			// store the upper and lower radius, remember everything is going to be drawn as triangles
			Radius1 = Radius * cos(inc3);
			Radius2 = Radius * cos(inc4);




			Z1 = Radius * sin(inc3);
			Z2 = Radius * sin(inc4);

			// insert the triangle coordinates
			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1).x);
			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1).y);
			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1).z);
			v.push_back(glm::vec3(Radius1*X2, Z1, Radius1*Y2).x);
			v.push_back(glm::vec3(Radius1*X2, Z1, Radius1*Y2).y);
			v.push_back(glm::vec3(Radius1*X2, Z1, Radius1*Y2).z);
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2).x);
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2).y);
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2).z);
			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1).x);
			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1).y);
			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1).z);
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2).x);
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2).y);
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2).z);
			v.push_back(glm::vec3(Radius2*X1, Z2, Radius2*Y1).x);
			v.push_back(glm::vec3(Radius2*X1, Z2, Radius2*Y1).y);
			v.push_back(glm::vec3(Radius2*X1, Z2, Radius2*Y1).z);
		}
	}
	unsigned int indexVbo, sphereVao;
	glGenBuffers(1, &indexVbo);
	glGenVertexArrays(1, &sphereVao);
	glBindVertexArray(sphereVao);
	glBindBuffer(GL_ARRAY_BUFFER, indexVbo);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), &v[0], GL_STATIC_DRAW); // this might not work
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	sphereVertSize = v.size();
	return 0;
}

Particle RandomParticle(double timeElapsed)
{
	float xVelocity = (float)rand() / RAND_MAX;
	float zVelocity = -(float)rand() / RAND_MAX;
	if (rand() % 2 == 1) { xVelocity = -xVelocity; }
	return Particle({ 0, .5, 0 }, { xVelocity, 1, zVelocity }, { 0, 0, 0 }, 100);
}
