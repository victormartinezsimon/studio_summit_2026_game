#pragma once
#include "State.h"
#include "GameConfig.h"
#include "Pool.h"

class EasingManager;
class Plane;

class InitialMovementState: public State
{
    public:
        InitialMovementState(Plane *player, PainterManager *painter, 
        NumberManager* numberManager, AlphaManager* alphaManager,
        EasingManager* easingManager, RandomManager* randomManager, ButtonA* buttonAManager,
         Pool<Plane, PLANES_POOL_SIZE>* enemiesPool);
        
    public:    
        STATES Update(const float deltaTime, float _currentFrameInputValueNormalized,
	                        int _currentFrameInputValue)override;
        void Paint()override;
        void OnEnter()override;
        void OnExit()override;

    private:
        STATES _nextState;
        Pool<Plane, PLANES_POOL_SIZE>* _enemiesPool;
};