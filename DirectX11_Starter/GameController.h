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

	enum GameState { SERVE = 0, PLAY = 1, ADVANCE = 2, END = 3 };

	Ball* GetBall();
	Player* GetPlayer();
	int GetGameState();
	int GetCompLives();
	int GetPlayerLives();
	int GetLevel();
	XMFLOAT3 GetMaxSpeed();
	XMFLOAT3 GetMaxAngularSpeed();

	void ResetCourt();
	void ResetGame();
	void Serve();
	void CheckBounds();
	void CalcMaxSpeed();
	void CalcMaxAngularSpeed();
	void CalcMaxComputerSpeed();
	void ChangeGameState(int s);

	void Update(XMFLOAT3 mPos, XMFLOAT2 window, Camera* cam, float wallWidth, float dt);

private:
	Ball* ball;
	Player* player;
	Computer* computer;
	GameState gameState;

	int gameLevel;
	int cpuLives;
	int playerLives;
	XMFLOAT3 maxSpeed;
	XMFLOAT3 maxAngularSpeed;
	XMFLOAT2 maxComputerSpeed;

	void LimitComputerPosition(float wallWidth);
};

