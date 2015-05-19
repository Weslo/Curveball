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
	float4 worldPos : TEXCOORD1;
	float3 normal : NORMAL;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.worldPos = mul(output.position, world);
	output.normal = mul(float3(0.0f, 0.0f, -1.0f), (float3x3)world);

	output.tex = input.tex;
	output.color = input.color;

	return output;
}