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
	// Release all of the D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);

	delete pixelShader;
	delete vertexShader;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}

	delete mat1;
	delete mat2;
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

	vertexShader = new SimpleVertexShader(device, deviceContext);
	pixelShader = new SimplePixelShader(device, deviceContext);

	vertexShader->LoadShaderFile(L"VertexShader.cso");
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	lightA.AmbientColor = XMFLOAT4(0.25f, .25f, .25f, 1);
	lightA.DiffuseColor = XMFLOAT4(0, .75f, .75f, 1);
	lightA.Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);


	lightB.AmbientColor = XMFLOAT4(0.1f, .1f, .1f, 1);
	lightB.DiffuseColor = XMFLOAT4(.75f, 0.0f, 0.0f, 1);
	lightB.Direction = XMFLOAT3(-1.0f, -1.0f, 0.0f);


	pixelShader->SetData(
		"lightA",
		&lightA,
		sizeof(DirectionalLight));

	pixelShader->SetData(
		"lightB",
		&lightB,
		sizeof(DirectionalLight));
	
	//Create the boundarys
	CreateBoundaries();

	CreateBall();

	CreatePlayer();

	collisionManager = Collisions(walls);

	// Set up camera-related matrices
	InitializeCameraMatrices();

	// Set up world matrix
	// In an actual game, each object will need one of these and they should
	//  update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	// Successfully initialized
	return true;
}

void MyDemoGame::CreateBoundaries()
{
	meshes.push_back(new Mesh("boundary2.obj", device));

	mat1 = new Material(vertexShader, pixelShader);
	mat1->CreateShaderSampler(device, deviceContext, L"gdorf.png");

	//Standard boundaries are 1 unit length (z-direction) x .25 unit wide (x direction)
	//Making these 40x10
	//Wall 1 - Bottom
	Boundary temp(40, 10, meshes[meshes.size() - 1], mat1);
	temp.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	temp.SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));
	temp.SetScale(XMFLOAT3(40.0f, 40.0f, 40.0f));
	walls.push_back(temp);

	
	//Wall 2 - Left
	temp.SetPosition(XMFLOAT3(-5.0f, 5.0f, 0.0f));
	temp.SetUp(XMFLOAT3(1.0f, 0.0f, 0.0f));
	temp.SetRotation(XMFLOAT3(0.0f, 0.0f, 3*XM_PI/2));
	walls.push_back(temp);

	//Wall 3 - Top
	temp.SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	temp.SetUp(XMFLOAT3(0.0f, -1.0f, 0.0f));
	temp.SetRotation(XMFLOAT3(0.0f, 0.0f, XM_PI)); 
	walls.push_back(temp);

	//Wall 3 - Right
	temp.SetPosition(XMFLOAT3(5.0f, 5.0f, 0.0f));
	temp.SetUp(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	temp.SetRotation(XMFLOAT3(0.0f, 0.0f, XM_PI/2));
	walls.push_back(temp);

	//Temporary back wall
	temp.SetPosition(XMFLOAT3(0.0f, 5.0f, 20.0f));
	temp.SetUp(XMFLOAT3(0.0f, 0.0f, -1.0f));
	temp.SetRotation(XMFLOAT3(-XM_PI/2, 0.0f, 0.0f));
	walls.push_back(temp);
}

void MyDemoGame::CreateBall()
{
	meshes.push_back(new Mesh("sphere.obj", device));

	mat2 = new Material(vertexShader, pixelShader);
	mat2->CreateShaderSampler(device, deviceContext, L"mario.png");

	ball = Ball(.5, meshes[meshes.size() - 1], mat2);
	ball.SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	ball.SetVelocity(XMFLOAT3(.5f, 1.0f, 1.2f));
	ball.SetAngularVelocity(XMFLOAT3(.3f, .3f, .3f));
}

