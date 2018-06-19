#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 localPos;

uniform mat4 projection;
uniform mat4 view;

void main(){
	localPos = aPos;
	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(localPos, 1.0);

	gl_Position = clipPos.xyzw;
}