#version 330 core
out vec4 FragColor;

in vec2 ourTexCoords;
uniform sampler2D particle_texture;
uniform float mixRate;
uniform vec4 color;

void main()
{
	vec3 colorFinal = mix(texture(particle_texture, ourTexCoords).rgb, color.rgb, mixRate);
	float alpha = min(texture(particle_texture, ourTexCoords).a, color.a); 
    FragColor = vec4(colorFinal, alpha);   
} 