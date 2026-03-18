#pragma once
#include <functional>
#include "WorldObject.h"

class Meteorite : public WorldObject
{
public:
	void Update(const float deltaTime)
	{
		float newX = deltaTime * _velocityX;
		_X += newX;

		float newY = deltaTime * _velocityY;
		_Y += newY;
	}

	void SetVelocities(float x, float y)
	{
		_velocityX = x;
		_velocityY = y;
	}

	void SetMoveLeft(bool value)
	{
		_moveLeft = value;
	}

	bool GetMoveLeft() const
	{
		return _moveLeft;
	}

private:
	float _velocityX = 0;
	float _velocityY = 0;
	bool _moveLeft;
};
