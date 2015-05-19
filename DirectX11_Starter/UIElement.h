#pragma once
#include "GameEntity.h"
class UIElement :
	public GameEntity
{
public:
	UIElement(XMFLOAT3 pos, Mesh* m, Material* ma);
	~UIElement();

	void PrepareToDraw();
	void SetMesh(Mesh* m);
};

