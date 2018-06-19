#version 330 core
#define NR_POINT_LIGHTS 4

in vec3 ourFragPos;
in vec2 ourTexCoords;
in mat3 TBN;

out vec4 fragColor;

uniform sampler2D diffuseSampler;
uniform sampler2D metallicSampler;
uniform sampler2D roughnessSampler;
uniform sampler2D normalSampler;
uniform sampler2D aoSampler;

uniform vec3 eyePos;
uniform vec3 sunAngle;
uniform vec4 sunColor;							//r,g,b,strength
uniform vec3 pointLightPos[NR_POINT_LIGHTS];	//x,y,z
uniform vec3 pointLightColors[NR_POINT_LIGHTS];

const float M_PI = 3.14159265359;

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

	vec3 numerator = D*G*F;
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
	float ambience = texture(aoSampler, ourTexCoords).r;

	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	vec3 Lo = vec3(0.0); //Initial radiance
	Lo += getSunLighting(N, V, albedo, metallic, roughness, F0);
	for (int i =0; i<NR_POINT_LIGHTS; i++){
		Lo += getPointLighting(N, V, i, albedo, metallic, roughness, F0);
	}

	//Add ambient lighting
	vec3 ambient = vec3(0.03) * ambience * albedo;
	vec3 color = ambient + Lo;

	//Gamma correction
	color = color / (color+vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	//output
    fragColor = vec4(color, 1.0f);
}