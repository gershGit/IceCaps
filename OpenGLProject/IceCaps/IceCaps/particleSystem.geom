#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 ourTexCoords;

uniform float size;

void build_plane(vec4 position){
	float radius = size/2;
	gl_Position = position + vec4(-0.5, -0.5, 0, 0);
	ourTexCoords = vec2(0,0);
	EmitVertex();
	gl_Position = position + vec4(0.5, -0.5, 0, 0);
	ourTexCoords = vec2(1,0);
	EmitVertex();
	gl_Position = position + vec4(-0.5, 0.5, 0, 0);
	ourTexCoords = vec2(0,1);
	EmitVertex();
	gl_Position = position + vec4(0.5, 0.5, 0, 0);
	ourTexCoords = vec2(1,1);
	EmitVertex();
	EndPrimitive();
}

void main(){
	build_plane(gl_in[0].gl_Position);
}