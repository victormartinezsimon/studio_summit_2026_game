#include "GameManager.h"
#include "InputManager.h"
#include "Plane.h"
#include "Bullet.h"
#include "Sprites.h"
#include <ctime>
#include <random>

GameManager::GameManager(InputManager *input, PainterManager *painterManager)
	: _inputManager(input),
	  _painterManager(painterManager), _currentState(STATES::ENTER_IN_MENU), _currentLevel(0)
{
	InitializeConstantValues();
	InitializeImprovementsFunctions();
	InitializeImprovementsUI();
	InitializeRandomImprovements();
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

void GameManager::InitializeImprovementsUI()
{
	_improvementsUI[std::string(IMPROVEMENT_3_SHOTS)] = PainterManager::SPRITE_ID::SHOT_3_TIMES;
	_improvementsUI[std::string(IMPROVEMENT_INCREASE_ORIGIN)] = PainterManager::SPRITE_ID::INCREASE_ORIGIN;
	_improvementsUI[std::string(IMPROVEMENT_INCREASE_FIRE_RATE)] = PainterManager::SPRITE_ID::INCRASE_FIRE_RATE;
	_improvementsUI[std::string(IMPROVEMENT_GIVE_PENETRATION)] = PainterManager::SPRITE_ID::GIVE_PENETRATION;
	_improvementsUI[std::string(IMPROVEMENT_GIVE_EXPLOSION)] = PainterManager::SPRITE_ID::GIVE_EXPLOSION;
	_improvementsUI[std::string(IMPROVEMENT_GIVE_SHIELD)] = PainterManager::SPRITE_ID::GIVE_SHIELD;
	_improvementsUI[std::string(IMPROVEMENT_FAST_SHOTS)] = PainterManager::SPRITE_ID::FAST_SHOTS;
	_improvementsUI[std::string(IMPROVEMENT_SLOW_SHOTS)] = PainterManager::SPRITE_ID::SLOW_SHOTS;
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

	std::mt19937 rng(std::random_device{}());
    std::shuffle(_randomImprovements.begin(), _randomImprovements.end(), rng);
}


void GameManager::Update(const float deltaTime)
{
	_currentFrameInputValue = _inputManager->GetInputValue();
	_currentFrameInputValueNormalized = _inputManager->NormalizeValue(_currentFrameInputValue);

	switch (_currentState)
	{
	case STATES::BATTLE:
		UpdateBattle(deltaTime);
		break;
	case STATES::ENTER_IN_MENU:
		UpdateEnterMenu( deltaTime );
	case STATES::MENU:
		UpdateMenu(deltaTime);
		break;
	case STATES::ENTER_IN_IMPROVEMENT_SELECTOR:
		UpdateEnterImprovement(deltaTime);
		break;
	case STATES::IMPROVEMENT_SELECTOR:
		UpdateImprovement(deltaTime);
		break;
	case STATES::INITIAL_MOVEMENT:
		UpdateInitialMovement(deltaTime);
		break;
	case STATES::ENTER_IN_INITIAL_MOVEMENT:
		UpdateEnterInicialMovement(deltaTime);
		
	}
}

void GameManager::Paint()
{
	_painterManager->ClearListPaint();
	switch (_currentState)
	{
	case STATES::BATTLE:
		PaintBattle();
		break;
	case STATES::ENTER_IN_MENU:
	case STATES::MENU:
		PaintMenu();
		break;
	case STATES::ENTER_IN_IMPROVEMENT_SELECTOR:
	case STATES::IMPROVEMENT_SELECTOR:
		PaintImprovements();
		break;
	case STATES::ENTER_IN_INITIAL_MOVEMENT:
	case STATES::INITIAL_MOVEMENT:
		PaintInitialMovement();
		break;
	}
}

void GameManager::UpdateEnterMenu(const float deltaTime)
{
	_buttonAManager.SelectInPosition(MAIN_MENU_TIME_TO_ENTER, {SCREEN_WIDTH * MAIN_MENU_MIN_VALUE, SCREEN_WIDTH * MAIN_MENU_MAX_VALUE}, 
		[this](int selection)
	{
		_currentState = STATES::ENTER_IN_INITIAL_MOVEMENT;

	});
	
	_currentState = STATES::MENU;

	_player.SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player.SetPosition(0, SCREEN_HEIGHT * 0.9);
}

void GameManager::UpdateMenu(const float deltaTime)
{
	_buttonAManager.Update(deltaTime, _currentFrameInputValueNormalized, _currentFrameInputValue);
	
	// move player
	MovePlayer();
}
void GameManager::UpdateBattle(const float deltaTime)
{
	// move player
	MovePlayer();

	// update player
	_player.Update(deltaTime);

	UpdateBullets(deltaTime);

	// update enemies
	UpdateEnemies(deltaTime);

	if (_enemiesPool.TotalInUse() == 0)
	{
		EndLevel();
	}
}
void GameManager::UpdateImprovement(const float deltaTime)
{
	MovePlayer();
	_buttonAManager.Update(deltaTime, _currentFrameInputValueNormalized, _currentFrameInputValue);
}

void GameManager::UpdateEnterImprovement(const float deltaTime)
{
	int levelToCheck = _currentLevel -1;
	if(levelToCheck * 2 >= TOTAL_IMPROVEMENTS_TO_SELECT)
	{
		_currentState = STATES::ENTER_IN_INITIAL_MOVEMENT;
		return;
	}


	_buttonAManager.SelectAfterTime(TIME_TO_SELECT_IMPROVEMENT, 
		[this](int selection)
		{
			_currentState = STATES::ENTER_IN_INITIAL_MOVEMENT;
			int levelToCheck = _currentLevel -1;

			if(levelToCheck * 2 >= TOTAL_IMPROVEMENTS_TO_SELECT)
			{
				return;
			}

			auto optionForPlayer = _randomImprovements[levelToCheck * 2 + selection];
			auto optionForEnemy = _randomImprovements[levelToCheck * 2 + (selection +1) %2];

			_improvementFunctions[optionForPlayer](playerData);
			_improvementFunctions[optionForEnemy](enemyData);
		}
	);

	_player.SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player.SetPosition(0, SCREEN_HEIGHT * 0.9);

	_currentState = STATES::IMPROVEMENT_SELECTOR;
}

void GameManager::UpdateInitialMovement(const float deltaTime)
{
	_easingManager.Update(deltaTime);
}

void GameManager::UpdateEnterInicialMovement( const float deltaTime)
{
	StartLevel();
	_currentState = STATES::INITIAL_MOVEMENT;
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
								 const modifiable_data &data, bool isPlayer)
{
	p.SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	p.SetPosition(posX, posY);
	p.SetBulletsTotalSources(data.bulletsSource);
	p.SetBulletsPerShot(data.bulletsPerShot);
	p.SetFireRate(data.fireRate);
	p.SetHasShield(data.hasShield);
	p.SetPlayerTeam(isPlayer);
	p.Reset();
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
			bullet.SetPlayerTeam(forPlayer);
			bullet.SetHasPenetration(data.bulletHasPenetration);
			bullet.SetHasExplostion(data.bulletHasExplosion); });
	}
}

