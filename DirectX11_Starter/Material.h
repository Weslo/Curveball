#pragma once

#include "SimpleShader.h"

class Material
{
public:
	Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~Material();

	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();

	ID3D11ShaderResourceView* GetResourceView();
	ID3D11SamplerState* GetSamplerState();
private:
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;

	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* samplerState;
};
