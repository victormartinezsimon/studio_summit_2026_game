#pragma once

#include "Pool.h"
#include "GameConfig.h"
#include <map>
#include <functional>

class InputManager;
class Plane;
class Bullet;
class PainterManager;

class GameManager
{
private:
	struct modifiable_data
	{
		float velocityBulletX;
		float velocityBulletY;
		float fireRate;
		int bulletsSource;
		bool bulletHasPenetration;
		bool bulletHasExplosion;
		bool hasShield;
		int bulletsPerShot;
	};

public:
	GameManager(InputManager *input, Plane *player, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
				Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, PainterManager *painterManager);

public:
	void Update(const float deltaTime);
	void Paint() const;

private:
	void UpdateMenu(const float deltaTime);
	void UpdateBattle(const float deltaTime);
	void UpdateImprovement(const float deltaTime);
	void UpdateInitialMovement(const float deltaTime);

private:
	void PaintMenu() const;
	void PaintBattle() const;
	void PaintImprovements() const;
	void PaintInitialMovement() const;

private:
	void InitializeConstantValues();
	void InitializeImprovementsFunctions();

private:
	void GetMinMaxXPosiblePosition(float &minX, float &maxX) const;
	void MovePlayer();

private:
	void ConfigurePlayer();
	void UpdateBullets(float deltaTime);
	bool HasCollision(const Bullet *bullet, Plane *plane) const;
	bool CollsisionDetection(float ax, float ay, float aw, float ah,
							 float bx, float by, float bw, float bh) const;

private:
	void UpdateEnemies(float deltaTime);
	void ConfigureEnemy(Plane *enemy);

private:
	void SpawnBullet(int sourceIndex, Plane *p, bool forPlayer, const modifiable_data &data);

private:
	void StartLevel();
	void EndLevel();
	void SpawnEnemies();
	void SpawnRowEnemies(int totalEnemies, float posY);
	void GetMinMaxXPosiblePositionForEnemies(float &minX, float &maxX) const;

private:
	void DoExplosion(Bullet* bullet, std::vector<Plane *>& enemiesToDelete);

private:
	enum class STATES
	{
		MENU,
		BATTLE,
		IMPROVEMENT_SELECTOR,
		INITIAL_MOVEMENT
	};

private:
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

private:
	std::map<std::string, std::function<void(modifiable_data &)>> _improvementFunctions;
};
