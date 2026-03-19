#pragma once
#include "State.h"

class ButtonA;
class NumberManager;
class AlphaManager;

class MainMenuState: public State
{
    public:
        MainMenuState(Plane* player, PainterManager* painter, ButtonA* buttonAManager, 
            NumberManager* numberManager, AlphaManager* alphaManager);
        
    public:    
        STATES Update(const float deltaTime, float _currentFrameInputValueNormalized,
	                        int _currentFrameInputValue)override;
        void Paint()override;
        void OnEnter()override;
        void OnExit()override;

    private:
        void StartGame();

    private:
        STATES _nextState;
        ButtonA *_buttonAManager;
        NumberManager* _numberManager;
        AlphaManager* _alphaManager;
        bool _startingGame = false;
};