#pragma once
#include "GameEntity.h"
class Boundary :
	public GameEntity
{
public:
	Boundary(int l, int w, Mesh* m, Material* ma);
	~Boundary();
	XMFLOAT3 GetUp();
	int GetLength();
	int GetWidth();
	void SetUp(XMFLOAT3 u);
	void SetColor(XMFLOAT4 _color);
	void RandomizeColor();

private:
	void CalcLength();
	void CalcWidth();

	XMFLOAT3 up;
	int length;
	int width;

	XMFLOAT4 color;
};