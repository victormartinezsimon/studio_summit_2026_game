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
#include "NumberManager.h"
#include "Spawner.h"
#include "Star.h"
#include "RandomManager.h"
#include "Meteorite.h"
#include "TrailManager.h"

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
	~GameManager();

public:
	bool Update(const float deltaTime);
	void Paint();

private:
	void InitializeConstantValues();
	void InitializeImprovementsFunctions();
	void InitializeRandomImprovements();
	void InitializeStatesBegin();
	void InitializeStates();
	void ConfigureStar(Star& star);

private:
	void GetMinMaxXPosiblePosition(float &minX, float &maxX) const;
	void MovePlayer();

private:
	void ConfigurePlane(Plane &p, const float posX, const float posY, const modifiable_data &data, bool isPlayer, float initialDelay);
	void DamagePlayer();
	void DamageEnemy(float x, float y);

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
	void ConfigureMeteoriteSpawn(Meteorite& meteorite);

private:

	template <unsigned int N>
	void AnimateNumberScore(const std::array<int, N> elements, bool up);

private:
	modifiable_data playerData;
	modifiable_data enemyData;

private:
	InputManager *_inputManager;
	Plane _player;
	Pool<Plane, PLANES_POOL_SIZE> _enemiesPool;
	Pool<Bullet, BULLETS_POOL_SIZE> _bulletsPool;
	PainterManager *_painterManager;
	EasingManager _easingManager;
	ButtonA _buttonAManager;
	NumberManager _numberManager;
	Spawner<Star, TOTAL_STARS> _spawnerStars;
	RandomManager _randomManager;
	TrailManager _trailManager;
	
private:
	float _currentFrameInputValueNormalized;
	float _lastFrameInputValueNormalized;
	int _countFramesToReadInput = NUM_FRAMES_TO_READ_INPUT;

private:
	int _currentLevel = 0;
	float _currentTimePlaying = 0;
	long long _currentScore = 0;
	float _lastDeltaTime;
	int _totalImprovementSelected = 0;

private:
	std::map<std::string, std::function<void(modifiable_data &)>> _improvementFunctions;
	std::array<std::string, TOTAL_DEFINED_IMPROVEMENTS> _randomImprovements;

private:
	std::map<State::STATES, State*> _statesLogic;
	State::STATES _currentStateLogic;
	State::STATES _oldStateLogic;
	std::map<State::STATES, std::function<void()>> _statesBeginFunction;
	Spawner<Meteorite, TOTAL_METEORITES> _spawnerMeteorites;
	Pool<WorldObject, TOTAL_NUMBER_ANIMATIONS> _numbersAnimation;
};
