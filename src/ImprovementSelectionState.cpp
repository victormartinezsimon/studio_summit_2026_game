#include "ImprovementSelectionState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "ButtonA.h"
#include "NumberManager.h"
#include "EasingManager.h"

ImprovementSelectionState::ImprovementSelectionState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager, 
            std::function<void(const std::string& optionForPlayer,const std::string& optionForEnemy )> callbackSeleccion) 
			: 
			State(player, painter, numberManager,
			easingManager, randomManager, buttonAManager), _callbackSeleccion(callbackSeleccion)
{
	InitializeImprovementsUI();
}

State::STATES ImprovementSelectionState::Update(const float deltaTime, float currentFrameInputValueNormalized)
{
	_currentFrameInputValueNormalized = currentFrameInputValueNormalized;

	_buttonAManager->Update(deltaTime, currentFrameInputValueNormalized);

	return _nextState;
}
void ImprovementSelectionState::Paint()
{
	if(_doingFadeOut){return;}

	{
		_player->Paint(_painterManager);
	}

	{
		
		float posY = _player->GetY();
		int time = _buttonAManager->GetLeftTime() + 1;
		float w = _painterManager->GetWidth(PainterManager::SPRITE_ID::PLAYER);
		_numberManager->PaintNumber(time, _player->GetX()-w/2, _player->GetY(), 1, NumberManager::PIVOT::RIGHT);
	}

	{
		float x = SCREEN_WIDTH - _player->GetX();
		float y = SCREEN_HEIGHT - _player->GetY();
		
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY, x, y);
	}
}
void ImprovementSelectionState::PaintUI()
{
	
	{
		_painterManager->AddToPaint(_improvementsUI[_leftSelection],
									  IMPROVEMENT_SELECTION_COORDS::OPTION_LEFT_X,
									  IMPROVEMENT_SELECTION_COORDS::OPTION_Y, _percentEase);
									  
		_painterManager->AddToPaint(_improvementsUI[_rightSelection],
									  IMPROVEMENT_SELECTION_COORDS::OPTION_RIGHT_X,
									  IMPROVEMENT_SELECTION_COORDS::OPTION_Y, _percentEase);
	}

	if(_doingFadeOut){return;}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									IMPROVEMENT_SELECTION_COORDS::OPTION_LEFT_X, 
  									IMPROVEMENT_SELECTION_COORDS::SELECTOR_Y_PLAYER);

		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									IMPROVEMENT_SELECTION_COORDS::OPTION_RIGHT_X, 
  									IMPROVEMENT_SELECTION_COORDS::SELECTOR_Y_PLAYER);

		_painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY_SELECTOR,
									IMPROVEMENT_SELECTION_COORDS::OPTION_LEFT_X, 
  									IMPROVEMENT_SELECTION_COORDS::SELECTOR_Y_ENEMY);

		_painterManager->AddToPaint(PainterManager::SPRITE_ID::ENEMY_SELECTOR,
									IMPROVEMENT_SELECTION_COORDS::OPTION_RIGHT_X, 
  									IMPROVEMENT_SELECTION_COORDS::SELECTOR_Y_ENEMY);
	}
}
void ImprovementSelectionState::OnEnter()
{
	auto halfZoneA = IMPROVEMENT_SELECTION_COORDS::OPTION_LEFT_X;
	std::pair<float, float> zoneA = {halfZoneA - PLAYER_SELECTOR_WIDTH / 2, halfZoneA + PLAYER_SELECTOR_WIDTH / 2};

	auto halfZoneB = IMPROVEMENT_SELECTION_COORDS::OPTION_RIGHT_X;
	std::pair<float, float> zoneB = {halfZoneB - PLAYER_SELECTOR_WIDTH / 2, halfZoneB + PLAYER_SELECTOR_WIDTH / 2};

	_buttonAManager->SelectInPosition(TIME_TO_SELECT_IMPROVEMENT, zoneA, zoneB,
									  [this](int selection)
									  {
										  if (_callbackSeleccion != nullptr)
										  {
											  auto optionForPlayer = _leftSelection;
											  auto optionForEnemy = _rightSelection;

											  if (selection == 1)
											  {
												  optionForPlayer = _rightSelection;
												  optionForEnemy = _leftSelection;
											  }

											  _callbackSeleccion(optionForPlayer, optionForEnemy);
										  }

									    _easingManager->AddEase(ALPHA_TIME_ENTER_GAME, 
										100, 100, 0, 0, Ease::EASE_TYPES::LINEAL, 
										[&](bool forced){_nextState = STATES::INITIAL_MOVEMENT;},
										[&](float x, float y, Ease& ease, float percent){_percentEase = 1 - percent;});

										_doingFadeOut = true;
									  });

	_nextState = STATES::IMPROVEMENT_SELECTOR;

	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->ConfigureSprite(_painterManager);
	_doingFadeOut = false;
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