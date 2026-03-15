#include "MainMenuState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "ButtonA.h"

MainMenuState::MainMenuState(Plane *player, PainterManager *painter,ButtonA* buttonAManager) : 
State(player, painter, buttonAManager)
{
}

State::STATES MainMenuState::Update(const float deltaTime, float _currentFrameInputValueNormalized, int _currentFrameInputValue)
{
    _buttonAManager->Update(deltaTime, _currentFrameInputValueNormalized, _currentFrameInputValue);
	
	return _nextState;
}
void MainMenuState::Paint()
{
    {
		float playerX, playerY;
		_player->GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, 
            _player->GetWidth(), _player->GetHeight(), playerX, playerY);
	}

	{
		_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::TITLE, 
            SCREEN_WIDTH*0.5f, SCREEN_HEIGHT * 0.3f);
	}
}
void MainMenuState::OnEnter()
{
	_buttonAManager->SelectInPosition(MAIN_MENU_TIME_TO_ENTER, {SCREEN_WIDTH * MAIN_MENU_MIN_VALUE, SCREEN_WIDTH * MAIN_MENU_MAX_VALUE}, 
		[this](int selection)
	{
		_nextState = STATES::INITIAL_MOVEMENT;

	});
	
	_nextState = STATES::MENU;

	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(SCREEN_HEIGHT * POSITION_Y_PLAYER);
}
void MainMenuState::OnExit()
{
}