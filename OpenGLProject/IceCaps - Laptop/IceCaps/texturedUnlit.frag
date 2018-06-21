#version 330 core

out vec4 fragColor;

in vec2 ourTexCoord;

uniform sampler2D diffuseSampler;

void main() {
    fragColor = texture(diffuseSampler, ourTexCoord);
}