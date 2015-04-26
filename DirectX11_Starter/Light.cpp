#include "Light.h"

Light::Light(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range)
{
	lightType = _lightType;
	ambient = _ambient;
	diffuse = _diffuse;
	range = _range;
}

Light::Light(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range, XMFLOAT3 _pos, XMFLOAT3 _att)
{
	lightType = _lightType;
	ambient = _ambient;
	diffuse = _diffuse;
	range = _range;
	pos = _pos;
	att = _att;
}

Light::Light(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range, XMFLOAT3 _pos = XMFLOAT3(0,0,0), XMFLOAT3 _att = XMFLOAT3(0,0,0), XMFLOAT3 _dir = XMFLOAT3(0,0,0), float _cone = 0)
{
	lightType = _lightType;
	ambient = _ambient;
	diffuse = _diffuse;
	range = _range;
	pos = _pos;
	att = _att;
	dir = _dir;
	cone = _cone;
}


Light::~Light()
{
}

XMFLOAT3 Light::GetPosition()
{
	return pos;
}

XMFLOAT4 Light::GetAmbient()
{
	return ambient;
}

XMFLOAT4 Light::GetDiffuse()
{
	return diffuse;
}

float Light::GetRange()
{
	return range;
}

XMFLOAT3 Light::GetAttenuation()
{
	return att;
}

XMFLOAT3 Light::GetDirection()
{
	return dir;
}

float Light::GetCone()
{
	return cone;
}

void Light::SetPosition(XMFLOAT3 _pos)
{
	pos = _pos;
}

void Light::SetAmbient(XMFLOAT4 _ambient)
{
	ambient = _ambient;
}

void Light::SetDiffuse(XMFLOAT4 _diffuse)
{
	diffuse = _diffuse;
}

void Light::SetRange(float _range)
{
	range = _range;
}

void Light::SetAttenuation(XMFLOAT3 _att)
{
	att = _att;
}

void Light::SetDirection(XMFLOAT3 _dir)
{
	dir = _dir;
}

void Light::SetCone(float _cone)
{
	cone = _cone;
}
