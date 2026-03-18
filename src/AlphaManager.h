#pragma once
#include "GameConfig.h"
#include <array>
#include <functional>
#include "PainterManager.h"
#include "EasingManager.h"

class AlphaManager
{

private:
	struct AlphaData
	{
		float acumTime;
		bool goDown;
		float duration;
		PainterManager::SPRITE_ID sprite;
		int currentX;
		int currentY;
		bool isUI;
		int width;
		int height;
		int easeID;
		std::function<void()> endCallback = nullptr;
	};

public:
	AlphaManager(PainterManager* painterManager, EasingManager* easingManager);


public:
	void Update(const float deltaTime);
	int AddUIAlpha(float duration, float x, float y, bool goDown, PainterManager::SPRITE_ID sprite);
	int AddAlpha(float duration, float x, float y, bool goDown, float width, float height, PainterManager::SPRITE_ID sprite);
	int AddUIAlpha(float duration, float x, float y, float endX, float endY, bool goDown, PainterManager::SPRITE_ID sprite);
	int AddAlpha(float duration, float x, float y,  float endX, float endY, bool goDown, float width, float height, PainterManager::SPRITE_ID sprite);
	void FinishAll();
	void FinishAlpha(int id);
	void Paint();
	void AddCallback(int id, std::function<void()> callback);

private:
int AddInternalAlpha(float duration, bool isUI, float startX, float startY,
                            float endX, float endY, bool goDown, float width, float height, 
							PainterManager::SPRITE_ID sprite);

private:
	std::array<AlphaData, MAX_ALPHA_VALUES> _alphas;
	std::array<bool, MAX_ALPHA_VALUES> _inUse;
	PainterManager* _painterManager;
	EasingManager* _easingManager;
};