#include "WallMaterial.h"


WallMaterial::WallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs,  ps, rv, ss)
{
	lineBounds = DirectX::XMFLOAT2(0, 0);
}

WallMaterial::~WallMaterial()
{
}

DirectX::XMFLOAT2 WallMaterial::GetLineBounds()
{
	return lineBounds;
}

void WallMaterial::SetLineBounds(DirectX::XMFLOAT2 ballInfo)
{
	lineBounds = CalcLineBounds(ballInfo);
}

void WallMaterial::SetCamPos(DirectX::XMFLOAT4 _camPos)
{
	camPos = _camPos;
}

DirectX::XMFLOAT2 WallMaterial::CalcLineBounds(DirectX::XMFLOAT2 ballInfo)
{
	float pos = ballInfo.x - ballInfo.y / 2;
	float pos2 = ballInfo.x + ballInfo.y / 2;

	return DirectX::XMFLOAT2(pos, pos2);
}

void WallMaterial::SetLArray(Light l[], int size)
{
	for (int i = 0; i < 8; i++)
	{
		lArray[i] = l[i];
	}

	arraySize = size;
}

void WallMaterial::PrepareToDraw(DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	Material::PrepareToDraw(world, view, proj);

	vertexShader->SetFloat2("lineBounds", lineBounds);
	vertexShader->SetFloat4("cameraPosition", camPos);

	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("diffuseTexture", resourceView);
	pixelShader->SetSamplerState("basicSampler", samplerState);

	pixelShader->SetFloat("numLights", arraySize);
	pixelShader->SetFloat4("cameraPosition", camPos);
	pixelShader->SetData("lights", &lArray, sizeof(Light) * 8);

	//pixelShader->SetShader();
}