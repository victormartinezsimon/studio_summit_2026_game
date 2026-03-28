#pragma once
#include <functional>
#include "WorldObject.h"
#include "PainterManager.h"

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

	void Paint(PainterManager* painter)override
	{
		painter->AddToPaint(PainterManager::SPRITE_ID::METEORITE, GetX(), GetY());
	}


private:
	float _velocityX = 0;
	float _velocityY = 0;
	bool _moveLeft;
};
