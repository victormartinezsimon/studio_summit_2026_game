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
	currentPlayerVelocitiyBulletX = PLAYER_DEFAULT_BULLET_VEL_X;
	currentPlayerVelocitiyBulletY = PLAYER_DEFAULT_BULLET_VEL_Y;
	currentPlayerFireRate = PLAYER_DEFAULT_FIRE_RATE;
	currentPlayerBulletsOrigin = PLAYER_DEFAULT_BULLETS_ORIGIN;

	currentEnemyVelocitiyBulletX = ENEMY_DEFAULT_BULLET_VEL_X;
	currentEnemyVelocitiyBulletY = ENEMY_DEFAULT_BULLET_VEL_Y;
	currentEnemyFireRate = ENEMY_DEFAULT_FIRE_RATE;
	currentEnemyBulletsOrigin = ENEMY_DEFAULT_BULLETS_ORIGIN;

	ConfigurePlayer();
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

	
	if(_enemiesPool->GetCurrentUsed().size() == 0)
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
	_player->SetBulletsOrigin(currentPlayerBulletsOrigin);
	_player->SetFireRate(currentPlayerFireRate);
	_player->SetCallbackFire([this](int index, Plane *p)
							 { this->SpawnPlayerBullet(index, p); });
}

void GameManager::SpawnPlayerBullet(int index, Plane *p)
{
	auto bullet = _bulletsPool->Get();
	bullet->SetPosition(p->GetX(), p->GetY());
	bullet->SetVelocity(currentPlayerVelocitiyBulletX, currentPlayerVelocitiyBulletY);
	bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
	bullet->SetPlayerTeam(true);
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
	int levelConfigID = std::min(_currentLevel, TOTAL_LEVELS_CONFIG);
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
	enemy->SetFireRate(currentEnemyFireRate);
	enemy->SetBulletsOrigin(currentEnemyBulletsOrigin);
	enemy->SetFireRate(currentEnemyFireRate);
	enemy->SetCallbackFire([this](int index, Plane *p)
						   { this->SpawnEnemyBullet(index, p); });
}

void GameManager::SpawnEnemyBullet(int index, Plane *p)
{
	auto bullet = _bulletsPool->Get();
	bullet->SetPosition(p->GetX(), p->GetY());
	bullet->SetVelocity(currentEnemyVelocitiyBulletX, currentEnemyVelocitiyBulletY);
	bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
	bullet->SetPlayerTeam(false);
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
				// if(!bullet->HasPenetration())
				{
					bulletsToDelete.push_back(bullet);
				}
				break;//only 1 collistion per bullet per frame
			}
		}

		if (HasCollision(bullet, _player))
		{
			// do something
		}
	}

	for (auto &bullet : bulletsToDelete)
	{
		_bulletsPool->Release(bullet);
		// do somthing with explosion
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

bool GameManager::HasCollision(const Bullet *bullet, const Plane *plane) const
{
	if (bullet->GetPlayerTeam() == plane->GetPlayerTeam())
	{
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