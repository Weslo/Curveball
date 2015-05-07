#pragma once
#include "Material.h"
class BallMaterial :
	public Material
{
public:
	BallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~BallMaterial();
	void PrepareToDraw(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);

private:
	
};

