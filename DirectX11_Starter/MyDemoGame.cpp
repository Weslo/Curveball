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
	delete manager;
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

	manager = new GameManager(device, deviceContext);


	//Create shaders first
	manager->CreatePixelShader();
	manager->CreateVertexShader();

	manager->CreatePixelShader();
	manager->CreateVertexShader();

	//Load the textures you want to use
	manager->CreateResourceView(L"../Assets/wall.png");
	manager->CreateResourceView(L"../Assets/ballTex.png");
	manager->CreateResourceView(L"../Assets/paddle.png");

	//Create the sampler state.
	//Could take U/V/W states later for more options for textures
	manager->CreateSamplerState();


	//Create materials and meshes that will be used based on previous creation of stuff

	//walls
	manager->CreateMaterial(manager->GetVertexShaders()[0], manager->GetPixelShaders()[0], manager->GetResourceViews()[0], manager->GetSamplerStates()[0], L"WallPixelShader.cso", L"WallVertexShader.cso");
	//ball																																					 
	manager->CreateMaterial(manager->GetVertexShaders()[0], manager->GetPixelShaders()[0], manager->GetResourceViews()[1], manager->GetSamplerStates()[0], L"PixelShader.cso", L"VertexShader.cso");
	//paddle
	manager->CreateMaterial(manager->GetVertexShaders()[0], manager->GetPixelShaders()[0], manager->GetResourceViews()[2], manager->GetSamplerStates()[0], L"PixelShader.cso", L"VertexShader.cso");

	manager->CreateMesh("../Assets/wall.obj");
	manager->CreateMesh("../Assets/sphere.obj");
	manager->CreateMesh("../Assets/paddle.obj");

	XMFLOAT3 wScale = XMFLOAT3(20.0f, 20.0f, 20.0f);

	manager->CreateWall(20, 5, XMFLOAT3(0, -2.5f, 0), XMFLOAT3(0, 0, 0), wScale, XMFLOAT3(0, 1.0f, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Bottom wall
	manager->CreateWall(20, 5, XMFLOAT3(-2.5f, 0, 0), XMFLOAT3(0, 0, -XM_PI / 2), wScale, XMFLOAT3(1.0f, 0, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Left Wall
	manager->CreateWall(20, 5, XMFLOAT3(0, 2.5f, 0), XMFLOAT3(0, 0, XM_PI), wScale, XMFLOAT3(0, -1.0f, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Top wall
	manager->CreateWall(20, 5, XMFLOAT3(2.5f, 0, 0), XMFLOAT3(0, 0, XM_PI / 2), wScale, XMFLOAT3(-1.0, 0, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Right wall
	manager->CreateWall(20, 5, XMFLOAT3(0, 2.5f, 10.0f), XMFLOAT3(-XM_PI / 2, 0, 0), wScale, XMFLOAT3(0, 0, -1.0f), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Temp back wall

	manager->CreateBall(.25f, manager->GetMeshes()[1], manager->GetMaterials()[1]);
	manager->GetBalls()[0]->SetScale(.5f, .5f, .5f);

	manager->CreatePlayer(XMFLOAT3(0, 0, -8), 1.33f, 1, manager->GetMeshes()[2], manager->GetMaterials()[2]);
	manager->GetPlayer()->SetRotation(0, XM_PI/2, 0);

	manager->CreateGameController(manager->GetBalls()[0], manager->GetPlayer());

	//Now that we have walls, create the collision manager
	collisionManager = Collisions(manager->GetWalls());

	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	//Create the camera in here. Pain in the ass to do in game manager
	camera = new Camera();
	camera->RecalculateViewMatrix();
	camera->RecalculateProjectionMatrix(AspectRatio());

	//Havent moved to game manager yet, need to finalize structs first
	directionalLight = DirectionalLight
	{
		XMFLOAT4(.2f, .2f, .2f, 1),
		XMFLOAT4(.6f, .6f, .6f, 1),
		XMFLOAT3(.5f, .5f, .5f)
	};

	secondLight = DirectionalLight
	{
		XMFLOAT4(0.2f, 0.2f, 0.2f, 1),
		XMFLOAT4(0.6f, 0.6f, 0.6f, 1),
		XMFLOAT3(-.5f, -.5f, -.5f)
	};


	manager->GetPixelShaders()[0]->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	manager->GetPixelShaders()[0]->SetData(
		"secondDirectionalLight",
		&secondLight,
		sizeof(DirectionalLight));

	manager->GetPixelShaders()[1]->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	manager->GetPixelShaders()[1]->SetData(
		"secondDirectionalLight",
		&secondLight,
		sizeof(DirectionalLight));


	// Successfully initialized
	return true;
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

	//Set ball prev position before updating, used for collision detection
	manager->GetBalls()[0]->SetPrevPos(manager->GetBalls()[0]->GetPosition());
	manager->GetBalls()[0]->Update(dt);

	manager->GetPlayer()->Update(mousePos, (float)manager->GetWalls()[0]->GetWidth(), XMFLOAT2((float)windowWidth, (float)windowHeight), dt);

	collisionManager.DetectCollisions(manager->GetBalls()[0], manager->GetPlayer(), dt);

	manager->GetGameController()->CheckBounds();
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
	for (unsigned int i = 0; i < manager->GetGameEntities().size(); i++)
	{

		// Copy CPU-side data to a single CPU-side structure
		//  - Allows us to send the data to the GPU buffer in one step
		//  - Do this PER OBJECT, before drawing it
		manager->GetGameEntities()[i]->RecalculateWorldMatrix();
		manager->GetGameEntities()[i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("world", manager->GetGameEntities()[i]->GetWorldMatrix());
		manager->GetGameEntities()[i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("view", camera->GetViewMatrix());
		manager->GetGameEntities()[i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("projection", camera->GetProjectionMatrix());

		if (manager->GetGameEntities()[i] = dynamic_cast<Boundary*>(manager->GetGameEntities()[i]))
		{
			manager->GetGameEntities()[i]->GetMaterial()->GetVertexShader()->SetFloat2("lineBounds", CalcDepthLines());
		}
		

		manager->GetGameEntities()[i]->GetMaterial()->GetVertexShader()->SetShader();

		manager->GetPixelShaders()[0]->SetShaderResourceView("diffuseTexture", manager->GetGameEntities()[i]->GetMaterial()->GetResourceView());
		manager->GetPixelShaders()[0]->SetSamplerState("basicSampler", manager->GetSamplerStates()[0]);

		manager->GetGameEntities()[i]->GetMaterial()->GetPixelShader()->SetShader();

		// Draw the mesh
		manager->GetGameEntities()[i]->Draw(deviceContext);
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
	manager->GetGameController()->Serve();
	mouseDown = false;
	ReleaseCapture();
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	LONG deltaX = x - prevMousePos.x;
	LONG deltaY = y - prevMousePos.y;

	mousePos = XMFLOAT3((float)x, (float)y, 0);

	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion


XMFLOAT2 MyDemoGame::CalcDepthLines()
{
	std::vector<XMFLOAT2> toReturn;
	float pos = manager->GetBalls()[0]->GetPosition().z - manager->GetBalls()[0]->GetRadius() / 2;
	float pos2 = manager->GetBalls()[0]->GetPosition().z + manager->GetBalls()[0]->GetRadius() / 2;

	return XMFLOAT2(pos, pos2);
}