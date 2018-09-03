#version 330 core

in vec3 ourColor;
in float depth;

out vec4 fragColor;

uniform float fogAmount;

void main() {
	vec3 result = mix(ourColor, vec3(0.5,0.5,0.52), min(depth*fogAmount,1.0));
    fragColor = vec4(result, 1.0f);
}
