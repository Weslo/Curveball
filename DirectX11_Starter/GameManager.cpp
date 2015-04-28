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
	for (std::vector< GameEntity* >::iterator it = entities.begin(); it != entities.end(); ++it) { delete (*it); }
	entities.clear();

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
std::vector<Ball*> GameManager::GetBalls()
{
	return balls;
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
void GameManager::CreateMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss, const wchar_t* psn, const wchar_t* vsn)
{
	materials.push_back(new Material(vs, ps, rv, ss));
	materials[materials.size() - 1]->GetPixelShader()->LoadShaderFile(psn);
	materials[materials.size() - 1]->GetVertexShader()->LoadShaderFile(vsn);
}

//Create mesh from file
void GameManager::CreateMesh(char* file)
{
	meshes.push_back(new Mesh(file, device));
}

//Create ball given radius, mesh, and material
void GameManager::CreateBall(float r, Mesh* m, Material* ma)
{
	balls.push_back(new Ball(r, m, ma));
	entities.push_back(balls[balls.size() - 1]);
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