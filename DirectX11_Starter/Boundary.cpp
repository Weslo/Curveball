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

void Boundary::SetUp(XMFLOAT3 u)
{
	up = u;
}