#pragma once
#include "Ball.h"
#include "Player.h"

class GameController
{
public:

	GameController(Ball* _ball, Player* _player);
	GameController(Ball* _ball, Player* _player, int cL, int pL, int l);
	~GameController();

	Ball* GetBall();
	Player* GetPlayer();
	int GetCompLives();
	int GetPlayerLives();
	int GetLevel();
	XMFLOAT3 GetMaxSpeed();
	XMFLOAT3 GetMaxAngularSpeed();

	void ResetCourt();
	void Serve();
	void CheckBounds();
	void CalcMaxSpeed();
	void CalcMaxAngularSpeed();

	void Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* cam, float dt);

private:

	Ball* ball;
	Player* player;

	int gameLevel;
	int cpuLives;
	int playerLives;
	XMFLOAT3 maxSpeed;
	XMFLOAT3 maxAngularSpeed;

	bool serving;

};

