cbuffer Dx11ConstantBuffer : register(b0)
{
	float4x4 model_transform;
	float4x4 view_transform;
	float4x4 projection_transform;
	float4x4 model_view_projection_transform;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	nointerpolation float3 normal : NORMAL;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET0
{
	float4 diffuse = { 0.7f, 0.7f, 0.7f, 1.0f};

    float4 ambient = {0.1, 0.1, 0.1, 1.0};
    return ambient + input.color * saturate(dot(diffuse, input.normal));
}