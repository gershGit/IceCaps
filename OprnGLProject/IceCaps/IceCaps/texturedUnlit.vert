#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 ourTexCoord;

float near = 0.1;
float far = 100;

float LinearizeDepth(float depth){
	return (1/depth - 1/near) / (1/far - 1/near);
}

void main() {
    vec4 tempPos = projection * view * model * vec4(aPos, 1.0);

	gl_Position = vec4(tempPos.x, tempPos.y, LinearizeDepth(tempPos.z), tempPos.w);

    ourTexCoord = texCoords;
}