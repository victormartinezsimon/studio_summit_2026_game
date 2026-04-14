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
	void SetPositionX(float x){}
	void SetPositionY(float y){}

public:
	bool Update(const float deltaTime);
	void BuildEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type);
	void BuildEase(float duration, float startX, float startY, 
		float endX, float endY, EASE_TYPES type, std::function<void(bool, int)> endCallback,
		std::function<void(float currentX, float currentY, Ease& ease, float percent)> tickCallback);

	void GetValues(float& x, float& y)const;
	void CallEndCallback(bool value);
	void SetDelay(float value);

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
	float _acumTime = 0;
	float _realAcumTime = 0;
	EASE_TYPES _type = EASE_TYPES::LINEAL;
	float _duration = 0;
	float _startX = 0;
	float _startY = 0;
	float _endX = 0;
	float _endY = 0;
	std::function<void(bool, int)> _endCallback = nullptr;
	std::function<void(float currentX, float currentY, Ease& ease, float percent)> _tickCallback = nullptr;
	int _id = 0;
	int _referenceID = 0;
	float _delayStart = 0;
};