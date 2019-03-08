#version 330 core
#define NR_POINT_LIGHTS 4

out vec4 fragColor;

in vec3 ourColor;
in vec3 ourNormal;
in vec3 ourFragPos;
in float depth;

uniform float fogAmount;
uniform vec3 eyePos;
uniform vec3 sunAngle;
uniform vec3 pointLightPos[NR_POINT_LIGHTS];
uniform vec3 pointLightColors[NR_POINT_LIGHTS];

float ambientStrength = 0.05;
float shininess = 128.0;
vec3 lightColor = vec3(0.4, 0.4, 0.4);
float specularStrength = 0.5;

void main() {
	vec3 lightDir = normalize(-sunAngle);
	//Blinn phong
	vec3 ambient = ambientStrength * ourColor;

    float diff = max(dot(ourNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(eyePos-ourFragPos);
	vec3 halfDir = normalize(lightDir + viewDir);  
    float specAngle = pow(max(dot(ourNormal, halfDir), 0.0), 1.0);
	//vec3 specular = lightColor * specAngle;
	vec3 specular = vec3(0);

	for (int i =0; i<NR_POINT_LIGHTS; i++){
		float lightDist = length(pointLightPos[i] - ourFragPos);
		vec3 thisLightDir = normalize(pointLightPos[i] - ourFragPos);
		diffuse += diff * pointLightColors[i] * (1/(lightDist * lightDist));

		vec3 thisHalfDir = normalize(thisLightDir + viewDir);  
		float thisSpecAngle = pow(max(dot(ourNormal, thisHalfDir), 0.0), 1.0);
		specular += pointLightColors[i] * thisSpecAngle * (1/(lightDist));
	}

	vec3 result = (ambient+diffuse+specular)*ourColor;
	result = mix(result, vec3(0.5,0.5,0.5), min(depth * fogAmount,1.0f));
    fragColor = vec4(result, 1.0f);
}