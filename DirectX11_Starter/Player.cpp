#include "Player.h"


Player::Player(XMFLOAT3 pos, float w, float h, Mesh* m, Material* ma) : GameEntity(m, ma)
{
	position = pos;
	zPos = pos.z;

	height = h;
	width = w;

	up = XMFLOAT3(0, 0, 1.0f);
}

Player::~Player()
{
}

XMFLOAT3 Player::GetVelocity()
{
	return velocity;
}

XMFLOAT3 Player::GetUp()
{
	return up;
}

float Player::GetWidth()
{
	return width;
}

float Player::GetHeight()
{
	return height;
}

void Player::Update(XMFLOAT3 mPos, float w,  XMFLOAT2 window)
{	
	//w is thw width of the wall
	//We know the size of the tunnel holding the paddle at all times
	//Only need to account so the ends don't go outside.
	//Map between the walls while accounting for paddle size

	//Bounds to follow based on the wall and the fact that our game is centered around 0,0
	float lBound = -w / 2 + width / 2;
	float rBound = w / 2 - width / 2;
	float uBound = -w / 2 + height / 2;
	float dBound = w / 2 - height / 2;

	float percent = (mPos.x / window.x) * (rBound - lBound) - rBound;

	position.x = percent;

	percent = (mPos.y / window.y) * (dBound - uBound) - dBound;

	position.y = -percent;

	previousPos = position;
}
