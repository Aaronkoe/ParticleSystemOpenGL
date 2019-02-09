#include "Quad.h"
// static member initialization
Shader * Quad::shader = NULL;
bool Quad::initialized = false;
GLuint Quad::VAO = 0;
GLuint Quad::defaultTexture = -1;


Quad::Quad(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4) :
	corner1(c1), corner2(c2), corner3(c3), corner4(c4)
{
	usesCorners = true;
}


void Quad::Draw(glm::mat4 view, glm::mat4 proj)
{
	GLenum err;
	if (usesCorners) {
		DrawWithCorners(view, proj);
		return;
	}
	if (initialized)
		glBindVertexArray(VAO);
	else
		InitializeVao();
	glm::mat4 model(1.0);
	glm::mat4 m(1.0);
	glm::mat4 m1(1.0);
	glm::mat4 m2(1.0);
	model = glm::translate(model, translation);
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);
	shader->use();
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", proj);
	if (texture != -1) 
		glBindTexture(GL_TEXTURE_2D, texture);
	else if (defaultTexture != -1)
		glBindTexture(GL_TEXTURE_2D, defaultTexture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "Error in cube.draw: " << err << std::endl;
	}
}

void Quad::DrawWithCorners(glm::mat4 view, glm::mat4 proj)
{
	GLenum err;
	if (!privateInit) {
		PrivateInit();
	}
	glBindVertexArray(privateVao);
	glm::mat4 model(1.0);
	shader->use();
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", proj);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "Error in cube.draw: " << err << std::endl;
	}
}

void Quad::InitializeVao()
{
	GLenum err;
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	initialized = true;
	while ((err = glGetError()) != GL_NO_ERROR) {

		std::cout << "error in initialize quad vao" << err << std::endl;
		initialized = false;
	}
}

void Quad::PrivateInit()
{
	float vertices[12]{
		corner2.x, corner2.y, corner2.z,
		corner1.x, corner1.y, corner1.z,
		corner3.x, corner3.y, corner3.z,
		corner4.x, corner4.y, corner4.z
	};
	glGenVertexArrays(1, &privateVao);
	glGenBuffers(1, &privateVbo);
	glBindVertexArray(privateVao);
	glBindBuffer(GL_ARRAY_BUFFER, privateVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	privateInit = true;
}

const GLfloat Quad::vertices[18] = {
	//   X      Y      Z     U     V
	 -0.5f, -0.5f, 0.f,
	  0.5f, -0.5f, 0.f,
	  0.5f,  0.5f, 0.f,
	  0.5f,  0.5f, 0.f,
	 -0.5f,  0.5f, 0.f,
	 -0.5f, -0.5f, 0.f
};

const GLfloat Quad::normals[18] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};