#include "GameManager.h"


//Store references to the device and deviceContext
GameManager::GameManager(ID3D11Device* d, ID3D11DeviceContext* dc)
{
	device = d;
	deviceContext = dc;
}

//Clean up all of the stuff created and managed here
GameManager::~GameManager()
{
	for (unsigned int i = 0; i < drawByShader.size(); i++)
	{
		drawByShader[i].clear();
	}
	drawByShader.clear();

	for (std::vector< GameEntity* >::iterator it = entities.begin(); it != entities.end(); ++it) { delete (*it); }
	entities.clear();

	// Release the particle system.
	//particleSystem->Shutdown();
	//delete particleSystem;

	for (std::vector< Lighting* >::iterator it = lights.begin(); it != lights.end(); ++it) { delete (*it); }
	lights.clear();

	for (std::vector< Mesh* >::iterator it = meshes.begin(); it != meshes.end(); ++it) { delete (*it); }
	meshes.clear();

	for (std::vector< Material* >::iterator it = materials.begin(); it != materials.end(); ++it) { delete (*it); }
	materials.clear();

	for (std::vector< SimplePixelShader* >::iterator it = pixelShaders.begin(); it != pixelShaders.end(); ++it) { delete (*it); }
	pixelShaders.clear();

	for (std::vector< SimpleVertexShader* >::iterator it = vertexShaders.begin(); it != vertexShaders.end(); ++it) { delete (*it); }
	vertexShaders.clear();

	for (unsigned int i = 0; i < resourceViews.size(); i++)
	{
		ReleaseMacro(resourceViews[i]);
	}

	for (unsigned int i = 0; i < samplerStates.size(); i++)
	{
		ReleaseMacro(samplerStates[i]);
	}

	delete gameController;
}

#pragma region Getters
Ball* GameManager::GetBall()
{
	return ball;
}

std::vector<Boundary*> GameManager::GetWalls()
{
	return walls;
}

std::vector<Mesh*> GameManager::GetMeshes()
{
	return meshes;
}

std::vector<Material*> GameManager::GetMaterials()
{
	return materials;
}

ParticleSystem* GameManager::GetParticleSystem()
{
	return particleSystem;
}

std::vector<SimplePixelShader*> GameManager::GetPixelShaders()
{
	return pixelShaders;
}

std::vector<SimpleVertexShader*> GameManager::GetVertexShaders()
{
	return vertexShaders;
}

std::vector<ID3D11ShaderResourceView*> GameManager::GetResourceViews()
{
	return resourceViews;
}

std::vector<ID3D11SamplerState*> GameManager::GetSamplerStates()
{
	return samplerStates;
}

std::vector<D3D11_SAMPLER_DESC> GameManager::GetSamplerDescs()
{
	return samplerDescs;
}

std::vector<GameEntity*> GameManager::GetGameEntities()
{
	return entities;
}

Player* GameManager::GetPlayer()
{
	return player;
}

Computer* GameManager::GetComputer()
{
	return computer;
}

GameController* GameManager::GetGameController()
{
	return gameController;
}

std::vector<std::vector<GameEntity*>> GameManager::GetDrawByShader()
{
	return drawByShader;
}

std::vector<Lighting*> GameManager::GetLights()
{
	return lights;
}
#pragma endregion

//Create the pixel shader
//Super basic for now
void GameManager::CreatePixelShader()
{
	pixelShaders.push_back(new SimplePixelShader(device, deviceContext));
}

//Create the vertex shader
//Super basic for now
void GameManager::CreateVertexShader()
{
	vertexShaders.push_back(new SimpleVertexShader(device, deviceContext));
}

//Create shader resource view based on file path
void GameManager::CreateResourceView(const wchar_t* path)
{
	ID3D11ShaderResourceView* srv;
	CreateWICTextureFromFile(device, deviceContext, path, 0, &srv);
	resourceViews.push_back(srv);
}

//Create a sampler state
//Can add parameters to set attributes later
void GameManager::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;

	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	samplerDescs.push_back(samplerDesc);
	samplerStates.push_back(samplerState);
}

//Create material given shaders to use and texture/sampler
void GameManager::CreateMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss)
{
	materials.push_back(new Material(vs, ps, rv, ss));
}

//Create material given shaders to use and texture/sampler
void GameManager::CreateWallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss)
{
	materials.push_back(new WallMaterial(vs, ps, rv, ss));
}

//Create material given shaders to use and texture/sampler
void GameManager::CreateBallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss)
{
	materials.push_back(new BallMaterial(vs, ps, rv, ss));
}

