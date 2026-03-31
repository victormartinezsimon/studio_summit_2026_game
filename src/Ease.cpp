#include "Ease.h"
#include "GameConfig.h"
#include <cmath>
#define M_PI       3.14159265358979323846   // pi

bool Ease::Update(const float deltaTime)
{
    _acumTime += deltaTime;

    float x, y;
    GetValues( x, y);

    if (_acumTime > _duration)
    {
        KillEase();
        return true;
    }
    else
    {
        if(_tickCallback)
        {
            _tickCallback(x, y, *this, _acumTime / _duration);
            GetValues( x, y);
        }
    }
    
    return false; 
}

void Ease::BuildEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type)
{
    return BuildEase(duration, startX, startY, endX, endY, type, nullptr, nullptr);
}

void Ease::BuildEase(float duration, float startX, float startY,
                            float endX, float endY, EASE_TYPES type, std::function<void(bool, int)> endCallback,
                            std::function<void(float currentX, float currentY, Ease& ease, float percent)> tickCallback)
{
    _acumTime = 0;
    _duration = duration;
    _endCallback = endCallback;
    _startX = startX;
    _startY = startY;
    _endX = endX;
    _endY = endY;
    _type = type;
    _tickCallback = tickCallback;
    _referenceID = -1;
}

void Ease::GetValues(float &x, float &y) const
{
    float progress = (_acumTime / _duration) * 100.0f;

    switch (_type)
    {
    case EASE_TYPES::INOUTCIRC:
        x = inOutCirc(progress, _startX, _endX);
        y = inOutCirc(progress, _startY, _endY);
        break;

        case EASE_TYPES::INOUTSINE:
        x = inOutSine(progress, _startX, _endX);
        y = inOutSine(progress, _startY, _endY);
        break;

        case EASE_TYPES::INOUTCUBE:
        x = inOutCube(progress, _startX, _endX);
        y = inOutCube(progress, _startY, _endY);
        break;

        case EASE_TYPES::INOUTQUINT:
        x = inOutQuint(progress, _startX, _endX);
        y = inOutQuint(progress, _startY, _endY);
        break;

        case EASE_TYPES::PINGPONG:
        x = pingPong(progress, _startX, _endX);
        y = pingPong(progress, _startY, _endY);
        break;

        case EASE_TYPES::LINEAL:
        x = lineal(progress, _startX, _endX);
        y = lineal(progress, _startY, _endY);
        break;
    }
}

void Ease::KillEase()
{
    _acumTime = _duration;
}

void Ease::CallEndCallback(bool value)
{
    if(_endCallback != nullptr)
    {
        _endCallback(value, GetReferenceID());
    }
}


float Ease::inOutSine(float progress, float startValue, float endValue) const
{
    float change = endValue - startValue;
    return -change / 2.0 * (cos(M_PI * progress / 100.0) - 1.0) + startValue;
}
float Ease::inOutCube(float progress, float startValue, float endValue) const
{
    float change = endValue - startValue;
    double t = progress / 50.0;
    if (t < 1.0)
        return change / 2.0 * t * t * t + startValue;
    t -= 2.0;
    return change / 2.0 * (t * t * t + 2.0) + startValue;
}
float Ease::inOutQuint(float progress, float startValue, float endValue) const
{
    float change = endValue - startValue;
    float t = progress / 50.0;
    if (t < 1.0)
        return change / 2.0 * t * t * t * t * t + startValue;
    t -= 2.0;
    return change / 2.0 * (t * t * t * t * t + 2.0) + startValue;
}
float Ease::inOutCirc(float progress, float startValue, float endValue) const
{
    double change = endValue - startValue;
    double t = progress / 50.0;
    if (t < 1.0)
        return -change / 2.0 * (sqrt(1.0 - t * t) - 1.0) + startValue;
    t -= 2.0;
    return change / 2.0 * (sqrt(1.0 - t * t) + 1.0) + startValue;
}

float Ease::pingPong(float progress, float startValue, float endValue) const
{
    // Normalize progress to [0, 1] using ping-pong logic
    // progress mod 2: [0,1] goes forward, [1,2] goes backward
    float t = std::fmod(progress, 2.0f);
    if (t > 1.0f)
        t = 2.0f - t;

    // Apply a smooth ease-in-out (cubic)
    t = t * t * (3.0f - 2.0f * t);

    return startValue + t * (endValue - startValue);
}

float Ease::lineal(float progress, float startValue, float endValue) const
{
    return startValue + (endValue - startValue) * progress/100.0f;
}