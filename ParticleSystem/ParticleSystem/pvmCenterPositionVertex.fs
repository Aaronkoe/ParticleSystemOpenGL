#version 330 core

layout (location = 0) in vec3 baseMesh;
layout (location = 1) in vec4 centerPositionAndSize;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;

void main() {
	vec3 scaled = baseMesh * centerPositionAndSize.w;
	vec3 trans = scaled + centerPositionAndSize.xyz;
	gl_Position = projection * view * model * vec4(trans.xyz, 1.0);
	TexCoord = vec2(.5, .5) + baseMesh.xy;
}