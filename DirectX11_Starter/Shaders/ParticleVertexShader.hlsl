cbuffer perModel
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.tex = input.tex;
	output.color = input.color;

	return output;
}