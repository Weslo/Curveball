#pragma once
#include "Boundary.h"
#include "Ball.h"
#include <DirectXMath.h>
#include <vector>

class Collisions
{
public:
	Collisions();
	Collisions(std::vector<Boundary> w);
	~Collisions();
	Ball DetectCollisions(Ball b, float dt);

private:

	std::vector<Boundary> walls;

	
	XMVECTOR NearestPointOnSphere(Ball b, Boundary w, XMVECTOR bPos, XMVECTOR wPos);
	XMVECTOR NearestPointOnPlane(Ball b, Boundary w, XMVECTOR bPos, XMVECTOR wPos);
	void AdvanceFrame();
	Ball ReflectBallWall(Ball b, Boundary w);
};

