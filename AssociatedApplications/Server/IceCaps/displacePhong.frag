#version 330 core

out vec4 fragColor;

in vec2 ourTexCoord;
in vec3 ourNormal;
in vec3 ourFragPos;

in vec3 ourTangent;
in vec3 ourBitangent;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;
uniform sampler2D normalSampler;
uniform vec3 eyePos;
uniform vec3 sunPos;

float ambientStrength = 0.2;
float shininess = 128.0;
vec3 lightColor = vec3(0.4, 0.4, 0.4);

void main() {
	//mat3 TBN = mat3(ourTangent, ourBitangent, ourNormal);

	vec4 ourColorFour = texture(diffuseSampler, ourTexCoord);
	vec3 ourColor = vec3(ourColorFour.x, ourColorFour.y, ourColorFour.z);

	vec3 specularStrength = vec3(texture(specularSampler, ourTexCoord).x, texture(specularSampler, ourTexCoord).y, texture(specularSampler, ourTexCoord).z);

	vec3 ambient = ambientStrength * lightColor;

	//vec4 ourSampledNormal = texture(normalSampler, ourTexCoord);
    //vec3 normal = normalize(ourNormal + vec3(ourSampledNormal.x, ourSampledNormal.y, ourSampledNormal.z));
	vec3 normal = normalize (texture(normalSampler, ourTexCoord).xyz*2.0 - 1.0);
	//vec3 normal = normalize(ourNormal);
    vec3 lightDir = normalize(sunPos - ourFragPos);
	//lightDir *= TBN;
	//lightDir = normalize(lightDir);

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