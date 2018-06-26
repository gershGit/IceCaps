#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 aColor;

out vec3 ourColor;

float near = 0.1;
float far = 100;

float LinearizeDepth(float depth){
	return depth/far;
}

void main() {
	vec4 tempPos = projection * view * model * vec4(aPos, 1.0);

	gl_Position = vec4(tempPos.x, tempPos.y, LinearizeDepth(tempPos.z), tempPos.w);
    ourColor = aColor;
}