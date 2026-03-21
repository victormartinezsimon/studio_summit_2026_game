#include "GameManager.h"
#include "InputManager.h"
#include "Plane.h"
#include "Bullet.h"
#include "Sprites.h"
#include <ctime>
#include <random>
#include "MainMenuState.h"
#include "ImprovementSelectionState.h"
#include "InitialMovementState.h"
#include "EndGameState.h"
#include "BattleState.h"
#include "Star.h"

GameManager::GameManager(InputManager *input, PainterManager *painterManager)
	: _inputManager(input),
	  _painterManager(painterManager), _currentLevel(0), 
	  _currentStateLogic(State::STATES::MENU),_currentScore(0), _numberManager(_painterManager),
	  _alphaManager(_painterManager, &_easingManager), _spawnerStars(TIME_SPAWN_STAR, painterManager), _generator(std::random_device{}())
{
	InitializeConstantValues();
	InitializeImprovementsFunctions();
	InitializeRandomImprovements();
	InitializeStatesBegin();
	InitializeStates();
	
	_spawnerStars.SetCallbackConfiguration([this](Star& star){ConfigureStar(star);});

	_statesBeginFunction[_currentStateLogic]();
	_statesLogic[_currentStateLogic]->OnEnter();
}

void GameManager::InitializeStates()
{
	_statesLogic[State::STATES::MENU] = new MainMenuState(&_player, _painterManager, &_buttonAManager, &_numberManager, &_alphaManager);
	
	_statesLogic[State::STATES::INITIAL_MOVEMENT] = new InitialMovementState(&_player, _painterManager, &_easingManager, &_enemiesPool);
	
	_statesLogic[State::STATES::IMPROVEMENT_SELECTOR] = new ImprovementSelectionState(&_player, _painterManager, &_buttonAManager,
	[this](const std::string& player, const std::string& enemy){ApplyImprovements(player, enemy);}, &_numberManager, &_alphaManager);
	
	_statesLogic[State::STATES::BATTLE] = new BattleState(&_player, _painterManager, &_enemiesPool, &_bulletsPool,
		[this](){DamagePlayer();}, 
		[this](float x, float y){DamageEnemy(x, y);}, 
		&_currentScore, &_currentTimePlaying, &_numberManager, &_alphaManager, &_easingManager);
	
	_statesLogic[State::STATES::END_GAME] = new EndGameState(&_player, _painterManager, &_buttonAManager, &_numberManager, &_alphaManager);
}

void GameManager::InitializeConstantValues()
{
	playerData.velocityBulletX = DEFAULT_BULLET_VEL_X;
	playerData.velocityBulletY = -DEFAULT_BULLET_VEL_Y;
	playerData.fireRate = DEFAULT_FIRE_RATE;
	playerData.bulletsSource = DEFAULT_BULLETS_ORIGIN;
	playerData.bulletHasPenetration = DEFAULT_BULLET_HAS_PENETRATION;
	playerData.bulletHasExplosion = DEFAULT_BULLET_HAS_EXPLOSION;
	playerData.hasShield = DEFAULT_HAS_SHIELD;
	playerData.bulletsPerShot = DEFAULT_BULLETS_PER_SHOT;

	enemyData.velocityBulletX = DEFAULT_BULLET_VEL_X;
	enemyData.velocityBulletY = DEFAULT_BULLET_VEL_Y;
	enemyData.fireRate = DEFAULT_FIRE_RATE;
	enemyData.bulletsSource = DEFAULT_BULLETS_ORIGIN;
	enemyData.bulletHasPenetration = DEFAULT_BULLET_HAS_PENETRATION;
	enemyData.bulletHasExplosion = DEFAULT_BULLET_HAS_EXPLOSION;
	enemyData.hasShield = DEFAULT_HAS_SHIELD;
	enemyData.bulletsPerShot = DEFAULT_BULLETS_PER_SHOT;
}

