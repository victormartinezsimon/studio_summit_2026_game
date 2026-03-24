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
            ease.CallEndCallback(true);
        }
    });

}

int EasingManager::AddEase(float duration, float startX, float startY, 
		float endX, float endY, Ease::EASE_TYPES type)
{
    return AddEase(duration, startX, startY, endX, endY, type, nullptr, nullptr);
}

int EasingManager::AddEase(float duration, float startX, float startY,
                            float endX, float endY, Ease::EASE_TYPES type, std::function<void(bool)> endCallback,
                            std::function<void(float currentX, float currentY, Ease& ease)> tickCallback)
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
        endCallback( false );//just in case    
    }
    return easeID;
}
void EasingManager::KillEase(int id)
{
    _poolEases.call_for_element(id, [&](Ease& ease)
    {
        ease.KillEase(); 
        _poolEases.Release(ease);
    });
}
void EasingManager::KillAll()
{
    _poolEases.for_each_active([](Ease& ease){ease.KillEase();});
    _poolEases.ReturnAll();
}

void EasingManager::SetReferenceIDToEase(int easeID, int referenceID)
{
    _poolEases.call_for_element(easeID, [&](Ease& ease){ease.SetReferenceID(referenceID);});
}