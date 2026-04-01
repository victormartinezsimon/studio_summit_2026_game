#include "EndGameState.h"
#include "PainterManager.h"
#include "Plane.h"
#include "GameConfig.h"
#include "Sprites.h"
#include "ButtonA.h"
#include "NumberManager.h"
#include "EasingManager.h"

constexpr float TIME_BLINK_LETTER = 0.5f;

EndGameState::EndGameState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager) : 
		State(player, painter, numberManager,  
			easingManager, randomManager, buttonAManager)
{

	_bestscores[0].name = "AAA"; _bestscores[0].points = 1000;
	_bestscores[1].name = "BBB"; _bestscores[1].points = 900;
	_bestscores[2].name = "VMS"; _bestscores[2].points = 800;
	_bestscores[3].name = "DDD"; _bestscores[3].points = 700;
	_bestscores[4].name = "EEE"; _bestscores[4].points = -1;//always is a best score


	_letters.Configure(painter, PainterManager::SPRITE_ID::LETTERS, 13, 2, -1);
}

State::STATES EndGameState::Update(const float deltaTime, float _currentFrameInputValueNormalized)
{
	//_buttonAManager->Update(deltaTime, _currentFrameInputValueNormalized);
	_timeAcumBlink += deltaTime;
	if(_timeAcumBlink > TIME_BLINK_LETTER)
	{
		_timeAcumBlink = 0;
	}
	return _nextState;
}
void EndGameState::Paint()
{
	/*/
	{
		_player->Paint(_painterManager);
	}

	{
		int time = _buttonAManager->GetLeftTime() + 1;
		float w = _painterManager->GetWidth(PainterManager::SPRITE_ID::PLAYER);
		_numberManager->PaintNumber(time, _player->GetX() -w/2, _player->GetY(), 1, NumberManager::PIVOT::RIGHT);
	}
	*/
}

void EndGameState::PaintUI()
{
	int positionX = SCREEN_WIDTH * 0.5f;
	int positionY = SCREEN_HEIGHT * 0.1f;

	for(int i = 0; i < _bestscores.size(); ++i)
	{
		PaintSavedScore(i, positionX, positionY, i == _playerIndexScore);
		positionY += (_letters.GetHeight() + 5);
	}


	/*
	{
		_numberManager->PaintNumber(_score, SCREEN_WIDTH * 0.5f, SCORE_Y, 3, NumberManager::PIVOT::CENTER);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::PLAYER_SELECTOR,
									  SELECTOR_X, SELECTOR_Y);
	}
	
	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::FINAL_SCORE,
									  SCREEN_WIDTH * 0.5f, FINAL_SCORE_Y);
	}

	{
		_painterManager->AddToPaint(PainterManager::SPRITE_ID::RETURN_MENU,
									  SELECTOR_X, RETURN_Y);
	}
	*/
}
void EndGameState::OnEnter()
{
	_nextState = STATES::END_GAME;
	_timeAcumBlink = 0;
	_indexLetterBlink = 0;

	if(_playerIndexScore == -1)
	{
		_nextState = STATES::MENU;
	}
	/*
	_buttonAManager->SelectInPosition(END_GAME_TIME_TO_MAIN_MENU, {SELECTOR_X- PLAYER_SELECTOR_WIDTH / 2, SELECTOR_X + PLAYER_SELECTOR_WIDTH / 2},
									  [this](int selection)
									  {
										  _nextState = STATES::MENU;
									  });

	_nextState = STATES::END_GAME;
	_player->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	_player->SetPositionY(POSITION_Y_PLAYER);
	_player->ConfigureSprite(_painterManager);
	*/
}
void EndGameState::OnExit()
{
}

void EndGameState::Configure(float score)
{
	_playerScore = score;
	CalculateIndexPlayerScore();
}

void EndGameState::CalculateIndexPlayerScore()
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


void EndGameState::PaintSavedScore(int index, float x, float y, bool forPlayer)
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
		letterX -= (_letters.GetWidth()+5);
	}

	//paint numbers
	int score = _bestscores[index].points;
	if(forPlayer)
	{
		score = _playerScore;
	}
	_numberManager->PaintNumber(score, x, y, 4, NumberManager::PIVOT::LEFT);
}
