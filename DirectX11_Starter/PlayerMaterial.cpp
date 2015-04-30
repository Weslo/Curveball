#include "PlayerMaterial.h"

PlayerMaterial::PlayerMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs, ps, rv, ss)
{
}


PlayerMaterial::~PlayerMaterial()
{
}

void PlayerMaterial::SetLArray(Light l[], int size)
{
	for (int i = 0; i < 8; i++)
	{
		lArray[i] = l[i];
	}

	arraySize = size;
}

void PlayerMaterial::SetCamPos(DirectX::XMFLOAT4 _camPos)
{
	camPos = _camPos;
}

void PlayerMaterial::PrepareToDraw(DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	Material::PrepareToDraw(world, view, proj);

	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("diffuseTexture", resourceView);
	pixelShader->SetSamplerState("basicSampler", samplerState);
	pixelShader->SetData("lights", &lArray, sizeof(Light)* 8);
	pixelShader->SetFloat4("cameraPosition", camPos);
	pixelShader->SetFloat("numLights", arraySize);

	pixelShader->SetShader();
}
