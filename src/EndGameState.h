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
    float _timeAcumState = 0;
    float _playerScore = 0;
};