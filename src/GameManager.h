#pragma once

#include "Pool.h"
#include "GameConfig.h"

class InputManager;
class Plane;
class Bullet;

class GameManager
{
public:
	GameManager(InputManager *input, Plane *player, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool, Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool);

public:
	void Update(const float deltaTime);
	void UpdateMenu(const float deltaTime);
	void UpdateBattle(const float deltaTime);
	void UpdateImprovement(const float deltaTime);
	void UpdateInitialMovement(const float deltaTime);

private:
	void GetMinMaxXPosiblePosition(float& minX, float& maxX)const;
	void MovePlayer();

private:
	enum class STATES
	{
		MENU,
		BATTLE,
		IMPROVEMENT_SELECTOR,
		INITIAL_MOVEMENT
	};

private:
	InputManager *_inputManager;
	STATES _currentState = STATES::MENU;
	int _currentLevel = 0;
	Plane *_player;
	Pool<Plane, PLANES_POOL_SIZE> *_enemiesPool;
	Pool<Bullet, BULLETS_POOL_SIZE> *_bulletsPool;
};
