#include "MainMenuState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "ButtonA.h"
#include "NumberManager.h"
#include "AlphaManager.h"

constexpr int TITLE_Y = 70;
constexpr int START_Y = 153;
constexpr int SELECTOR_Y = 205;

MainMenuState::MainMenuState(Plane *player, PainterManager *painter, ButtonA *buttonAManager,
							 NumberManager *numberManager, AlphaManager *alphaManager) : State(player, painter), _buttonAManager(buttonAManager), _numberManager(numberManager), _alphaManager(alphaManager), _startingGame(false)
{
}

State::STATES MainMenuState::Update(const float deltaTime, float _currentFrameInputValueNormalized, int _currentFrameInputValue)
{
	_buttonAManager->Update(deltaTime, _currentFrameInputValueNormalized, _currentFrameInputValue);

	return _nextState;
}
void MainMenuState::Paint()
{
	if (_startingGame)
	{
		return;
	}

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
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									  SCREEN_WIDTH * 0.5f, SELECTOR_Y);
	}
	
	{
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::START_GAME,
									  SCREEN_WIDTH * 0.5f, START_Y);
	}

	{
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::TITLE,
									  SCREEN_WIDTH * 0.5f, TITLE_Y);
	}
	
}
void MainMenuState::OnEnter()
{
	_buttonAManager->SelectInPosition(MAIN_MENU_TIME_TO_ENTER, {SCREEN_WIDTH * 0.5f - PLAYER_SELECTOR_WIDTH / 2, SCREEN_WIDTH * 0.5f + PLAYER_SELECTOR_WIDTH / 2},
									  [this](int selection)
									  {
										  StartGame();
									  });

	_nextState = STATES::MENU;

	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
}
void MainMenuState::OnExit()
{
}

void MainMenuState::StartGame()
{
	int id = _alphaManager->AddUIAlpha(ALPHA_TIME_ENTER_GAME, SCREEN_WIDTH * 0.5f, TITLE_Y, false, PainterManager::SPRITE_ID::TITLE);
	_alphaManager->AddCallback(id, [this]()
							   { _nextState = STATES::INITIAL_MOVEMENT; });
	_startingGame = true;
}