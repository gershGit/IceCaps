#version 330 core
#define NR_POINT_LIGHTS 4
out vec4 fragColor;

in vec2 ourTexCoord;
in vec3 ourNormal;
in vec3 ourFragPos;

in mat3 TBN;

uniform sampler2D albedoSampler;
uniform sampler2D metallicSampler;
uniform sampler2D roughnessSampler;
uniform sampler2D normalSampler;
uniform vec3 eyePos;

uniform vec3 sunAngle;			//x,y,z, strength
uniform vec4 sunColor;
uniform vec3 pointLightPos[NR_POINT_LIGHTS];	//x,y,z, strength
uniform vec3 pointLightColors[NR_POINT_LIGHTS];

float ambientStrength = 0.2;
float shininess = 256.0;
vec3 lightColor = vec3(23.47, 21.31, 20.79);
const float PI = 3.14159265359;


vec3 fresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0-F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a*a;
	float NdotH = max(dot(N,H),0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num/denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) * k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 CalcSunlight(vec3 N, vec3 V, float roughness, float metallic, vec3 albedo, vec3 F0){
	vec3 L = normalize(-sunAngle);
	vec3 H = normalize(V + L);
	vec3 radiance = sunColor.rgb * sunColor.w;

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 ks = F;
	vec3 kd = vec3(1.0) - ks;
	kd *= 1.0 - metallic;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	float NdotL = max(dot(N, L), 0.0);
	return (kd * albedo/PI + specular) * radiance * NdotL;
}

void main() {
	vec3 albedo = texture(albedoSampler, ourTexCoord).rgb;
	float metallic = texture(metallicSampler, ourTexCoord).r;
	float roughness = texture(roughnessSampler, ourTexCoord).r;

	vec3 F0 = vec3(0.04);
	F0 = mix (F0, albedo, metallic);

	vec3 N = texture(normalSampler, ourTexCoord).rgb;
	N = normalize(TBN * N);
	vec3 V = normalize(eyePos - ourFragPos);

	vec3 Lo = vec3(0.0);
	Lo += CalcSunlight(N, V, roughness, metallic, albedo, F0);
	for (int i=0; i < 4; i++){
		vec3 L = normalize(pointLightPos[i] - ourFragPos);
		vec3 H = normalize(V + L);
		float distance = length(pointLightPos[i] - ourFragPos);
		float attenuation = 1.0 / (distance*distance);
		vec3 radiance = pointLightColors[i].rgb * attenuation;

		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 ks = F;
		vec3 kd = vec3(1.0) - ks;
		kd *= 1.0 - metallic;

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = numerator / max(denominator, 0.001);

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kd * albedo/PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo;
	vec3 color = ambient + Lo;

	color = color/(color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	fragColor = vec4(color, 1.0);
}