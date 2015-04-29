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

	manager->GetPixelShaders()[0]->LoadShaderFile(L"WallPixelShader.cso");
	manager->GetVertexShaders()[0]->LoadShaderFile(L"WallVertexShader.cso");

	manager->CreatePixelShader();
	manager->CreateVertexShader();

	manager->GetPixelShaders()[1]->LoadShaderFile(L"BallPixelShader.cso");
	manager->GetVertexShaders()[1]->LoadShaderFile(L"BallVertexShader.cso");

	manager->CreatePixelShader();
	manager->CreateVertexShader();

	manager->GetPixelShaders()[2]->LoadShaderFile(L"PlayerPixelShader.cso");
	manager->GetVertexShaders()[2]->LoadShaderFile(L"PlayerVertexShader.cso");

	// Create Particle shaders
	manager->CreatePixelShader();
	manager->CreateVertexShader();
	manager->GetPixelShaders()[2]->LoadShaderFile(L"ParticlePixelShader.cso");
	manager->GetVertexShaders()[2]->LoadShaderFile(L"ParticleVertexShader.cso");

	//Load the textures you want to use
	manager->CreateResourceView(L"../Assets/wall.png");
	manager->CreateResourceView(L"../Assets/ballTex.png");
	manager->CreateResourceView(L"../Assets/paddle.png");

	//Create the sampler state.
	//Could take U/V/W states later for more options for textures
	manager->CreateSamplerState();


	//Create materials and meshes that will be used based on previous creation of stuff

	//walls
	manager->CreateMaterial(manager->GetVertexShaders()[0], manager->GetPixelShaders()[0], manager->GetResourceViews()[0], manager->GetSamplerStates()[0]);
	//ball																																					 
	manager->CreateMaterial(manager->GetVertexShaders()[1], manager->GetPixelShaders()[1], manager->GetResourceViews()[1], manager->GetSamplerStates()[0]);
	//paddle
	manager->CreateMaterial(manager->GetVertexShaders()[1], manager->GetPixelShaders()[1], manager->GetResourceViews()[2], manager->GetSamplerStates()[0]);
	// particles
	manager->CreateMaterial(manager->GetVertexShaders()[2], manager->GetPixelShaders()[2], manager->GetResourceViews()[0], manager->GetSamplerStates()[0]);

	manager->CreateMesh("../Assets/wall.obj");
	manager->CreateMesh("../Assets/sphere.obj");
	manager->CreateMesh("../Assets/paddle.obj");

	XMFLOAT3 wScale = XMFLOAT3(20.0f, 20.0f, 20.0f);

	manager->CreateWall(20, 5, XMFLOAT3(0, -2.5f, 0), XMFLOAT3(0, 0, 0), wScale, XMFLOAT3(0, 1.0f, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Bottom wall
	manager->CreateWall(20, 5, XMFLOAT3(-2.5f, 0, 0), XMFLOAT3(0, 0, -XM_PI / 2), wScale, XMFLOAT3(1.0f, 0, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Left Wall
	manager->CreateWall(20, 5, XMFLOAT3(0, 2.5f, 0), XMFLOAT3(0, 0, XM_PI), wScale, XMFLOAT3(0, -1.0f, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Top wall
	manager->CreateWall(20, 5, XMFLOAT3(2.5f, 0, 0), XMFLOAT3(0, 0, XM_PI / 2), wScale, XMFLOAT3(-1.0, 0, 0), manager->GetMeshes()[0], manager->GetMaterials()[0]); //Right wall
	
	manager->CreateBall(.25f, manager->GetMeshes()[1], manager->GetMaterials()[1]);
	manager->GetBalls()[0]->SetScale(.5f, .5f, .5f);

	manager->CreatePlayer(XMFLOAT3(0, 0, -8), 1.33f, 1, manager->GetMeshes()[2], manager->GetMaterials()[2]);
	manager->GetPlayer()->SetRotation(0, XM_PI/2, 0);
	
	manager->CreateParticleSystem(manager->GetMaterials()[3]);

	manager->CreateComputer(XMFLOAT3(0, 0, 8), 1.33f, 1, manager->GetMeshes()[2], manager->GetMaterials()[2]);
	manager->GetComputer()->SetRotation(0, XM_PI / 2, 0);

	manager->CreateGameController(manager->GetBalls()[0], manager->GetPlayer(), manager->GetComputer(), 3, 3, 1);

	//Now that we have walls, create the collision manager
	collisionManager = Collisions(manager->GetWalls());

	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	//Create the camera in here. Pain in the ass to do in game manager
	camera = new Camera();
	camera->RecalculateViewMatrix();
	camera->RecalculateProjectionMatrix(AspectRatio());

	manager->CreateLight(0, XMFLOAT4(.3f, .3f, .3f, 1.0f), XMFLOAT4(.7f, .7f, .7f, 1.0f), 0, XMFLOAT3(0, 0, -10), XMFLOAT3(0, 0, 0), XMFLOAT3(.5f, .5f, .5f), 0);
	manager->CreateLight(0, XMFLOAT4(.3f, .3f, .3f, 1.0f), XMFLOAT4(.7f, .7f, .7f, 1.0f), 0, XMFLOAT3(0, 0, -10), XMFLOAT3(0, 0, 0), XMFLOAT3(-.5f, -.5f, -.5f), 0);

	//Organize by shader for drawing
	//Shaders come in pairs for now so this may need to change if that changes
	for (unsigned int i = 0; i < manager->GetVertexShaders().size(); i++)
	{
		std::vector<GameEntity*> sorted;
		for (unsigned int j = 0; j < manager->GetGameEntities().size(); j++)
		{
			if (manager->GetGameEntities()[j]->GetMaterial()->GetVertexShader() == manager->GetVertexShaders()[i])
			{
				sorted.push_back(manager->GetGameEntities()[j]);
			}
		}
		manager->AddDraw(sorted);

		sorted.clear();
	}


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
	manager->GetGameController()->Update(mousePos, XMFLOAT2((float)windowWidth, (float)windowHeight), camera, manager->GetWalls()[0]->GetWidth(), dt);

	collisionManager.DetectCollisions(manager->GetBalls()[0], manager->GetPlayer(), manager->GetComputer(), manager->GetGameController()->GetMaxSpeed(), manager->GetGameController()->GetMaxAngularSpeed(), dt);
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

	Light lArray[8];

	for (unsigned int i = 0; i < manager->GetLights().size(); i++)
	{
		lArray[i] = manager->GetLights()[i]->ConvertToStruct();
	}

	XMFLOAT4 camPos = XMFLOAT4(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z, 1.0f);
	int size = manager->GetLights().size();

	
	for (int i = 0; i < manager->GetMaterials().size(); i++)
	{

		//manager->GetMaterials()[i].Initialize();

	}



	for (unsigned int i = 0; i < manager->GetDrawByShader()[0].size(); i++)
	{
		// Copy CPU-side data to a single CPU-side structure
		//  - Allows us to send the data to the GPU buffer in one step
		//  - Do this PER OBJECT, before drawing it
		manager->GetDrawByShader()[0][i]->RecalculateWorldMatrix();
		manager->GetDrawByShader()[0][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("world", manager->GetDrawByShader()[0][i]->GetWorldMatrix());
		manager->GetDrawByShader()[0][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("view", camera->GetViewMatrix());
		manager->GetDrawByShader()[0][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("projection", camera->GetProjectionMatrix());
		manager->GetDrawByShader()[0][i]->GetMaterial()->GetVertexShader()->SetFloat2("lineBounds", CalcDepthLines());
		manager->GetDrawByShader()[0][i]->GetMaterial()->GetVertexShader()->SetData("cameraPosition", &camPos, sizeof(XMFLOAT4));
		
		manager->GetGameEntities()[i]->GetMaterial()->GetVertexShader()->SetShader();

		manager->GetGameEntities()[i]->GetMaterial()->GetPixelShader()->SetShaderResourceView("diffuseTexture", manager->GetGameEntities()[i]->GetMaterial()->GetResourceView());
		manager->GetGameEntities()[i]->GetMaterial()->GetPixelShader()->SetSamplerState("basicSampler", manager->GetSamplerStates()[0]);
		manager->GetGameEntities()[i]->GetMaterial()->GetPixelShader()->SetShader();

		// Draw the mesh
		manager->GetGameEntities()[i]->Draw(deviceContext);

		manager->GetDrawByShader()[0][i]->GetMaterial()->GetVertexShader()->SetShader();

		manager->GetDrawByShader()[0][i]->GetMaterial()->GetPixelShader()->SetShaderResourceView("diffuseTexture", manager->GetDrawByShader()[0][i]->GetMaterial()->GetResourceView());
		manager->GetDrawByShader()[0][i]->GetMaterial()->GetPixelShader()->SetSamplerState("basicSampler", manager->GetDrawByShader()[0][i]->GetMaterial()->GetSamplerState());

		manager->GetDrawByShader()[0][i]->GetMaterial()->GetPixelShader()->SetData("lights", &lArray, sizeof(Light) * 8);
		
		manager->GetDrawByShader()[0][i]->GetMaterial()->GetPixelShader()->SetData("numLights", &size, sizeof(float));

		manager->GetDrawByShader()[0][i]->GetMaterial()->GetPixelShader()->SetShader();

		// Draw the mesh
		manager->GetDrawByShader()[0][i]->Draw(deviceContext);
	}

	
	for (unsigned int i = 0; i < manager->GetDrawByShader()[1].size(); i++)
	{
		// Copy CPU-side data to a single CPU-side structure
		//  - Allows us to send the data to the GPU buffer in one step
		//  - Do this PER OBJECT, before drawing it
		manager->GetDrawByShader()[1][i]->RecalculateWorldMatrix();
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("world", manager->GetDrawByShader()[1][i]->GetWorldMatrix());
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("view", camera->GetViewMatrix());
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("projection", camera->GetProjectionMatrix());
								   
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetVertexShader()->SetShader();
								   
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetPixelShader()->SetShaderResourceView("diffuseTexture", manager->GetDrawByShader()[1][i]->GetMaterial()->GetResourceView());
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetPixelShader()->SetSamplerState("basicSampler", manager->GetDrawByShader()[1][i]->GetMaterial()->GetSamplerState());
								   
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetPixelShader()->SetData("lights", &lArray, sizeof(Light));
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetPixelShader()->SetData("numLights", &size, sizeof(float));
								   
		manager->GetDrawByShader()[1][i]->GetMaterial()->GetPixelShader()->SetShader();
								   
		// Draw the mesh		   
		manager->GetDrawByShader()[1][i]->Draw(deviceContext);
	}
	
	for (unsigned int i = 0; i < manager->GetDrawByShader()[2].size(); i++)
	{
		// Copy CPU-side data to a single CPU-side structure
		//  - Allows us to send the data to the GPU buffer in one step
		//  - Do this PER OBJECT, before drawing it
		manager->GetDrawByShader()[2][i]->RecalculateWorldMatrix();
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("world", manager->GetDrawByShader()[2][i]->GetWorldMatrix());
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("view", camera->GetViewMatrix());
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetVertexShader()->SetMatrix4x4("projection", camera->GetProjectionMatrix());
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetVertexShader()->SetFloat2("lineBounds", CalcDepthLines());
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetVertexShader()->SetData("cameraPosition", &camPos, sizeof(XMFLOAT4));
								   
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetVertexShader()->SetShader();
								   
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetPixelShader()->SetShaderResourceView("diffuseTexture", manager->GetDrawByShader()[2][i]->GetMaterial()->GetResourceView());
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetPixelShader()->SetSamplerState("basicSampler", manager->GetDrawByShader()[2][i]->GetMaterial()->GetSamplerState());
								   
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetPixelShader()->SetData("lights", &lArray, sizeof(Light) * 8);
								   
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetPixelShader()->SetData("numLights", &size, sizeof(float));
								   
		manager->GetDrawByShader()[2][i]->GetMaterial()->GetPixelShader()->SetShader();
								   
		// Draw the mesh
		manager->GetDrawByShader()[2][i]->Draw(deviceContext);
	}

	// Draw the particle system.
	manager->GetParticleSystem()->Draw(deviceContext);
	
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

	if (manager->GetGameController()->GetGameState() == manager->GetGameController()->END)
	{
		manager->GetGameController()->ResetGame();
	}

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