#include "InitialMovementState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "EasingManager.h"


InitialMovementState::InitialMovementState(Plane* player, PainterManager* painter, EasingManager* easingManager, Pool<Plane, PLANES_POOL_SIZE>* enemiesPool) : 
State(player, painter), _easingManager(easingManager), _enemiesPool(enemiesPool)
{
}

State::STATES InitialMovementState::Update(const float deltaTime, float _currentFrameInputValueNormalized, int _currentFrameInputValue)
{
    _easingManager->Update(deltaTime);
	
	return _nextState;
}
void InitialMovementState::Paint()
{
    {
		float playerX, playerY;
		_player->GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player->GetWidth(), _player->GetHeight(), playerX, playerY);
	}

	{
		_enemiesPool->for_each_active([this](const Plane &p)
									 {
			float posX, posY;
			p.GetPaintPosition(posX, posY);
			_painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY, 
				p.GetWidth(), p.GetHeight(), posX, posY); });
	}
}
void InitialMovementState::OnEnter()
{
	_nextState = STATES::INITIAL_MOVEMENT;

	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
	_easingManager->ClearAll();

	_enemiesPool->for_each_active(
		[this](Plane &p)
		{
			_easingManager->AddEase(INTIAL_ANIMATION_DURATION, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
				 p.GetX(), p.GetY(), EasingManager::EASE_TYPES::INOUTCUBE, 
				 [this] 
				 {
					_nextState = STATES::BATTLE;
				}, 
				 [&p](float x, float y)
					{ p.SetPosition(x, y); });
		}
	);
}
void InitialMovementState::OnExit()
{
}