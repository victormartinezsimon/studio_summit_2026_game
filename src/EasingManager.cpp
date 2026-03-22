#include "EasingManager.h"
#include "GameConfig.h"
#include <cmath>

void EasingManager::Update(const float deltaTime)
{

    _poolEases.for_each_active([&](Ease& ease)
    {
        bool finished = ease.Update(deltaTime);
        if(finished)
        {
            _poolEases.Release(ease);
        }
    });

}

int EasingManager::AddEase(float duration, float startX, float startY, 
		float endX, float endY, Ease::EASE_TYPES type)
{
    return AddEase(duration, startX, startY, endX, endY, type, nullptr, nullptr);
}

int EasingManager::AddEase(float duration, float startX, float startY,
                            float endX, float endY, Ease::EASE_TYPES type, std::function<void()> endCallback,
                            std::function<void(float currentX, float currentY)> tickCallback)
{

    int easeID = _poolEases.Get();

    if(easeID != -1)
    {
        _poolEases.call_for_element(easeID, [&](Ease& ease)
        {
            ease.BuildEase(duration, startX, startY, endX, endY, type, endCallback, tickCallback);
        });
    }

    if(easeID == -1)
    {
        endCallback();//just in case    
    }
    return easeID;
}
void EasingManager::FinishAll()
{
    _poolEases.for_each_active([](Ease& ease){ease.EndEase();});
    _poolEases.ReturnAll();
}
void EasingManager::FinishEase(int id)
{
     _poolEases.call_for_element(id, [](Ease& ease){ease.EndEase();});
}
void EasingManager::KillEase(int id)
{
    _poolEases.call_for_element(id, [](Ease& ease){ease.KillEase();});
}
void EasingManager::KillAll()
{
    _poolEases.for_each_active([](Ease& ease){ease.KillEase();});
    _poolEases.ReturnAll();
}