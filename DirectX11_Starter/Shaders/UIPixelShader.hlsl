// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//return(float4(1.0f, 1.0f, 1.0f, 1.0f));

	input.normal = normalize(input.normal);

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	
	return surfaceColor;
}