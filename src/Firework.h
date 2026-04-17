#pragma once
#include <array>
#include "WorldObject.h"

class PainterManager;
class TrailManager;



class Firework: public WorldObject
{
	private:
		struct MiniFirework
		{
			public:
				float _x;
				float _y;
				float _velX;
				float _velY;
		};

	public:
		void Configure(TrailManager* trailManager, float startX, float startY, float minY);

	public:
		void Update(const float deltaTime);
		void Paint(PainterManager* painter)const;
		bool OutOfScreen() const;

	private:
		void DoExplosion();

	private:
		float _minY = 0;
		bool _inExplosion = false;
		TrailManager* _trailManager;
		std::array<MiniFirework, 8> _minFireworks;
};
