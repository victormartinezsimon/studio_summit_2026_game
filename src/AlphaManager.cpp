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

int AlphaManager::AddInternalAlpha(float duration, float startX, float startY,
					 PainterManager::SPRITE_ID sprite,
                     float width, float height)
{

    int poolID = _alphaPool.Get();
    
    if(poolID != -1)
    {
        _alphaPool.call_for_element(poolID, [&](Alpha& alpha)
        {
            alpha.ConfigureAlpha(duration,startX, startY, sprite, width, height);
        });
    }

    return poolID;
}

int AlphaManager::AddAlpha(float duration, float x, float y, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, x, y, sprite, -1, -1);
}
int AlphaManager::AddAlpha(float duration, float x, float y, PainterManager::SPRITE_ID sprite, float width, float height)
{
    return AddInternalAlpha(duration, x, y, sprite, width, height );
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

void AlphaManager::CallFunctionInPool(int id, std::function<void(Alpha& alpha)> fun)
{
    _alphaPool.call_for_element(id, [&](Alpha& alpha)
        {
            fun(alpha);
        }
    );
}
