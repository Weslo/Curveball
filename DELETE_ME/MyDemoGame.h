#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "GameEntity.h"
#include <ctime>
#include <vector>
#include "Camera.h"
#include <iostream>
#include "SimpleShader.h"
#include <WICTextureLoader.h>
#include "Boundary.h"
#include "Ball.h"
#include "Collisions.h"
#include "Player.h"

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;

// Struct to match vertex shader's constant buffer
// You update one of these locally, then push it to the corresponding
// constant buffer on the device when it needs to be updated
struct VertexShaderConstantBufferLayout
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct DirectionalLight
{
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

// Demo class which extends the base DirectXGame class
class MyDemoGame : public DirectXGame
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo
	void CreateBoundaries();
	void CreateBall();
	void CreatePlayer();
	void LoadShadersAndInputLayout();
	void InitializeCameraMatrices();

private:
	// Buffers to hold actual geometry
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// The matrices to go from model space
	// to screen space
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	Material* mat1;
	Material* mat2;

	DirectionalLight lightA;
	DirectionalLight lightB;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//Mesh objects
	std::vector<Mesh*> meshes;

	//Game objects
	std::vector<GameEntity> entities;

	//Boundaries
	std::vector<Boundary> walls;
	
	Camera cam;

	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Ball ball;
	Player player;

	Collisions collisionManager;
};