#pragma once

class Plane;
class PainterManager;

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
    State(Plane *player, PainterManager *painter)
        : _player(player), _painterManager(painter) {};

public:
    virtual STATES Update(const float deltaTime, float _currentFrameInputValueNormalized,
                        int _currentFrameInputValue) {return STATES::MENU;};
    virtual void Paint() {};
    virtual void OnEnter() {};
    virtual void OnExit() {};

protected:
    Plane *_player;
    PainterManager *_painterManager;
};