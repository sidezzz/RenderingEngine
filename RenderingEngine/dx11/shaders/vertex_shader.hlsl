cbuffer Dx11ConstantBuffer : register(b0)
{
	float4x4 model_transform;
	float4x4 view_transform;
	float4x4 projection_transform;
	float4x4 model_view_projection_transform;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.position = mul(float4(input.position.xyz, 1.f), model_view_projection_transform);
	output.normal = normalize(mul(input.normal, model_transform));
	output.color = input.color;
	output.uv = input.uv;
	return output;
}