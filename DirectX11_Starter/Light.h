#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 direction;
	float range;
	XMFLOAT3 position;
	float cone;
	XMFLOAT3 attenuation;
	float lightType;
};

class Light
{
public:
	//For directional
	Light(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range);
	//For Point
	Light(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range, XMFLOAT3 _pos, XMFLOAT3 _att);
	//For Spot
	Light(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range, XMFLOAT3 _pos, XMFLOAT3 _att, XMFLOAT3 _dir, float _cone);
	~Light();

	XMFLOAT3 GetPosition();
	XMFLOAT4 GetAmbient();
	XMFLOAT4 GetDiffuse();
	float GetRange();
	XMFLOAT3 GetAttenuation();
	XMFLOAT3 GetDirection();
	float GetCone();

	void SetPosition(XMFLOAT3 _pos);
	void SetAmbient(XMFLOAT4 _ambient);
	void SetDiffuse(XMFLOAT4 _diffuse);
	void SetRange(float _range);
	void SetAttenuation(XMFLOAT3 _att);
	void SetDirection(XMFLOAT3 _dir);
	void SetCone(float _cone);

private:
	
	//assign a num based on the type of light to make calculations easy, add more and comment as needed
	//0 - Directional
	//1 - Point
	//2 - Spot
	int lightType;

	//For all lights
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	float range;
	
	//For specific lights

	//Point and spot
	XMFLOAT3 att;
	XMFLOAT3 pos;

	//Spot
	XMFLOAT3 dir;
	float cone;
	
	

};

