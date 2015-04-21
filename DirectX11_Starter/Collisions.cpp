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

inline float squared(float v) { return v * v; }

//Detect collisions between object.
//Needs to track player and enemy in the future
void Collisions::DetectCollisions(Ball* b, Player* p, float dt)
{
	//Store a reference to the balls position
	XMVECTOR bPos = XMLoadFloat3(&b->GetPosition());

	//Check against every wall. Doesn't take dimensions into account because it doesn't need to
	for (unsigned int i = 0; i < walls.size(); i++)
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
			XMVECTOR nearestPt = NearestPointOnPlane(b, walls[i], bPos, wPos);

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

	//Player collisions
	float dist_squared = b->GetRadius() * b->GetRadius();

	XMFLOAT3 S = XMFLOAT3(b->GetPosition().x, b->GetPosition().y, b->GetPosition().z);
	XMFLOAT3 C1 = XMFLOAT3(p->GetPosition().x - p->GetWidth() / 2, p->GetPosition().y - p->GetHeight() / 2, p->GetPosition().z);
	XMFLOAT3 C2 = XMFLOAT3(p->GetPosition().x + p->GetWidth() / 2, p->GetPosition().y + p->GetHeight() / 2, p->GetPosition().z);

	if (S.x < C1.x) dist_squared -= squared(S.x - C1.x);
	else if (S.x > C2.x) dist_squared -= squared(S.x - C2.x);
	if (S.y < C1.y) dist_squared -= squared(S.y - C1.y);
	else if (S.y > C2.y) dist_squared -= squared(S.y - C2.y);
	if (S.z < C1.z) dist_squared -= squared(S.z - C1.z);
	else if (S.z > C2.z) dist_squared -= squared(S.z - C2.z);

	if (dist_squared > 0)
	{
		//Collision was found, respond by moving the ball to the closest point inside the plane and reflecting it
		XMVECTOR nearestPt = NearestPointOnPlayer(b, p, bPos);

		//Difference between the center and th point of contact, determines extra spin applied to the bal to speed up the game
		XMVECTOR contactPt = bPos - XMLoadFloat3(&p->GetPosition());

		XMVECTOR prevP = XMLoadFloat3(&b->GetPrevPos());

		//Store radius as an XMVector to multiply by
		XMFLOAT3 radius = XMFLOAT3(b->GetRadius(), b->GetRadius(), b->GetRadius());
		XMVECTOR r = XMLoadFloat3(&radius);

		//r is now strictly in the direction the position needs to move
		r = XMVectorMultiply(r, XMLoadFloat3(&p->GetUp()));

		//Add the radius to the nearest point to scale the ball in the correct direction
		prevP = nearestPt + r;

		//Reflect the ball
		ReflectBallPlayer(b, p, contactPt);
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

//Return the closest 
XMVECTOR Collisions::NearestPointOnPlayer(Ball* b, Player* p, XMVECTOR bPos)
{
	XMVECTOR axisBallPos = bPos * XMLoadFloat3(&p->GetUp());
	XMVECTOR axisPlayerPos = XMLoadFloat3(&p->GetPosition()) * XMLoadFloat3(&p->GetUp());

	axisBallPos = axisPlayerPos - axisBallPos;

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
	float numerator = -(1.0f + 1.0f) * XMVectorGetX(XMVector3Dot(v, wUp));

	//Impulse to apply to the ball
	XMVECTOR J = wUp * numerator;

	//Add the impulse to the velocity
	v = bVel + J;

	//The new angular velocity based on impulse and moment of inertia
	XMVECTOR aW = bAVel * .9f;

	//Set the new velocities
	XMFLOAT3 newVel = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&newVel, v);
	b->SetVelocity(newVel);

	XMFLOAT3 newAVel = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&newAVel, aW);
	//Don't set z or it will just bounce around the middle
	b->SetAngularVelocity(XMFLOAT3(newAVel.x, newAVel.y, b->GetAngularVelocity().z));
}

//Reflect the ball when a collision is found
void Collisions::ReflectBallPlayer(Ball* b, Player* p, XMVECTOR contact)
{
	//Store XMVector references to necessary values
	XMVECTOR bVel = XMLoadFloat3(&b->GetVelocity());
	XMVECTOR bAVel = XMLoadFloat3(&b->GetAngularVelocity());
	XMVECTOR bPos = XMLoadFloat3(&b->GetPosition());

	XMVECTOR pUp = XMLoadFloat3(&p->GetUp());

	//Moment of inertia for the ball
	float I = .4f * b->GetRadius() * b->GetRadius();

	//Get the radius in the direction of the wall up vector
	XMVECTOR R = XMVectorMultiply(pUp, XMLoadFloat3(&XMFLOAT3(-b->GetRadius(), -b->GetRadius(), -b->GetRadius())));

	//adjusted velocity based on the previous vector and angular velocity
	XMVECTOR v = bVel + XMVector3Cross(R, bAVel);

	//Normally gets divided by a value, but since we have a perfect sphere it breaks the equation. Don't need it for now (may need later)
	float numerator = -(1.0f + 1.0f) * XMVectorGetX(XMVector3Dot(v, pUp));

	//Impulse to apply to the ball
	XMVECTOR J = pUp * numerator;

	//Add the impulse to the velocity
	v = bVel + J;

	//The new angular velocity based on impulse and moment of inertia
	XMVECTOR playerVelocity = XMLoadFloat3(&p->GetVelocity());
	XMVECTOR aW = bAVel - playerVelocity * .8f;

	//Set the new velocities
	XMFLOAT3 newVel = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&newVel, v);
	b->SetVelocity(newVel);

	XMFLOAT3 newAVel = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&newAVel, aW);
	//Don't set z or it will just bounce around the middle
	b->SetAngularVelocity(XMFLOAT3(newAVel.x, newAVel.y, b->GetAngularVelocity().z));
}