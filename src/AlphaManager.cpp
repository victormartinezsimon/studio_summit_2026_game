#include "AlphaManager.h"

AlphaManager::AlphaManager(PainterManager *painterManager, EasingManager* easingManager) 
: _painterManager(painterManager),_easingManager(easingManager){}

void AlphaManager::Update(const float deltaTime)
{
    _alphaPool.for_each_active([deltaTime, this](Alpha& alpha)
    {
        bool finished = alpha.Update(deltaTime);
        
        if(finished)
        {
            int easeID = alpha.GetEaseID();
            _easingManager->FinishEase(easeID);
            _alphaPool.Release(alpha);
        }
    });
   
}

int AlphaManager::AddInternalAlpha(float duration, bool isUI, float startX, float startY,
                            float endX, float endY, float width, float height, PainterManager::SPRITE_ID sprite)
{

    int poolID = _alphaPool.Get();
    
    if(poolID != -1)
    {
        _alphaPool.call_for_element(poolID, [&](Alpha& alpha)
        {
            alpha.ConfigureAlpha(duration,isUI, startX, startY, width, height, sprite);
        });
    }


    if(poolID != -1)
    {
        if(startX != endX || startY != endY)
        {
            int easeID = _easingManager->AddEase(duration, startX, startY, endX, endY, Ease::EASE_TYPES::INOUTCIRC,
                        [&](){_alphaPool.call_for_element(poolID, [&](Alpha& a){a.SetPosition(endX, endY);});},
                        [&](const float x, const float y){_alphaPool.call_for_element(poolID, [&](Alpha& a){a.SetPosition(x, y);});}
                        );
            if(easeID != -1)
            {
                _alphaPool.call_for_element(poolID, [&](Alpha& alpha){alpha.SetEaseID(easeID);});
            }
        }
    }

    return poolID;
}

int AlphaManager::AddUIAlpha(float duration, float x, float y, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, true, x, y, x, y, -1, -1, sprite );
}
int AlphaManager::AddAlpha(float duration, float x, float y, float width, float height, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, false, x, y, x, y, width, height, sprite );
}
int AlphaManager::AddUIAlpha(float duration, float x, float y, float endX, float endY, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, true, x, y, endX, endY, -1, -1, sprite );
}
int AlphaManager::AddAlpha(float duration, float x, float y,  float endX, float endY, float width, float height, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, false, x, y, endX, endY, width, height, sprite );
}

void AlphaManager::FinishAll()
{
   _alphaPool.for_each_active([](Alpha& alpha){alpha.EndAlpha();});
    _alphaPool.ReturnAll();
}
void AlphaManager::FinishAlpha(int id)
{
    _alphaPool.call_for_element(id, [&](Alpha& alpha)
    {
        alpha.EndAlpha();
        _alphaPool.Release(alpha);
    });
}

void AlphaManager::Paint()
{
   _alphaPool.for_each_active([&](Alpha& alpha)
   {
        alpha.Paint(_painterManager);
   });
}

void AlphaManager::AddCallback(int id, std::function<void()> callback)
{
    if(id < 0){return;}

    _alphaPool.call_for_element(id, [&](Alpha& alpha)
    {
        alpha.AddCallback(callback);
    });
}   
