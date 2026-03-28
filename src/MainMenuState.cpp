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
constexpr float START_GAME_SELECTOR_X = 0.2 * SCREEN_WIDTH;
constexpr float START_GAME_X = START_GAME_SELECTOR_X;
constexpr float EXIT_GAME_X = 0.8 * SCREEN_WIDTH;

MainMenuState::MainMenuState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager) 
		: State(player, painter, numberManager, alphaManager, 
			easingManager, randomManager, buttonAManager), _startingGame(false)
{
}

State::STATES MainMenuState::Update(const float deltaTime, float _currentFrameInputValueNormalized)
{
	_buttonAManager->Update(deltaTime, _currentFrameInputValueNormalized);

	return _nextState;
}
void MainMenuState::Paint()
{
	if (_startingGame)
	{
		return;
	}

	{
		_player->Paint(_painterManager);
	}

	{
		float posY = _player->GetY();
		int time = _buttonAManager->GetLeftTime() + 1;
		float w = _painterManager->GetWidth(PainterManager::SPRITE_ID::PLAYER);
		_numberManager->PaintNumber(time, _player->GetX() - w/2, _player->GetY(), 1, NumberManager::PIVOT::RIGHT);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::TITLE,
									  SCREEN_WIDTH * 0.5f, TITLE_Y);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									  START_GAME_SELECTOR_X, SELECTOR_Y);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									  EXIT_GAME_X, SELECTOR_Y);
	}
	
	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::START_GAME,
									  START_GAME_X, START_Y);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::EXIT_GAME,
									  EXIT_GAME_X, START_Y);
	}

	
	
}
void MainMenuState::OnEnter()
{
	_buttonAManager->SelectInPosition(MAIN_MENU_TIME_TO_ENTER, 
		{START_GAME_X - PLAYER_SELECTOR_WIDTH / 2, START_GAME_X + PLAYER_SELECTOR_WIDTH / 2},
		{EXIT_GAME_X - PLAYER_SELECTOR_WIDTH / 2, EXIT_GAME_X + PLAYER_SELECTOR_WIDTH / 2},
									  [this](int selection)
									  {
										if(selection == 0)
										{
										  StartGame();
										}
										else
										{
											ExitGame();
										}
									  });

	_nextState = STATES::MENU;
	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->SetPlayerTeam(TEAM_PLAYER);
	_startingGame = false;
}
void MainMenuState::OnExit()
{
}

void MainMenuState::StartGame()
{
	_alphaManager->FinishAll();
	_easingManager->KillAll();

	int id = _alphaManager->AddAlpha(ALPHA_TIME_ENTER_GAME, SCREEN_WIDTH * 0.5f, TITLE_Y, PainterManager::SPRITE_ID::TITLE);
	_alphaManager->AddCallback(id, [this]() { _nextState = STATES::INITIAL_MOVEMENT; });
	_startingGame = true;
}

void MainMenuState::ExitGame()
{
	_nextState = STATES::EXIT;
}
