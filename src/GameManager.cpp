#include "GameManager.h"
#include "InputManager.h"
#include "Plane.h"
#include "Bullet.h"
#include "PainterManager.h"
#include "Sprites.h"

GameManager::GameManager(InputManager *input, Plane *player, Pool<Plane, PLANES_POOL_SIZE> *enemiesPool,
						 Pool<Bullet, BULLETS_POOL_SIZE> *bulletsPool, PainterManager *painterManager)
	: _inputManager(input), _player(player), _enemiesPool(enemiesPool), _bulletsPool(bulletsPool),
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

void GameManager::Paint() const
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
	_currentState = STATES::INITIAL_MOVEMENT;
}
void GameManager::UpdateBattle(const float deltaTime)
{
	// move player
	MovePlayer();

	// update player
	_player->Update(deltaTime);

	UpdateBullets(deltaTime);

	// update enemies
	UpdateEnemies(deltaTime);

	if (_enemiesPool->GetCurrentUsed().size() == 0)
	{
		EndLevel();
	}
}
void GameManager::UpdateImprovement(const float deltaTime)
{
	_currentState = STATES::INITIAL_MOVEMENT;
}
void GameManager::UpdateInitialMovement(const float deltaTime)
{
	_player->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.9);
	StartLevel();
	_currentState = STATES::BATTLE;
}

void GameManager::GetMinMaxXPosiblePosition(float &minX, float &maxX) const
{
	minX = 0 + _player->GetWidth() / 2;
	maxX = SCREEN_WIDTH - _player->GetWidth() / 2;
}

void GameManager::MovePlayer()
{
	float minX, maxX;
	GetMinMaxXPosiblePosition(minX, maxX);
	float percent = _inputManager->GetInputValueNormalized();

	float diffWidth = maxX - minX;
	float position = diffWidth * percent;

	float realPosition = position + minX;

	_player->SetPositionX(realPosition);
}

void GameManager::ConfigurePlayer()
{
	_player->SetSize(PLANE_WIDTH, PLANE_HEIGHT);
	_player->SetBulletsTotalSources(playerData.bulletsSource);
	_player->SetFireRate(playerData.fireRate);
	_player->SetHasShield(playerData.hasShield);
	_player->SetCallbackFire([this](int sourceIndex, Plane *p)
							 { this->SpawnBullet(sourceIndex, p, true, playerData); });
}

void GameManager::SpawnBullet(int sourceIndex, Plane *p, bool forPlayer, const modifiable_data &data)
{
	
	auto bullet = _bulletsPool->Get();
	bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);

	float positionX = p->GetX();
	if (sourceIndex == 1)
	{
		positionX = p->GetX() - p->GetWidth() / 2;
	}

	if (sourceIndex == 2)
	{
		positionX = p->GetX() + p->GetWidth() / 2;
	}

	bullet->SetPosition(positionX, p->GetY());
	bullet->SetVelocity(data.velocityBulletX, data.velocityBulletY);
	bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
	bullet->SetPlayerTeam(forPlayer);
	//bullet->SetHasPenetration(data.bulletHasPenetration);//TODO => review why this fails
	bullet->SetHasExplostion(data.bulletHasExplosion);
	
}

void GameManager::PaintMenu() const
{
}
void GameManager::PaintBattle() const
{
	{
		auto currentUsed = _bulletsPool->GetCurrentUsed();
		for (auto &&bullet : currentUsed)
		{
			float posX, posY;
			bullet->GetPaintPosition(posX, posY);
			_painterManager->AddToPaint(PainterManager::SPRITE_ID::BULLET, bullet->GetWidth(), bullet->GetHeight(), posX, posY);
		}
	}

	{
		auto currentUsed = _enemiesPool->GetCurrentUsed();
		for (auto &&enemy : currentUsed)
		{
			float posX, posY;
			enemy->GetPaintPosition(posX, posY);
			_painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY, enemy->GetWidth(), enemy->GetHeight(), posX, posY);
		}
	}

	{
		float playerX, playerY;
		_player->GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player->GetWidth(), _player->GetHeight(), playerX, playerY);
	}
}
void GameManager::PaintImprovements() const
{
}
void GameManager::PaintInitialMovement() const
{
}

