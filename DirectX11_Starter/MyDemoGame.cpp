// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include <Windows.h>
#include <d3dcompiler.h>
#include "MyDemoGame.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Make the game, initialize and run
	MyDemoGame game(hInstance);
	
	// If we can't initialize, we can't run
	if( !game.Init() )
		return 0;
	
	// All set to run the game
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

MyDemoGame::MyDemoGame(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Demo DX11 Game";
	windowWidth = 800;
	windowHeight = 600;
}

MyDemoGame::~MyDemoGame()
{
	delete camera;

	// Delete game entities and the game entities array.
	for (std::vector< GameEntity* >::iterator it = entities.begin(); it != entities.end(); ++it) { delete (*it); }
	entities.clear();

	// Delete meshes and the mesh array.
	for (std::vector< Mesh* >::iterator it = meshes.begin(); it != meshes.end(); ++it) { delete (*it); }
	meshes.clear();

	// Delete meshes and the mesh array.
	for (std::vector< Material* >::iterator it = materials.begin(); it != materials.end(); ++it) { delete (*it); }
	materials.clear();

	delete pixelShader;
	delete vertexShader;

	ReleaseMacro(srv);
	ReleaseMacro(samplerState);
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool MyDemoGame::Init()
{
	// Make sure DirectX initializes properly
	if( !DirectXGame::Init() )
		return false;

	// Create the necessary DirectX buffers to draw something
	CreateGeometryBuffers();

	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	// Set up camera-related matrices
	InitializeCameraMatrices();

	directionalLight = DirectionalLight
	{
		XMFLOAT4(0, 0, 0.1f, 1),
		XMFLOAT4(0, 0, 1, 1),
		XMFLOAT3(1, 0, 1)
	};

	secondLight = DirectionalLight
	{
		XMFLOAT4(0.1f, 0, 0, 1),
		XMFLOAT4(1, 0, 0, 1),
		XMFLOAT3(-1, 0, 1)
	};


	pixelShader->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	pixelShader->SetData(
		"secondDirectionalLight",
		&secondLight,
		sizeof(DirectionalLight));

	// Successfully initialized
	return true;
}

// Creates the vertex and index buffers for a single triangle
void MyDemoGame::CreateGeometryBuffers()
{
	// Create entities

	pixelShader = new SimplePixelShader(device, deviceContext);
	vertexShader = new SimpleVertexShader(device, deviceContext);

	CreateWICTextureFromFile(device, deviceContext, L"../Assets/checkers.png", 0, &srv);

	// Sampler State
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	materials.push_back(new Material(pixelShader, vertexShader, srv, samplerState));

	meshes.push_back(new Mesh("../Assets/sphere.obj", device));
	meshes.push_back(new Mesh("../Assets/boundary2.obj", device));

	ball = new Ball (0.5f, meshes[0], materials[0]);
	entities.push_back(ball);

	// Bottom Wall
	walls.push_back(new Boundary(40, 10, meshes[1], materials[0]));
	walls[0]->SetPosition(XMFLOAT3(0.0, -5.0f, 0.0f));
	walls[0]->SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));
	walls[0]->SetScale(XMFLOAT3(40.0f, 40.0f, 40.0f));
	entities.push_back(walls[0]);

	// Left Wall
	walls.push_back(new Boundary(40, 10, meshes[1], materials[0]));
	walls[1]->SetPosition(XMFLOAT3(-5.0f, 0.0f, 0.0f));
	walls[1]->SetUp(XMFLOAT3(1.0f, 0.0f, 0.0f));
	walls[1]->SetRotation(XMFLOAT3(0.0f, 0.0f, 3 * XM_PI / 2));
	walls[1]->SetScale(XMFLOAT3(40.0f, 40.0f, 40.0f));
	entities.push_back(walls[1]);

	// Top Wall
	walls.push_back(new Boundary(40, 10, meshes[1], materials[0]));
	walls[2]->SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	walls[2]->SetUp(XMFLOAT3(0.0f, -1.0f, 0.0f));
	walls[2]->SetRotation(XMFLOAT3(0.0f, 0.0f, XM_PI));
	walls[2]->SetScale(XMFLOAT3(40.0f, 40.0f, 40.0f));
	entities.push_back(walls[2]);

	// Right Wall
	walls.push_back(new Boundary(40, 10, meshes[1], materials[0]));
	walls[3]->SetPosition(XMFLOAT3(5.0f, 0.0f, 0.0f));
	walls[3]->SetUp(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	walls[3]->SetRotation(XMFLOAT3(0.0f, 0.0f, XM_PI / 2));
	walls[3]->SetScale(XMFLOAT3(40.0f, 40.0f, 40.0f));
	entities.push_back(walls[3]);

	// Back Wall
	walls.push_back(new Boundary(40, 10, meshes[1], materials[0]));
	walls[4]->SetPosition(XMFLOAT3(0.0f, 5.0f, 20.0f));
	walls[4]->SetUp(XMFLOAT3(0.0f, 0.0f, -1.0f));
	walls[4]->SetRotation(XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f));
	walls[4]->SetScale(XMFLOAT3(40.0f, 40.0f, 40.0f));
	entities.push_back(walls[4]);
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void MyDemoGame::LoadShadersAndInputLayout()
{
	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	camera = new Camera();
	camera->RecalculateViewMatrix();
	camera->RecalculateProjectionMatrix(AspectRatio());
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();
	
	// Recalculate projection matrix
	if (camera != NULL)
		camera->RecalculateProjectionMatrix(AspectRatio());
}
#pragma endregion

#pragma region Game Loop

// Update your game state
void MyDemoGame::UpdateScene(float dt)
{
	camera->Update(dt);
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the buffer (erases what's on the screen)
	//  - Do this once per frame
	//  - At the beginning (before drawing anything)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Set up the input assembler
	//  - These technically don't need to be set every frame, unless you're changing the
	//    input layout (different kinds of vertices) or the topology (different primitives)
	//    between draws
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Iterate through each mesh to perform draw operations on each
	for (int i = 0; i < entities.size(); i++)
	{

		// Copy CPU-side data to a single CPU-side structure
		//  - Allows us to send the data to the GPU buffer in one step
		//  - Do this PER OBJECT, before drawing it
		entities[i]->RecalculateWorldMatrix();
		entities[i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("world", entities[i]->GetWorldMatrix());
		entities[i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("view", camera->GetViewMatrix());
		entities[i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("projection", camera->GetProjectionMatrix());

		entities[i]->GetMaterial()->GetVertexShader()->SetShader();

		pixelShader->SetShaderResourceView("diffuseTexture", srv);
		pixelShader->SetSamplerState("basicSampler", samplerState);

		entities[i]->GetMaterial()->GetPixelShader()->SetShader();

		// Draw the mesh
		entities[i]->Draw(deviceContext);
	}

	// Present the buffer
	//  - Puts the stuff on the screen
	//  - Do this EXACTLY once per frame
	//  - Always at the end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	prevMousePos.x = x;
	prevMousePos.y = y;
	mouseDown = true;

	SetCapture(hMainWnd);
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	mouseDown = false;
	ReleaseCapture();
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	LONG deltaX = x - prevMousePos.x;
	LONG deltaY = y - prevMousePos.y;

	if (mouseDown)
	{
		//entities[0]->Rotate(-deltaY * 0.01f, -deltaX * 0.01f, 0);
	}

	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion