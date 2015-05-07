#pragma once
#include "Material.h"
#include "Lighting.h"

class PlayerMaterial :
	public Material
{
public:
	PlayerMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~PlayerMaterial();

	void SetCamPos(DirectX::XMFLOAT4 _camPos);	
	void SetLArray(Light l[]);
	void PrepareToDraw(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);

private:

	DirectX::XMFLOAT4 camPos;

	Light lArray[8];
};

