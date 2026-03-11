#pragma once
#include "GameConfig.h"
#include <array>
#include <functional>

class EasingManager
{
public:
	enum class EASE_TYPES
	{
		INOUTSINE,
		INOUTCUBE,
		INOUTQUINT,
		INOUTCIRC
	};

private:
	struct EasingData
	{
		float acumTime;
		EASE_TYPES type;
		float duration;
		float startX;
		float startY;
		float endX;
		float endY;
		std::function<void()> endCallback = nullptr;
		std::function<void(float currentX, float currentY)> tickCallback = nullptr;
	};

public:
	EasingManager();


public:
	void Update(const float deltaTime);
	bool AddEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type);
	bool AddEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type, std::function<void()> endCallback,
		std::function<void(float currentX, float currentY)> tickCallback);
	void FinishAll();
	void FinishEase(int id);

private:
	void GetValues(int id, float& x, float& y)const;
	float inOutSine(float percent, float startValue, float endValue)const;
	float inOutCube(float percent, float startValue, float endValue)const;
	float inOutQuint(float percent, float startValue, float endValue)const;
	float inOutCirc(float percent, float startValue, float endValue)const;

private:
	std::array<EasingData, MAX_EASING_VALUES> _eases;
	std::array<bool, MAX_EASING_VALUES> _inUse;
};