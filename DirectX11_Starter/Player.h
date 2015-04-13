#pragma once
#include "GameEntity.h"
class Player :
	public GameEntity
{
public:
	Player(XMFLOAT3 pos, float w, float h, Mesh* m, Material* ma);
	~Player();

	XMFLOAT3 GetVelocity();
	XMFLOAT3 GetUp();
	float GetWidth();
	float GetHeight();
	void Update(XMFLOAT3 mPos, float w, XMFLOAT2 window);

private:
	XMFLOAT3 velocity;
	XMFLOAT3 previousPos;
	XMFLOAT3 up;
	float zPos;
	float height;
	float width;

	void calcVelocity();
};

