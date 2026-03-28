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

State::STATES InitialMovementState::Update(const float deltaTime, float _currentFrameInputValueNormalized)
{
	return _nextState;
}
void InitialMovementState::Paint()
{
    {
		_player->Paint(_painterManager);
	}

	{
		_enemiesPool->for_each_active([this](Plane &p)
									 {
										p.Paint(_painterManager);
									});
	}
}
void InitialMovementState::OnEnter()
{
	_nextState = STATES::INITIAL_MOVEMENT;

	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->SetPlayerTeam(TEAM_PLAYER);
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