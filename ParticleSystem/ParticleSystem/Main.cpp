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

GLFWwindow * InitializeWindow();

int main() {
	GLFWwindow * window = InitializeWindow();
	Shader shader("pctVertex.fs", "caFragment.fs");
	float verts[] = {
		-.5f, -.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		.5f, -.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		.5f, .5f, 0.0f, .5f, .5f, .5f, 1.0f, 1.0f,
		-.5f, -.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-.5f, .5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, .5f, 0.0f, .5f, .5f, 0.0f, 1.0f, 1.0f,
	};
	// VAO AND VBO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	//TEXTURE LOADING
	int width, height, nrChannels;
	unsigned char *data = stbi_load("squareAlphaTexture.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
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
	GLFWwindow * window = glfwCreateWindow(800, 600, "window", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}