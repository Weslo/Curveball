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

XMFLOAT3 Ball::GetPrevPos()
{
	return prevPosition;
}

void Ball::SetVelocity(XMFLOAT3 v)
{
	velocity = v;
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

void Ball::SetPrevPos(XMFLOAT3 p)
{
	prevPosition = p;
}

void Ball::Update(float dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	position.z += velocity.z * dt;

	rotation.x += angularVelocity.x * dt;
	rotation.y += angularVelocity.y * dt;
	rotation.z += angularVelocity.z * dt;

	velocity.x += angularVelocity.x / 50;
	velocity.y += angularVelocity.y / 50;
}
