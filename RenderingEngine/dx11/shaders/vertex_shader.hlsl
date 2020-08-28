cbuffer Dx11ConstantBuffer : register(b0)
{
	float4x4 model_transform;
	float4x4 view_transform;
	float4x4 projection_transform;
	float4x4 view_projection_transform;
	float4x4 model_view_projection_transform;
	float3 camera_position;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;

	float3 ambient : COLOR0;
	float3 diffuse : COLOR1;
	float3 specular : COLOR2;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;

	float3 ambient : COLOR0;
	float3 diffuse : COLOR1;
	float3 specular : COLOR2;

	float3 light_normal : NORMAL1;
	float3 reflect_normal : NORMAL2;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	float4 world_position = mul(float4(input.position.xyz, 1.f), model_transform);

	output.position = mul(world_position, view_projection_transform);
	output.normal = normalize(mul(input.normal, model_transform));
	output.uv = input.uv;

	output.ambient = input.ambient;
	output.diffuse = input.diffuse;
	output.specular = input.specular;

	output.light_normal = normalize(camera_position - world_position.xyz);
	output.reflect_normal = reflect(-output.light_normal, output.normal);

	return output;
}