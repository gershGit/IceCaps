#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 texCoords;

uniform mat3 itModel;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 ourTexCoord;
out vec3 ourFragPos;

out vec3 ourNormal;
out vec3 ourTangent;
out vec3 ourBitangent;

float near = 0.1;
float far = 100;

float LinearizeDepth(float depth){
	return (1/depth - 1/near) / (1/far - 1/near);
}

void main() {
    vec4 tempPos = projection * view * model * vec4(aPos, 1.0);

	gl_Position = vec4(tempPos.x, tempPos.y, LinearizeDepth(tempPos.z), tempPos.w);

	ourNormal = mat3(transpose(inverse(model))) * aNormal;
	ourTangent = mat3(transpose(inverse(model))) * aTangent;
	ourBitangent = mat3(transpose(inverse(model))) * aBitangent;

	ourFragPos = vec3(model * vec4(aPos, 1.0));
    ourTexCoord = texCoords;
}