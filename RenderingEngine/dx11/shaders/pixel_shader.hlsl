cbuffer Dx11ConstantBuffer : register(b0)
{
	float4x4 model_transform;
	float4x4 view_transform;
	float4x4 projection_transform;
	float4x4 view_projection_transform;
	float4x4 model_view_projection_transform;
	float3 camera_position;
	float4 model_color_multiplier;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;

	float3 ambient : COLOR0;
	float3 diffuse : COLOR1;
	float3 specular : COLOR2;
	float shininess : PSIZE;

	float3 light_normal : NORMAL1;
	float3 reflect_normal : NORMAL2;
	float light_intensity : PSIZE1;
};

float4 main(PS_INPUT input) : SV_TARGET0
{
	float3 normal = normalize(input.normal);
	float3 light_normal = normalize(input.light_normal);
	float3 reflect_normal = normalize(input.reflect_normal);

	float ambient_intensity = 0.25;
	float diffuse_intensity = saturate(input.light_intensity) * dot(normal, light_normal);
	float specular_intensity = saturate(pow(max(dot(light_normal, reflect_normal), 0), input.shininess));

	return float4((input.diffuse * diffuse_intensity + input.ambient * ambient_intensity + input.specular * specular_intensity), 1.f) * model_color_multiplier;
}