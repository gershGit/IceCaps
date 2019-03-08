#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec3 ourNormal;
in vec3 ourFragPos;
in float depth;

uniform float fogAmount;
uniform vec3 eyePos;
uniform vec3 sunAngle;

float ambientStrength = 0.2;
float shininess = 32.0;
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
	vec3 specular = lightColor * specAngle;

    vec3 result = (ambient+diffuse+specular)*ourColor;
	result = mix(result, vec3(0.5,0.5,0.52), min(depth*fogAmount,1.0));
    fragColor = vec4(result, 1.0);
}