//Create material given shaders to use and texture/sampler
void GameManager::CreatePlayerMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss)
{
	materials.push_back(new PlayerMaterial(vs, ps, rv, ss));
}

//Create material given shaders to use and texture/sampler
void GameManager::CreateMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss, const wchar_t* psn, const wchar_t* vsn)
{
	materials.push_back(new Material(vs, ps, rv, ss));
	materials[materials.size() - 1]->GetPixelShader()->LoadShaderFile(psn);
	materials[materials.size() - 1]->GetVertexShader()->LoadShaderFile(vsn);
}

// Craete particle system.
void GameManager::CreateParticleSystem(Material* material)
{
	particleSystem = new ParticleSystem();
	particleSystem->Initialize(device, material);
}

//Create mesh from file
void GameManager::CreateMesh(char* file)
{
	meshes.push_back(new Mesh(file, device));
}

//Create ball given radius, mesh, and material
void GameManager::CreateBall(float r, Mesh* m, Material* ma)
{
	ball = new Ball(r, m, ma);
	entities.push_back(ball);
}

//Create a wall given length, width, position, rotation, scale, up vector, mesh, and material
void GameManager::CreateWall(int l, int w, XMFLOAT3 p, XMFLOAT3 r, XMFLOAT3 s, XMFLOAT3 u, Mesh* m, Material* ma)
{
	Boundary* temp;
	temp = new Boundary(l, w, m, ma);
	temp->SetPosition(p);
	temp->SetRotation(r);
	temp->SetScale(s);
	temp->SetUp(u);

	walls.push_back(temp);

	entities.push_back(walls[walls.size() - 1]);
}

void GameManager::CreatePlayer(XMFLOAT3 pos, float w, float h, Mesh* m, Material* ma)
{
	player = new Player(pos, w, h, m, ma);
	entities.push_back(player);
}

void GameManager::CreateComputer(XMFLOAT3 pos, float w, float h, Mesh* m, Material* ma)
{
	computer = new Computer(pos, w, h, m, ma);
	entities.push_back(computer);
}

void GameManager::CreateGameController(Ball* ball, Player* player)
{
	gameController = new GameController(ball, player);
}

void GameManager::CreateGameController(Ball* ball, Player* player, Computer* computer, int cL, int pL, int l)
{
	gameController = new GameController(ball, player, computer, cL, pL, l);
}

void GameManager::CreateLight(int _lightType, XMFLOAT4 _ambient, XMFLOAT4 _diffuse, float _range, XMFLOAT3 _pos, XMFLOAT3 _att, XMFLOAT3 _dir, float _cone)
{
	Lighting* l;

	l = new Lighting(_lightType, _ambient, _diffuse, _range, _pos, _att, _dir, _cone);

	lights.push_back(l);
}

void GameManager::AddDraw(std::vector<GameEntity*> draw)
{
	drawByShader.push_back(draw);
}

