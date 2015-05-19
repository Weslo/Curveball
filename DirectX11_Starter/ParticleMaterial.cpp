#include "ParticleMaterial.h"


ParticleMaterial::ParticleMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs, ps, rv, ss)
{
}


ParticleMaterial::~ParticleMaterial()
{
}

void ParticleMaterial::PrepareToDraw(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	Material::PrepareToDraw(view, proj);

	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("particleTexture", resourceView);
	pixelShader->SetSamplerState("samplerState", samplerState);
	pixelShader->SetShader();
}