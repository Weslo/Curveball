#pragma once
#include "Boundary.h"
#include "Ball.h"
#include <DirectXMath.h>
#include <vector>
#include "Player.h"
#include "Computer.h"

class Collisions
{
public:
	Collisions();
	Collisions(std::vector<Boundary*> w);
	~Collisions();
	void DetectCollisions(Ball* b, Player* p, Computer* c, XMFLOAT3 maxSpeed, XMFLOAT3 maxAngularSpeed, float dt);

private:

	std::vector<Boundary*> walls;

	XMVECTOR NearestPointOnSphere(Ball* b, Boundary* w, XMVECTOR bPos, XMVECTOR wPos);
	XMVECTOR NearestPointOnPlane(Ball* b, Boundary* w, XMVECTOR bPos, XMVECTOR wPos);
	XMVECTOR NearestPointOnPlayer(Ball* b, Player* p);
	XMVECTOR NearestPointOnComputer(Ball* b, Computer* c);
	void AdvanceFrame();
	void ReflectBallWall(Ball* b, Boundary* w, XMFLOAT3 maxSpeed, XMFLOAT3 maxAngularSpeed);
	void ReflectBallPlayer(Ball* b, Player* p, XMFLOAT3 maxSpeed, XMFLOAT3 maxAngularSpeed);
	void ReflectBallComputer(Ball* b, Computer* c, XMFLOAT3 maxSpeed, XMFLOAT3 maxAngularSpeed);
};

