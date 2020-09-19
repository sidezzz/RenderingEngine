#include "common.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	float4 world_position = mul(float4(input.position.xyz, 1.f), MODEL_TRANSFORM);

	output.position = mul(world_position, VIEW_PROJECTION_TRANSFORM);
	output.normal = normalize(mul(input.normal, MODEL_TRANSFORM));
	output.uv = input.uv;

	output.ambient = input.ambient;
	output.diffuse = input.diffuse;
	output.specular = input.specular;
	output.shininess = input.shininess;

	output.light_normal = normalize(CAMERA_POSITION - world_position.xyz);
	output.reflect_normal = reflect(-output.light_normal, output.normal);
	output.light_intensity = 30000.f / pow(length(CAMERA_POSITION - world_position.xyz), 2);

	return output;
}