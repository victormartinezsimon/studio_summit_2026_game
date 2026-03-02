#pragma once
#include <array>
#include "GameConfig.h"

class Bullet;
class Plane;

class TextPainter
{
public:
	void BuildScreen();
	void Paint();
	void ClearScreen();

	void PaintBullet(const Bullet* b);
	void PaintEnemyPlane(const Plane* p);

private:
	std::array<std::array<char, SCREEN_WIDTH>, SCREEN_HEIGHT> _screen;
};