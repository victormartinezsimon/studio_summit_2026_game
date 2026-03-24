#include "Alpha.h"

bool Alpha::Update(const float deltaTime)
{
    _acumTime += deltaTime;

    float percent = _acumTime / _duration;
    if(percent >= 1.0)
    {
        EndAlpha();
        return true;
    }
    return false;
}

void Alpha::ConfigureAlpha(float duration, bool isUI, float x, float y,
                            float width, float height, PainterManager::SPRITE_ID sprite)
{
    _acumTime = 0;
    _duration = duration;
    _sprite = sprite;
    _isUI = isUI;
    _currentX = x;
    _currentY = y;
    _width = width;
    _height = height;
    _endCallback = nullptr;
}

void Alpha::SetPosition(float x, float y)
{
    _currentX = x;
    _currentY = y;
}


void Alpha::AddCallback(std::function<void()> callback)
{
    _endCallback= callback;
}   


void Alpha::EndAlpha()
{
    if(_endCallback)
    {
        _endCallback();
    }

    _endCallback = nullptr;
}


void Alpha::Paint(PainterManager* _painter)
{
    
    float percent = _acumTime / _duration;

    int maskIndex = 0;
    if(percent >= 0.25 && percent <= 0.75)
    {
        maskIndex = 1;
    }
    if(percent > 0.75)
    {
        maskIndex = 2;
    }

    if(_isUI)
    {
        _painter->AddUIToPaintWithAlpha(_sprite, _currentX, _currentY, maskIndex);
    }
    else
    {
        _painter->AddToPaintWithAlpha(_sprite, _width, _height,
            _currentX, _currentY, maskIndex);
    }
}

