#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 vertColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertColor = vec3(0, 0, 0);
}