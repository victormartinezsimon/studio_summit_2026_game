#include "InitialMovementState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "EasingManager.h"
#include "AlphaManager.h"


InitialMovementState::InitialMovementState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager,
		 Pool<Plane, PLANES_POOL_SIZE>* enemiesPool) : 
State(player, painter, numberManager, alphaManager, 
			easingManager, randomManager, buttonAManager), _enemiesPool(enemiesPool)
{
}

State::STATES InitialMovementState::Update(const float deltaTime, float _currentFrameInputValueNormalized, int _currentFrameInputValue)
{
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
	_easingManager->KillAll();
	_alphaManager->FinishAll();

	_enemiesPool->for_each_active(
		[this](Plane &p)
		{
			_easingManager->AddEase(INTIAL_ANIMATION_DURATION, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
				p.GetX(), p.GetY(), Ease::EASE_TYPES::INOUTCUBE, 
				[this] (bool normalEnded)
				{
					_nextState = STATES::BATTLE;
				}, 
				[&p](float x, float y, Ease& ease)	{ p.SetPosition(x, y); }
			);
		}
	);
}
void InitialMovementState::OnExit()
{
}