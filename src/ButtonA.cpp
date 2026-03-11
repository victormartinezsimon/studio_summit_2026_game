#include "ButtonA.h"
#include "GameConfig.h"

void ButtonA::SelectInPosition(float duration, std::pair<int, int> marginA, std::function<void(int)> callback)
{
    _duration = duration;
    _callback = callback;
    _isSelectInPosition = true;
    _enabled = true;

    _marginA = marginA;
    _acumTime = 0;
}

void ButtonA::SelectAfterTime(float duration, std::function<void(int)> callback)
{
    _duration = duration;
    _callback = callback;
    _isSelectInPosition = false;
    _enabled = true;
    _acumTime = 0;
}

void ButtonA::Update(float deltaTime,const float currentInputValueNormalized, const float currentInputValue)
{
    if (!_enabled)
    {
        return;
    }

    if (_isSelectInPosition)
    {
        UpdateSelectInPosition(deltaTime,currentInputValueNormalized, currentInputValue);
    }
    else
    {
        UpdateSelectAfterTime(deltaTime,currentInputValueNormalized, currentInputValue);
    }
}

void ButtonA::UpdateSelectInPosition(const float deltaTime, const float currentInputValueNormalized, const float currentInputValue)
{
    float screenValue = SCREEN_WIDTH * currentInputValueNormalized;
    if(_marginA.first <= screenValue && screenValue <= _marginA.second)
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
void ButtonA::UpdateSelectAfterTime(const float deltaTime, const float currentInputValueNormalized, const float currentInputValue)
{
    _acumTime += deltaTime;

    if (_acumTime > _duration)
    {
        auto value = currentInputValueNormalized;
        _callback(value <= 0.5f ? 0 : 1);
        _enabled = false;
    }
}