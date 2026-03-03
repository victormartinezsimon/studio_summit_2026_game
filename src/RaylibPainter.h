#pragma once
#include <array>
#include "GameConfig.h"
#include "raylib.h"


class Bullet;
class Plane;

class RaylibPainter
{
public:
	RaylibPainter();
	~RaylibPainter();

public:
	void BeginPaint();
	void EndPaint();
	float GetDeltaTime();

public:
	void PaintBackground();
	void PaintPlayer(Plane* player);
	void PaintEnemy(Plane* enemy);
	void PaintBullet(Bullet* bullet);

private:
	void PaintPlane(Plane* player, Texture2D& texture);

public:
	bool HasEnded();

private:
	
	//raylib::Window _window;

	Texture2D _player;
	Texture2D _enemy;
	Texture2D _background;
	Texture2D _bullet;
	
};