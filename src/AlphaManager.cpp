#include "AlphaManager.h"

AlphaManager::AlphaManager(PainterManager *painterManager, EasingManager* easingManager) 
: _painterManager(painterManager),_easingManager(easingManager)
{
    _inUse.fill(false);
}

void AlphaManager::Update(const float deltaTime)
{
    for (int i = 0; i < _alphas.size(); ++i)
    {
        if (_inUse[i])
        {
            _alphas[i].acumTime += deltaTime;
        }
    }
}

int AlphaManager::AddInternalAlpha(float duration, bool isUI, float startX, float startY,
                            float endX, float endY, bool goDown, float width, float height, PainterManager::SPRITE_ID sprite)
{
    for (int i = 0; i < _inUse.size(); ++i)
    {
        if (!_inUse[i])
        {
            _inUse[i] = true;
            _alphas[i].acumTime = 0;
            _alphas[i].duration = duration;
            _alphas[i].goDown = goDown;
            _alphas[i].sprite = sprite;
            _alphas[i].isUI = isUI;
            _alphas[i].currentX = startX;
            _alphas[i].currentY = startY;
            _alphas[i].width = width;
            _alphas[i].height = height;
            _alphas[i].endCallback = nullptr;

            if(startX != endX || startY != endY)
            {
                //add ease
                int id = _easingManager->AddEase(duration, startX, startY, endX, endY, 
                    Ease::EASE_TYPES::INOUTCUBE,
                    [this, i, endX, endY]
                    {
                        _alphas[i].currentX = endX;
                        _alphas[i].currentY = endY;
                    },
                    [this, i](const float x, const float y)
                    {
                        _alphas[i].currentX = x;
                        _alphas[i].currentY = y;
                    }
                );
                if(id != -1)
                {
                    _alphas[i].easeID = id;
                }
            }

            return i;
        }
    }
    return -1;
}

int AlphaManager::AddUIAlpha(float duration, float x, float y, bool goDown, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, true, x, y, x, y, goDown, -1, -1, sprite );
}
int AlphaManager::AddAlpha(float duration, float x, float y, bool goDown, float width, float height, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, false, x, y, x, y, goDown, width, height, sprite );
}
int AlphaManager::AddUIAlpha(float duration, float x, float y, float endX, float endY, bool goDown, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, true, x, y, endX, endY, goDown, -1, -1, sprite );
}
int AlphaManager::AddAlpha(float duration, float x, float y,  float endX, float endY, bool goDown, float width, float height, PainterManager::SPRITE_ID sprite)
{
    return AddInternalAlpha(duration, false, x, y, endX, endY, goDown, width, height, sprite );
}

void AlphaManager::FinishAll()
{
    for (int i = 0; i < _inUse.size(); ++i)
    {
        FinishAlpha(i);
    }
}
void AlphaManager::FinishAlpha(int id)
{
    if (!_inUse[id])
    {
        return;
    }

    if(_alphas[id].endCallback)
    {
        _alphas[id].endCallback();
    }

    _alphas[id].endCallback = nullptr;
    _inUse[id] = false;
    _easingManager->FinishEase(_alphas[id].easeID);
}

void AlphaManager::Paint()
{
    for (int i = 0; i < _inUse.size(); ++i)
    {
        if (_inUse[i])
        {
            float percent = _alphas[i].acumTime / _alphas[i].duration;

            int maskIndex = 0;
            if(percent >= 0.33 && percent <= 0.66)
            {
                maskIndex = 1;
            }
            if(percent > 0.66)
            {
                maskIndex = 2;
            }

            if(_alphas[i].isUI)
            {
                _painterManager->AddUIToPaintWithAlpha(_alphas[i].sprite, _alphas[i].currentX, _alphas[i].currentY, maskIndex);
            }
            else
            {
                _painterManager->AddToPaintWithAlpha(_alphas[i].sprite, _alphas[i].width, _alphas[i].height,
                    _alphas[i].currentX, _alphas[i].currentY, maskIndex);
            }

            if(percent >= 1.0)
            {
                FinishAlpha(i);
            }
        }
    }
}

void AlphaManager::AddCallback(int id, std::function<void()> callback)
{
    if(id < 0){return;}

    _alphas[id].endCallback= callback;
}   
