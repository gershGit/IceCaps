#version 330 core

in vec3 ourColor;

out vec4 fragColor;


float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth){
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far-near));
}

void main() {
    fragColor = vec4(ourColor, 1.0f);
}
