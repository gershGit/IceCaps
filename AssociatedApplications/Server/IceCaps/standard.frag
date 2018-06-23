#version 330 core
#define NR_POINT_LIGHTS 4
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

uniform vec3 sunAngle;			//x,y,z, strength
uniform vec4 sunColor;
uniform vec3 pointLightPos[NR_POINT_LIGHTS];	//x,y,z, strength
uniform vec4 pointLightColors[NR_POINT_LIGHTS];

float ambientStrength = 0.2;
float shininess = 128.0;
vec3 lightColor = vec3(0.4, 0.4, 0.4);

vec3 CalcSunlight(vec3 normal, vec3 halfDir){
	vec3 lightDir = normalize(-sunAngle);

	//Diffuse sun shading
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular sun shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(halfDir, reflectDir), 0.0), shininess);

	//multiply by textures
	vec3 ambient = ambientStrength * vec3(texture(diffuseSampler, ourTexCoord));
	vec3 diffuse = diff * sunColor.w * vec3(texture(diffuseSampler, ourTexCoord));
	vec3 specular = sunColor.w * sunColor.xyz * spec * vec3(texture(specularSampler, ourTexCoord));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(int index, vec3 normal, vec3 halfDir){
	vec3 lightDir = normalize(pointLightPos[index] - ourFragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(halfDir, reflectDir), 0.0), shininess);

	float distance = length(pointLightPos[index] - ourFragPos);
	float attenuation = 1.0/(1.0 + (1/(pointLightColors[index].w * 14)) * distance + (1/(pointLightColors[index].w * .32)) * (distance * distance));

	//combination
	vec3 ambient = ambientStrength * vec3(texture(diffuseSampler, ourTexCoord)) * attenuation;
	vec3 diffuse = diff * vec3(texture(diffuseSampler, ourTexCoord)) * attenuation;
	vec3 specular = spec * pointLightColors[index].xyz * vec3(texture(specularSampler, ourTexCoord)) * attenuation;
	return (ambient + diffuse + specular);
}

void main() {
	mat3 TBN = mat3(ourTangent, ourBitangent, ourNormal);

	vec4 ourColorFour = texture(diffuseSampler, ourTexCoord);
	vec3 ourColor = vec3(ourColorFour.x, ourColorFour.y, ourColorFour.z);

	vec3 specularStrength = vec3(texture(specularSampler, ourTexCoord).x, texture(specularSampler, ourTexCoord).y, texture(specularSampler, ourTexCoord).z);

	vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize (texture(normalSampler, ourTexCoord).xyz*2.0 - 1.0);

    vec3 lightDir = normalize(-sunAngle);
	lightDir *= TBN;
	lightDir = normalize(lightDir);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	//Blinn phong
	vec3 viewDir = normalize(eyePos - ourFragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float specAngle = max(dot(halfDir, normal), 0.0);
	vec3 specular = pow(specAngle, shininess/4.0) * specularStrength * lightColor;

	vec3 result = CalcSunlight(normal, halfDir);
	for (int i=0; i< NR_POINT_LIGHTS; i++){
		result+= CalcPointLight(i, normal, halfDir);
	}

	fragColor = vec4(result, 1.0);
}