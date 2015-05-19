#include "ParticleMaterial.h"

ParticleMaterial::ParticleMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs, ps, rv, ss)
{
}


ParticleMaterial::~ParticleMaterial()
{
}

void ParticleMaterial::SetLArray(Light l[])
{
	for (int i = 0; i < 8; i++)
	{
		lArray[i] = l[i];
	}
}

void ParticleMaterial::SetCamPos(DirectX::XMFLOAT4 _camPos)
{
	camPos = _camPos;
}

void ParticleMaterial::PrepareToDraw(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	Material::PrepareToDraw(view, proj);

	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("particleTexture", resourceView);
	pixelShader->SetSamplerState("samplerState", samplerState);
	pixelShader->SetFloat4("cameraPosition", camPos);
	pixelShader->SetData("lights", &lArray, sizeof(lArray));
	pixelShader->SetShader();
}