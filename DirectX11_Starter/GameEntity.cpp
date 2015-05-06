#include "GameEntity.h"

#pragma region Constructors/Destructors

GameEntity::GameEntity(Mesh* _mesh, Material* _material)
{
	mesh = _mesh;
	material = _material;
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);
	RecalculateWorldMatrix();
}

GameEntity::~GameEntity()
{
}

#pragma endregion

#pragma region Accessors/Mutators

XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}

void GameEntity::RecalculateWorldMatrix()
{

	// Translation
	XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

	// Rotation
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	// Scale
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	// Transformation
	XMMATRIX transformationMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(transformationMatrix));
}

XMFLOAT3 GameEntity::GetPosition()
{
	return position;
}

void GameEntity::SetPosition(XMFLOAT3 _position)
{
	position = _position;
}

void GameEntity::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}

void GameEntity::Move(float x, float y, float z)
{
	SetPosition(position.x + x, position.y + y, position.z + z);
}

XMFLOAT3 GameEntity::GetRotation()
{
	return rotation;
}

/*
XMVECTOR GameEntity::GetRotationQuaternion()
{
	return XMQuaternionRotationRollPitchYawFromVector(GetRotation());
}*/

void GameEntity::SetRotation(XMFLOAT3 _rotation)
{
	rotation = _rotation;
}

void GameEntity::SetRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
}

XMFLOAT3 GameEntity::GetScale()
{
	return scale;
}

void GameEntity::SetScale(XMFLOAT3 _scale)
{
	scale = _scale;
}

void GameEntity::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
}

Mesh* GameEntity::GetMesh()
{
	return mesh;
}

Material* GameEntity::GetMaterial()
{
	return material;
}

#pragma endregion

#pragma region Rendering

void GameEntity::PrepareToDraw()
{
}

void GameEntity::Draw(ID3D11DeviceContext* deviceContext)
{
	mesh->DrawMesh(deviceContext);
}

#pragma endregion