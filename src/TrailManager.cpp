#include "TrailManager.h"

void TrailManager::Update(const float deltaTime)
{
	_poolTrail.for_each_active([&](Trail& t){UpdateTrail(t, deltaTime);});
}

void TrailManager::AddTrail(float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite)
{
	AddTrail(x, y, width, height, duration, sprite, sprite, sprite);
}


void TrailManager::AddTrail(float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite,
	PainterManager::SPRITE_ID small_sprite,PainterManager::SPRITE_ID extra_small_sprite)
{
	int id = _poolTrail.Get();
	_poolTrail.call_for_element(id, [&](Trail& t){Configure(t,x,y, width, height, duration, sprite, small_sprite, extra_small_sprite);});
}
void TrailManager::Paint(PainterManager* painter)
{
	_poolTrail.for_each_active([&](Trail& t){PaintTrail(painter, t);});
}

void TrailManager::Configure(Trail& trail, float x, float y, int width, int height, float duration, 
	PainterManager::SPRITE_ID sprite,
	PainterManager::SPRITE_ID small_sprite,PainterManager::SPRITE_ID extra_small_sprite)
{
	trail.x = x;
	trail.y = y;
	trail.width = width;
	trail.height = height;
	trail.sprite = sprite;
	trail.small_sprite = small_sprite;
	trail.extra_small_sprite = extra_small_sprite;
	trail.maxLive = duration;
	trail.currentLive = duration;
}

void TrailManager::UpdateTrail(Trail& trail, const float deltaTime)
{
	trail.currentLive -= deltaTime;

	if(trail.currentLive <= 0)
	{
		_poolTrail.Release(trail);
	}
}

void TrailManager::PaintTrail(PainterManager* painter, Trail& trail)
{
	float percent = trail.currentLive / trail.maxLive;
	PainterManager::SPRITE_ID spriteToPaint = trail.sprite;

	float alpha = percent * percent;

	if(percent < 0.5)
	{
		spriteToPaint = trail.small_sprite;
	}

	painter->AddToPaint(spriteToPaint, trail.x, trail.y, alpha);
}
