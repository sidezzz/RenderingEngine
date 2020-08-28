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

float4 main(PS_INPUT input) : SV_TARGET0
{
	float ambient_intensity = 0.25;
	float diffuse_intensity = saturate(dot(input.normal, input.light_normal) / (length(input.normal) * length(input.light_normal)));
	float specular_intensity = saturate(pow(max(dot(input.light_normal, input.reflect_normal), 0), 64));
	
	//float3 reflect_normal = reflect(-input.light_normal, input.normal);
	//float specular_intensity = pow(max(dot(input.light_normal, reflect_normal), 0), 32);


	return float4((input.diffuse * diffuse_intensity + input.ambient * ambient_intensity + input.specular * specular_intensity).xyz, 1.f);
}