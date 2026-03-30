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
#include "Profiler.h"
#include "TrailManager.h"

constexpr std::array<PainterManager::SPRITE_ID, 2> SPRITE_IDS_ANIMATION_HIT = {PainterManager::SPRITE_ID::NUMBER_0, PainterManager::SPRITE_ID::NUMBER_1};
constexpr std::array<PainterManager::SPRITE_ID, 2> SPRITE_IDS_ANIMATION_KILL = {PainterManager::SPRITE_ID::NUMBER_0, PainterManager::SPRITE_ID::NUMBER_5};
constexpr std::array<PainterManager::SPRITE_ID, 3> SPRITE_IDS_ANIMATION_LEVEL = {PainterManager::SPRITE_ID::NUMBER_0, PainterManager::SPRITE_ID::NUMBER_0, PainterManager::SPRITE_ID::NUMBER_1};

GameManager::GameManager(InputManager *input, PainterManager *painterManager)
	: _inputManager(input),
	  _painterManager(painterManager), _currentLevel(0), 
	  _currentStateLogic(State::STATES::MENU),_currentScore(0), _numberManager(_painterManager),
	  _alphaManager(_painterManager), _spawnerStars(TIME_SPAWN_STAR, painterManager),
	  _spawnerMeteorites(TIME_SPAWN_METEORITE, painterManager)
{
	InitializeImprovementsFunctions();
	InitializeStatesBegin();
	InitializeStates();
	
	_spawnerStars.SetCallbackConfiguration([this](Star& star){ConfigureStar(star);});
    _spawnerMeteorites.SetCallbackConfiguration([this](Meteorite& m){ConfigureMeteoriteSpawn(m);});

	_statesBeginFunction[_currentStateLogic]();
	_statesLogic[_currentStateLogic]->OnEnter();
}
void GameManager::InitializeStates()
{
	_statesLogic[State::STATES::MENU] = new MainMenuState(&_player, _painterManager, &_numberManager, &_alphaManager, 
		&_easingManager, &_randomManager, &_buttonAManager);
	
	_statesLogic[State::STATES::INITIAL_MOVEMENT] = new InitialMovementState(&_player, _painterManager, &_numberManager, &_alphaManager, 
		&_easingManager, &_randomManager, &_buttonAManager, &_enemiesPool);
	
	_statesLogic[State::STATES::IMPROVEMENT_SELECTOR] = new ImprovementSelectionState(&_player, _painterManager, &_numberManager, &_alphaManager, 
		&_easingManager, &_randomManager, &_buttonAManager,
	[this](const std::string& player, const std::string& enemy){ApplyImprovements(player, enemy);});
	
	_statesLogic[State::STATES::BATTLE] = new BattleState(&_player, _painterManager, &_numberManager, &_alphaManager, 
		&_easingManager, &_randomManager, &_buttonAManager,&_enemiesPool, &_bulletsPool,
		[this](){DamagePlayer();}, 
		[this](float x, float y){DamageEnemy(x, y);}, 
		&_currentScore, &_currentTimePlaying, &_spawnerMeteorites, &_trailManager);
	
	_statesLogic[State::STATES::END_GAME] = new EndGameState(&_player, _painterManager, &_numberManager, &_alphaManager, 
		&_easingManager, &_randomManager, &_buttonAManager);
}
void GameManager::InitializeConstantValues()
{
	playerData.velocityBulletX = DEFAULT_BULLET_VEL_X;
	playerData.velocityBulletY = -DEFAULT_BULLET_VEL_Y;
	playerData.fireRate = DEFAULT_FIRE_RATE;
	playerData.bulletsSource = DEFAULT_BULLETS_ORIGIN;
	playerData.bulletHasPenetration = DEFAULT_BULLET_HAS_PENETRATION;
	playerData.bulletHasExplosion = true;//DEFAULT_BULLET_HAS_EXPLOSION;
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

	_totalImprovementSelected = 0;
	InitializeRandomImprovements();
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
	std::mt19937 generator(std::random_device{}());
    std::shuffle(_randomImprovements.begin(), _randomImprovements.end(), generator);
}
void GameManager::InitializeStatesBegin()
{
	_statesBeginFunction[State::STATES::MENU] = []{};
	_statesBeginFunction[State::STATES::BATTLE] = [this]
	{
		static_cast<BattleState*>(_statesLogic[State::STATES::BATTLE])->SetCurrentLevel(_currentLevel);
	};
	_statesBeginFunction[State::STATES::IMPROVEMENT_SELECTOR] = [this]()
	{
		auto leftImprovement = _randomImprovements[_totalImprovementSelected * 2];
		auto rightImprovement = _randomImprovements[_totalImprovementSelected * 2 + 1];
		++_totalImprovementSelected;
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
	//PROFILE_BEGIN_FRAME();
	_lastDeltaTime = deltaTime;
	if(_countFramesToReadInput <= 0)
	{	PROFILE_BEGIN(0, "INPUT");
		_currentFrameInputValueNormalized = _inputManager->GetInputValueNormalized();
		_lastFrameInputValueNormalized = _currentFrameInputValueNormalized;
		_countFramesToReadInput = NUM_FRAMES_TO_READ_INPUT;
		PROFILE_END(0);
	}
	else
	{
		_currentFrameInputValueNormalized = _lastFrameInputValueNormalized;
	}
	--_countFramesToReadInput;
	

	if(_currentStateLogic == State::STATES::BATTLE)
	{
		_currentTimePlaying += deltaTime;
	}

	PROFILE_BEGIN(1, "update easing");
	_easingManager.Update(deltaTime);
	PROFILE_END(1);

	PROFILE_BEGIN(2, "update alphamanager");
	_alphaManager.Update(deltaTime);
	PROFILE_END(2);

	PROFILE_BEGIN(3, "update spawner starts");
	if(deltaTime < MIN_TIME_TO_NOT_UPDATE_STARS )
	{
		_spawnerStars.Update(deltaTime);
	}
	PROFILE_END(3);

	PROFILE_BEGIN(4, "update spawner meteorites");
	_spawnerMeteorites.Update(deltaTime);
	PROFILE_END(4);

	PROFILE_BEGIN(8, "update spawner meteorites");
	_trailManager.Update(deltaTime);
	PROFILE_END(8);

	PROFILE_BEGIN(5, "move player");
	MovePlayer();
	PROFILE_END(5);

	PROFILE_BEGIN(6, "update state");
	auto nextState = _statesLogic[_currentStateLogic]->Update(deltaTime, _currentFrameInputValueNormalized);
	PROFILE_END(6);

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
	PROFILE_BEGIN(7, "try change state");
	if(nextState != _currentStateLogic)
	{
		_statesLogic[_currentStateLogic]->OnExit();

		if(nextState == State::STATES::IMPROVEMENT_SELECTOR)
		{
			if(_totalImprovementSelected >= LEVELS_WITH_IMPROVEMENT_SELECTION.size() || _totalImprovementSelected >= TOTAL_IMPROVEMENTS_TO_SELECT)
			{
				nextState = State::STATES::INITIAL_MOVEMENT;
			}
			else
			{
				if(LEVELS_WITH_IMPROVEMENT_SELECTION[_totalImprovementSelected] != _currentLevel)
				{
					nextState = State::STATES::INITIAL_MOVEMENT;
				}
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
			_currentLevel = 0;
			InitializeConstantValues();
		}
		if(_oldStateLogic == State::STATES::END_GAME)
		{
			InitializeConstantValues();
		}

		_statesBeginFunction[nextState]();
		_statesLogic[nextState]->OnEnter();
		_currentStateLogic = nextState;
	}
	PROFILE_END(7);


	//PROFILE_END_FRAME();
	return false;
}
void GameManager::Paint()
{
	_painterManager->ClearListPaint();

	#ifdef DEBUG
	int frameRate = 1 / _lastDeltaTime;
	_numberManager.PaintNumber(frameRate, SCREEN_WIDTH, NUMBER_0_HEIGHT, 2, NumberManager::PIVOT::RIGHT);
	#endif
	
	_statesLogic[_oldStateLogic]->Paint();
	_statesLogic[_oldStateLogic]->PaintUI();
	_alphaManager.Paint();
	_spawnerMeteorites.Paint(_painterManager);
	_trailManager.Paint(_painterManager);
	_spawnerStars.Paint(_painterManager);
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
	p.ConfigureSprite(_painterManager);

	p.SetCallbackFire([this, isPlayer, data](int sourceIndex, const Plane &p)
					  { this->SpawnBullet(sourceIndex, p, isPlayer, data); });
}
void GameManager::SpawnBullet(int sourceIndex, const Plane &p, bool forPlayer, const modifiable_data &data)
{
	int totalBulletsPerShot = p.GetBulletsPerShot();

	for (int bulletPerShot = 0; bulletPerShot < totalBulletsPerShot; ++bulletPerShot)
	{
		auto id = _bulletsPool.Get();

		if(id != -1)
		{
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
				bullet.ConfigureSprite(_painterManager);

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

				if(forPlayer)
				{
					bullet.SetPlayerTeam(TEAM_PLAYER);
				}
				else
				{
					bullet.SetPlayerTeam(TEAM_ENEMY);
				}
				bullet.SetHasPenetration(data.bulletHasPenetration);
				bullet.SetHasExplostion(data.bulletHasExplosion); 
			});
		}
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

	AnimateNumberScore(SPRITE_IDS_ANIMATION_LEVEL, true);
}
template <unsigned int N>
void GameManager::AnimateNumberScore(const std::array<PainterManager::SPRITE_ID, N> elements, bool up)
{
	float currentX = SCORE_POSITION_X - NUMBER_0_WIDTH;
	float currentY = SCORE_POSITION_Y - NUMBER_0_HEIGHT/2;
	float endY = currentY - NUMBER_0_HEIGHT;
	if(!up)
	{
		endY = currentY + NUMBER_0_HEIGHT;
	}

	for(auto spriteID : elements)
	{
		int alphaID = _alphaManager.AddAlpha(DURATION_EASING_SCORE,
			currentX, currentY, spriteID, NUMBER_0_WIDTH, NUMBER_0_HEIGHT
			);

		int easeID = _easingManager.AddEase(DURATION_EASING_SCORE, currentX, currentY, 
			currentX, endY, Ease::EASE_TYPES::INOUTCIRC, 
			[](bool forced){},
			[&](float x, float y, Ease& ease, float percent)
			{
				int idAlpha = ease.GetReferenceID();
				_alphaManager.CallFunctionInPool(idAlpha, [&](Alpha& alpha){alpha.SetPosition(x, y);});
			}
		);
		_easingManager.SetReferenceIDToEase(easeID, alphaID);
		currentX -= NUMBER_0_WIDTH;
	}
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

	float currentY = SCREEN_HEIGHT * MIN_Y_ENEMY;

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
		if(id != -1)
		{
			float delay =_randomManager.GetValue(MIN_SHOOTING_DELAY, MAX_SHOOTING_DELAY, 100.0f);
			_enemiesPool.call_for_element(id, [posX, posY, this, delay](Plane &enemy)
									  { ConfigurePlane(enemy, posX, posY, enemyData, false, delay); });
		}
	}
}
void GameManager::DamagePlayer()
{
	_currentScore -= SCORE_PER_PLAYER_HIT;
	if(_currentScore < 0)
	{
		_currentScore = 0;
	}
	else
	{
		AnimateNumberScore(SPRITE_IDS_ANIMATION_HIT, false);
	}
}
void GameManager::DamageEnemy(float x, float y)
{
	_currentScore += SCORE_PER_KILL;
	AnimateNumberScore(SPRITE_IDS_ANIMATION_KILL, true);
}
void GameManager::ConfigureStar(Star& star)
{
    float velocity = _randomManager.GetValue(MIN_VELOCITY_STAR, MAX_VELOCITY_STAR, 100.0f);
    float y = _randomManager.GetValue(MIN_HEIGHT_STAR, MAX_HEIGHT_STAR, 100.0f) * SCREEN_HEIGHT;

	int typeValue = _randomManager.GetValue(0,9);
	float x =-static_cast<int>(MID_STAR_WIDTH);

	if( y < 0)
	{
		x = _randomManager.GetValue(0, SCREEN_WIDTH * 0.5, 100.0f);
		//y = 0;
	}

	int type = 0;//0 1 2 3 4
	if(typeValue >= 5 && typeValue < 8)
	{
		type = 1;//5 6 7
	}
	else
	{
		type = 2;//8 9
	}

	switch (type)
	{
	case 0:
		star.SetSize(NEAR_STAR_WIDTH, NEAR_STAR_HEIGHT);
		star.SetPosition(x, y);
		star.SetTypeStar(Star::Type::NEAR);
		velocity *= VELOCITY_STAR_NEAR;
		break;
	case 1:
		star.SetSize(MID_STAR_WIDTH, MID_STAR_HEIGHT);
		star.SetPosition(x, y);
		star.SetTypeStar(Star::Type::MID);
		velocity *= VELOCITY_STAR_MID;
		break;
	case 2:
		star.SetSize(FAR_STAR_WIDTH, FAR_STAR_HEIGHT);
		star.SetPosition(x, y);
		star.SetTypeStar(Star::Type::FAR);
		velocity *= VELOCITY_STAR_FAR;
		break;
	}

	star.SetVelocities(velocity, velocity*0.5);
	star.SetMoveLeft(false);
	star.ConfigureSprite(_painterManager);
}
void GameManager::ConfigureMeteoriteSpawn(Meteorite& meteorite)
{
    meteorite.SetSize(METEORITE_WIDTH, METEORITE_HEIGHT);
	meteorite.ConfigureSprite(_painterManager);

    bool goingLeft = _randomManager.GetNextIntValue() % 2;
    float velocity = _randomManager.GetValue(MIN_VELOCITY_METEORITE, MAX_VELOCITY_METEORITE, 100.0f);
    float height =  _randomManager.GetValue(MIN_HEIGHT_METEORITE, MAX_HEIGHT_METEORITE, 100.0f);

    if(goingLeft)
    {
        meteorite.SetSize(METEORITE_WIDTH, METEORITE_HEIGHT);
        meteorite.SetPosition(SCREEN_WIDTH + METEORITE_WIDTH, SCREEN_HEIGHT*height);
        meteorite.SetVelocities(-DEFAULT_BULLET_VEL_Y * velocity, 0);
        meteorite.SetMoveLeft(true);
    }
    else
    {
        meteorite.SetSize(METEORITE_WIDTH, METEORITE_HEIGHT);
        meteorite.SetPosition(-static_cast<int>(METEORITE_WIDTH), SCREEN_HEIGHT*height);
        meteorite.SetVelocities(DEFAULT_BULLET_VEL_Y * velocity,0);
        meteorite.SetMoveLeft(false);
    }
}