#include "Firework.h"
#include "PainterManager.h"
#include "TrailManager.h"
#include <cmath>

constexpr float velocityY = -40;
constexpr float DURATION_TRAIL_MOVING = 2;
constexpr float GRAVITY = 10;
constexpr float DURATION_TRAIL_EXPLODED = 1;
constexpr float INITIAL_VELOCITY = 30;

void Firework::Configure(TrailManager* trailManager, float startX, float startY, float minY)
{
	_trailManager = trailManager;
	_x = startX;
	_y = startY;
	_minY = minY;
	_inExplosion = false;
}

void Firework::Update(const float deltaTime)
{
	if(!_inExplosion)
	{
		_y += deltaTime * velocityY;
		if(_y < _minY)
		{
			DoExplosion();
		}
	}
	else
	{
		for(auto& mini : _minFireworks)
		{
			mini._x += mini._velX * deltaTime;
			mini._y += mini._velY * deltaTime;
			mini._velY += GRAVITY * deltaTime;
		}
	}
}
void Firework::Paint(PainterManager* painter)const
{
	if(!_inExplosion)
	{
		painter->AddToPaint(PainterManager::SPRITE_ID::NEAR_STAR, _x, _y);
		unsigned int w, h;
		painter->GetSpriteSize(PainterManager::SPRITE_ID::NEAR_STAR, w, h);
		_trailManager->AddTrail(painter, _x, _y, w, h, 
			DURATION_TRAIL_MOVING, PainterManager::SPRITE_ID::NEAR_STAR);
	}
	else
	{
		for(auto&& mini : _minFireworks)
		{
			painter->AddToPaint(PainterManager::SPRITE_ID::FAR_STAR, mini._x, mini._y);
			unsigned int w, h;
			painter->GetSpriteSize(PainterManager::SPRITE_ID::FAR_STAR, w, h);
			_trailManager->AddTrail(painter,  mini._x, mini._y, w, h, 
				DURATION_TRAIL_EXPLODED, PainterManager::SPRITE_ID::FAR_STAR);
		}
	}
}

void Firework::DoExplosion()
{
	_inExplosion = true;

	float increaseAngle = 360.0 / _minFireworks.size();
	const double PI = 3.14159265358979323846;
	float currentAngle = 0;

	for(int i = 0; i < _minFireworks.size(); ++i)
	{
		float rad = currentAngle * (PI / 180.0);
		float f_sen = std::sin(rad);
		float f_cos = std::cos(rad);
		currentAngle += increaseAngle;

		_minFireworks[i]._x = _x;
		_minFireworks[i]._y = _y;
		_minFireworks[i]._velX = f_sen * INITIAL_VELOCITY;
		_minFireworks[i]._velY = f_cos * INITIAL_VELOCITY;
	}
}

bool Firework::OutOfScreen() const
{
	for(auto&& mini : _minFireworks)
	{
		if(mini._x >= 0 && mini._x <= SCREEN_WIDTH){return false;}
		if(mini._y >= 0 && mini._y <= SCREEN_HEIGHT){return false;}
	}

	return true;
}
