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
		PINGPONG,
		LINEAL
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
	void SetReferenceID( int id )
	{
		_referenceID = id;
	}

	int GetReferenceID() const
	{
		return _referenceID;
	}

public:
	bool Update(const float deltaTime);
	void BuildEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type);
	void BuildEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type, std::function<void(bool, int)> endCallback,
		std::function<void(float currentX, float currentY, Ease& ease, float percent)> tickCallback);

	void GetValues(float& x, float& y)const;
	void CallEndCallback(bool value);

public:
	void KillEase();

private:
	float inOutSine(float percent, float startValue, float endValue)const;
	float inOutCube(float percent, float startValue, float endValue)const;
	float inOutQuint(float percent, float startValue, float endValue)const;
	float inOutCirc(float percent, float startValue, float endValue)const;
	float pingPong(float percent, float startValue, float endValue)const;
	float lineal(float percent, float startValue, float endValue)const;

private:
	float _acumTime;
	EASE_TYPES _type;
	float _duration;
	float _startX;
	float _startY;
	float _endX;
	float _endY;
	std::function<void(bool, int)> _endCallback = nullptr;
	std::function<void(float currentX, float currentY, Ease& ease, float percent)> _tickCallback = nullptr;
	int _id;
	int _referenceID;
};