#pragma once
#include "GameEntity.h"

class Ball :
	public GameEntity
{
public:
	Ball(float r, Mesh* m, Material* ma);
	~Ball();
	float GetRadius();
	std::vector<XMFLOAT3> GetPrevPos();
	void AddPrevPos(XMFLOAT3 p);
	void ResetPrevPos();
	void SetVelocity(XMFLOAT3 v);
	void ApplyVelocity(XMFLOAT3 v);
	XMFLOAT3 GetVelocity();
	XMFLOAT3 GetAngularVelocity();
	void SetAngularVelocity(XMFLOAT3);
	void Update(float dt);

private:

	float radius;
	float mass;
	XMFLOAT3 velocity;
	XMFLOAT3 angularVelocity;
	std::vector<XMFLOAT3> prevPosition;
};

