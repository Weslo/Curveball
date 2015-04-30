#include "Material.h"

Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss)
{
	pixelShader = ps;
	vertexShader = vs;
	resourceView = rv;
	samplerState = ss;
}

Material::~Material()
{
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

ID3D11ShaderResourceView* Material::GetResourceView()
{
	return resourceView;
}

ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}

void Material::PrepareToDraw(DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	vertexShader->SetMatrix4x4("view", view);
	vertexShader->SetMatrix4x4("projection", proj);
}