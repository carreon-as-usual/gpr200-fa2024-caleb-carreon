#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 Color; // Varying
out vec2 TexCoord;

uniform float uTime;
uniform mat4 transform;

void main() 
{
	Color = aColor; // Pass-through
	TexCoord = aTexCoord;
	gl_Position = transform * vec4(aPos, 1.0);
}