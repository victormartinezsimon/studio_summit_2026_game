#pragma once

class Plane;
class PainterManager;
class NumberManager;
class EasingManager;
class RandomManager;
class ButtonA;

class State
{
public:
    enum class STATES
    {
        MENU,
        BATTLE,
        IMPROVEMENT_SELECTOR,
        INITIAL_MOVEMENT,
        HIGH_SCORES,
        EXIT
    };

public:
    State(Plane *player, PainterManager *painter, 
        NumberManager* numberManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager)
        : _player(player), _painterManager(painter), _numberManager(numberManager),
         _easingManager(easingManager), 
        _randomManager(randomManager), _buttonAManager(buttonAManager) {};

public:
    virtual STATES Update(const float deltaTime, float _currentFrameInputValueNormalized) {return STATES::MENU;};
    virtual void Paint() {};
    virtual void PaintUI() {};
    virtual void OnEnter() {};
    virtual void OnExit() {};

protected:
    Plane *_player = nullptr;
    PainterManager *_painterManager = nullptr;
    NumberManager* _numberManager = nullptr;
    EasingManager* _easingManager = nullptr;
    RandomManager* _randomManager = nullptr;
    ButtonA* _buttonAManager = nullptr;
};