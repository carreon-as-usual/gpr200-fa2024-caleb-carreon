#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 Color; // Varying
out vec2 TexCoords;

uniform float uTime;

void main() 
{
	Color = aColor; // Pass-through
	TexCoords = aTexCoord;
	float xPos = aPos.x + sin(uTime) / 2;
	float yPos = aPos.y + abs(cos(uTime)) / 2;
	gl_Position = vec4(xPos, yPos, aPos.z, 1.0);
}