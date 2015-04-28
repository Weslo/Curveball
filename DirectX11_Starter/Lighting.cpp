#include "Lighting.h"

Lighting::Lighting(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range)
{
	lightType = _lightType;
	ambient = _ambient;
	diffuse = _diffuse;
	range = _range;
}

Lighting::Lighting(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range, XMFLOAT3 _pos, XMFLOAT3 _att)
{
	lightType = _lightType;
	ambient = _ambient;
	diffuse = _diffuse;
	range = _range;
	pos = _pos;
	att = _att;
}

Lighting::Lighting(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range, XMFLOAT3 _pos = XMFLOAT3(0,0,0), XMFLOAT3 _att = XMFLOAT3(0,0,0), XMFLOAT3 _dir = XMFLOAT3(0,0,0), float _cone = 0)
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


Lighting::~Lighting()
{
}

XMFLOAT3 Lighting::GetPosition()
{
	return pos;
}

XMFLOAT4 Lighting::GetAmbient()
{
	return ambient;
}

XMFLOAT4 Lighting::GetDiffuse()
{
	return diffuse;
}

float Lighting::GetRange()
{
	return range;
}

XMFLOAT3 Lighting::GetAttenuation()
{
	return att;
}

XMFLOAT3 Lighting::GetDirection()
{
	return dir;
}

float Lighting::GetCone()
{
	return cone;
}

void Lighting::SetPosition(XMFLOAT3 _pos)
{
	pos = _pos;
}

void Lighting::SetAmbient(XMFLOAT4 _ambient)
{
	ambient = _ambient;
}

void Lighting::SetDiffuse(XMFLOAT4 _diffuse)
{
	diffuse = _diffuse;
}

void Lighting::SetRange(float _range)
{
	range = _range;
}

void Lighting::SetAttenuation(XMFLOAT3 _att)
{
	att = _att;
}

void Lighting::SetDirection(XMFLOAT3 _dir)
{
	dir = _dir;
}

void Lighting::SetCone(float _cone)
{
	cone = _cone;
}

Light Lighting::ConvertToStruct()
{
	Light l;
	l.ambient = ambient;
	l.attenuation = att;
	l.cone = cone;
	l.diffuse = diffuse;
	l.direction = dir;
	l.lightType = lightType;
	l.position = pos;
	l.range = range;

	return l;
}
