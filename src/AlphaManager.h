#pragma once
#include "GameConfig.h"
#include <array>
#include <functional>
#include "PainterManager.h"
#include "Pool.h"
#include "Alpha.h"

class AlphaManager
{

public:
	AlphaManager(PainterManager* painterManager);


public:
	void Update(const float deltaTime);
	int AddUIAlpha(float duration, float x, float y, PainterManager::SPRITE_ID sprite);
	int AddAlpha(float duration, float x, float y, float width, float height, PainterManager::SPRITE_ID sprite);
	void FinishAll();
	void FinishAlpha(int id);
	void Paint();
	void AddCallback(int id, std::function<void()> callback);
	void CallFunctionInPool(int id, std::function<void(Alpha& alpha)> function);

private:
int AddInternalAlpha(float duration, bool isUI, float startX, float startY,
                            float width, float height, 
							PainterManager::SPRITE_ID sprite);

private:
	Pool<Alpha, MAX_ALPHA_VALUES> _alphaPool;
	PainterManager* _painterManager;
};