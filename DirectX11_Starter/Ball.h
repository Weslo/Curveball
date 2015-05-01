#pragma once
#include "GameEntity.h"
#include "Lighting.h"

class Ball :
	public GameEntity
{
public:
	Ball(float r, Mesh* m, Material* ma);
	~Ball();

	float GetRadius();
	XMFLOAT3 GetVelocity();
	XMFLOAT3 GetAngularVelocity();
	std::vector<XMFLOAT3> GetPrevPos();
	Lighting* GetBallLight();
	void SetVelocity(XMFLOAT3 v);
	void SetAngularVelocity(XMFLOAT3);
	void SetBallLight(Lighting* l);
	void AddPrevPos(XMFLOAT3 p);
	void ResetPrevPos();
	void ApplyVelocity(XMFLOAT3 v);
	void RandomizeBallLight();
	
	void Update(float dt);

private:

	float radius;
	float mass;
	XMFLOAT3 velocity;
	XMFLOAT3 angularVelocity;
	std::vector<XMFLOAT3> prevPosition;

	Lighting* ballLight;
};