void GameManager::PaintMenu()
{
	{
		float playerX, playerY;
		_player.GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player.GetWidth(), _player.GetHeight(), playerX, playerY);
	}

	{
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::TITLE, SCREEN_WIDTH*0.5f, SCREEN_HEIGHT * 0.3f);
	}
}
void GameManager::PaintBattle()
{
	{
		_bulletsPool.for_each_active([&](const Bullet &bullet)
									 {
			float posX, posY;
			bullet.GetPaintPosition(posX, posY);
			_painterManager->AddToPaint(PainterManager::SPRITE_ID::BULLET, 
				bullet.GetWidth(), bullet.GetHeight(), posX, posY); });
	}

	{
		_enemiesPool.for_each_active([this](const Plane &p)
									 {
			float posX, posY;
			p.GetPaintPosition(posX, posY);
			_painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY, 
				p.GetWidth(), p.GetHeight(), posX, posY); });
	}

	{
		float playerX, playerY;
		_player.GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player.GetWidth(), _player.GetHeight(), playerX, playerY);
	}
}
void GameManager::PaintImprovements()
{
	{
		float percentLeft = 0.3;
		float percentRight = 1- percentLeft;
		if(_currentFrameInputValueNormalized < 0.5f)
		{
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::IMPROVEMENT_SELECTOR_PLAYER, SCREEN_WIDTH*percentLeft, SCREEN_HEIGHT * 0.4f);
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::IMPROVEMENT_SELECTOR_ENEMY,  SCREEN_WIDTH*percentRight, SCREEN_HEIGHT * 0.4f);
	
		}
		else
		{
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::IMPROVEMENT_SELECTOR_ENEMY,  SCREEN_WIDTH*percentLeft, SCREEN_HEIGHT * 0.4f);
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::IMPROVEMENT_SELECTOR_PLAYER, SCREEN_WIDTH*percentRight, SCREEN_HEIGHT * 0.4f);
		}
		/*
		for(auto kvp: _improvementsUI)
		{
			_painterManager->AddUIToPaint(kvp.second, SCREEN_WIDTH*percentRight, SCREEN_HEIGHT * 0.4f);
		}
		*/
	}


	{
		float playerX, playerY;
		_player.GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player.GetWidth(), _player.GetHeight(), playerX, playerY);
	}
}
void GameManager::PaintInitialMovement()
{
	{
		float playerX, playerY;
		_player.GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player.GetWidth(), _player.GetHeight(), playerX, playerY);
	}

	{
		_enemiesPool.for_each_active([this](const Plane &p)
									 {
			float posX, posY;
			p.GetPaintPosition(posX, posY);
			_painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY, 
				p.GetWidth(), p.GetHeight(), posX, posY); });
	}
}

