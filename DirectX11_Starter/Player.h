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
	std::vector<XMFLOAT3> GetPreviousPosition();
	void Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* camera);
	void AddPrevPos(XMFLOAT3);
	void ResetPrevPos();
	void CalcVelocity(float dt);

private:
	XMFLOAT3 velocity;
	std::vector<XMFLOAT3> prevPosition;
	XMFLOAT3 up;
	float zPos;
	float height;
	float width;

	XMFLOAT3 projectMouseToWorld(XMFLOAT3 mousePos, XMFLOAT2 window, Camera* camera);
};