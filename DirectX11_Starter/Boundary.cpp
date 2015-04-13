#include "Boundary.h"


Boundary::Boundary(int l, int w, Mesh* m, Material* ma) : GameEntity(m, ma)
{
	length = l;
	width = w;
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