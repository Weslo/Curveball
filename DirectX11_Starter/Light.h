#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
public:
	Light();
	~Light();

private:
	
	//assign a num based on the type of light to make calculations easy, add more and comment as needed
	//1 - Directional
	//2 - Point
	//3 - Spot
	int lightType;

	//For all lights
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	float range;
	
	//For specific lights
	XMFLOAT3 pos;
	
	XMFLOAT3 dir;
	float cone;
	XMFLOAT3 att;
	

};

