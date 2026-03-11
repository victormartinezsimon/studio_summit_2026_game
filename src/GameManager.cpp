#include "GameManager.h"
#include "InputManager.h"
#include "Plane.h"
#include "Bullet.h"
#include "PainterManager.h"
#include "Sprites.h"

GameManager::GameManager(InputManager *input, PainterManager *painterManager)
	: _inputManager(input),
	  _painterManager(painterManager), _currentState(STATES::MENU), _currentLevel(0)
{
	InitializeConstantValues();
	InitializeImprovementsFunctions();
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
	{ data.bulletsPerShot = 3; };
	_improvementFunctions[std::string(IMPROVEMENT_INCREASE_ORIGIN)] = [](modifiable_data &data)
	{ data.bulletsSource = 3; };
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

void GameManager::Update(const float deltaTime)
{
	switch (_currentState)
	{
	case STATES::BATTLE:
		UpdateBattle(deltaTime);
		break;
	case STATES::MENU:
		UpdateMenu(deltaTime);
		break;
	case STATES::IMPROVEMENT_SELECTOR:
		UpdateImprovement(deltaTime);
		break;
	case STATES::INITIAL_MOVEMENT:
		UpdateInitialMovement(deltaTime);
		break;
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
	case STATES::MENU:
		PaintMenu();
		break;
	case STATES::IMPROVEMENT_SELECTOR:
		PaintImprovements();
		break;
	case STATES::INITIAL_MOVEMENT:
		PaintInitialMovement();
		break;
	}
}

void GameManager::UpdateMenu(const float deltaTime)
{
	_playingStartAnimation = false;
	_currentState = STATES::INITIAL_MOVEMENT;
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
	_playingStartAnimation = false;
	_currentState = STATES::INITIAL_MOVEMENT;
}
void GameManager::UpdateInitialMovement(const float deltaTime)
{
	if (!_playingStartAnimation)
	{
		StartLevel();
	}
	else
	{
		_easingManager.Update(deltaTime);
	}
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
	float percent = _inputManager->GetInputValueNormalized();

	float diffWidth = maxX - minX;
	float position = diffWidth * percent;

	float realPosition = position + minX;

	_player.SetPositionX(realPosition);
}

void GameManager::ConfigurePlayer()
{
	_player.SetSize(PLANE_WIDTH, PLANE_HEIGHT);
	_player.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.9);
	_player.SetBulletsTotalSources(playerData.bulletsSource);
	_player.SetFireRate(playerData.fireRate);
	_player.SetHasShield(playerData.hasShield);
	_player.SetPlayerTeam(true);
	_player.SetCallbackFire([this](int sourceIndex, const Plane &p)
							{ this->SpawnBullet(sourceIndex, p, true, playerData); });
}

void GameManager::SpawnBullet(int sourceIndex, const Plane &p, bool forPlayer, const modifiable_data &data)
{
	auto id = _bulletsPool.Get();

	_bulletsPool.call_for_element(id, [this, sourceIndex, p, forPlayer, data](Bullet &bullet)
								  {
		bullet.SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);

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
		bullet.SetVelocity(data.velocityBulletX, data.velocityBulletY);
		bullet.SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
		bullet.SetPlayerTeam(forPlayer);
		bullet.SetHasPenetration(data.bulletHasPenetration);
		bullet.SetHasExplostion(data.bulletHasExplosion); });
}

void GameManager::PaintMenu()
{
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
	ConfigurePlayer();
	SpawnEnemies();
	//_currentState = STATES::BATTLE;
	PlayInitialAnimation();
	_playingStartAnimation = true;
}

void GameManager::EndLevel()
{
	_currentState = STATES::IMPROVEMENT_SELECTOR;
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

		_enemiesPool.call_for_element(id, [this, posX, posY](Plane &enemy)
									  {
			enemy.SetPosition(posX, posY);
			enemy.SetSize(ENEMY_WIDTH, ENEMY_HEIGHT);
			enemy.SetBulletsTotalSources(enemyData.bulletsSource);
			enemy.SetFireRate(enemyData.fireRate);
			enemy.SetHasShield(enemyData.hasShield);
			enemy.SetPlayerTeam(false);
			enemy.SetCallbackFire([this](int sourceIndex, const Plane &p)
						  { this->SpawnBullet(sourceIndex, p, false, enemyData); }); });
	}
}

void GameManager::UpdateBullets(float deltaTime)
{
	_bulletsPool.for_each_active([deltaTime](Bullet &bullet)
								 { bullet.Update(deltaTime); });

	// check if it should be deleted any bullet
	_bulletsPool.for_each_active([this](Bullet &bullet)
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
										if (HasCollision(bullet, enemy))
										{
											bool v = HasCollision(bullet, enemy);
											if (!bullet.GetHasPenetration())
											{
												_bulletsPool.Release(bullet);
												isDestroyed = true;
											}
											_enemiesPool.Release(enemy);
										} });

									  if (HasCollision(bullet, _player))
									  {
										  DamagePlayer();
										  isDestroyed = true;
									  }

									  if (isDestroyed)
									  {
										  if (bullet.GetHasExplostion())
										  {
											  DoExplosion(bullet);
										  }
									  } });
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
	_easingManager.AddEase(INTIAL_ANIMATION_DURATION, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 
		_player.GetX(), _player.GetY(), EasingManager::EASE_TYPES::INOUTCUBE, 
		[this]()
		{
			_currentState = STATES::BATTLE;
			_easingManager.FinishAll(); 
		}, 
		[this](float x, float y)
		{ 
			_player.SetPosition(x, y); 
		}
	);

	_enemiesPool.for_each_active(
		[this](Plane &p)
		{
			_easingManager.AddEase(INTIAL_ANIMATION_DURATION, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 
				p.GetX(), p.GetY(), EasingManager::EASE_TYPES::INOUTCUBE, [] {}, 
				[&p](float x, float y)
				{ p.SetPosition(x, y); });
		});
}