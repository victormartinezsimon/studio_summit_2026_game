#pragma once

#include "Pool.h"
#include "GameConfig.h"
#include <map>
#include <functional>
#include "Plane.h"
#include "Bullet.h"
#include "EasingManager.h"
#include "ButtonA.h"

class InputManager;
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
	GameManager(InputManager *input, PainterManager *painterManager);

public:
	void Update(const float deltaTime);
	void Paint();

private:
	void UpdateEnterMenu(const float deltaTime);
	void UpdateMenu(const float deltaTime);
	void UpdateBattle(const float deltaTime);
	void UpdateImprovement(const float deltaTime);
	void UpdateInitialMovement(const float deltaTime);
	void UpdateEnterInicialMovement( const float deltaTime);

private:
	void PaintMenu();
	void PaintBattle();
	void PaintImprovements();
	void PaintInitialMovement();

private:
	void InitializeConstantValues();
	void InitializeImprovementsFunctions();

private:
	void GetMinMaxXPosiblePosition(float &minX, float &maxX) const;
	void MovePlayer();

private:
	void ConfigurePlane(Plane& p, const float posX, const float posY, const modifiable_data& data, bool isPlayer);
	void UpdateBullets(float deltaTime);
	void ManageBulletCollisions(Bullet& bullet);
	bool ManageCollisionBetweenBulletAndEnemy(Bullet& bullet, Plane& enemy);
	bool HasCollision(const Bullet& bullet, Plane& plane) const;
	bool CollsisionDetection(float ax, float ay, float aw, float ah,
							 float bx, float by, float bw, float bh) const;
	void DamagePlayer(); 
private:
	void SpawnEnemies();
	void SpawnRowEnemies(int totalEnemies, float posY);
	void GetMinMaxXPosiblePositionForEnemies(float &minX, float &maxX) const;

private:
	void UpdateEnemies(float deltaTime);

private:
	void SpawnBullet(int sourceIndex, const Plane& p, bool forPlayer, const modifiable_data &data);

private:
	void StartLevel();
	void EndLevel();


private:
	void DoExplosion(Bullet& bullet);

private:
	void PlayInitialAnimation();

private:
	enum class STATES
	{
		ENTER_IN_MENU,
		MENU,
		BATTLE,
		IMPROVEMENT_SELECTOR,
		ENTER_IN_INITIAL_MOVEMENT,
		INITIAL_MOVEMENT
	};

private:
	modifiable_data playerData;
	modifiable_data enemyData;

private:
	InputManager *_inputManager;
	STATES _currentState = STATES::MENU;
	int _currentLevel = 0;
	Plane _player;
	Pool<Plane, PLANES_POOL_SIZE> _enemiesPool;
	Pool<Bullet, BULLETS_POOL_SIZE> _bulletsPool;
	PainterManager *_painterManager;
	EasingManager _easingManager;
	ButtonA _buttonAManager;
	float _currentFrameInputValueNormalized;
	int _currentFrameInputValue;

private:
	std::map<std::string, std::function<void(modifiable_data &)>> _improvementFunctions;
};