void GameManager::InitGame(Camera* cam)
{
	CreatePixelShader();
	CreateVertexShader();

	pixelShaders[0]->LoadShaderFile(L"WallPixelShader.cso");
	vertexShaders[0]->LoadShaderFile(L"WallVertexShader.cso");

	CreatePixelShader();
	CreateVertexShader();

	pixelShaders[1]->LoadShaderFile(L"BallPixelShader.cso");
	vertexShaders[1]->LoadShaderFile(L"BallVertexShader.cso");

	CreatePixelShader();
	CreateVertexShader();

	pixelShaders[2]->LoadShaderFile(L"PlayerPixelShader.cso");
	vertexShaders[2]->LoadShaderFile(L"PlayerVertexShader.cso");

	CreatePixelShader();
	CreateVertexShader();
	pixelShaders[3]->LoadShaderFile(L"ParticlePixelShader.cso");
	vertexShaders[3]->LoadShaderFile(L"ParticleVertexShader.cso");

	CreateResourceView(L"../Assets/wall.png");
	CreateResourceView(L"../Assets/ballTex.png");
	CreateResourceView(L"../Assets/paddle.png");

	//Create the sampler state.
	//Could take U/V/W states later for more options for textures
	CreateSamplerState();


	//Create materials and meshes that will be used based on previous creation of stuff

	//walls
	CreateWallMaterial(vertexShaders[0], pixelShaders[0], resourceViews[0], samplerStates[0]);
	//ball																																					 
	CreateBallMaterial(vertexShaders[1], pixelShaders[1], resourceViews[1],samplerStates[0]);
	//paddle
	CreatePlayerMaterial(vertexShaders[2], pixelShaders[2], resourceViews[2], samplerStates[0]);
	// particles
	//CreateMaterial(GetVertexShaders()[3], GetPixelShaders()[3], GetResourceViews()[0], GetSamplerStates()[0]);

	CreateMesh("../Assets/wall2.obj");
	CreateMesh("../Assets/sphere.obj");
	CreateMesh("../Assets/paddle.obj");

	XMFLOAT3 wScale = XMFLOAT3(20.0f, 20.0f, 20.0f);

	CreateWall(16, 4, XMFLOAT3(0, -2.0f, 0), XMFLOAT3(0, 0, 0), wScale, XMFLOAT3(0, 1.0f, 0), meshes[0], materials[0]); //Bottom wall
	CreateWall(16, 4, XMFLOAT3(-2.0f, 0, 0), XMFLOAT3(0, 0, -XM_PI / 2), wScale, XMFLOAT3(1.0f, 0, 0), meshes[0], materials[0]); //Left Wall
	CreateWall(16, 4, XMFLOAT3(0, 2.0f, 0), XMFLOAT3(0, 0, XM_PI), wScale, XMFLOAT3(0, -1.0f, 0), meshes[0], materials[0]); //Top wall
	CreateWall(16, 4, XMFLOAT3(2.0f, 0, 0), XMFLOAT3(0, 0, XM_PI / 2), wScale, XMFLOAT3(-1.0, 0, 0), meshes[0], materials[0]); //Right wall

	CreateBall(.25f, meshes[1], materials[1]);
	ball->SetScale(.5f, .5f, .5f);

	CreatePlayer(XMFLOAT3(0, 0, -8), 1.33f, 1, meshes[2], materials[2]);
	player->SetRotation(0, XM_PI / 2, 0);

	//CreateParticleSystem(material[3]);

	CreateComputer(XMFLOAT3(0, 0, 8), 1.33f, 1, meshes[2], materials[2]);
	computer->SetRotation(0, XM_PI / 2, 0);

	CreateGameController(ball, player, computer, 3, 3, 1);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateLight(0, XMFLOAT4(.05f, .05f, .05f, 1.0f), XMFLOAT4(.1f, .1f, .1f, 1.0f), 0, XMFLOAT3(0, 0, -10), XMFLOAT3(0, 0, 0), XMFLOAT3(.5f, .5f, .5f), 0);
	CreateLight(0, XMFLOAT4(.05f, .05f, .05f, 1.0f), XMFLOAT4(.1f, .1f, .1f, 1.0f), 0, XMFLOAT3(0, 0, -10), XMFLOAT3(0, 0, 0), XMFLOAT3(-.5f, -.5f, -.5f), 0);
	CreateLight(0, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(.3f, .3f, .3f, 1.0f), 0, XMFLOAT3(0, 0, -10), XMFLOAT3(0, 0, 0), XMFLOAT3(0.0f, 0.0f, 1.0f), 0);
	CreateLight(1, XMFLOAT4(.2f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.7f, 0.0f, 0.0f, 1.0f), 0, ball->GetPosition(), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0, 0, 0), 0);

	ball->SetBallLight(lights[lights.size() - 1]);

	//Organize by shader for drawing
	//Shaders come in pairs for now so this may need to change if that changes
	for (unsigned int i = 0; i < vertexShaders.size(); i++)
	{
		std::vector<GameEntity*> sorted;
		for (unsigned int j = 0; j < entities.size(); j++)
		{
			if (entities[j]->GetMaterial()->GetVertexShader() == vertexShaders[i])
			{
				sorted.push_back(entities[j]);
			}
		}
		AddDraw(sorted);

		sorted.clear();
	}

	//Update the materials with necessary stuff
	Light lArray[8];

	for (unsigned int i = 0; i < GetLights().size(); i++)
	{
		lArray[i] = lights[i]->ConvertToStruct();
	}

	XMFLOAT4 camPos = XMFLOAT4(cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z, 1.0f);

	static_cast<WallMaterial*>(materials[0])->SetCamPos(camPos);
	static_cast<WallMaterial*>(materials[0])->SetLArray(lArray);
	static_cast<WallMaterial*>(materials[0])->SetLineBounds(XMFLOAT2(ball->GetPosition().z, ball->GetRadius()));

	static_cast<PlayerMaterial*>(materials[2])->SetCamPos(camPos);
	static_cast<PlayerMaterial*>(materials[2])->SetLArray(lArray);
}
