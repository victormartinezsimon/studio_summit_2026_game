#pragma once
#include <functional>
#include "WorldObject.h"
#include "PainterManager.h"

class Explosion : public WorldObject
{
public:

	void Update(const float deltaTime)
	{
		_duration -= deltaTime;
		if(_duration <0)
		{
			_callbackEnd(this);
		}
	}

	void SetCallbackEnd(std::function<void(Explosion*)> callbackEnd)
	{
		_callbackEnd = callbackEnd;
	}

	void SetDuration(float duration)
	{
		_duration = duration;
	}

	void Paint(PainterManager* painter) override
	{
		painter->AddToPaint(PainterManager::SPRITE_ID::EXPLOSION, GetX(), GetY());
	}

private:
	float _duration = 0;
	std::function<void(Explosion*)> _callbackEnd;
};
