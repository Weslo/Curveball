#include "BallMaterial.h"


BallMaterial::BallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs, ps, rv, ss)
{
}


BallMaterial::~BallMaterial()
{
}

void BallMaterial::PrepareToDraw(DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	Material::PrepareToDraw(world, view, proj);

	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("diffuseTexture", resourceView);
	pixelShader->SetSamplerState("basicSampler", samplerState);

	pixelShader->SetShader();
}