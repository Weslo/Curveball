

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VertexShaderInput
{ 
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
};

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// The entry point for our vertex shader
VertexToPixel main( VertexShaderInput input )
{
	// Set up output
	VertexToPixel output;
	
	// I know the input position is already expressed in screen space
	output.position = float4(input.position, 1);
	output.uv = input.uv;

	return output;
}