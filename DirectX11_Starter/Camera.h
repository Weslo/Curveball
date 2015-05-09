#pragma once

#include<DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera(XMFLOAT3 pos);
	~Camera();

	XMFLOAT4X4 GetViewMatrix();
	void RecalculateViewMatrix();

	XMFLOAT4X4 GetProjectionMatrix();
	void RecalculateProjectionMatrix(float aspectRatio);

	XMFLOAT4X4 GetInverseMatrix();

	XMFLOAT3 GetPosition();
	void TranslateLocalAxis(XMVECTOR localDirection, float distance);
	void TranslateWorldAxis(XMVECTOR worldDirection, float distance);

	XMVECTOR GetDirection();
	void Rotate(float x, float y);

	void HandleInput(float dt);
	void Update(float dt);
private:
	float xRotation;
	float yRotation;

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	XMFLOAT3 position;
	XMFLOAT3 direction;
};