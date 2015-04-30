#include "WallMaterial.h"


WallMaterial::WallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs,  ps, rv, ss)
{
	lineBounds = DirectX::XMFLOAT2(0, 0);
}

WallMaterial::~WallMaterial()
{
}

DirectX::XMFLOAT2 WallMaterial::GetLineBounds()
{
	return lineBounds;
}

void WallMaterial::SetLineBounds(DirectX::XMFLOAT2 ballInfo)
{
	CalcLineBounds(ballInfo);
}

DirectX::XMFLOAT2 WallMaterial::CalcLineBounds(DirectX::XMFLOAT2 ballInfo)
{
	float pos = ballInfo.x - ballInfo.y / 2;
	float pos2 = ballInfo.x + ballInfo.y / 2;

	return DirectX::XMFLOAT2(pos, pos2);
}