void MyDemoGame::CreatePlayer()
{
	meshes.push_back(new Mesh("paddle.obj", device));
	player = Player(meshes[meshes.size() - 1], mat1);
	player.SetPosition(XMFLOAT3(0.0f, 5.0f, -19.0f));
	player.SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	// Create the Projection matrix
	// This should match the window's aspect ratio, and also update anytime
	// the window resizes (which is already happening in OnResize() below)
	cam.createProjectionMatrix(AspectRatio());
	
	// Create the View matrix
	// In an actual game, update this when the camera moves (every frame)
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookAtLH(XMLoadFloat3(&cam.GetPosition()), target, up);
                                                      
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	// Update our projection matrix since the window size changed
	cam.createProjectionMatrix(AspectRatio());
}
#pragma endregion

#pragma region Game Loop

// Update your game state
void MyDemoGame::UpdateScene(float dt)
{
	cam.Update(dt);

	//Set ball prev position before updating, used for collision detection
	ball.SetPrevPos(ball.GetPosition());
	ball.Update(dt);

	ball = collisionManager.DetectCollisions(ball, dt);
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

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Copy CPU-side data to a single CPU-side structure
	//  - Allows us to send the data to the GPU buffer in one step
	//  - Do this PER OBJECT, before drawing it
	for (unsigned int i = 0; i < walls.size(); i++)
	{	
		
		//Set vertex stuff and send to the gpu
		vertexShader = walls[i].GetMaterial()->GetVertexShader();

		vertexShader->SetMatrix4x4("world", walls[i].GetWorld());
		vertexShader->SetMatrix4x4("view", cam.GetView());
		vertexShader->SetMatrix4x4("projection", cam.GetProjection());

		vertexShader->SetShader();

		//Set pixel stuff and send to the gpu
		
		pixelShader->SetShaderResourceView("diffuseTexture", walls[i].GetMaterial()->GetSRV());
		pixelShader->SetSamplerState("basicSampler", walls[i].GetMaterial()->GetSamplerState());

		pixelShader->SetShader();
		
		walls[i].Draw(deviceContext);
	}

	for (unsigned int i = 0; i < entities.size(); i++)
	{

		//Set vertex stuff and send to the gpu
		vertexShader = entities[i].GetMaterial()->GetVertexShader();

		vertexShader->SetMatrix4x4("world", entities[i].GetWorld());
		vertexShader->SetMatrix4x4("view", cam.GetView());
		vertexShader->SetMatrix4x4("projection", cam.GetProjection());

		vertexShader->SetShader();

		//Set pixel stuff and send to the gpu

		pixelShader->SetShaderResourceView("diffuseTexture", entities[i].GetMaterial()->GetSRV());
		pixelShader->SetSamplerState("basicSampler", entities[i].GetMaterial()->GetSamplerState());

		pixelShader->SetShader();

		entities[i].Draw(deviceContext);
	}

	//the ball
	//Set vertex stuff and send to the gpu
	vertexShader = ball.GetMaterial()->GetVertexShader();

	vertexShader->SetMatrix4x4("world", ball.GetWorld());
	vertexShader->SetMatrix4x4("view", cam.GetView());
	vertexShader->SetMatrix4x4("projection", cam.GetProjection());

	vertexShader->SetShader();

	//Set pixel stuff and send to the gpu

	pixelShader->SetShaderResourceView("diffuseTexture", ball.GetMaterial()->GetSRV());
	pixelShader->SetSamplerState("basicSampler", ball.GetMaterial()->GetSamplerState());

	pixelShader->SetShader();

	ball.Draw(deviceContext);

	//player
	vertexShader = player.GetMaterial()->GetVertexShader();

	vertexShader->SetMatrix4x4("world", player.GetWorld());
	vertexShader->SetMatrix4x4("view", cam.GetView());
	vertexShader->SetMatrix4x4("projection", cam.GetProjection());

	vertexShader->SetShader();

	//Set pixel stuff and send to the gpu

	pixelShader->SetShaderResourceView("diffuseTexture", player.GetMaterial()->GetSRV());
	pixelShader->SetSamplerState("basicSampler", player.GetMaterial()->GetSamplerState());

	pixelShader->SetShader();

	player.Draw(deviceContext);

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

	SetCapture(hMainWnd);
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

//While the mouse is down and moving, allow the camera rotation to change
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (btnState & 0x0001)
	{
		cam.rotateCamera(x - prevMousePos.x, y - prevMousePos.y);
	}

	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion