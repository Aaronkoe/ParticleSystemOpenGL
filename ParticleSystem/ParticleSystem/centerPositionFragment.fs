#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec4 vertexColor;
uniform sampler2D alphaTexture;

void main() {
	FragColor = vec4(vertexColor.xyz, vertexColor * texture(alphaTexture, TexCoord).r);
}