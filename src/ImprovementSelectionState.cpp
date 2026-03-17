#include "ImprovementSelectionState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "ButtonA.h"

ImprovementSelectionState::ImprovementSelectionState(Plane *player, PainterManager *painter,ButtonA* buttonAManager, 
	std::function<void(const std::string& optionForPlayer,const std::string& optionForEnemy)> callbackSeleccion) : 
State(player, painter),_callbackSeleccion(callbackSeleccion), _buttonAManager(buttonAManager)
{
	InitializeImprovementsUI();
}

State::STATES ImprovementSelectionState::Update(const float deltaTime, float currentFrameInputValueNormalized, int currentFrameInputValue)
{
	_currentFrameInputValueNormalized = currentFrameInputValueNormalized;
    
	_buttonAManager->Update(deltaTime, currentFrameInputValueNormalized, currentFrameInputValue);
	
	return _nextState;
}
void ImprovementSelectionState::Paint()
{
    {
		float percentLeft = 0.3;
		float percentRight = 1- percentLeft;

		_painterManager->AddUIToPaint(_improvementsUI[_leftSelection], SCREEN_WIDTH*percentLeft, SCREEN_HEIGHT * 0.4f);
		_painterManager->AddUIToPaint(_improvementsUI[_rightSelection], SCREEN_WIDTH*percentRight, SCREEN_HEIGHT * 0.4f);
	

		if(_currentFrameInputValueNormalized < 0.5f)
		{
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR, SCREEN_WIDTH*percentLeft, SCREEN_HEIGHT * 0.4f);
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::ENEMY_SELECTOR,  SCREEN_WIDTH*percentRight, SCREEN_HEIGHT * 0.4f);
	
		}
		else
		{
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::ENEMY_SELECTOR,  SCREEN_WIDTH*percentLeft, SCREEN_HEIGHT * 0.4f);
			_painterManager->AddUIToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR, SCREEN_WIDTH*percentRight, SCREEN_HEIGHT * 0.4f);
		}
	}

	{
		float playerX, playerY;
		_player->GetPaintPosition(playerX, playerY);
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER, _player->GetWidth(), _player->GetHeight(), playerX, playerY);
	}
}
void ImprovementSelectionState::OnEnter()
{
	_buttonAManager->SelectAfterTime(TIME_TO_SELECT_IMPROVEMENT, 
		[this](int selection)
		{
			_nextState = STATES::INITIAL_MOVEMENT;
			if(_callbackSeleccion != nullptr)
			{
				auto optionForPlayer = _leftSelection;
				auto optionForEnemy = _rightSelection;

				if(selection == 1)
				{
					optionForPlayer = _rightSelection;
					optionForEnemy = _leftSelection;
				}

				_callbackSeleccion(optionForPlayer, optionForEnemy);
			}
		}
	);
	
	_nextState = STATES::IMPROVEMENT_SELECTOR;

	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(SCREEN_HEIGHT * POSITION_Y_PLAYER);
}
void ImprovementSelectionState::OnExit()
{
}

void ImprovementSelectionState::InitializeImprovementsUI()
{
	_improvementsUI[std::string(IMPROVEMENT_3_SHOTS)] = PainterManager::SPRITE_ID::SHOT_3_TIMES;
	_improvementsUI[std::string(IMPROVEMENT_INCREASE_ORIGIN)] = PainterManager::SPRITE_ID::INCREASE_ORIGIN;
	_improvementsUI[std::string(IMPROVEMENT_INCREASE_FIRE_RATE)] = PainterManager::SPRITE_ID::INCRASE_FIRE_RATE;
	_improvementsUI[std::string(IMPROVEMENT_GIVE_PENETRATION)] = PainterManager::SPRITE_ID::GIVE_PENETRATION;
	_improvementsUI[std::string(IMPROVEMENT_GIVE_EXPLOSION)] = PainterManager::SPRITE_ID::GIVE_EXPLOSION;
	_improvementsUI[std::string(IMPROVEMENT_GIVE_SHIELD)] = PainterManager::SPRITE_ID::GIVE_SHIELD;
	_improvementsUI[std::string(IMPROVEMENT_FAST_SHOTS)] = PainterManager::SPRITE_ID::FAST_SHOTS;
	_improvementsUI[std::string(IMPROVEMENT_SLOW_SHOTS)] = PainterManager::SPRITE_ID::SLOW_SHOTS;
}
void ImprovementSelectionState::Configure(std::string leftSelection, std::string rightSelection)
{
	_leftSelection = leftSelection;
	_rightSelection = rightSelection;
}