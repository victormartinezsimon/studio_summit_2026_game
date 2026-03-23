#include "AlphaManager.h"

AlphaManager::AlphaManager(PainterManager *painterManager) 
: _painterManager(painterManager){}

void AlphaManager::Update(const float deltaTime)
{
    _alphaPool.for_each_active([deltaTime, this](Alpha& alpha)
    {
        bool finished = alpha.Update(deltaTime);
        
        if(finished)
        {
            int easeID = alpha.GetEaseID();
            _alphaPool.Release(alpha);
        }
    });
   
}

int AlphaManager::AddInternalAlpha(float duration, bool isUI, float startX, float startY,
                            float width, float height, PainterManager::SPRITE_ID sprite)
{

    int poolID = _alphaPool.Get();
    
    if(poolID != -1)
    {
        _alphaPool.call_for_element(poolID, [&](Alpha& alpha)
        {
            alpha.ConfigureAlpha(duration,isUI, startX, startY, width, height, sprite);
        });
    }

    return poolID;
}

int AlphaManager::AddUIAlpha(float duration, float x, float y, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, true, x, y,  -1, -1, sprite );
}
int AlphaManager::AddAlpha(float duration, float x, float y, float width, float height, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, false, x, y,  width, height, sprite );
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
