#include "HighScoreState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h" //TODO: remove this inclusion
#include "ButtonA.h"
#include "NumberManager.h"
#include "EasingManager.h"
#include <cmath>

constexpr float TIME_BLINK_LETTER = 0.5f;
constexpr float TIME_TO_SELECT_OPTION = 2;
constexpr float LETTER_SEPARATION = 5;

HighScoreState::HighScoreState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager) : 
		State(player, painter, numberManager,  
			easingManager, randomManager, buttonAManager)
{

	_bestscores[0].name = "AAA"; _bestscores[0].points = 1000;
	_bestscores[1].name = "BBB"; _bestscores[1].points = 900;
	_bestscores[2].name = "VMS"; _bestscores[2].points = 800;
	_bestscores[3].name = "DDD"; _bestscores[3].points = -1;//always is a best score


	_letters.Configure(painter, PainterManager::SPRITE_ID::LETTERS, 13, 2, -1);
}

State::STATES HighScoreState::Update(const float deltaTime, float _currentFrameInputValueNormalized)
{
	_buttonAManager->Update(deltaTime, _currentFrameInputValueNormalized);
	_timeAcumBlink += deltaTime;
	if(_timeAcumBlink > TIME_BLINK_LETTER)
	{
		_timeAcumBlink = 0;
	}
	return _nextState;
}
void HighScoreState::Paint()
{
	{
		_player->Paint(_painterManager);
	}

	{
		int time = std::round(_buttonAManager->GetLeftTime());
		float w = _painterManager->GetWidth(PainterManager::SPRITE_ID::PLAYER);
		_numberManager->PaintNumber(time, _player->GetX() -w/2, _player->GetY(), 1, NumberManager::PIVOT::RIGHT);
	}
}

void HighScoreState::PaintUI()
{
	int positionX = HIGH_SCORE_COORDS::SCORE_START_X;
	int positionY = HIGH_SCORE_COORDS::SCORE_START_Y;

	for(int i = 0; i < _bestscores.size(); ++i)
	{
		PaintSavedScore(i, positionX, positionY, i == _playerIndexScore);
		positionY += (_letters.GetHeight() + LETTER_SEPARATION);
	}

	if(_playerIndexScore != -1)
	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
										HIGH_SCORE_COORDS::DECREASE_LETTER_X, 
										HIGH_SCORE_COORDS::SELECTOR_Y);

		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									HIGH_SCORE_COORDS::INCREASE_LETTER_X, 
									HIGH_SCORE_COORDS::SELECTOR_Y);

		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									HIGH_SCORE_COORDS::ACCEPT_LETTER_X, 
									HIGH_SCORE_COORDS::SELECTOR_Y);
	}
	else
	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									HIGH_SCORE_COORDS::ACCEPT_LETTER_X, 
									HIGH_SCORE_COORDS::SELECTOR_Y);
		
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::RETURN_MENU,
									HIGH_SCORE_COORDS::RETURN_MENU_X, 
									HIGH_SCORE_COORDS::RETURN_MENU_Y);
		
	}
	
}
void HighScoreState::OnEnter()
{
	_nextState = STATES::HIGH_SCORES;
	_timeAcumBlink = 0;
	_indexLetterBlink = 0;

	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->ConfigureSprite(_painterManager);_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->SetPlayerTeam(TEAM_PLAYER);
	_player->SetHasShield(false);
	_player->ConfigureSprite(_painterManager);
}
void HighScoreState::OnExit()
{
}

void HighScoreState::Configure(float score)
{
	_playerScore = score;
	CalculateIndexPlayerScore();
	if(_playerIndexScore == -1)
	{
		ConfigureReturnToMenu();
	}
	else
	{
		
		for(int i = _bestscores.size() -1; i > _playerIndexScore; --i)
		{
			_bestscores[i].name = _bestscores[i-1].name;
			_bestscores[i].points = _bestscores[i-1].points;
		}


		float playerSelectorWidth = _painterManager->GetWidth(PainterManager::SPRITE_ID::PLAYER_SELECTOR);
	
		_buttonAManager->SelectInPosition(TIME_TO_SELECT_OPTION, 
			{HIGH_SCORE_COORDS::DECREASE_LETTER_X - playerSelectorWidth/2, HIGH_SCORE_COORDS::DECREASE_LETTER_X + playerSelectorWidth/2 },
			{HIGH_SCORE_COORDS::INCREASE_LETTER_X - playerSelectorWidth/2, HIGH_SCORE_COORDS::INCREASE_LETTER_X + playerSelectorWidth/2 },
			{HIGH_SCORE_COORDS::ACCEPT_LETTER_X - playerSelectorWidth/2, HIGH_SCORE_COORDS::ACCEPT_LETTER_X + playerSelectorWidth/2 },
			[&](int option){CallbackButtonA(option);}
		);

		_buttonAManager->SetAutoRestart(true);
	}
}

void HighScoreState::CalculateIndexPlayerScore()
{
	for(int i = 0; i < _bestscores.size(); ++i)
	{
		if(_bestscores[i].points < _playerScore)
		{
			_playerIndexScore = i;
			return;
		}
	}
	_playerIndexScore = -1;
}

void HighScoreState::PaintSavedScore(int index, float x, float y, bool forPlayer)
{
	//paint letters
	int letterX = x -  _letters.GetWidth();
	for(int i = 2; i>= 0;--i)
	{
		char letter = _bestscores[index].name[i];
		int frame =  letter - 'A';
		if((i == _indexLetterBlink && _timeAcumBlink < TIME_BLINK_LETTER/2) || !forPlayer || i!= _indexLetterBlink)
		{
			_letters.PaintFrame(_painterManager, letterX, y, frame);
		}
		letterX -= (_letters.GetWidth() + LETTER_SEPARATION);
	}

	//paint numbers
	int score = _bestscores[index].points;
	if(forPlayer)
	{
		score = _playerScore;
	}
	_numberManager->PaintNumber(score, x, y, 4, NumberManager::PIVOT::LEFT);
}

void HighScoreState::CallbackButtonA(int option)
{
	if(option == 2)
	{
		++_indexLetterBlink;
		if(_indexLetterBlink >= 3)
		{
			_bestscores[_playerIndexScore].points = _playerScore;
			_playerIndexScore = -1;
			ConfigureReturnToMenu();
		}
		return;
	}

	int increase = 0;
	if(option == 0)
	{
		increase = -1;
	}
	else
	{
		increase = 1;
	}

	char currentLetter = _bestscores[_playerIndexScore].name[_indexLetterBlink];
	currentLetter += increase;

	if(currentLetter < 'A'){currentLetter = 'Z';}
	if(currentLetter > 'Z'){currentLetter = 'A';}

	_bestscores[_playerIndexScore].name[_indexLetterBlink] = currentLetter;	
}

void HighScoreState::ConfigureReturnToMenu()
{
	_playerIndexScore = -1;
	_indexLetterBlink = -1;

	float playerSelectorWidth = _painterManager->GetWidth(PainterManager::SPRITE_ID::PLAYER_SELECTOR);

	_buttonAManager->SelectInPosition(TIME_TO_SELECT_OPTION, 
			{HIGH_SCORE_COORDS::ACCEPT_LETTER_X - playerSelectorWidth/2, HIGH_SCORE_COORDS::ACCEPT_LETTER_X + playerSelectorWidth/2 },
			[&](int option){
				
				_nextState = STATES::MENU;
			}
		);
}