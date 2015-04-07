#pragma once
#include "GameEntity.h"
#include "Boundary.h"

class Ball :
	public GameEntity
{
public:
	Ball();
	Ball(float r, Mesh* m, Material* ma);
	~Ball();
	float GetRadius();
	XMFLOAT3 GetPrevPos();
	void SetPrevPos(XMFLOAT3 p);
	void SetVelocity(XMFLOAT3 v);
	XMFLOAT3 GetVelocity();
	XMFLOAT3 GetAngularVelocity();
	void SetAngularVelocity(XMFLOAT3);
	void Update(float dt);

private:

	float radius;
	float mass;
	XMFLOAT3 velocity;
	XMFLOAT3 angularVelocity;

	XMFLOAT3 prevPosition;
};

