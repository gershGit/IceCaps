#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAX_LIGHTS 4

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set=0, binding = 1) uniform lightObject {
    vec4 lightPosition;
    vec4 lightColor;
} lights[MAX_LIGHTS];

layout(set = 1, binding = 0) uniform sampler2D texSamplerArray[5];

void main() {
    outColor = texture(texSamplerArray[0], fragTexCoord) * max(0.5f, texture(texSamplerArray[1], fragTexCoord).x);
}