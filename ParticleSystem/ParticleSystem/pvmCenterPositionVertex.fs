#version 330 core

layout (location = 0) in vec3 baseMesh;
layout (location = 1) in vec4 centerPositionAndSize;
layout (location = 2) in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec4 vertexColor;

void main() {
	vec3 CameraRight_worldspace = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 CameraUp_worldspace = vec3(view[0][1], view[1][1], view[2][1]);
	vec3 worldSpace = centerPositionAndSize.xyz + CameraRight_worldspace * baseMesh.x * centerPositionAndSize.w
		+ CameraUp_worldspace * baseMesh.y * centerPositionAndSize.w;
	mat4 modelView = view * model;
	// set rotation part of the view * model to identity, only care about translation
	//modelView[0][0] = 1;
	//modelView[0][1] = 0;
	//modelView[0][2] = 0;
	//
	//modelView[1][0] = 0;
	//modelView[1][1] = 1;
	//modelView[1][2] = 0;
	//
	//modelView[2][0] = 0;
	//modelView[2][1] = 0;
	//modelView[2][2] = 1;
	gl_Position = projection * modelView * vec4(worldSpace.xyz, 1.0);
	vertexColor = color;
	TexCoord = vec2(.5, .5) + baseMesh.xy;
}