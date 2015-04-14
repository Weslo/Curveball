#pragma once
#include "Ball.h"
#include "Player.h"

class GameController
{
public:

	GameController(Ball* _ball, Player* _player);
	~GameController();

	Ball* GetBall();
	Player* GetPlayer();

	void ResetCourt();
	void Serve();

private:

	Ball* ball;
	Player* player;

	bool serving;

};

