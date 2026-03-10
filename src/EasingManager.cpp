#include "EasingManager.h"
#include "GameConfig.h"
#include <cmath>

EasingManager::EasingManager()
{
    _inUse.fill(false);
}

void EasingManager::Update(const float deltaTime)
{
    for (int i = 0; i < _eases.size(); ++i)
    {
        if (_inUse[i])
        {
            _eases[i].acumTime += deltaTime;

            float x, y;
            GetValues(i, x, y);

            if (_eases[i].acumTime > _eases[i].duration)
            {
                _eases[i].endCallback();
                _inUse[i] = false;
            }
        }
    }
}
bool EasingManager::AddEase(float duration, float startX, float startY,
                            float endX, float endY, EASE_TYPES type, std::function<void()> endCallback,
                            std::function<void(float currentX, float currentY)> tickCallback)
{
    for (int i = 0; i < _inUse.size(); ++i)
    {
        if (!_inUse[i])
        {
            _inUse[i] = true;
            _eases[i].acumTime = 0;
            _eases[i].duration = duration;
            _eases[i].endCallback = endCallback;
            _eases[i].startX = startX;
            _eases[i].startY = startY;
            _eases[i].endX = endX;
            _eases[i].endY = endY;
            _eases[i].type = type;
            _eases[i].tickCallback = tickCallback;
        }
    }
    return false;
}

void EasingManager::GetValues(int id, float &x, float &y) const
{
    float progress = (_eases[id].acumTime / _eases[id].duration) * 100.0f;

    switch (_eases[id].type)
    {
    case EASE_TYPES::INOUTCIRC:
        x = inOutCirc(progress, _eases[id].startX, _eases[id].endX);
        y = inOutCirc(progress, _eases[id].startY, _eases[id].endY);
        break;

        case EASE_TYPES::INOUTSINE:
        x = inOutSine(progress, _eases[id].startX, _eases[id].endX);
        y = inOutSine(progress, _eases[id].startY, _eases[id].endY);
        break;

        case EASE_TYPES::INOUTCUBE:
        x = inOutCube(progress, _eases[id].startX, _eases[id].endX);
        y = inOutCube(progress, _eases[id].startY, _eases[id].endY);
        break;

        case EASE_TYPES::INOUTQUINT:
        x = inOutQuint(progress, _eases[id].startX, _eases[id].endX);
        y = inOutQuint(progress, _eases[id].startY, _eases[id].endY);
        break;
    break;
    }
}

float EasingManager::inOutSine(float progress, float startValue, float endValue) const
{
    float change = endValue - startValue;
    return -change / 2.0 * (cos(M_PI * progress / 100.0) - 1.0) + startValue;
}
float EasingManager::inOutCube(float progress, float startValue, float endValue) const
{
    float change = endValue - startValue;
    double t = progress / 50.0;
    if (t < 1.0)
        return change / 2.0 * t * t * t + startValue;
    t -= 2.0;
    return change / 2.0 * (t * t * t + 2.0) + startValue;
}
float EasingManager::inOutQuint(float progress, float startValue, float endValue) const
{
    float change = endValue - startValue;
    float t = progress / 50.0;
    if (t < 1.0)
        return change / 2.0 * t * t * t * t * t + startValue;
    t -= 2.0;
    return change / 2.0 * (t * t * t * t * t + 2.0) + startValue;
}
float EasingManager::inOutCirc(float progress, float startValue, float endValue) const
{
    double change = endValue - startValue;
    double t = progress / 50.0;
    if (t < 1.0)
        return -change / 2.0 * (sqrt(1.0 - t * t) - 1.0) + startValue;
    t -= 2.0;
    return change / 2.0 * (sqrt(1.0 - t * t) + 1.0) + startValue;
}