void GameManager::StartLevel()
{
	ConfigurePlane(_player, SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.9, playerData, true);
	SpawnEnemies();
	PlayInitialAnimation();
}

void GameManager::EndLevel()
{
	_enemiesPool.ReturnAll();
	_bulletsPool.ReturnAll();

	_currentState = STATES::ENTER_IN_IMPROVEMENT_SELECTOR;
	++_currentLevel;
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
		_enemiesPool.call_for_element(id, [posX, posY, this](Plane &enemy)
									  { ConfigurePlane(enemy, posX, posY, enemyData, false); });
	}
}

void GameManager::UpdateBullets(float deltaTime)
{
	_bulletsPool.for_each_active([deltaTime](Bullet &bullet)
								 { bullet.Update(deltaTime); });

	// check if it should be deleted any bullet
	_bulletsPool.for_each_active([this, deltaTime](Bullet &bullet) {

		ManageBulletCollisions( bullet );
	});
}

void GameManager::ManageBulletCollisions( Bullet &bullet)
{
	bool isDestroyed = false;
	// check out of screen
	if (bullet.GetY() < 0 || bullet.GetY() > SCREEN_HEIGHT)
	{
		_bulletsPool.Release(bullet);
		isDestroyed = true;
	}

	// check collision against enemies
	_enemiesPool.for_each_active([&](Plane &enemy)
								{
									bool solution = ManageCollisionBetweenBulletAndEnemy(bullet, enemy);
									isDestroyed |= solution;
								});

	if (HasCollision(bullet, _player))
	{
		DamagePlayer();
		_bulletsPool.Release(bullet);
		isDestroyed = true;
	}

	if (isDestroyed)
	{
		if (bullet.GetHasExplostion())
		{
			DoExplosion(bullet);
		}
	}
}

bool GameManager::ManageCollisionBetweenBulletAndEnemy(Bullet& bullet, Plane& enemy)
{
	bool isBulletDestroyed = false;
	if (HasCollision(bullet, enemy))
	{
		if (!bullet.GetHasPenetration())
		{
			_bulletsPool.Release(bullet);
			isBulletDestroyed = true;
		}
		_enemiesPool.Release(enemy);
	} 
	return isBulletDestroyed;
}


void GameManager::UpdateEnemies(float deltaTime)
{
	_enemiesPool.for_each_active([deltaTime](Plane &enemy)
								 { enemy.Update(deltaTime); });
}

bool GameManager::HasCollision(const Bullet &bullet, Plane &plane) const
{
	if (bullet.GetPlayerTeam() == plane.GetPlayerTeam())
	{
		return false;
	}

	if (plane.GetHasShield())
	{
		plane.SetHasShield(false);
		return false;
	}

	return CollsisionDetection(bullet.GetX(), bullet.GetY(), bullet.GetWidth(), bullet.GetHeight(),
							   plane.GetX(), plane.GetY(), plane.GetWidth(), plane.GetHeight());
}

bool GameManager::CollsisionDetection(float ax, float ay, float aw, float ah,
									  float bx, float by, float bw, float bh) const
{
	return (std::abs(ax - bx) < (aw + bw) / 2.0f &&
			std::abs(ay - by) < (ah + bh) / 2.0f);
}

void GameManager::DoExplosion(Bullet &bullet)
{
	bullet.SetSize(EXPLOSION_SIZE, EXPLOSION_SIZE);

	_enemiesPool.for_each_active([&bullet, this](Plane &enemy)
								 {
									  if (HasCollision(bullet, enemy))
									  {
										  _enemiesPool.Release(enemy);
									  } });

	if (HasCollision(bullet, _player))
	{
		DamagePlayer();
	}
}

void GameManager::DamagePlayer()
{
}

void GameManager::PlayInitialAnimation()
{
	_enemiesPool.for_each_active(
		[this](Plane &p)
		{
			_easingManager.AddEase(INTIAL_ANIMATION_DURATION, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
				 p.GetX(), p.GetY(), EasingManager::EASE_TYPES::INOUTCUBE, 
				 [this] {_currentState = STATES::BATTLE;}, 
				 [&p](float x, float y)
					{ p.SetPosition(x, y); });
		});
}