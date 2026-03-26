#pragma once
#include "GameConfig.h"
#include <array>
#include "Pool.h"
#include "PainterManager.h"

class TrailManager
{

private:
	struct Trail
	{
		float x;
		float y;
		int width;
		int height;
		PainterManager::SPRITE_ID sprite;
		float currentLive;
		float maxLive;
		int _id;

		public:
			void SetID(int id)
			{
				_id = id;
			}
			int GetID() const
			{
				return _id;
			}
	};

public:
	void Update(const float deltaTime);
	void AddTrail(float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite);
	void Paint(PainterManager* painter);

private:
	void Configure(Trail& trail, float x, float y, int width, int height, float duration, PainterManager::SPRITE_ID sprite);
	void UpdateTrail(Trail& trail, const float deltaTime);
	void PaintTrail(PainterManager* painter, Trail& trail);
	PainterManager::MASK_ID GetMaskFromFrame(Trail& trail) const;
	
private:
	Pool<Trail, MAX_TRAILS> _poolTrail;
};