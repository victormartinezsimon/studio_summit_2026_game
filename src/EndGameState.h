#pragma once
#include "State.h"

class ButtonA;
class NumberManager;
class AlphaManager;

class EndGameState: public State
{
    public:
        EndGameState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager);
        
    public:    
        STATES Update(const float deltaTime, float _currentFrameInputValueNormalized,
	                        int _currentFrameInputValue)override;
        void Paint()override;
        void OnEnter()override;
        void OnExit()override;

    public:
        void Configure(float score);

    private:
        STATES _nextState;
        float _score;
};