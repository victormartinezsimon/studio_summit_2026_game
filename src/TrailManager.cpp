#include "TrailManager.h"

void TrailManager::Update(const float deltaTime)
{
	_poolTrail.for_each_active([&](Trail& t){UpdateTrail(t, deltaTime);});
}

int TrailManager::AddTrail(PainterManager* painter, float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite)
{
	return AddTrail(painter, x, y, width, height, duration, sprite, sprite);
}

int TrailManager::AddTrail(PainterManager* painter, float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite,
	PainterManager::SPRITE_ID small_sprite)
{
	int id = _poolTrail.Get();
	_poolTrail.call_for_element(id, [&](Trail& t){Configure(painter, t,x,y, width, height, duration, sprite, small_sprite);});
	return id;
}
void TrailManager::Paint(PainterManager* painter)
{
	_poolTrail.for_each_active([&](Trail& t){PaintTrail(painter, t);});
}

void TrailManager::Configure(PainterManager* painter, Trail& trail, float x, float y, int width, int height, float duration, 
	PainterManager::SPRITE_ID sprite,
	PainterManager::SPRITE_ID small_sprite)
{
	trail.x = x;
	trail.y = y;
	trail.width = width;
	trail.height = height;
	trail.spriteSheetNormal.Configure(painter, sprite);
	trail.spriteSheetSmall.Configure(painter, small_sprite);
	trail.maxLive = duration;
	trail.currentLive = duration;
}

void TrailManager::UpdateTrail(Trail& trail, const float deltaTime)
{
	trail.currentLive -= deltaTime;
	trail.spriteSheetNormal.Update(deltaTime);
	trail.spriteSheetSmall.Update(deltaTime);

	if(trail.currentLive <= 0)
	{
		_poolTrail.Release(trail);
	}
}

void TrailManager::PaintTrail(PainterManager* painter, Trail& trail)
{
	float percent = trail.currentLive / trail.maxLive;

	float alpha = percent * percent;

	if(percent < 0.5)
	{
		trail.spriteSheetSmall.Paint(painter, trail.x, trail.y, alpha);
	}
	else
	{
		trail.spriteSheetNormal.Paint(painter, trail.x, trail.y, alpha);
	}
}

SpriteSheetController* TrailManager::GetSpriteSheetNormal(int id)
{
	Trail* t =_poolTrail.GetElement(id);
	if(t != nullptr)
	{
		return &t->spriteSheetNormal;
	}
	return nullptr;
}
SpriteSheetController* TrailManager::GetSpriteSheetSmall(int id)
{
	Trail* t =_poolTrail.GetElement(id);
	if(t != nullptr)
	{
		return &t->spriteSheetSmall;
	}
	return nullptr;
}
