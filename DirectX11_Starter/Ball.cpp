#include "Ball.h"

Ball::Ball(float r, Mesh* m, Material* ma) : GameEntity(m, ma)
{
	radius = r;
	velocity = XMFLOAT3(0, 0, 0);
	angularVelocity = XMFLOAT3(0, 0, 0);
}


Ball::~Ball()
{
}

float Ball::GetRadius()
{
	return radius;
}

std::vector<XMFLOAT3> Ball::GetPrevPos()
{
	return prevPosition;
}

void Ball::SetVelocity(XMFLOAT3 v)
{
	velocity = v;
}

void Ball::ApplyVelocity(XMFLOAT3 v)
{
	velocity = XMFLOAT3(velocity.x + v.x, velocity.y + v.y, velocity.z + v.z);
}

void Ball::SetAngularVelocity(XMFLOAT3 v)
{
	angularVelocity = v;
}

XMFLOAT3 Ball::GetVelocity()
{
	return velocity;
}

XMFLOAT3 Ball::GetAngularVelocity()
{
	return angularVelocity;
}

void Ball::AddPrevPos(XMFLOAT3 p)
{
	prevPosition.push_back(p);
}

void Ball::ResetPrevPos()
{
	prevPosition.clear();
}

void Ball::Update(float dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	position.z += velocity.z * dt;

	rotation.x += angularVelocity.y * dt;
	rotation.y += angularVelocity.x * dt;

	velocity.x += angularVelocity.x / 150;
	velocity.y += angularVelocity.y / 150;
}
