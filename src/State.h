#pragma once

class Plane;
class PainterManager;
class NumberManager;
class AlphaManager;
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
        END_GAME,
        EXIT
    };

public:
    State(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager)
        : _player(player), _painterManager(painter), _numberManager(numberManager),
        _alphaManager(alphaManager), _easingManager(easingManager), 
        _randomManager(randomManager), _buttonAManager(buttonAManager) {};

public:
    virtual STATES Update(const float deltaTime, float _currentFrameInputValueNormalized,
                        int _currentFrameInputValue) {return STATES::MENU;};
    virtual void Paint() {};
    virtual void OnEnter() {};
    virtual void OnExit() {};

protected:
    Plane *_player = nullptr;
    PainterManager *_painterManager = nullptr;
    NumberManager* _numberManager = nullptr;
    AlphaManager* _alphaManager = nullptr;
    EasingManager* _easingManager = nullptr;
    RandomManager* _randomManager = nullptr;
    ButtonA* _buttonAManager = nullptr;
};