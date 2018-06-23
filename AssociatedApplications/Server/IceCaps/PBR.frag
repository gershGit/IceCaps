#version 330 core
#define NR_POINT_LIGHTS 4

in vec3 ourFragPos;
in vec2 ourTexCoords;
in mat3 TBN;
in vec3 linearPos;

out vec4 fragColor;

uniform sampler2D diffuseSampler;
uniform sampler2D metallicSampler;
uniform sampler2D roughnessSampler;
uniform sampler2D normalSampler;
uniform sampler2D aoSampler;
uniform sampler2D irradianceMap;
uniform sampler2D environmentMap;

uniform vec3 eyePos;
uniform vec3 sunAngle;
uniform vec4 sunColor;							//r,g,b,strength
uniform vec3 pointLightPos[NR_POINT_LIGHTS];	//x,y,z
uniform vec3 pointLightColors[NR_POINT_LIGHTS];

const float M_PI = 3.14159265359;
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
vec3 getFromIMap(vec3 N){
    vec2 uv = SampleSphericalMap(N);
    return texture(irradianceMap, uv).rgb;
}
vec3 getFromEMap(vec3 N){
    vec2 uv = SampleSphericalMap(N);
    return texture(environmentMap, uv).rgb;
}
vec3 getReflection(vec3 V, vec3 N, float roughness){
	vec3 R = reflect(-V, N);
	vec3 I = getFromIMap(R);
	vec3 E = getFromEMap(R);
	return mix(E, I, roughness);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  

vec3 fresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0-F0) * pow(1.0 - cosTheta*cosTheta, 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness*roughness; //Alpha calculate by squaring roughness
	float a2 = a*a;
	float NdotH = max(dot(N,H),0.0);
	float NdotH2 = NdotH*NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 -1.0) + 1.0);
	denom = M_PI * denom * denom;

	return num/denom;
}

float geometrySchlickGGX(float NdotV, float roughness){
	float r = roughness + 1.0;
	float k = (r*r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num/denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

//Calculates DFG for cook torrence
vec3 BRDF(vec3 N, vec3 V, vec3 H, vec3 L, vec3 radiance, vec3 albedo, float metallic, vec3 F0, float roughness){
	float D = distributionGGX(N, H, roughness);
	float G = geometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H,N), 0.0), F0);

	vec3 diffuseFraction = vec3(1.0) - F;
	diffuseFraction *= (1.0 - metallic);

	vec3 numerator = D*G*F * (getReflection(V,N,roughness));
	float denominator = 4.0 * max(dot(N,V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	float diffuseAngle = max(dot(N,L), 0.0);
	return (diffuseFraction * albedo / M_PI + specular) * radiance * diffuseAngle;
}

//Calculate radiance for a single light
vec3 getPointLighting(vec3 N, vec3 V, int index, vec3 albedo, float metallic, float roughness, vec3 F0){
	vec3 L = normalize(pointLightPos[index] - ourFragPos);	//Light direction
	vec3 H = normalize(V + L);							//Half direction
	float d = length(pointLightPos[index] - ourFragPos);	//distance from light to object
	float attenuation = 1.0 / (d*d);					//attenuation due to inverse square law
	vec3 radiance = pointLightColors[index] * attenuation;	//Single light's radiance scaled by attenuation

	return BRDF(N, V, H, L, radiance, albedo, metallic, F0, roughness);
}

//Calculate radiance for the sun
vec3 getSunLighting(vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, vec3 F0){
	vec3 L = normalize(-sunAngle);					//Light direction
	vec3 H = normalize(V + L);						//Half direction
	float attenuation = sunColor.w;					//attenuation due to inverse square law
	vec3 radiance = sunColor.rgb * attenuation;		//Sun's radiance

	return BRDF(N, V, H, L, radiance, albedo, metallic, F0, roughness);
}

vec3 getEnvironmentLighting(vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, vec3 F0){
	vec3 L = reflect(-V,N);
	vec3 H = normalize(V + L);
	vec3 radiance = getFromIMap(L) / 128;		

	return BRDF(N, V, H, L, radiance, albedo, metallic, F0, roughness);
}

vec3 getNormalFromMap(){
	vec3 N = texture(normalSampler, ourTexCoords).rgb;
	N = normalize(N * 2.0 - 1.0);
	return normalize(TBN * N);
}



void main() {
	//TODO allow for reflectivity map
	vec3 N = getNormalFromMap();
	vec3 V = normalize(eyePos-ourFragPos);

	vec3 albedo = texture(diffuseSampler, ourTexCoords).rgb;
	albedo = vec3(pow(albedo.x, 2.2), pow(albedo.y, 2.2), pow(albedo.z, 2.2));
	float metallic = texture(metallicSampler, ourTexCoords).r;
	float roughness = texture(roughnessSampler, ourTexCoords).r;
	float occlusion = texture(aoSampler, ourTexCoords).r;
	vec3 ambience = texture(irradianceMap, ourTexCoords).rgb;

	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	vec3 Lo = getEnvironmentLighting(N, V, albedo, metallic, roughness, F0);
	Lo += getSunLighting(N, V, albedo, metallic, roughness, F0);
	for (int i =0; i<NR_POINT_LIGHTS; i++){
		Lo += getPointLighting(N, V, i, albedo, metallic, roughness, F0);
	}

	//Add ambient lighting
	vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); 
	vec3 kD = 1.0 - kS;
	vec3 irradiance = getFromIMap(N);
	vec3 diffuse    = irradiance * (albedo*albedo);	
	vec3 ambient    = (kD * diffuse) * (occlusion/16); 

	//vec3 ambient = vec3(0.03) * ambience * albedo;
	//vec3 color = ambient + Lo;
	vec3 color = ambient + Lo;

	//Gamma correction
	color = color / (color+vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	//output
    fragColor = vec4(color, 1.0f);
}