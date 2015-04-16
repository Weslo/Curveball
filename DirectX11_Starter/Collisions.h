#pragma once
#include "Boundary.h"
#include "Ball.h"
#include <DirectXMath.h>
#include <vector>
#include "Player.h"

class Collisions
{
public:
	Collisions();
	Collisions(std::vector<Boundary*> w);
	~Collisions();
	void DetectCollisions(Ball* b, Player* p, float dt);

private:

	std::vector<Boundary*> walls;


	XMVECTOR NearestPointOnSphere(Ball* b, Boundary* w, XMVECTOR bPos, XMVECTOR wPos);
	XMVECTOR NearestPointOnPlane(Ball* b, Boundary* w, XMVECTOR bPos, XMVECTOR wPos);
	XMVECTOR NearestPointOnPlayer(Ball* b, Player* p, XMVECTOR bPos);
	void AdvanceFrame();
	void ReflectBallWall(Ball* b, Boundary* w);
	void ReflectBallPlayer(Ball* b, Player* p);
};

