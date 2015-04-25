#include "Computer.h"


Computer::Computer(XMFLOAT3 pos, float w, float h, Mesh* m, Material* ma) : GameEntity(m, ma)
{
	position = pos;
	zPos = pos.z;

	height = h;
	width = w;

	up = XMFLOAT3(0, 0, -1.0f);
}

Computer::~Computer()
{
}

XMFLOAT3 Computer::GetVelocity()
{
	return velocity;
}

XMFLOAT3 Computer::GetUp()
{
	return up;
}

std::vector<XMFLOAT3> Computer::GetPreviousPosition()
{
	return prevPosition;
}

float Computer::GetWidth()
{
	return width;
}

float Computer::GetHeight()
{
	return height;
}

void Computer::AddPrevPos(XMFLOAT3 p)
{
	prevPosition.push_back(p);
}

void Computer::CalcVelocity(float dt)
{
	velocity = XMFLOAT3((position.x - prevPosition[0].x) / .1f, (position.y - prevPosition[0].y) / .1f, 0);
}

void Computer::SetVelocity(XMFLOAT3 v)
{
	velocity = v;
}

void Computer::ResetPrevPos()
{
	prevPosition.clear();
}

void Computer::TrackBall(Ball* b, XMFLOAT2 maxSpeed, int level, boolean loc, float dt)
{
	//look ahead some number of frames based on the game level
	//This will not take into account collisions for now
	XMFLOAT2 computerPos = XMFLOAT2(position.x, position.y);
	XMVECTOR dif;

	if (!loc)
	{
		XMFLOAT2 predictedLoc;
		predictedLoc.x = b->GetPosition().x;// +(b->GetVelocity().x * dt * level);
		predictedLoc.y = b->GetPosition().y;// +(b->GetVelocity().y * dt * level);

		//Now find the difference between computer pos and predicted ball location
		dif = XMLoadFloat2(&predictedLoc) - XMLoadFloat2(&computerPos);
	}

	else
	{
		dif = XMLoadFloat2(new XMFLOAT2(0, 0)) - XMLoadFloat2(&computerPos);
	}
	//Now that we have the difference, start moving that way. Limit the movement per frame based on game level
	//Scale to a ratio to have it arrive at the predicted location x/y at the same time
	
	XMFLOAT2 hPerV;

	if (XMVectorGetX(dif) == 0 && XMVectorGetY(dif) == 0)
	{
		//Dont need to move
		hPerV = XMFLOAT2(0, 0);
	}
	else if (XMVectorGetX(dif) == 0 && XMVectorGetY(dif) != 0)
	{
		hPerV = XMFLOAT2(0, 1.0f);
	}
	else if (XMVectorGetX(dif) != 0 && XMVectorGetY(dif) == 0)
	{
		hPerV = XMFLOAT2(1.0f, 0);
	}
	else
	{
		//HAVING TROUBLE WITH SIGNS. FIND A WAY TO KEEP THE SIGN WHEN CALCULATING HOW MUCH TO MOVE IN EACH DIRECTION

		//Units horizontal per units vertical to move
		hPerV = XMFLOAT2(XMVectorGetX(dif) / XMVectorGetY(dif), (XMVectorGetY(dif) / 1.0f));
		//Cant have a # larger than 1. If it needs to move more horizontal than vertical, scale horizontal back to 1 and vertical to units/horizontal
		if (abs(hPerV.x) > 1)
		{
			hPerV.y /= hPerV.x;
			hPerV.x /= hPerV.x;
		}
	}

	//This is now the # of units to move this frame
	hPerV = XMFLOAT2((hPerV.x * maxSpeed.x) * dt, (hPerV.y * maxSpeed.y) * dt);

	position.x += hPerV.x;
	position.y += hPerV.y;
}

void Computer::Update(Ball* b, XMFLOAT2 maxSpeed, int level, boolean loc, float dt)
{
	TrackBall(b, maxSpeed, level, loc, dt);
}