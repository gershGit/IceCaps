#version 330 core
#define NR_POINT_LIGHTS 4

in vec3 ourColor;
in vec3 ourNormal;
in vec3 ourFragPos;

out vec4 fragColor;

uniform vec3 eyeDir;
uniform vec3 sunAngle;
uniform vec3 pointLightPos[NR_POINT_LIGHTS];	//x,y,z
uniform vec3 pointLightColors[NR_POINT_LIGHTS];

float ambientStrength = 0.15;
float specularStrength = 0.7;
float shininess = 128.0;
vec3 lightColor = vec3(0.75, 0.7, 0.7);


vec3 fresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0-F0) * pow(1.0 - cosTheta, 5.0);
}

float getAttenuation(vec3 objectPos, vec3 lightPos){
	float d = length(lightPos - objectPos);
	return 1.0/(d*d);
}

vec3 getPointLighting(vec3 V, vec3 N, int index){
	vec3 L = normalize(pointLightPos[index] - ourFragPos);
	vec3 H = normalize(L + V);

	float attenuation = getAttenuation(ourFragPos, pointLightPos[index]);
	vec3 diffuse = max(dot(N, L), 0.0) * pointLightColors[index] * attenuation;
	vec3 specular = pow(max(dot(H, N), 0.0),shininess/4.0) * specularStrength * pointLightColors[index] * attenuation;

	return (diffuse + specular) * ourColor;
}

vec3 getSunLighting(vec3 H, vec3 N, vec3 L){
	vec3 ambient = ambientStrength * lightColor;
	vec3 diffuse = max(dot(N, L), 0.0) * lightColor;
	vec3 specular = pow(max(dot(H, N), 0.0),shininess/4.0) * specularStrength * lightColor;

	return (ambient + diffuse + specular) * ourColor;
}

void main() {
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(ourNormal);
    vec3 lightDir = normalize(-sunAngle);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	//Blinn phong
	//vec3 viewDir = normalize(eyePos-ourFragPos);
	vec3 viewDir = eyeDir;
	vec3 halfDir = normalize(lightDir + viewDir);
	float specAngle = max(dot(halfDir, normal), 0.0);
	vec3 specular = pow(specAngle, shininess/4.0) * specularStrength * lightColor;

    vec3 result = getSunLighting(halfDir, normal, lightDir);
	for (int i =0; i<NR_POINT_LIGHTS; i++){
		result += getPointLighting(viewDir, normal, i);
	}

    fragColor = vec4(result, 1.0f);
}