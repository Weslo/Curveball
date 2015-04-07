#include "Collisions.h"


Collisions::Collisions()
{

}

Collisions::Collisions(std::vector<Boundary> w)
{
	walls = w;
}

Collisions::~Collisions()
{
}

//Needs to take enemy/player as well
Ball Collisions::DetectCollisions(Ball b, float dt)
{
	XMVECTOR bPos = XMLoadFloat3(&b.GetPosition());
	
	for (int i = 0; i < walls.size(); i++)
	{
		XMVECTOR wPos = XMLoadFloat3(&walls[i].GetPosition());
		
		XMVECTOR nearestPt = NearestPointOnPlane(b, walls[i], bPos, wPos);

		//Get the difference between the ball and the plane
		XMVECTOR dif = bPos - nearestPt;

		//Store the length
		dif = XMVector3Length(dif);

		XMFLOAT3 axisDif = XMFLOAT3(0, 0, 0);
		XMStoreFloat3(&axisDif, dif);

		float length = abs(axisDif.x);

		//If the length of the difference is less than the radius, collision happened. Also need to check if it went through the wall
		if (length < b.GetRadius())
		{
			//Collision was found, respond by moving the ball to the closest point inside the plane and reflecting it
			XMVECTOR prevP = XMLoadFloat3(&b.GetPrevPos());
			
			//dif = prevP - nearestPt;
			//XMStoreFloat3(&axisDif, dif);

			//XMVECTOR axisWallPos = wPos * XMLoadFloat3(&walls[i].GetUp());
			XMFLOAT3 radius = XMFLOAT3(b.GetRadius(), b.GetRadius(), b.GetRadius());
			XMVECTOR r = XMLoadFloat3(&radius);

			//r is now strictly in the direction the position needs to move
			r = XMVectorMultiply(r, XMLoadFloat3(&walls[i].GetUp()));

			prevP = nearestPt + r;

			

			//Reflect the ball
			b = ReflectBallWall(b, walls[i]);

			return b;
		}
	}
	return b;
}

XMVECTOR Collisions::NearestPointOnSphere(Ball b, Boundary w, XMVECTOR bPos, XMVECTOR wPos)
{
	XMFLOAT3 radius = XMFLOAT3(-b.GetRadius(), -b.GetRadius(), -b.GetRadius());
	XMVECTOR r = XMLoadFloat3(&radius);
	//Get the Ball position and wall position based on wall up vector. Subtract wall pos from ball pos from the radius variable to get the closest you can in that direction.
	XMVECTOR axisBallPos = bPos * XMLoadFloat3(&w.GetUp());
	XMVECTOR axisWallPos = wPos * XMLoadFloat3(&w.GetUp());
	r = r * XMLoadFloat3(&w.GetUp());

	XMVECTOR result = axisBallPos - axisWallPos - r;
	
	//Add result to the ball vector to get the closest point
	result = bPos - result;

	return result;
}



XMVECTOR Collisions::NearestPointOnPlane(Ball b, Boundary w, XMVECTOR bPos, XMVECTOR wPos)
{
	XMVECTOR axisBallPos = bPos * XMLoadFloat3(&w.GetUp());
	XMVECTOR axisWallPos = wPos * XMLoadFloat3(&w.GetUp());

	axisBallPos = axisWallPos - axisBallPos;

	bPos += axisBallPos;

	return bPos;
}

Ball Collisions::ReflectBallWall(Ball b, Boundary w)
{
	XMVECTOR bVel = XMLoadFloat3(&b.GetVelocity());
	XMVECTOR bAVel = XMLoadFloat3(&b.GetAngularVelocity());
	XMVECTOR bPos = XMLoadFloat3(&b.GetPosition());
	XMVECTOR wUp = XMLoadFloat3(&w.GetUp());
	float I = .4f * b.GetRadius() * b.GetRadius();
	
	XMVECTOR R = XMVectorMultiply(wUp, XMLoadFloat3(&XMFLOAT3(-b.GetRadius(), -b.GetRadius(), -b.GetRadius())));

	XMVECTOR v = bVel + XMVector3Cross(R, bAVel);

	float numerator = -(1.01f + 1.0f) * XMVectorGetX(XMVector3Dot(v, wUp));

	XMVECTOR J = wUp * numerator;

	v = bVel + J;
	XMVECTOR aW = bAVel - XMVector3Cross(J, R) / I;

	XMFLOAT3 newVel = XMFLOAT3(0, 0, 0); 
	XMStoreFloat3(&newVel, v);

	b.SetVelocity(newVel);

	XMFLOAT3 newAVel = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&newAVel, aW);

	//Don't set z or it will just bounce around the middle
	b.SetAngularVelocity(XMFLOAT3(newAVel.x, newAVel.y, b.GetAngularVelocity().z));

	return b;
}