#include "EndGameState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "ButtonA.h"
#include "NumberManager.h"
#include "EasingManager.h"
#include <cmath>

constexpr int TOTAL_TIME_STATE = 5;

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

	if(_timeAcumState > TOTAL_TIME_STATE)
	{
		return State::STATES::HIGH_SCORES;
	}

	return State::STATES::END_GAME;
}
void EndGameState::Paint()
{
	_player->Paint(_painterManager);

	{
		int time = std::round( TOTAL_TIME_STATE - _timeAcumState );
		float w =  _player->GetWidth();
		_numberManager->PaintNumber(time, _player->GetX() -w/2, _player->GetY(), 1, NumberManager::PIVOT::RIGHT);
	}
}

void EndGameState::PaintUI()
{
	_painterManager->AddToPaint(PainterManager::SPRITE_ID::FINAL_SCORE, END_GAME_COORDS::TITLE_X, END_GAME_COORDS::TITLE_Y);
	_numberManager->PaintNumber(_playerScore, END_GAME_COORDS::SCORE_X, END_GAME_COORDS::SCORE_Y, 4, NumberManager::PIVOT::CENTER);
}
void EndGameState::OnEnter()
{
	_timeAcumState = 0;
	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->ConfigureSprite(_painterManager);
	_player->SetPlayerTeam(TEAM_PLAYER);
}
void EndGameState::OnExit()
{
}

void EndGameState::Configure(float score)
{
	_playerScore = score;
	_timeAcumState = 0;

	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->ConfigureSprite(_painterManager);
	_player->SetPlayerTeam(TEAM_PLAYER);
}
