#include "RaylibPainter.h"
#include "Plane.h"
#include "Bullet.h"

RaylibPainter::RaylibPainter() : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "SUMMIT_2026"), _player(std::string(PLANE_IMAGE)), _background(std::string(BACKGROUND_IMAGE)), _enemy(std::string(ENEMY_IMAGE)), _bullet(std::string(BULLET_IMAGE))
{
}

void RaylibPainter::PaintBackground()
{
	_background.Draw(0, 0);
}

void RaylibPainter::BeginPaint()
{
	BeginDrawing();
	_window.ClearBackground(RAYWHITE);
}

void RaylibPainter::EndPaint()
{
	EndDrawing();
}

float RaylibPainter::GetDeltaTime()
{
	return GetFrameTime();
}


bool RaylibPainter::HasEnded()
{
	return _window.ShouldClose();
}

void RaylibPainter::PaintPlayer(Plane* player)
{
	PaintPlane(player, _player);
}
void RaylibPainter::PaintEnemy(Plane* enemy)
{
	PaintPlane(enemy, _enemy);
}

void RaylibPainter::PaintPlane(Plane* plane, raylib::Texture& texture)
{
	int x = plane->GetX() - plane->GetWidth()/2;
	int y = plane->GetY() - plane->GetHeight()/2;

	texture.Draw(x, y);
}

void RaylibPainter::PaintBullet(Bullet* bullet)
{
	int x = bullet->GetX() - bullet->GetWidth() / 2;
	int y = bullet->GetY() - bullet->GetHeight() / 2;

	_bullet.Draw(x, y);
}