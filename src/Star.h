#pragma once
#include <functional>
#include "WorldObject.h"
#include "PainterManager.h"

class Star : public WorldObject
{
public:
	enum class Type {NEAR, MID, FAR};

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

	void SetTypeStar(Type t)
	{
		_typeStar = t;
	}

	PainterManager::SPRITE_ID GetSprite() const
	{
		switch (_typeStar)
		{
		case Type::NEAR: return PainterManager::SPRITE_ID::NEAR_STAR;
		case Type::MID: return PainterManager::SPRITE_ID::MID_STAR;
		case Type::FAR: return PainterManager::SPRITE_ID::FAR_STAR;
		}
		return PainterManager::SPRITE_ID::FAR_STAR;
	}

private:
	float _velocityX = 0;
	float _velocityY = 0;
	bool _moveLeft;
	Type _typeStar;
};
