struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 uv  : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET0
{
	return input.color;
}