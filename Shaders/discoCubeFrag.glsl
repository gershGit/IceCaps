#version 330 core

in vec3 ourColor;
in vec3 ourNormal;
in vec3 ourFragPos;

out vec4 fragColor;

uniform vec3 eye;
uniform vec3 lightPos;

float ambientStrength = 0.2;
float specularStrength = 0.5;
vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main() {
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(ourNormal);
    vec3 lightDir = normalize(lightPos - ourFragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(eye - ourFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = diffuse*specularStrength * spec * lightColor;

    vec3 result = (ambient+diffuse+specular)*ourColor;
    fragColor = vec4(result, 1.0f);
}
