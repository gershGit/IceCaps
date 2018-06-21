#version 330 core

out vec4 fragColor;

in vec2 ourTexCoord;
in vec3 ourNormal;
in vec3 ourFragPos;

uniform sampler2D diffuseSampler;
uniform vec3 eyePos;
uniform vec3 sunPos;

float ambientStrength = 0.2;
float specularStrength = 0.4;
float shininess = 128.0;
vec3 lightColor = vec3(0.4, 0.4, 0.4);

void main() {
	vec4 ourColorFour = texture(diffuseSampler, ourTexCoord);
	vec3 ourColor = vec3(ourColorFour.x, ourColorFour.y, ourColorFour.z);

	vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(ourNormal);
    vec3 lightDir = normalize(sunPos - ourFragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	//Blinn phong
	vec3 viewDir = normalize(-ourFragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float specAngle = max(dot(halfDir, normal), 0.0);
	vec3 specular = pow(specAngle, shininess/4.0) * specularStrength * lightColor;

    vec3 result = (ambient+diffuse+specular)*ourColor;
    fragColor = vec4(result, 1.0f);
}