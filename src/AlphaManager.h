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
	int AddAlpha(float duration, float x, float y, PainterManager::SPRITE_ID sprite);
	int AddAlpha(float duration, float x, float y, PainterManager::SPRITE_ID sprite, float width, float height);
	void FinishAll();
	void FinishAlpha(int id);
	void Paint();
	void AddCallback(int id, std::function<void()> callback);
	void CallFunctionInPool(int id, std::function<void(Alpha& alpha)> function);

private:
int AddInternalAlpha(float duration, float startX, float startY,
					 PainterManager::SPRITE_ID sprite,
                     float width, float height
							);

private:
	Pool<Alpha, MAX_ALPHA_VALUES> _alphaPool;
	PainterManager* _painterManager;
};