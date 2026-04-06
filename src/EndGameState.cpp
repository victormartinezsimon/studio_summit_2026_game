#include "EndGameState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h" //TODO: remove this inclusion
#include "ButtonA.h"
#include "NumberManager.h"
#include "EasingManager.h"
#include <cmath>

EndGameState::EndGameState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager) : 
		State(player, painter, numberManager,  
			easingManager, randomManager, buttonAManager)
{
}

State::STATES EndGameState::Update(const float deltaTime, float _currentFrameInputValueNormalized)
{
	_timeAcumState += deltaTime;

	if(_timeAcumState > 5)
	{
		return State::STATES::HIGH_SCORES;
	}

	return State::STATES::END_GAME;
}
void EndGameState::Paint()
{
}

void EndGameState::PaintUI()
{
	_numberManager->PaintNumber(_playerScore, SCREEN_WIDTH*0.5, SCREEN_HEIGHT* 0.5, 4, NumberManager::PIVOT::CENTER);
}
void EndGameState::OnEnter()
{
	_timeAcumState = 0;
}
void EndGameState::OnExit()
{
}

void EndGameState::Configure(float score)
{
	_playerScore = score;
	_timeAcumState = 0;
}
