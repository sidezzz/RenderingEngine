#include "common.hlsli"

float4 main(PS_INPUT input) : SV_TARGET0
{
	float3 normal = normalize(input.normal);
	float3 light_normal = normalize(input.light_normal);
	float3 reflect_normal = normalize(input.reflect_normal);

	float ambient_intensity = 0.25;
	float diffuse_intensity = saturate(input.light_intensity) * dot(normal, light_normal);
	float specular_intensity = saturate(pow(max(dot(light_normal, reflect_normal), 0), input.shininess));

	return float4((input.diffuse * diffuse_intensity + input.ambient * ambient_intensity + input.specular * specular_intensity), 1.f) * MODEL_COLOR_MULTIPLIER;
}