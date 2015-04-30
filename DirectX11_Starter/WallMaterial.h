#pragma once
#include "Material.h"
class WallMaterial :
	public Material
{
public:
	WallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~WallMaterial();

	DirectX::XMFLOAT2 GetLineBounds();
	void SetLineBounds(DirectX::XMFLOAT2 ballInfo);

private:
	DirectX::XMFLOAT2 lineBounds;

	DirectX::XMFLOAT2 CalcLineBounds(DirectX::XMFLOAT2 ballInfo);
};

