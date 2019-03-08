#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 ourTexCoords;

uniform float radius;
uniform vec2 quad;
uniform vec3 worldspace_up;
uniform vec3 worldspace_right;

uniform mat4 view;
uniform mat4 projection;

float far = 100;

float LinearizeDepth(float depth){
	return depth/far;
}

void build_square(vec4 position) {    
	vec3 moveRight = vec3(worldspace_right)*radius;
	vec3 moveUp =  vec3(worldspace_up)*radius;

	vec4 tempPos = projection * view * vec4(position.xyz - moveRight - moveUp, 1.0);
	gl_Position = vec4(tempPos.xy, LinearizeDepth(tempPos.z), tempPos.w);
	ourTexCoords = vec2(quad.x, quad.y);
    EmitVertex();   

	tempPos = projection * view * vec4(position.xyz + moveRight - moveUp, 1.0);
	gl_Position = vec4(tempPos.xy, LinearizeDepth(tempPos.z), tempPos.w);
	ourTexCoords = vec2(0.5+quad.x, quad.y);
    EmitVertex(); 

    tempPos = projection * view * vec4(position.xyz - moveRight + moveUp, 1.0);
	gl_Position = vec4(tempPos.xy, LinearizeDepth(tempPos.z), tempPos.w);
	ourTexCoords = vec2(quad.x, 0.5+quad.y);
    EmitVertex(); 

    tempPos = projection * view * vec4(position.xyz + moveRight + moveUp, 1.0);
	gl_Position = vec4(tempPos.xy, LinearizeDepth(tempPos.z), tempPos.w);
	ourTexCoords = vec2(0.5+quad.x, 0.5+quad.y);
    EmitVertex(); 
} 

void main() {    
    build_square(gl_in[0].gl_Position);
} 