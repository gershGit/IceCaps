#version 330 core
out vec4 FragColor;
in vec3 localPos;
in float depth;

uniform sampler2D equirectangularMap;
uniform float fogAmount;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(localPos)); // make sure to normalize localPos
    vec3 color = texture(equirectangularMap, uv).rgb;
    
	color = mix(color, vec3(0.5,0.5,0.5), min(depth*fogAmount,1.0f));
    FragColor = vec4(color, 1.0);
}