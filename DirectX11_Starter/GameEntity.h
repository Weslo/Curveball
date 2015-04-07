#pragma once

#include<DirectXMath.h>
#include "Mesh.h"
#include "Material.h"

using namespace DirectX;

class GameEntity
{
public:
	// Constructors/Destructors
	GameEntity(Mesh* _mesh, Material* material);
	virtual ~GameEntity();

	// Accessors/Mutators
	XMFLOAT4X4 GetWorldMatrix();
	void RecalculateWorldMatrix();

	XMVECTOR GetPosition();
	void SetPosition(XMFLOAT3 _position);
	void SetPosition(float x, float y, float z);
	void Move(float x, float y, float z);

	XMVECTOR GetRotation();
	XMVECTOR GetRotationQuaternion();
	void SetRotation(XMFLOAT3 _rotation);
	void SetRotation(float x, float y, float z);
	void Rotate(float x, float y, float z);

	XMVECTOR GetScale();
	void SetScale(XMFLOAT3 _scale);
	void SetScale(float x, float y, float z);

	Mesh* GetMesh();
	Material* GetMaterial();

	// Rendering
	void Draw(ID3D11DeviceContext* deviceContext);
protected:
	// Transformations
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	
	// Rendering
	Mesh* mesh;
	Material* material;
};