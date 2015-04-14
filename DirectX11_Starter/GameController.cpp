#include "GameController.h"


GameController::GameController(Ball* _ball, Player* _player)
{
	ball = _ball;
	player = _player;

	ResetCourt();
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

void GameController::ResetCourt()
{
	player->SetPosition(XMFLOAT3(0, 0, -8));
	ball->SetPosition(XMFLOAT3(0, 0, -7));
	
	ball->SetVelocity(XMFLOAT3(0, 0, 0));
	ball->SetAngularVelocity(XMFLOAT3(0, 0, 0));
	
	serving = true;
}

void GameController::Serve()
{
	if (serving)
	{
		ball->ApplyVelocity(XMFLOAT3(0, 0, 10));
		serving = false;
	}
}