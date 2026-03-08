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

	// update enemies

	// update bullets
	_bulletsPool->for_each_active([deltaTime](Bullet &obj)
								  { obj.Update(deltaTime); });

	// clear bulets
	std::vector<Bullet *> bulletsToDelete;
	_bulletsPool->for_each_active([&bulletsToDelete](Bullet &obj)
								  {
									  if (obj.GetY() < 0 || obj.GetY() > SCREEN_HEIGHT)
									  {
										  bulletsToDelete.push_back(&obj);
									  }
									  // search for collision
								  });

	for (auto &bullet : bulletsToDelete)
	{
		_bulletsPool->Release(bullet);
	}
}
void GameManager::UpdateImprovement(const float deltaTime)
{
	_currentState = STATES::BATTLE;
}
void GameManager::UpdateInitialMovement(const float deltaTime)
{
	_player->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.9);
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
	_player->SetBulletsOrigin(1);
	_player->SetFireRate(currentPlayerFireRate);
	_player->SetCallbackFire([this](int index, Plane *p)
							 { this->SpanwPlayerBullet(index, p); });
}

void GameManager::SpanwPlayerBullet(int index, Plane *p)
{
	auto bullet = _bulletsPool->Get();
	bullet->SetPosition(p->GetX(), p->GetY());
	bullet->SetVelocity(currentPlayerVelocitiyBulletX, currentPlayerVelocitiyBulletY);
	bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
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