#include "EndGameState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "ButtonA.h"
#include "NumberManager.h"
#include "AlphaManager.h"

constexpr int FINAL_SCORE_Y = FINAL_SCORE_HEIGHT;
constexpr int RETURN_Y = 153;
constexpr int SELECTOR_Y = 205;
constexpr int SCORE_Y = 110;
constexpr int SELECTOR_X = SCREEN_WIDTH/2;

EndGameState::EndGameState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager) : 
		State(player, painter, numberManager, alphaManager, 
			easingManager, randomManager, buttonAManager)
{
}

State::STATES EndGameState::Update(const float deltaTime, float _currentFrameInputValueNormalized, int _currentFrameInputValue)
{
	_buttonAManager->Update(deltaTime, _currentFrameInputValueNormalized, _currentFrameInputValue);

	return _nextState;
}
void EndGameState::Paint()
{
	{
		float playerX, playerY;
		_player->GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER,
									_player->GetWidth(), _player->GetHeight(), playerX, playerY);
	}

	{
		float playerX, playerY;
		_player->GetPaintPosition(playerX, playerY);
		float posY = _player->GetY();
		int time = _buttonAManager->GetLeftTime() + 1;
		_numberManager->PaintNumber(time, playerX, posY, 1, NumberManager::PIVOT::RIGHT);
	}

	{
		_numberManager->PaintNumber(_score, SCREEN_WIDTH * 0.5f, SCORE_Y, 3, NumberManager::PIVOT::CENTER);
	}

	{
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									  SELECTOR_X, SELECTOR_Y);
	}
	
	{
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::FINAL_SCORE,
									  SCREEN_WIDTH * 0.5f, FINAL_SCORE_Y);
	}

	{
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::RETURN_MENU,
									  SELECTOR_X, RETURN_Y);
	}
}
void EndGameState::OnEnter()
{
	_alphaManager->FinishAll();
	_easingManager->KillAll();

	_buttonAManager->SelectInPosition(END_GAME_TIME_TO_MAIN_MENU, {SELECTOR_X- PLAYER_SELECTOR_WIDTH / 2, SELECTOR_X + PLAYER_SELECTOR_WIDTH / 2},
									  [this](int selection)
									  {
										  _nextState = STATES::MENU;
									  });

	_nextState = STATES::END_GAME;
	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
}
void EndGameState::OnExit()
{
}

void EndGameState::Configure(float score)
{
	_score = score;
}
