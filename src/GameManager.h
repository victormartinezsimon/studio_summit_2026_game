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
	void PaintInitialMovement() const;

private:
	void GetMinMaxXPosiblePosition(float &minX, float &maxX) const;
	void MovePlayer();

private:
	void ConfigurePlayer();
	void SpawnPlayerBullet(int index, Plane *p);
	void UpdateBullets(float deltaTime);
	bool HasCollision(const Bullet *bullet, const Plane *plane) const;
	bool CollsisionDetection(float ax, float ay, float aw, float ah,
							 float bx, float by, float bw, float bh) const;

private:
	void UpdateEnemies(float deltaTime);
	void ConfigureEnemy(Plane *enemy);
	void SpawnEnemyBullet(int index, Plane *p);

private:
	void StartLevel();
	void EndLevel();
	void SpawnEnemies();
	void SpawnRowEnemies(int totalEnemies, float posY);
	void GetMinMaxXPosiblePositionForEnemies(float &minX, float &maxX) const;

private:
	enum class STATES
	{
		MENU,
		BATTLE,
		IMPROVEMENT_SELECTOR,
		INITIAL_MOVEMENT
	};

private:
	struct modifiable_data
	{
		float velocityBulletX;
		float velocityBulletY;
		float fireRate;
		int bulletsOrigin;
		bool bulletHasPenetration;
		bool bulletHasExplosion;
		bool hasShield;
	};
	
	modifiable_data playerData;
	modifiable_data enemyData;

private:
	InputManager *_inputManager;
	STATES _currentState = STATES::MENU;
	int _currentLevel = 0;
	Plane *_player;
	Pool<Plane, PLANES_POOL_SIZE> *_enemiesPool;
	Pool<Bullet, BULLETS_POOL_SIZE> *_bulletsPool;
	PainterManager *_painterManager;
};
