#pragma once
#include "Ball.h"
#include "Player.h"
#include "Computer.h"
#include "Boundary.h"

class GameController
{
public:

	GameController(Ball* _ball, Player* _player);
	GameController(Ball* _ball, Player* _player, Computer* _computer, int cL, int pL, int l);
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
	void CalcMaxComputerSpeed();

	void Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* cam, float wallWidth, float dt);

private:

	Ball* ball;
	Player* player;
	Computer* computer;

	int gameLevel;
	int cpuLives;
	int playerLives;
	XMFLOAT3 maxSpeed;
	XMFLOAT3 maxAngularSpeed;
	XMFLOAT2 maxComputerSpeed;

	bool serving;

	void LimitComputerPosition(float wallWidth);

};

