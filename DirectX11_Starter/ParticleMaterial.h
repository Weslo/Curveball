#pragma once
#include "Material.h"
#include "Lighting.h"

class ParticleMaterial
	:public Material
{
public:
	ParticleMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~ParticleMaterial();

	void PrepareToDraw(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	void SetCamPos(DirectX::XMFLOAT4 _camPos);
	void SetLArray(Light l[]);

private:
	DirectX::XMFLOAT4 camPos;

	Light lArray[8];
};

