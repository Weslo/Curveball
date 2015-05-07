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

	virtual void PrepareToDraw(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);

protected:
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;

	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* samplerState;
};