void GameManager::InitializeImprovementsFunctions()
{
	_improvementFunctions[std::string(IMPROVEMENT_3_SHOTS)] = [](modifiable_data &data)
	{ data.bulletsPerShot = SHOTS_IN_3_SHOTS; };
	_improvementFunctions[std::string(IMPROVEMENT_INCREASE_ORIGIN)] = [](modifiable_data &data)
	{ data.bulletsSource = NEW_EXTRA_SOURCES; };
	_improvementFunctions[std::string(IMPROVEMENT_INCREASE_FIRE_RATE)] = [](modifiable_data &data)
	{ data.fireRate *INCREASE_FIRE_RATE; };
	_improvementFunctions[std::string(IMPROVEMENT_GIVE_PENETRATION)] = [](modifiable_data &data)
	{ data.bulletHasPenetration = true; };
	_improvementFunctions[std::string(IMPROVEMENT_GIVE_EXPLOSION)] = [](modifiable_data &data)
	{ data.bulletHasExplosion = true; };
	_improvementFunctions[std::string(IMPROVEMENT_GIVE_SHIELD)] = [](modifiable_data &data)
	{ data.hasShield = true; };
	_improvementFunctions[std::string(IMPROVEMENT_FAST_SHOTS)] = [](modifiable_data &data)
	{ data.velocityBulletX *= FAST_SHOT_MULTIPLICATION, data.velocityBulletY *= FAST_SHOT_MULTIPLICATION; };
	_improvementFunctions[std::string(IMPROVEMENT_SLOW_SHOTS)] = [](modifiable_data &data)
	{ data.velocityBulletX *= SLOW_SHOT_MULTIPLICATION, data.velocityBulletY *= SLOW_SHOT_MULTIPLICATION; };
}

void GameManager::InitializeRandomImprovements()
{
	int index = 0;
	for(auto kvp: _improvementFunctions)
	{
		std::string key = kvp.first;
		_randomImprovements[index] = key;
		++index;
	}

    std::shuffle(_randomImprovements.begin(), _randomImprovements.end(), _generator);
}

void GameManager::InitializeStatesBegin()
{
	_statesBeginFunction[State::STATES::MENU] = []{};
	_statesBeginFunction[State::STATES::BATTLE] = [this]{};
	_statesBeginFunction[State::STATES::IMPROVEMENT_SELECTOR] = [this]()
	{
		int levelToCheck = _currentLevel -1;
		auto leftImprovement = _randomImprovements[levelToCheck * 2];
		auto rightImprovement = _randomImprovements[levelToCheck * 2 + 1];
		static_cast<ImprovementSelectionState*>(_statesLogic[State::STATES::IMPROVEMENT_SELECTOR])->Configure(leftImprovement, rightImprovement);
	};
	_statesBeginFunction[State::STATES::INITIAL_MOVEMENT] = [this]()
	{
		StartLevel();
	};
	_statesBeginFunction[State::STATES::END_GAME] = [this]()
	{
		static_cast<EndGameState*>(_statesLogic[State::STATES::END_GAME])->Configure(_currentScore);
	};
}

bool GameManager::Update(const float deltaTime)
{
	_lastDeltaTime = deltaTime;
	_currentFrameInputValue = _inputManager->GetInputValue();
	_currentFrameInputValueNormalized = _inputManager->NormalizeValue(_currentFrameInputValue);

	if(_currentStateLogic == State::STATES::BATTLE)
	{
		_currentTimePlaying += deltaTime;
	}

	_alphaManager.Update(deltaTime);
	_spawnerStars.Update(deltaTime);
	_easingManager.Update(deltaTime);
	
	MovePlayer();
	
	auto nextState = _statesLogic[_currentStateLogic]->Update(deltaTime, _currentFrameInputValueNormalized, _currentFrameInputValue);

	if(nextState == State::STATES::EXIT)
	{
		return true;
	}

	if(_currentTimePlaying >= MAX_SECS_PLAYING)
	{
		nextState = State::STATES::END_GAME;
		_currentTimePlaying = 0;
	}

	_oldStateLogic = _currentStateLogic;
	if(nextState != _currentStateLogic)
	{
		_statesLogic[_currentStateLogic]->OnExit();

		if(nextState == State::STATES::IMPROVEMENT_SELECTOR)
		{
			int levelToCheck = _currentLevel;

			if(levelToCheck >= TOTAL_IMPROVEMENTS_TO_SELECT)
			{
				nextState = State::STATES::INITIAL_MOVEMENT;
			}
		}

		if(_oldStateLogic == State::STATES::BATTLE && nextState != State::STATES::END_GAME)
		{
			EndLevel();
		}

		if(_oldStateLogic == State::STATES::MENU)
		{
			_currentScore = 0;
			_currentTimePlaying = 0;
		}

		_statesBeginFunction[nextState]();
		_statesLogic[nextState]->OnEnter();
		_currentStateLogic = nextState;
	}
	return false;
}

