#include "Camera.h"
#include "Windows.h"

Camera::Camera(XMFLOAT3 pos)
{
	xRotation = 0;
	yRotation = 0;
	position = pos;
	direction = XMFLOAT3(0, 0, 1);
}

Camera::~Camera()
{
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

void Camera::RecalculateViewMatrix()
{
	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0);
	XMVECTOR forward = XMVectorSet(0, 0, 1, 0);
	XMVECTOR _direction = XMVector3Rotate(forward, rotationQuaternion);
	XMStoreFloat3(&direction, _direction);

	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(XMLoadFloat3(&position), GetDirection(), up);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

XMFLOAT4X4 Camera::GetInverseMatrix()
{
	// Multiply matrices and take inverse
	XMMATRIX _inverse = XMMatrixInverse(NULL, XMMatrixMultiply(XMLoadFloat4x4(&projectionMatrix), XMLoadFloat4x4(&viewMatrix)));
	XMFLOAT4X4 inverse;
	XMStoreFloat4x4(&inverse, _inverse);
	return inverse;
}

void Camera::RecalculateProjectionMatrix(float aspectRatio)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

XMFLOAT3 Camera::GetPosition()
{
	return position;
}

void Camera::TranslateLocalAxis(XMVECTOR localDirection, float distance)
{
	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0);
	XMVECTOR _direction = XMVector3Rotate(localDirection, rotationQuaternion);
	_direction = XMVector3Normalize(_direction);
	XMVECTOR translation = XMVectorScale(_direction, distance);
	XMVECTOR _position = XMVectorAdd(XMLoadFloat3(&position), translation);
	XMStoreFloat3(&position, _position);
}

void Camera::TranslateWorldAxis(XMVECTOR worldDirection, float distance)
{
	XMVECTOR _direction = XMVector3Normalize(worldDirection);
	XMVECTOR translation = XMVectorScale(_direction, distance);
	XMVECTOR _position = XMVectorAdd(XMLoadFloat3(&position), translation);
	XMStoreFloat3(&position, _position);
}

XMVECTOR Camera::GetDirection()
{
	return XMLoadFloat3(&direction);
}

void Camera::Rotate(float x, float y)
{
	xRotation += x;
	yRotation += y;
}

void Camera::HandleInput(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000) { TranslateLocalAxis(XMVectorSet(0, 0, 1, 0), dt); }
	if (GetAsyncKeyState('S') & 0x8000) { TranslateLocalAxis(XMVectorSet(0, 0, -1, 0), dt); }
	if (GetAsyncKeyState('A') & 0x8000) { TranslateLocalAxis(XMVectorSet(-1, 0, 0, 0), dt); }
	if (GetAsyncKeyState('D') & 0x8000) { TranslateLocalAxis(XMVectorSet(1, 0, 0, 0), dt); }
	if (GetAsyncKeyState(' ') & 0x8000) { TranslateWorldAxis(XMVectorSet(0, 1, 0, 0), dt); }
	if (GetAsyncKeyState('X') & 0x8000) { TranslateWorldAxis(XMVectorSet(0, -1, 0, 0), dt); }
}

void Camera::Update(float dt)
{
	HandleInput(dt);
	RecalculateViewMatrix();
}