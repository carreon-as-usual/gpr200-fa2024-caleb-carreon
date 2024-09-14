#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color; // Varying
uniform float uTime;
void main() 
{
	Color = aColor; // Pass-through
	float yPos = aPos.y * sin(uTime);
	gl_Position = vec4(aPos.x, yPos, aPos.z, 1.0f);
}