void GameManager::Paint()
{
	_painterManager->ClearListPaint();

	int frameRate = 1 / _lastDeltaTime;
	_numberManager.PaintNumber(frameRate, 0, SCREEN_HEIGHT - NUMBER_0_HEIGHT, 2, NumberManager::PIVOT::LEFT);
	
	_statesLogic[_oldStateLogic]->Paint();
	_alphaManager.Paint();
	_spawnerStars.Paint();
}	

void GameManager::ApplyImprovements(const std::string& playerSelection, const std::string& enemySelection)
{
	_improvementFunctions[playerSelection](playerData);
	_improvementFunctions[enemySelection](enemyData);
}

void GameManager::GetMinMaxXPosiblePosition(float &minX, float &maxX) const
{
	minX = 0 + _player.GetWidth() / 2;
	maxX = SCREEN_WIDTH - _player.GetWidth() / 2;
}

void GameManager::MovePlayer()
{
	float minX, maxX;
	GetMinMaxXPosiblePosition(minX, maxX);

	float diffWidth = maxX - minX;
	float position = diffWidth * _currentFrameInputValueNormalized;

	float realPosition = position + minX;

	_player.SetPositionX(realPosition);
}

void GameManager::ConfigurePlane(Plane &p, const float posX, const float posY,
								 const modifiable_data &data, bool isPlayer, float initialDelay)
{
	p.SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	if(posX >=0 && posY >= 0)
	{
		p.SetPosition(posX, posY);
	}
	p.SetBulletsTotalSources(data.bulletsSource);
	p.SetBulletsPerShot(data.bulletsPerShot);
	p.SetFireRate(data.fireRate);
	p.SetHasShield(data.hasShield);
	if(isPlayer)
	{
		p.SetPlayerTeam(TEAM_PLAYER);
	}
	else
	{
		p.SetPlayerTeam(TEAM_ENEMY);
	}
	p.Reset(initialDelay);
	p.SetCallbackFire([this, isPlayer, data](int sourceIndex, const Plane &p)
					  { this->SpawnBullet(sourceIndex, p, isPlayer, data); });
}

void GameManager::SpawnBullet(int sourceIndex, const Plane &p, bool forPlayer, const modifiable_data &data)
{
	int totalBulletsPerShot = p.GetBulletsPerShot();

	for (int bulletPerShot = 0; bulletPerShot < totalBulletsPerShot; ++bulletPerShot)
	{
		auto id = _bulletsPool.Get();
		float velocityBulletX = 0;
		if (bulletPerShot == 1)
		{
			velocityBulletX = data.velocityBulletY * 0.5;
		}
		if (bulletPerShot == 2)
		{
			velocityBulletX = -data.velocityBulletY * 0.5;
		}

		_bulletsPool.call_for_element(id, [this, sourceIndex, p, forPlayer, data, velocityBulletX](Bullet &bullet)
									  {
			bullet.SetSize(BULLET_WIDTH, BULLET_HEIGHT);

			float positionX = p.GetX();
			if (sourceIndex == 1)
			{
				positionX = p.GetX() - p.GetWidth() / 2;
			}

			if (sourceIndex == 2)
			{
				positionX = p.GetX() + p.GetWidth() / 2;
			}

			bullet.SetPosition(positionX, p.GetY());
			bullet.SetVelocity(velocityBulletX, data.velocityBulletY);
			bullet.SetSize(BULLET_WIDTH, BULLET_HEIGHT);
			if(forPlayer)
			{
				bullet.SetPlayerTeam(TEAM_PLAYER);
			}
			else
			{
				bullet.SetPlayerTeam(TEAM_ENEMY);
			}
			bullet.SetHasPenetration(data.bulletHasPenetration);
			bullet.SetHasExplostion(data.bulletHasExplosion); });
	}
}
void GameManager::StartLevel()
{
	SpawnEnemies();
	ConfigurePlane(_player, -1, -1, playerData, true, 0);
}

