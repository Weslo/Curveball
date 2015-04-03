// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct DirectionalLight
{
	float4 ambient : AMBIENT;
	float4 diffuse : DIFFUSE;
	float3 direction : DIRECTION;
};

cbuffer perLight : register(b0)
{
	DirectionalLight directionalLight;
	DirectionalLight secondDirectionalLight;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	normalize(input.normal);

	// Texture

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	// First Light

	float3 directionToLight = -directionalLight.direction;
	normalize(directionToLight);

	float lightAmount = dot(input.normal, directionToLight);
	saturate(lightAmount);

	float4 light = directionalLight.diffuse;
	light *= lightAmount;
	light += directionalLight.ambient;

	// Second Light

	float3 directionToSecondLight = -secondDirectionalLight.direction;
	normalize(directionToSecondLight);

	float secondLightAmount = dot(input.normal, directionToSecondLight);
	saturate(secondLightAmount);

	float4 secondLight = secondDirectionalLight.diffuse;
	secondLight *= secondLightAmount;
	secondLight += secondDirectionalLight.ambient;

	float4 finalLight = light + secondLight;

	return finalLight * surfaceColor;
}