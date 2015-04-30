#pragma once
#include "Material.h"
class PlayerMaterial :
	public Material
{
public:
	PlayerMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~PlayerMaterial();
};