void GameManager::EndLevel()
{
	++_currentLevel;
	_currentScore += SCORE_PER_FINISH_LEVEL;
}

void GameManager::GetMinMaxXPosiblePositionForEnemies(float &minX, float &maxX) const
{
	minX = 0 + ENEMY_WIDTH / 2;
	maxX = SCREEN_WIDTH - ENEMY_WIDTH / 2;
}

void GameManager::SpawnEnemies()
{
	int levelConfigID = std::min(_currentLevel, TOTAL_LEVELS_CONFIG - 1);
	int enemiesToSpawn = LEVELS_CONFIGS[levelConfigID];

	float currentY = SCREEN_HEIGHT * 0.1f;

	while (enemiesToSpawn > 0)
	{
		int enemiesRow = std::min(enemiesToSpawn, MAX_ENEMIES_PER_ROW);
		enemiesToSpawn -= enemiesRow;
		SpawnRowEnemies(enemiesRow, currentY);
		currentY += SCREEN_HEIGHT * 0.05f + ENEMY_HEIGHT;
	}
}
void GameManager::SpawnRowEnemies(int enemiesToSpawn, float posY)
{
	float minX, maxX;
	GetMinMaxXPosiblePositionForEnemies(minX, maxX);

	float screenWidth = maxX - minX;
	float holeDistance = (screenWidth - (enemiesToSpawn * ENEMY_WIDTH)) / enemiesToSpawn;

	float currentPositionX = minX + holeDistance / 2 + ENEMY_WIDTH / 2;
	for (int enemyID = 0; enemyID < enemiesToSpawn; ++enemyID)
	{
		float posX = currentPositionX;
		currentPositionX += ENEMY_WIDTH + holeDistance;

		auto id = _enemiesPool.Get();

		std::uniform_real_distribution<float> delayDist(MIN_SHOOTING_DELAY, MAX_SHOOTING_DELAY);
		float delay = delayDist(_generator);

		_enemiesPool.call_for_element(id, [posX, posY, this, delay](Plane &enemy)
									  { ConfigurePlane(enemy, posX, posY, enemyData, false, delay); });
	}
}

void GameManager::DamagePlayer()
{
	_currentScore -= SCORE_PER_PLAYER_HIT;
	if(_currentScore < 0)
	{
		_currentScore = 0;
	}
}


void GameManager::DamageEnemy(float x, float y)
{
	_currentScore += SCORE_PER_KILL;
}

void GameManager::ConfigureStar(Star& star)
{
	std::uniform_real_distribution<float> velocityDist(MIN_VELOCITY_STAR, MAX_VELOCITY_STAR);
    float velocity = velocityDist(_generator);

    std::uniform_real_distribution<float> heightDist(MIN_HEIGHT_STAR, MAX_HEIGHT_STAR);
    float height = heightDist(_generator);

	std::uniform_int_distribution<int> typeDist(0, 2);
	int type = typeDist(_generator);

	switch (type)
	{
	case 0:
		star.SetSize(NEAR_STAR_WIDTH, NEAR_STAR_HEIGHT);
		star.SetPosition(-static_cast<int>(NEAR_STAR_WIDTH), SCREEN_HEIGHT*height);
		star.SetTypeStar(Star::Type::NEAR);
		velocity *= VELOCITY_STAR_NEAR;
		break;
	case 1:
		star.SetSize(MID_STAR_WIDTH, MID_STAR_HEIGHT);
		star.SetPosition(-static_cast<int>(MID_STAR_WIDTH), SCREEN_HEIGHT*height);
		star.SetTypeStar(Star::Type::MID);
		velocity *= VELOCITY_STAR_MID;
		break;
	case 2:
		star.SetSize(FAR_STAR_WIDTH, FAR_STAR_HEIGHT);
		star.SetPosition(-static_cast<int>(FAR_STAR_WIDTH), SCREEN_HEIGHT*height);
		star.SetTypeStar(Star::Type::FAR);
		velocity *= VELOCITY_STAR_FAR;
		break;
	}

	star.SetVelocities(velocity, velocity*0.5);
	star.SetMoveLeft(false);
}
