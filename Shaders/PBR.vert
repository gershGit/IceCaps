#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;
layout (location = 4) in vec2 aTexCoords;

uniform mat4 itModel;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 ourTexCoords;
out vec3 ourFragPos;
out mat3 TBN;
out vec3 linearPos;
out float depth;

float near = 0.1;
float far = 100;

float LinearizeDepth(float depth){
	//return (1/depth - 1/near) / (1/far - 1/near);
	return depth/far;
}

void main() {
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N);

	vec4 tempPos = projection * view * model * vec4(aPos, 1.0);

	gl_Position = vec4(tempPos.x, tempPos.y, LinearizeDepth(tempPos.z), tempPos.w);
	depth = LinearizeDepth(tempPos.z);
	ourFragPos = vec3(model * vec4(aPos, 1.0));
	ourTexCoords = aTexCoords;
	linearPos = vec3(tempPos.x, tempPos.y, LinearizeDepth(tempPos.z));
}