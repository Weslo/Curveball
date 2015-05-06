#include "Boundary.h"


Boundary::Boundary(int l, int w, Mesh* m, Material* ma) : GameEntity(m, ma)
{
	length = l;
	width = w;
	SetColor(XMFLOAT4(1, 1, 1, 1));
}


Boundary::~Boundary()
{
}

XMFLOAT3 Boundary::GetUp()
{
	return up;
}

int Boundary::GetLength()
{
	return length;
}

int Boundary::GetWidth()
{
	return width;
}

void Boundary::SetUp(XMFLOAT3 u)
{
	up = u;
}

void Boundary::SetColor(XMFLOAT4 _color)
{
	color = _color;
}

void Boundary::RandomizeColor()
{
	int index = rand() % 3;
	XMFLOAT4 _color = XMFLOAT4(index == 0, index == 1, index == 2, 1.0f);
	SetColor(_color);
}

void Boundary::PrepareToDraw()
{
	material->GetVertexShader()->SetFloat4("tint", color);
}