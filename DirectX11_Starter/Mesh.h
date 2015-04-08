#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "Vertex.h"

#include <fstream>
#include <vector>

// For DirectX Math
using namespace DirectX;

class Mesh
{
public:
	// Constructors/Destructors
	Mesh(char* file, ID3D11Device* device);
	~Mesh();

	// Accessors
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	// Drawing
	void DrawMesh(ID3D11DeviceContext* deviceContext);
private:
	// Buffers to hold actual geometry
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
	// Specifies how many indices are in the mesh's index buffer
	UINT indexCount;
};