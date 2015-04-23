#include "GameController.h"


GameController::GameController(Ball* _ball, Player* _player)
{
	ball = _ball;
	player = _player;

	ResetCourt();
}

GameController::GameController(Ball* _ball, Player* _player, int cL, int pL, int l)
{
	ball = _ball;
	player = _player;

	ResetCourt();
	CalcMaxSpeed();
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

void GameController::ResetCourt()
{
	player->SetPosition(XMFLOAT3(0, 0, -8));
	ball->SetPosition(XMFLOAT3(0, 0, -7));
	
	ball->SetVelocity(XMFLOAT3(0, 0, 0));
	ball->SetAngularVelocity(XMFLOAT3(0, 0, 0));
	
	serving = true;

	CalcMaxAngularSpeed();
	CalcMaxSpeed();
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
		//just reset the court for now, this will do something with lives later
		ResetCourt();
	}
}

void GameController::Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* cam, float dt)
{
	//update player and ball and stuff in here since it controls the game

	//If the ball is set to arrive at the paddle in less than .15 seconds
	float x = abs(ball->GetPosition().z - player->GetPosition().z) / abs(ball->GetVelocity().z);
	if (abs(ball->GetPosition().z - player->GetPosition().z) / abs(ball->GetVelocity().z) < .1 && ball->GetVelocity().z < 0)
	{
		player->AddPrevPos(player->GetPosition());
	}

	ball->Update(dt);
	player->Update(mPos, window, cam);
}