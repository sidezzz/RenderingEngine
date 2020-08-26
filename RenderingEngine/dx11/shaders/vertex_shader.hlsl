cbuffer vertexBuffer : register(b0)
{
	float4x4 model_transform;
	float4x4 view_transform;
	float4x4 projection_transform;
};

struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 uv  : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.position = mul(model_transform, input.position);
	output.position = mul(view_transform, output.position);
	output.position = mul(projection_transform, output.position);
	output.color = input.color;
	output.uv = input.uv;
	return output;
}