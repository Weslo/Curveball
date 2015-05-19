#include "UIElement.h"


UIElement::UIElement(XMFLOAT3 pos, Mesh* m, Material* ma) : GameEntity(m, ma)
{
	position = pos;
}


UIElement::~UIElement()
{
}

void UIElement::PrepareToDraw()
{
	GameEntity::PrepareToDraw();
}

void UIElement::SetMesh(Mesh* m)
{
	mesh = m;
}
