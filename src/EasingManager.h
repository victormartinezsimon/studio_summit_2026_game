#pragma once
#include "GameConfig.h"
#include <array>
#include <functional>
#include "Pool.h"
#include "Ease.h"

class EasingManager
{

public:
	void Update(const float deltaTime);

	int AddEase(float duration, float startX, float startY, 
		float endX, float endY, Ease::EASE_TYPES type);

	int AddEase(float duration, float startX, float startY, 
		float endX, float endY, Ease::EASE_TYPES type, std::function<void()> endCallback,
		std::function<void(float currentX, float currentY)> tickCallback);

	void FinishAll();
	void FinishEase(int id);
	void KillEase(int id);
	void KillAll();

private:
	Pool<Ease, MAX_EASING_VALUES> _poolEases;
};