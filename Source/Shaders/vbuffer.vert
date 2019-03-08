#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAX_LIGHTS 4
layout(set = 0, binding = 0) uniform ViewPersp {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;

layout(push_constant) uniform modelBlock {
    layout(offset=0) mat4 model;
} modelPushConstant;

//Main function
void main() {
    gl_Position = ubo.proj * ubo.view * modelPushConstant.model * vec4(inPosition, 1.0);
    vec3 biTanagent = cross(inNormal, inTangent);
    fragNormal = inNormal;
    fragTexCoord = inTexCoord;
}