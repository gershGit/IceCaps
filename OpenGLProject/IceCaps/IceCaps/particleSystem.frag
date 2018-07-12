#version 330 core
out vec4 FragColor;

in vec2 ourTexCoords;
uniform sampler2D particle_texture;

void main()
{
    FragColor = texture(particle_texture, ourTexCoords);   
} 