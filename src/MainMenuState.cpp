#include "MainMenuState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h" //TODO: remove this inclusion
#include "ButtonA.h"
#include "NumberManager.h"
#include "EasingManager.h"

MainMenuState::MainMenuState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager) 
		: State(player, painter, numberManager, 
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
		int time = std::round(_buttonAManager->GetLeftTime());
		float w = _painterManager->GetWidth(PainterManager::SPRITE_ID::PLAYER);
		_numberManager->PaintNumber(time, _player->GetX() - w/2, _player->GetY(), 1, NumberManager::PIVOT::RIGHT);
	}
}

void MainMenuState::PaintUI()
{
	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::TITLE,
									  MAIN_MENU_COORDS::TITLE_X, MAIN_MENU_COORDS::TITLE_Y, _alphaMenu);
	}

	if (_startingGame)
	{
		return;
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::START_GAME,
									  MAIN_MENU_COORDS::START_X, MAIN_MENU_COORDS::START_Y);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::EXIT_GAME,
									  MAIN_MENU_COORDS::EXIT_GAME_X, MAIN_MENU_COORDS::EXIT_GAME_Y);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									  MAIN_MENU_COORDS::SELECTOR_START_X, MAIN_MENU_COORDS::SELECTOR_START_Y);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									   MAIN_MENU_COORDS::SELECTOR_EXIT_X, MAIN_MENU_COORDS::SELECTOR_EXIT_Y);
	}
	
	
}

void MainMenuState::OnEnter()
{
	_buttonAManager->SelectInPosition(MAIN_MENU_TIME_TO_ENTER, 
		{MAIN_MENU_COORDS::SELECTOR_START_X - PLAYER_SELECTOR_WIDTH / 2, MAIN_MENU_COORDS::SELECTOR_START_X + PLAYER_SELECTOR_WIDTH / 2},
		{MAIN_MENU_COORDS::SELECTOR_EXIT_X - PLAYER_SELECTOR_WIDTH / 2, MAIN_MENU_COORDS::SELECTOR_EXIT_X + PLAYER_SELECTOR_WIDTH / 2},
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
	_player->SetHasShield(false);
	_player->ConfigureSprite(_painterManager);
	_startingGame = false;
	_alphaMenu = 1.0f;
}
void MainMenuState::OnExit()
{
}

void MainMenuState::StartGame()
{
	_easingManager->KillAll();

	_easingManager->AddEase(ALPHA_TIME_ENTER_GAME, 100.0f, 100.0f, 0.0f, 0.0f, Ease::EASE_TYPES::LINEAL, 
		[&](bool forced, int noUsed){_nextState =STATES::INITIAL_MOVEMENT; },
		[&](float x, float y, Ease& ease, float percent){
			_alphaMenu = 1 - percent;
		}
	);
	_startingGame = true;

}

void MainMenuState::ExitGame()
{
	_nextState = STATES::EXIT;
}
