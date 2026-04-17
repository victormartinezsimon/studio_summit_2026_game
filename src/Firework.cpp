#include "Firework.h"
#include "PainterManager.h"
#include "TrailManager.h"
#include <cmath>

void Firework::Configure(TrailManager* trailManager, float startX, float startY, float minY)
{
	_trailManager = trailManager;
	_X = startX;
	_Y = startY;
	_minY = minY;
	_inExplosion = false;
}

void Firework::Update(const float deltaTime)
{
	if(!_inExplosion)
	{
		_Y += deltaTime * FIREWORK_ASCEND_VELOCITY;
		if(_Y < _minY)
		{
			DoExplosion();
		}
	}
	else
	{
		_timeSinceExplosion += deltaTime;
		for(auto& mini : _minFireworks)
		{
			mini._x += mini._velX * deltaTime;
			mini._y += mini._velY * deltaTime;
			mini._velY += FIREWORK_GRAVITY * deltaTime;
		}
	}
}
void Firework::Paint(PainterManager* painter)const
{
	if(!_inExplosion)
	{
		painter->AddToPaint(PainterManager::SPRITE_ID::NEAR_STAR, _X, _Y);
		unsigned int w, h;
		painter->GetSpriteSize(PainterManager::SPRITE_ID::NEAR_STAR, w, h);
		_trailManager->AddTrail(painter, _X, _Y, w, h, 
			FIREWORK_DURATION_TRAIL_MOVING, PainterManager::SPRITE_ID::NEAR_STAR);
	}
	else
	{
		for(auto&& mini : _minFireworks)
		{
			float percent =  (_timeSinceExplosion / FIREWORK_TIME_EXPLOSION_LIVE);
			float alpha = 1 - percent;
			painter->AddToPaint(PainterManager::SPRITE_ID::FAR_STAR, mini._x, mini._y, alpha);

			if(percent > FIREWORK_MIN_PERCENT_FOR_EXPLOSION_TRAIL)
			{
				unsigned int w, h;
				painter->GetSpriteSize(PainterManager::SPRITE_ID::FAR_STAR, w, h);
				_trailManager->AddTrail(painter,  mini._x, mini._y, w, h, 
					FIREWORK_DURATION_TRAIL_EXPLOSION, PainterManager::SPRITE_ID::FAR_STAR);
			}
		}
	}
}

void Firework::DoExplosion()
{
	_inExplosion = true;
	_timeSinceExplosion = 0;

	float increaseAngle = 360.0 / _minFireworks.size();
	const double PI = 3.14159265358979323846;
	float currentAngle = 0;

	for(int i = 0; i < _minFireworks.size(); ++i)
	{
		float rad = currentAngle * (PI / 180.0);
		float f_sen = std::sin(rad);
		float f_cos = std::cos(rad);
		currentAngle += increaseAngle;

		_minFireworks[i]._x = _X;
		_minFireworks[i]._y = _Y;
		_minFireworks[i]._velX = f_sen * FIREWORK_INITIAL_VELOCITY;
		_minFireworks[i]._velY = f_cos * FIREWORK_INITIAL_VELOCITY;
	}
}

bool Firework::OutOfScreen() const
{
	if(!_inExplosion){return false;}

	return _timeSinceExplosion >= FIREWORK_TIME_EXPLOSION_LIVE;
}
