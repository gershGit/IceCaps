#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAX_LIGHTS 4
#define MAX_WEIGHTS 4
#define MAX_BONES 20
layout(set = 0, binding = 0) uniform ViewPersp {
    mat4 view;
    mat4 proj;
} ubo;

layout(set = 0, binding = 2) uniform ModelMatrix {
    mat4 model;
} bone_positions[MAX_BONES];

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in int[MAX_WEIGHTS] bone_id;
layout(location = 8) in float[MAX_WEIGHTS] bone_weight;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;

//Modelview matrix via push constant
layout(push_constant) uniform modelBlock {
    layout(offset=0) mat4 model;
} modelPushConstant;

//Main function
void main() {
    vec4 localPos = vec4(0.0f);
    vec4 totalNormal = vec4(0.0f);
    vec4 totalTangent = vec4(0.0f);

    for (int i=0; i<MAX_WEIGHTS; i++) {
        mat4 boneTransform = bone_positions[bone_id[i]].model;
        vec4 posePosition = boneTransform * vec4(inPosition, 1.0f);
        localPos += posePosition * bone_weight[i];

        vec4 worldNormal = boneTransform * vec4(inNormal, 0.0f);
        vec4 worldTangent = boneTransform * vec4(inTangent, 0.0f);
        totalNormal += worldNormal * bone_weight[i];
        totalTangent += worldTangent * bone_weight[i];
    }

    gl_Position = ubo.proj * ubo.view * modelPushConstant.model * localPos;
    vec3 biTanagent = cross(totalNormal.xyz, totalTangent.xyz);
    fragNormal = totalNormal.xyz;
    fragTexCoord = inTexCoord;
}