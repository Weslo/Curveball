#include "GameController.h"


GameController::GameController(Ball* _ball, Player* _player)
{
	ball = _ball;
	player = _player;

	ResetCourt();
}

GameController::GameController(Ball* _ball, Player* _player, Computer* _computer, int cL, int pL, int l)
{
	ball = _ball;
	player = _player;
	computer = _computer;
	cpuLives = cL;
	playerLives = pL;
	gameLevel = l;

	ResetCourt();
	CalcMaxSpeed();
	CalcMaxAngularSpeed();
	CalcMaxComputerSpeed();
}


GameController::~GameController()
{
}

Ball* GameController::GetBall()
{
	return ball;
}

Player* GameController::GetPlayer()
{
	return player;
}

int GameController::GetCompLives()
{
	return cpuLives;
}

int GameController::GetPlayerLives()
{
	return playerLives;
}

int GameController::GetLevel()
{
	return gameLevel;
}

XMFLOAT3 GameController::GetMaxSpeed()
{
	return maxSpeed;
}

XMFLOAT3 GameController::GetMaxAngularSpeed()
{
	return maxAngularSpeed;
}

//This can change later
void GameController::CalcMaxSpeed()
{
	maxSpeed = XMFLOAT3(6.0f + gameLevel, 6.0f + gameLevel, 8.0f + (gameLevel * 2));
}

//This can change later
void GameController::CalcMaxAngularSpeed()
{
	maxAngularSpeed = XMFLOAT3(4.0f + gameLevel, 4.0f + gameLevel, 0);
}

void GameController::CalcMaxComputerSpeed()
{
	maxComputerSpeed = XMFLOAT2(1.0f * gameLevel, 1.0f * gameLevel);
}

void GameController::ResetCourt()
{
	player->SetPosition(XMFLOAT3(0, 0, -8));
	computer->SetPosition(XMFLOAT3(0, 0, 8));
	computer->SetVelocity(XMFLOAT3(0, 0, 0));
	player->ResetPrevPos();
	computer->ResetPrevPos();

	ball->SetPosition(XMFLOAT3(0, 0, -7));
	
	ball->SetVelocity(XMFLOAT3(0, 0, 0));
	ball->SetAngularVelocity(XMFLOAT3(0, 0, 0));
	
	serving = true;

	CalcMaxAngularSpeed();
	CalcMaxSpeed();
	CalcMaxComputerSpeed();
}

void GameController::Serve()
{
	if (serving)
	{
		ball->ApplyVelocity(XMFLOAT3(0, 0, 8));
		serving = false;
	}
}

void GameController::CheckBounds()
{
	if (ball->GetPosition().z - ball->GetRadius() / 2 < player->GetPosition().z)
	{
		playerLives--;
		ResetCourt();
	}

	else if (ball->GetPosition().z + ball->GetRadius() / 2 > computer->GetPosition().z)
	{
		cpuLives--;
		ResetCourt();
	}	
}

void GameController::LimitComputerPosition(float wallWidth)
{
	if (computer->GetPosition().x + computer->GetWidth() / 2 > wallWidth / 2)
	{
		computer->SetPosition(XMFLOAT3(wallWidth / 2 - computer->GetWidth() / 2, computer->GetPosition().y, computer->GetPosition().z));
	}
	else if (computer->GetPosition().x - computer->GetWidth() / 2 < -wallWidth / 2)
	{
		computer->SetPosition(XMFLOAT3(-wallWidth / 2 + computer->GetWidth() / 2, computer->GetPosition().y, computer->GetPosition().z));
	}

	if (computer->GetPosition().y + computer->GetHeight() / 2 > wallWidth / 2)
	{
		computer->SetPosition(XMFLOAT3(computer->GetPosition().x, wallWidth / 2 - computer->GetHeight() / 2, computer->GetPosition().z));
	}
	else if (computer->GetPosition().y - computer->GetHeight() / 2 < -wallWidth / 2)
	{
		computer->SetPosition(XMFLOAT3(computer->GetPosition().x, -wallWidth / 2 + computer->GetHeight() / 2, computer->GetPosition().z));
	}
}

void GameController::Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* cam, float wallWidth, float dt)
{
	//update player and ball and stuff in here since it controls the game

	//If the ball is set to arrive at the paddle in less than .1 seconds
	if (abs(ball->GetPosition().z - player->GetPosition().z) / abs(ball->GetVelocity().z) < .15 && ball->GetVelocity().z < 0)
	{
		player->AddPrevPos(player->GetPosition());
	}

	else if (abs(ball->GetPosition().z - computer->GetPosition().z) / abs(ball->GetVelocity().z) < .15 && ball->GetVelocity().z > 0)
	{
		computer->AddPrevPos(computer->GetPosition());
	}

	ball->Update(dt);
	player->Update(mPos, window, cam);

	if (!serving && ball->GetVelocity().z > 0)
	{
		computer->Update(ball, maxComputerSpeed, gameLevel, false, dt);
		//LimitComputerPosition(wallWidth);
	}
	else
	{
		computer->TrackBall(ball, maxComputerSpeed, gameLevel, true, dt);
	}
}