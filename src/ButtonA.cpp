#include "ButtonA.h"
#include "GameConfig.h"

void ButtonA::SelectInPosition(float duration, std::pair<int, int> optionA, std::function<void(int)> callback)
{
    SelectInPosition(duration, optionA, {-1,-1}, callback);
}

void ButtonA::SelectInPosition(float duration, std::pair<int, int> optionA,std::pair<int, int> optionB, std::function<void(int)> callback)
{
    _duration = duration;
    _callback = callback;
    _enabled = true;

    _optionA = optionA;
    _optionB = optionB;
    _acumTime = 0;
}

void ButtonA::Update(float deltaTime,const float currentInputValueNormalized, const float currentInputValue)
{
    if (!_enabled)
    {
        return;
    }

    float screenValue = SCREEN_WIDTH * currentInputValueNormalized;
    if(_optionA.first <= screenValue && screenValue <= _optionA.second)
    {
        _acumTime += deltaTime;
    }
    else
    {
        _acumTime = 0;
    }
    
    if(_acumTime >= _duration)
    {
        _callback(0);//always 0
        _enabled = false;
    }
}

float ButtonA::GetLeftTime() const
{
    return _duration - _acumTime;
}
