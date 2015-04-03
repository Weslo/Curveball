#pragma once

#include <DirectXMath.h>

// For DirectX Math
using namespace DirectX;

// Vertex struct for triangles
struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 UV;
};