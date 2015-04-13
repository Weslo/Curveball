#pragma once
#include "GameEntity.h"
class Player :
	public GameEntity
{
public:
	Player(XMFLOAT3 pos, float w, float h, Mesh* m, Material* ma);
	~Player();

	XMFLOAT3 GetVelocity();
	void Update(XMFLOAT3 mPos, float w, XMFLOAT2 window);

private:
	XMFLOAT3 velocity;
	XMFLOAT3 previousPos;
	float zPos;
	float height;
	float width;

	void calcVelocity();
};

