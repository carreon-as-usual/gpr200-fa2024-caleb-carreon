#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;

uniform sampler2D ourTexture;
uniform float uTime;

void main()
{
	vec2 modTexCoords = vec2(TexCoords.x + uTime * 0.1, TexCoords.y + sin(TexCoords.x * 5) * 0.1);
	vec4 texColor = texture(ourTexture, modTexCoords);
	if(texColor.a < 0.1)
		discard;
	FragColor = texColor;
}