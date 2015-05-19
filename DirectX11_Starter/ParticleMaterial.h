#pragma once
#include "Material.h"

class ParticleMaterial
	:public Material
{
public:
	ParticleMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss);
	~ParticleMaterial();

	void PrepareToDraw(DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);

private:

};

