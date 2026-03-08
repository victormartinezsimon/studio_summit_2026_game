#pragma once

#include "Pool.h"
#include "GameConfig.h"

class InputManager;
class Plane;
class Bullet;
class PainterManager;

class GameManager
{
public:
	GameManager(InputManager *input, Plane *player, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
				Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, PainterManager *painterManager);

public:
	void Update(const float deltaTime);

private:
	void UpdateMenu(const float deltaTime);
	void UpdateBattle(const float deltaTime);
	void UpdateImprovement(const float deltaTime);
	void UpdateInitialMovement(const float deltaTime);

public:
	void Paint() const;

private:
	void PaintMenu() const;
	void PaintBattle() const;
	void PaintImprovements() const;
	void PaintInitialMovement()const;

private:
	void GetMinMaxXPosiblePosition(float &minX, float &maxX) const;
	void MovePlayer();

private:
	void ConfigurePlayer();
	void SpanwPlayerBullet(int index, Plane *p);

private:
	enum class STATES
	{
		MENU,
		BATTLE,
		IMPROVEMENT_SELECTOR,
		INITIAL_MOVEMENT
	};

	private:
	float currentPlayerVelocitiyBulletX; 
	float currentPlayerVelocitiyBulletY; 
	float currentPlayerFireRate;


private:
	InputManager *_inputManager;
	STATES _currentState = STATES::MENU;
	int _currentLevel = 0;
	Plane *_player;
	Pool<Plane, PLANES_POOL_SIZE> *_enemiesPool;
	Pool<Bullet, BULLETS_POOL_SIZE> *_bulletsPool;
	PainterManager *_painterManager;
};
