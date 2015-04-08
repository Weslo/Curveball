#include "Collisions.h"


Collisions::Collisions()
{

}

Collisions::Collisions(std::vector<Boundary*> w)
{
	walls = w;
}

Collisions::~Collisions()
{
}

//Detect collisions between object.
//Needs to track player and enemy in the future
void Collisions::DetectCollisions(Ball* b, float dt)
{
	//Store a reference to the balls position
	XMVECTOR bPos = XMLoadFloat3(&b->GetPosition());

	//Check against every wall. Doesn't take dimensions into account because it doesn't need to
	for (int i = 0; i < walls.size(); i++)
	{
		//Store the wall position and the point on the wall closest to the ball
		XMVECTOR wPos = XMLoadFloat3(&walls[i]->GetPosition());
		XMVECTOR nearestPt = NearestPointOnPlane(b, walls[i], bPos, wPos);

		//Get the difference between the ball and the plane
		XMVECTOR dif = bPos - nearestPt;

		//Store the length
		dif = XMVector3Length(dif);

		//If the length of the difference is less than the radius, collision happened. Also need to check if it went through the wall
		if (abs(XMVectorGetX(dif)) < b->GetRadius())
		{
			//Collision was found, respond by moving the ball to the closest point inside the plane and reflecting it
			XMVECTOR prevP = XMLoadFloat3(&b->GetPrevPos());

			//Store radius as an XMVector to multiply by
			XMFLOAT3 radius = XMFLOAT3(b->GetRadius(), b->GetRadius(), b->GetRadius());
			XMVECTOR r = XMLoadFloat3(&radius);

			//r is now strictly in the direction the position needs to move
			r = XMVectorMultiply(r, XMLoadFloat3(&walls[i]->GetUp()));

			//Add the radius to the nearest point to scale the ball in the correct direction
			prevP = nearestPt + r;

			//Reflect the ball
			ReflectBallWall(b, walls[i]);
		}
	}
}

//Closest point on the sphere to a wall
//Currently unused
XMVECTOR Collisions::NearestPointOnSphere(Ball* b, Boundary* w, XMVECTOR bPos, XMVECTOR wPos)
{
	//Store radius as an XMVector. Negative used for directional purposes
	XMFLOAT3 radius = XMFLOAT3(-b->GetRadius(), -b->GetRadius(), -b->GetRadius());
	XMVECTOR r = XMLoadFloat3(&radius);

	//Get the Ball position and wall position based on wall up vector. Subtract wall pos from ball pos from the radius variable to get the closest you can in that direction.
	XMVECTOR axisBallPos = bPos * XMLoadFloat3(&w->GetUp());
	XMVECTOR axisWallPos = wPos * XMLoadFloat3(&w->GetUp());
	r = r * XMLoadFloat3(&w->GetUp());

	XMVECTOR result = axisBallPos - axisWallPos - r;

	//subtract result from the ball vector to get the closest point
	result = bPos - result;

	return result;
}

//Return the closest 
XMVECTOR Collisions::NearestPointOnPlane(Ball* b, Boundary* w, XMVECTOR bPos, XMVECTOR wPos)
{
	XMVECTOR axisBallPos = bPos * XMLoadFloat3(&w->GetUp());
	XMVECTOR axisWallPos = wPos * XMLoadFloat3(&w->GetUp());

	axisBallPos = axisWallPos - axisBallPos;

	bPos += axisBallPos;

	return bPos;
}

//Reflect the ball when a collision is found
void Collisions::ReflectBallWall(Ball* b, Boundary* w)
{
	//Store XMVector references to necessary values
	XMVECTOR bVel = XMLoadFloat3(&b->GetVelocity());
	XMVECTOR bAVel = XMLoadFloat3(&b->GetAngularVelocity());
	XMVECTOR bPos = XMLoadFloat3(&b->GetPosition());
	XMVECTOR wUp = XMLoadFloat3(&w->GetUp());

	//Moment of inertia for the ball
	float I = .4f * b->GetRadius() * b->GetRadius();

	//Get the radius in the direction of the wall up vector
	XMVECTOR R = XMVectorMultiply(wUp, XMLoadFloat3(&XMFLOAT3(-b->GetRadius(), -b->GetRadius(), -b->GetRadius())));

	//adjusted velocity based on the previous vector and angular velocity
	XMVECTOR v = bVel + XMVector3Cross(R, bAVel);

	//Normally gets divided by a value, but since we have a perfect sphere it breaks the equation. Don't need it for now (may need later)
	float numerator = -(1.01f + 1.0f) * XMVectorGetX(XMVector3Dot(v, wUp));

	//Impulse to apply to the ball
	XMVECTOR J = wUp * numerator;

	//Add the impulse to the velocity
	v = bVel + J;

	//The new angular velocity based on impulse and moment of inertia
	XMVECTOR aW = bAVel - XMVector3Cross(J, R) / I;

	//Set the new velocities
	XMFLOAT3 newVel = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&newVel, v);
	b->SetVelocity(newVel);

	XMFLOAT3 newAVel = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&newAVel, aW);
	//Don't set z or it will just bounce around the middle
	b->SetAngularVelocity(XMFLOAT3(newAVel.x, newAVel.y, b->GetAngularVelocity().z));
}