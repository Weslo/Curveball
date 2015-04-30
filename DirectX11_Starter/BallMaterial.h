#pragma once
#include "Material.h"
class BallMaterial :
	public Material
{
public:
	BallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~BallMaterial();

private:

};

