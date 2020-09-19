cbuffer Dx11ConstantBuffer : register(b0)
{
	float4x4 MODEL_TRANSFORM;
	float4x4 VIEW_TRANSFORM;
	float4x4 PROJECTION_TRANSFORM;
	float4x4 VIEW_PROJECTION_TRANSFORM;
	float4x4 MODEL_VIEW_PROJECTION_TRANSFORM;
	float3 CAMERA_POSITION;
	float4 MODEL_COLOR_MULTIPLIER;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;

	float3 ambient : COLOR0;
	float3 diffuse : COLOR1;
	float3 specular : COLOR2;
	float shininess : PSIZE;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;

	float3 ambient : COLOR0;
	float3 diffuse : COLOR1;
	float3 specular : COLOR2;
	float shininess : PSIZE0;

	float3 light_normal : NORMAL1;
	float3 reflect_normal : NORMAL2;
	float light_intensity : PSIZE1;
};