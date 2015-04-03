#pragma once

#include <DirectXMath.h>
#include <vector>
#include "DirectXGame.h"
#include "Camera.h"
#include "GameEntity.h"
#include "Material.h"
#include "Ball.h"
#include "Boundary.h"

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

// Directional light struct for shaders
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
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void InitializeCameraMatrices();

private:

	char* assetsPath;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	bool mouseDown;

	// Main camera
	Camera* camera;

	// Pointers to Mesh objects
	std::vector<GameEntity*> entities;

	// TODO: MAD MEMORY LEAKS MAYBE
	// Game Objects
	Ball* ball;
	std::vector<Boundary*> walls;

	// Meshes
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	
	// Shaders
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
	ID3D11ShaderResourceView* srv;

	// Sampler State
	ID3D11SamplerState* samplerState;
	D3D11_SAMPLER_DESC samplerDesc;

	// Lights
	DirectionalLight directionalLight;
	DirectionalLight secondLight;
};