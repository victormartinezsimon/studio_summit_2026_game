#include "ButtonA.h"
#include "GameConfig.h"

constexpr int selectionA = 0;
constexpr int selectionB = 1;
constexpr int selectionC = 2;
constexpr int selectionNone = -1;

void ButtonA::SelectInPosition(float duration, std::pair<int, int> optionA, std::function<void(int)> callback)
{
    SelectInPosition(duration, optionA, {-1, -1}, {-1, -1}, callback);
}

void ButtonA::SelectInPosition(float duration, std::pair<int, int> optionA, std::pair<int, int> optionB, std::function<void(int)> callback)
{
    SelectInPosition(duration, optionA, optionB, {-1, -1}, callback);
}

void ButtonA::SelectInPosition(float duration, std::pair<int, int> optionA, std::pair<int, int> optionB,
                               std::pair<int, int> optionC, std::function<void(int)> callback)
{
    _duration = duration;
    _callback = callback;
    _enabled = true;

    _optionA = optionA;
    _optionB = optionB;
    _optionC = optionC;

    _acumTime = 0;
    _currentSelection = selectionNone;
    _autoRestart = false;
}

void ButtonA::Update(float deltaTime, const float currentInputValueNormalized)
{
    if (!_enabled)
    {
        return;
    }

    float screenValue = SCREEN_WIDTH * currentInputValueNormalized;
    if (_optionA.first <= screenValue && screenValue <= _optionA.second)
    {
        if (_currentSelection == selectionA)
        {
            _acumTime += deltaTime;
        }
        _currentSelection = selectionA;
    }
    else
    {
        if (_optionB.first <= screenValue && screenValue <= _optionB.second)
        {
            if (_currentSelection == selectionB)
            {
                _acumTime += deltaTime;
            }
            _currentSelection = selectionB;
        }
        else
        {
            if (_optionC.first <= screenValue && screenValue <= _optionC.second)
            {
                if (_currentSelection == selectionC)
                {
                    _acumTime += deltaTime;
                }
                _currentSelection = selectionC;
            }
            else
            {
                _acumTime = 0;
                _currentSelection = selectionNone;
            }
        }
    }

    if (_acumTime >= _duration)
    {
        if(_autoRestart)
        {
            _acumTime = 0;
        }
        else
        {
            _enabled = false;
        }
        _callback(_currentSelection);
        _currentSelection = selectionNone;
    }
}

float ButtonA::GetLeftTime() const
{
    return _duration - _acumTime;
}

void ButtonA::SetAutoRestart(bool value)
{
    _autoRestart = value;
}
