#pragma once
#include "State.h"
#include "SpriteSheetController.h"
#include <array>
#include <string>

class ButtonA;
class NumberManager;

class EndGameState : public State
{
private:
    struct Score
    {
        std::string name;
        int points;
    };


public:
    EndGameState(Plane *player, PainterManager *painter,
                 NumberManager *numberManager,
                 EasingManager *easingManager, RandomManager *randomManager, ButtonA *buttonAManager);

public:
    STATES Update(const float deltaTime, float _currentFrameInputValueNormalized) override;
    void Paint() override;
    void PaintUI() override;
    void OnEnter() override;
    void OnExit() override;

public:
    void Configure(float score);

private:
    void PaintSavedScore(int index, float x, float y, bool forPlayer);
    void CalculateIndexPlayerScore();
private:
    STATES _nextState;
    float _playerScore;
    SpriteSheetController _letters;
    NumberManager *_numbers;

    std::array<Score, 5> _bestscores;
    int _playerIndexScore;
    float _timeAcumBlink = 0;
    int _indexLetterBlink =0;
};