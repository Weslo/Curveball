#pragma once
#include "Material.h"
#include "Lighting.h"

class WallMaterial :
	public Material
{
public:
	WallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~WallMaterial();

	DirectX::XMFLOAT2 GetLineBounds();
	void SetLineBounds(DirectX::XMFLOAT2 ballInfo);
	void SetCamPos(DirectX::XMFLOAT4 camPos);
	void PrepareToDraw(DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	void SetLArray(Light l[]);

private:
	DirectX::XMFLOAT2 lineBounds;
	DirectX::XMFLOAT4 camPos;

	Light lArray[8];

	DirectX::XMFLOAT2 CalcLineBounds(DirectX::XMFLOAT2 ballInfo);

	
};

