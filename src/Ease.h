#pragma once
#include "GameConfig.h"
#include <array>
#include <functional>

class Ease
{
public:
	enum class EASE_TYPES
	{
		INOUTSINE,
		INOUTCUBE,
		INOUTQUINT,
		INOUTCIRC,
		PINGPONG
	};

public:
	void SetID(int id)
	{
		_id = id;
	}
	int GetID() const
	{
		return _id;
	}

public:
	bool Update(const float deltaTime);
	void BuildEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type);
	void BuildEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type, std::function<void()> endCallback,
		std::function<void(float currentX, float currentY)> tickCallback);

	void GetValues(float& x, float& y)const;

public:
	void EndEase();
	void KillEase();

private:
	float inOutSine(float percent, float startValue, float endValue)const;
	float inOutCube(float percent, float startValue, float endValue)const;
	float inOutQuint(float percent, float startValue, float endValue)const;
	float inOutCirc(float percent, float startValue, float endValue)const;
	float pingPong(float percent, float startValue, float endValue)const;

private:
	float _acumTime;
	EASE_TYPES _type;
	float _duration;
	float _startX;
	float _startY;
	float _endX;
	float _endY;
	std::function<void()> _endCallback = nullptr;
	std::function<void(float currentX, float currentY)> _tickCallback = nullptr;
	int _id;
};