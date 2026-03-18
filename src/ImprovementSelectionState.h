#pragma once
#include "State.h"
#include <functional>
#include <string>
#include <map>
#include "PainterManager.h"

class ButtonA;
class NumberManager;

class ImprovementSelectionState: public State
{
    public:
        ImprovementSelectionState(Plane* player, PainterManager* painter, ButtonA* buttonAManager, 
            std::function<void(const std::string& optionForPlayer,const std::string& optionForEnemy )> callbackSeleccion, 
             NumberManager* numberManager);
        
    public:    
        STATES Update(const float deltaTime, float currentFrameInputValueNormalized,
	                        int currentFrameInputValue)override;
        void Paint()override;
        void OnEnter()override;
        void OnExit()override;

    public:
        void Configure(std::string leftSelection, std::string rightSelection);
    private:
        void InitializeImprovementsUI();

    private:
        STATES _nextState;
        float _currentFrameInputValueNormalized;
        std::function<void(const std::string& optionForPlayer,const std::string& optionForEnemy)> _callbackSeleccion;
        std::string _leftSelection;
        std::string _rightSelection;
        std::map<std::string, PainterManager::SPRITE_ID> _improvementsUI;
        ButtonA *_buttonAManager;
        NumberManager* _numberManager;
};