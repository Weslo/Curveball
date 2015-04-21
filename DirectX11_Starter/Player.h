#pragma once
#include "GameEntity.h"
#include "Camera.h"
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
	void Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* camera, float dt);

private:
	XMFLOAT3 velocity;
	XMFLOAT3 previousPos;
	XMFLOAT3 up;
	float zPos;
	float height;
	float width;

	XMFLOAT3 projectMouseToWorld(XMFLOAT3 mousePos, XMFLOAT2 window, Camera* camera);
};

