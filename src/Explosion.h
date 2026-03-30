#pragma once
#include <functional>
#include "WorldObject.h"
#include "PainterManager.h"

class Explosion : public WorldObject
{
public:

	void Update(const float deltaTime) override
	{
		bool ended = _spriteController.Update(deltaTime);

		if(ended)
		{
			_callbackEnd(*this);
		}
	}

	void SetCallbackEnd(std::function<void(Explosion&)> callbackEnd)
	{
		_callbackEnd = callbackEnd;
	}

	void ConfigureSprite(PainterManager* painter) override
	{
		_spriteController.Configure(painter, PainterManager::SPRITE_ID::EXPLOSION, 3,3, 0.2);
	}

	void GetSize(float& w, float& h)
	{
		_spriteController.GetSize(w, h);
	}

private:
	std::function<void(Explosion&)> _callbackEnd;
};
