#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color; // Varying
uniform float uTime;

void main() 
{
	Color = aColor; // Pass-through
	float xPos = aPos.x + sin(uTime) / 5;
	float yPos = aPos.y * sin(uTime);
	gl_Position = vec4(xPos, yPos, aPos.z, 1.0f);
}