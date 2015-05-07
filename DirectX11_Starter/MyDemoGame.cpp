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

	srand(time(NULL));

	manager = new GameManager(device, deviceContext);

	//Create the camera in here. Pain in the ass to do in game manager
	camera = new Camera();
	camera->RecalculateViewMatrix();
	camera->RecalculateProjectionMatrix(AspectRatio());

	manager->InitGame(camera);
	//Now that we have walls, create the collision manager
	collisionManager = Collisions(manager->GetWalls());

	manager->GetGameController()->ResetCourt();

	// Successfully initialized
	return true;
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

	collisionManager.DetectCollisions(manager->GetBall(), manager->GetPlayer(), manager->GetComputer(), manager->GetGameController()->GetMaxSpeed(), manager->GetGameController()->GetMaxAngularSpeed(), dt);

	camera->Update(dt);

	//Recalc all world matricies
	for (unsigned int i = 0; i < manager->GetGameEntities().size(); i++)
	{
		manager->GetGameEntities()[i]->RecalculateWorldMatrix();
	}
	
	//Update the materials with necessary stuff
	Light lArray[8];

	for (unsigned int i = 0; i < manager->GetLights().size(); i++)
	{
		lArray[i] = manager->GetLights()[i]->ConvertToStruct();
	}

	XMFLOAT4 camPos = XMFLOAT4(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z, 1.0f);

	static_cast<WallMaterial*>(manager->GetMaterials()[0])->SetCamPos(camPos);
	static_cast<WallMaterial*>(manager->GetMaterials()[0])->SetLArray(lArray);
	static_cast<WallMaterial*>(manager->GetMaterials()[0])->SetLineBounds(XMFLOAT2(manager->GetBall()->GetPosition().z, manager->GetBall()->GetRadius()));

	static_cast<PlayerMaterial*>(manager->GetMaterials()[2])->SetCamPos(camPos);
	static_cast<PlayerMaterial*>(manager->GetMaterials()[2])->SetLArray(lArray);
	
	manager->GetParticleSystem()->Update(deviceContext, dt);
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.9f, 0.9f, 0.9f, 0.0f};

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

	// Iterate by material
	for (unsigned int i = 0; i < manager->GetDrawByShader().size(); i++)
	{
		// Iterate by entity.
		for (unsigned int j = 0; j < manager->GetDrawByShader()[i].size(); j++)
		{
			// Copy CPU-side data to a single CPU-side structure
			//  - Allows us to send the data to the GPU buffer in one step
			//  - Do this PER OBJECT, before drawing it
			manager->GetDrawByShader()[i][j]->GetMaterial()->GetVertexShader()->SetMatrix4x4("world", manager->GetDrawByShader()[i][j]->GetWorldMatrix());
			manager->GetDrawByShader()[i][j]->PrepareToDraw();
			manager->GetMaterials()[i]->PrepareToDraw(manager->GetGameEntities()[0]->GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
			// Draw the mesh
			manager->GetDrawByShader()[i][j]->Draw(deviceContext);
		}
	}

	// Draw the particle system.
	manager->GetParticleSystem()->GetMaterial()->GetVertexShader()->SetMatrix4x4("world", manager->GetBall()->GetWorldMatrix());
	manager->GetParticleSystem()->GetMaterial()->PrepareToDraw(manager->GetGameEntities()[0]->GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
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
	float pos = manager->GetBall()->GetPosition().z - manager->GetBall()->GetRadius() / 2;
	float pos2 = manager->GetBall()->GetPosition().z + manager->GetBall()->GetRadius() / 2;

	return XMFLOAT2(pos, pos2);
}