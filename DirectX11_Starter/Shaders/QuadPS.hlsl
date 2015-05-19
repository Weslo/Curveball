
cbuffer whatever : register(b0)
{
	float2 pixelSize;
	int blurAmount;
}

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// TEXTURE STUFF
Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float2 uvOffset[9];
	uvOffset[0] = float2(0, 0);
	uvOffset[1] = float2(-pixelSize.x, -pixelSize.y);
	uvOffset[2] = float2(-pixelSize.x, 0);
	uvOffset[3] = float2(-pixelSize.x, pixelSize.y);
	uvOffset[4] = float2(0, pixelSize.y);
	uvOffset[5] = float2(pixelSize.x, pixelSize.y);
	uvOffset[6] = float2(pixelSize.x, 0);
	uvOffset[7] = float2(pixelSize.x, -pixelSize.y);
	uvOffset[8] = float2(0, -pixelSize.y);

	// Sample all pixels
	float4 finalColor = diffuseTexture.Sample(basicSampler, input.uv);

	for (int blur = 0; blur < blurAmount; blur++)
	{
		[unroll]
		for (int i = 1; i < 9; i++)
		{
			finalColor += diffuseTexture.Sample(basicSampler, input.uv + uvOffset[i] * blur);
		}
	}
	finalColor /= max(1, 9 * blurAmount);

	return finalColor;
}