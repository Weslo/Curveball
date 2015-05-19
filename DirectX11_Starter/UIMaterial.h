#pragma once
#include "Material.h"
class UIMaterial :
	public Material
{
public:
	UIMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~UIMaterial();
	void PrepareToDraw(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
};

