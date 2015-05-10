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

std::vector<XMFLOAT3> Player::GetPreviousPosition()
{
	return prevPosition;
}

float Player::GetWidth()
{
	return width;
}

float Player::GetHeight()
{
	return height;
}

XMFLOAT3 Player::projectMouseToWorld(XMFLOAT3 mousePos, XMFLOAT2 window, Camera* camera)
{
	// http://webglfactory.blogspot.com/2011/05/how-to-convert-world-to-screen.html

	XMVECTOR cubeCoord = XMVectorSet(
		2 * (mousePos.x / window.x) - 1,
		-2 * (mousePos.y / window.y) + 1,
		0,	// Keep zero
		0);
	XMMATRIX cameraInverse = XMLoadFloat4x4(&(camera->GetInverseMatrix()));
	XMVECTOR worldPos = XMVector3Transform(cubeCoord, cameraInverse);

	XMFLOAT3 _worldPos;
	XMStoreFloat3(&_worldPos, worldPos);
	_worldPos.x *= 5;
	_worldPos.y *= 5; // This somehow scales it to work
	return _worldPos;
}

void Player::AddPrevPos(XMFLOAT3 p)
{
	prevPosition.push_back(p);
}

void Player::CalcVelocity(float dt)
{
	velocity = XMFLOAT3((position.x - prevPosition[0].x) / .1f, (position.y - prevPosition[0].y) / .1f, 0);
}

void Player::ResetPrevPos()
{
	prevPosition.clear();
}

void Player::Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* camera)
{
	XMFLOAT3 worldMousePos = projectMouseToWorld(mPos, window, camera);
	position.x = worldMousePos.x;
	position.y = worldMousePos.y;
}