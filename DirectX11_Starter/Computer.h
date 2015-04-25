#pragma once
#include "GameEntity.h"
#include "Ball.h"

class Computer :
	public GameEntity
{
public:
	Computer(XMFLOAT3 pos, float w, float h, Mesh* m, Material* ma);
	~Computer();
	XMFLOAT3 GetVelocity();
	XMFLOAT3 GetUp();
	float GetWidth();
	float GetHeight();
	std::vector<XMFLOAT3> GetPreviousPosition();
	void Update(Ball* b, XMFLOAT2 maxSpeed, int level, boolean loc, float dt);
	void AddPrevPos(XMFLOAT3);
	void ResetPrevPos();
	void CalcVelocity(float dt);
	void SetVelocity(XMFLOAT3 v);
	void TrackBall(Ball* b, XMFLOAT2 maxSpeed, int level, boolean loc, float dt);

private:
	XMFLOAT3 velocity;
	std::vector<XMFLOAT3> prevPosition;
	XMFLOAT3 up;
	XMFLOAT2 maxSpeed;
	float zPos;
	float height;
	float width;
};
