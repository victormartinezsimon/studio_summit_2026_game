#pragma once
#include "State.h"

class ButtonA;
class NumberManager;
class AlphaManager;

class MainMenuState: public State
{
    public:
        MainMenuState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager);
        
    public:    
        STATES Update(const float deltaTime, float _currentFrameInputValueNormalized)override;
        void Paint()override;
        void PaintUI()override;
        void OnEnter()override;
        void OnExit()override;

    private:
        void StartGame();
        void ExitGame();

    private:
        STATES _nextState;
        bool _startingGame = false;
};