#include "TrailManager.h"

void TrailManager::Update(const float deltaTime)
{
	_poolTrail.for_each_active([&](Trail& t){UpdateTrail(t, deltaTime);});
}
void TrailManager::AddTrail(float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite)
{
	int id = _poolTrail.Get();
	_poolTrail.call_for_element(id, [&](Trail& t){Configure(t,x,y, width, height, duration, sprite);});
}
void TrailManager::Paint(PainterManager* painter)
{
	_poolTrail.for_each_active([&](Trail& t){PaintTrail(painter, t);});
}

void TrailManager::Configure(Trail& trail, float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite)
{
	trail.x = x;
	trail.y = y;
	trail.width = width;
	trail.height = height;
	trail.sprite = sprite;
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
	painter->AddToPaintWithAlpha(trail.sprite, trail.width, trail.height, trail.x, trail.y, GetMaskFromFrame(trail));
}

PainterManager::MASK_ID TrailManager::GetMaskFromFrame(Trail& trail) const
{
	float percent = trail.currentLive / trail.maxLive;

	if(0.75f <= percent)
	{
		return PainterManager::MASK_ID::FULL; 
	}

	if(0.25f < percent && percent <= 0.75f)
	{
		return PainterManager::MASK_ID::HALF;
	}
	return PainterManager::MASK_ID::QUARTER;

}
