#pragma once
#include "State.h"
#include "SpriteSheetController.h"
#include "TrailManager.h"
#include "Firework.h"
#include "Spawner.h"
#include <array>
#include <string>

class ButtonA;
class NumberManager;
class TrailManager;

class HighScoreState : public State
{
private:
    struct Score
    {
        std::string name;
        int points;
    };


public:
    HighScoreState(Plane *player, PainterManager *painter,
                 NumberManager *numberManager,
                 EasingManager *easingManager, RandomManager *randomManager, 
                 ButtonA *buttonAManager, TrailManager* trailManager);

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
    void CallbackButtonA(int option);
    void ConfigureReturnToMenu();
    void ConfigureFirework(Firework& firework);
    
private:
    STATES _nextState;
    float _playerScore;
    SpriteSheetController _letters;
    NumberManager *_numbers;
    TrailManager* _trailManager;

    std::array<Score, 4> _bestscores;
    int _playerIndexScore;
    float _timeAcumBlink = 0;
    int _indexLetterBlink = 0;

    Spawner<Firework, FIREWORK_TOTAL_AMOUNT> _spawnerFirework;
};