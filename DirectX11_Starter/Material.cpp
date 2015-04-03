#include "Material.h"

Material::Material(SimplePixelShader* ps, SimpleVertexShader* vs, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss)
{
	pixelShader = ps;
	vertexShader = vs;
	resourceView = rv;
	samplerState = ss;
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	vertexShader->LoadShaderFile(L"VertexShader.cso");
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