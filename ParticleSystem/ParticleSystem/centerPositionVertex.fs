#version 330 core
layout (location = 0) in vec3 baseMesh;
layout (location = 1) in vec4 centerPosition;

out vec2 TexCoord;

void main() {
	vec3 scaled = baseMesh * centerPosition.w;
	vec3 trans = scaled + centerPosition.xyz;
	gl_Position = vec4(trans.xyz, 1.0);
	TexCoord = vec2(.5, .5) + baseMesh.xy;
}