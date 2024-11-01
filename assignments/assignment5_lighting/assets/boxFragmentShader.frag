#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float uTime;
uniform float ambientStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool override;

float specularStrength = 0.5;
int shininess = 32;

void main()
{
    if(override) {
        FragColor = vec4(lightColor, 0.0);
    } else {
        // Direction
        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        // Mix textures into objectColor
        vec3 objectColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2).rgb;

        // Ambient lighting
        vec3 ambient = ambientStrength * lightColor;

        // Normal and light direction
        vec3 norm = normalize(Normal);

        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // View position
        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;

        // Combine lighting effects
        vec3 result = (ambient + diffuse + specular) * objectColor;
        FragColor = vec4(result, 1.0);
    }
}