void GameManager::StartLevel()
{
	ConfigurePlayer();
	SpawnEnemies();
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

		auto enemy = _enemiesPool->Get();
		enemy->SetPosition(posX, posY);
		ConfigureEnemy(enemy);
	}
}

void GameManager::ConfigureEnemy(Plane *enemy)
{
	enemy->SetSize(ENEMY_WIDTH, ENEMY_HEIGHT);
	enemy->SetBulletsTotalSources(enemyData.bulletsSource);
	enemy->SetFireRate(enemyData.fireRate);
	enemy->SetHasShield(enemyData.hasShield);
	enemy->SetCallbackFire([this](int sourceIndex, Plane *p)
						   { this->SpawnBullet(sourceIndex, p, false, enemyData); });
}

void GameManager::UpdateBullets(float deltaTime)
{
	std::vector<Bullet *> bulletsToDelete;
	std::vector<Plane *> enemiesToDelete;

	auto currentBullets = _bulletsPool->GetCurrentUsed();
	auto currentEnemies = _enemiesPool->GetCurrentUsed();

	for (auto &bullet : currentBullets)
	{
		bullet->Update(deltaTime);

		// check out of bound
		if (bullet->GetY() < 0 || bullet->GetY() > SCREEN_HEIGHT)
		{
			bulletsToDelete.push_back(bullet);
		}

		for (auto &&enemy : currentEnemies)
		{
			if (HasCollision(bullet, enemy))
			{
				enemiesToDelete.push_back(enemy);
				if (!bullet->GetHasPenetration())
				{
					bulletsToDelete.push_back(bullet);
				}

				break; // only 1 collistion per bullet per frame
			}
		}

		if (HasCollision(bullet, _player))
		{
			// do something
		}
	}

	for (auto &bullet : bulletsToDelete)
	{
		if (bullet->GetHasExplostion())
		{
			DoExplosion(bullet, enemiesToDelete);
		}
		_bulletsPool->Release(bullet);
	}

	for (auto &enemy : enemiesToDelete)
	{
		_enemiesPool->Release(enemy);
	}
}

void GameManager::UpdateEnemies(float deltaTime)
{
	auto currentEnemies = _enemiesPool->GetCurrentUsed();
	for (auto &enemy : currentEnemies)
	{
		enemy->Update(deltaTime);
	}
}

bool GameManager::HasCollision(const Bullet *bullet, Plane *plane) const
{
	if (bullet->GetPlayerTeam() == plane->GetPlayerTeam())
	{
		return false;
	}

	if (plane->GetHasShield())
	{
		plane->SetHasShield(false);
		return false;
	}

	return CollsisionDetection(bullet->GetX(), bullet->GetY(), bullet->GetWidth(), bullet->GetHeight(),
							   plane->GetX(), plane->GetY(), plane->GetWidth(), plane->GetHeight());
}

bool GameManager::CollsisionDetection(float ax, float ay, float aw, float ah,
									  float bx, float by, float bw, float bh) const
{
	return (std::abs(ax - bx) < (aw + bw) / 2.0f &&
			std::abs(ay - by) < (ah + bh) / 2.0f);
}

void GameManager::DoExplosion(Bullet *bullet, std::vector<Plane *> &enemiesToDelete)
{
	bullet->SetSize(EXPLOSION_SIZE, EXPLOSION_SIZE);
	auto currentEnemies = _enemiesPool->GetCurrentUsed();
	for (auto &&enemy : currentEnemies)
	{
		if (HasCollision(bullet, enemy))
		{
			enemiesToDelete.push_back(enemy);
		}
	}
	if (HasCollision(bullet, _player))
	{
		// do something
	}
}