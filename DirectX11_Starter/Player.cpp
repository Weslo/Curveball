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

void Player::Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* camera, float dt)
{

	XMFLOAT3 worldMousePos = projectMouseToWorld(mPos, window, camera);
	position.x = worldMousePos.x;
	position.y = worldMousePos.y;

	velocity = XMFLOAT3((position.x - previousPos.x) / dt, (position.y - previousPos.y) / dt, (position.z - previousPos.z) / dt);
	previousPos = position;
}

XMFLOAT3 Player::projectMouseToWorld(XMFLOAT3 mousePos, XMFLOAT2 window, Camera* camera)
{
	XMVECTOR cubeCoord = XMVectorSet((mousePos.x / window.x * 2) - 1, -((mousePos.y / window.y * 2) - 1), 10, 0);
	XMMATRIX cameraInverse = XMLoadFloat4x4(&(camera->GetInverseMatrix()));
	XMVECTOR worldPos = XMVector3Transform(cubeCoord, cameraInverse);
	XMFLOAT3 _worldPos;
	XMStoreFloat3(&_worldPos, worldPos);
	return _worldPos;
}