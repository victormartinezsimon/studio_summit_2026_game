#pragma once
#include <array>

class PainterManager;
class TrailManager;


class Firework
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
		float _x = 0;
		float _y = 0;
		float _minY = 0;
		bool _inExplosion = false;
		TrailManager* _trailManager;
		std::array<MiniFirework, 8> _minFireworks;
};
