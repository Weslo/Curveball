Texture2D particleTexture;
SamplerState samplerState;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 finalColor;

	textureColor = particleTexture.Sample(samplerState, input.tex);

	finalColor = textureColor * input.color;

	return finalColor;
}