#pragma once

#include "Pool.h"
#include "GameConfig.h"
#include <map>
#include <functional>
#include "Plane.h"
#include "Bullet.h"
#include "EasingManager.h"
#include "ButtonA.h"
#include <array>
#include "PainterManager.h"
#include "State.h"

class InputManager;

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
	void InitializeConstantValues();
	void InitializeImprovementsFunctions();
	void InitializeRandomImprovements();
	void InitializeStatesBegin();

private:
	void GetMinMaxXPosiblePosition(float &minX, float &maxX) const;
	void MovePlayer();

private:
	void ConfigurePlane(Plane &p, const float posX, const float posY, const modifiable_data &data, bool isPlayer);
	void DamagePlayer();

private:
	void SpawnEnemies();
	void SpawnRowEnemies(int totalEnemies, float posY);
	void GetMinMaxXPosiblePositionForEnemies(float &minX, float &maxX) const;

private:
	void SpawnBullet(int sourceIndex, const Plane &p, bool forPlayer, const modifiable_data &data);

private:
	void StartLevel();
	void EndLevel();

private:
	void ApplyImprovements(const std::string &playerSelection, const std::string &enemySelection);

private:
	modifiable_data playerData;
	modifiable_data enemyData;

private:
	InputManager *_inputManager;
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
	std::array<std::string, TOTAL_IMPROVEMENTS_TO_SELECT * 2> _randomImprovements;

private:
	std::map<State::STATES, State *> _statesLogic;
	State::STATES _currentStateLogic;
	State::STATES _oldStateLogic;
	std::map<State::STATES, std::function<void()>> _statesBeginFunction;
};
