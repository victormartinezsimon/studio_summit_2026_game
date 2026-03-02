#pragma once
#include <array>
#include "GameConfig.h"
#include "raylib-cpp.hpp"


class Bullet;
class Plane;

class RaylibPainter
{
public:
	RaylibPainter();

public:
	void BeginPaint();
	void EndPaint();

public:
	void PaintBackground();
	void PaintPlayer(Plane* player);
	void PaintEnemy(Plane* enemy);

private:
	void PaintPlane(Plane* player, raylib::Texture& texture);

public:
	bool HasEnded();

private:
	raylib::Window _window;

	raylib::Texture _player;
	raylib::Texture _enemy;
	raylib::Texture _background;
	raylib::Texture _bullet;
};