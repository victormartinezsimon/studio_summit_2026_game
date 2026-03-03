#include "RaylibPainter.h"
#include "Plane.h"
#include "Bullet.h"

RaylibPainter::RaylibPainter() //:  _player = LoadTexture(std::string(PLANE_IMAGE)), _background(std::string(BACKGROUND_IMAGE)), _enemy(std::string(ENEMY_IMAGE)), _bullet(std::string(BULLET_IMAGE))
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SUMMIT_2026");

	_player = LoadTexture(PLANE_IMAGE.data());
	_background = LoadTexture(BACKGROUND_IMAGE.data());
	_enemy = LoadTexture(ENEMY_IMAGE.data());
	_bullet = LoadTexture(BULLET_IMAGE.data());
}

RaylibPainter::~RaylibPainter()
{
	UnloadTexture(_player);        // Texture unloading
	UnloadTexture(_background);        // Texture unloading
	UnloadTexture(_enemy);        // Texture unloading
	UnloadTexture(_bullet);        // Texture unloading
	CloseWindow();                // Close window and OpenGL context
}

void RaylibPainter::PaintBackground()
{
	DrawTexture(_background, 0, 0, RAYWHITE);
}

void RaylibPainter::BeginPaint()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
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
	return WindowShouldClose();
}

void RaylibPainter::PaintPlayer(Plane* player)
{
	PaintPlane(player, _player);
}
void RaylibPainter::PaintEnemy(Plane* enemy)
{
	PaintPlane(enemy, _enemy);
}

void RaylibPainter::PaintPlane(Plane* plane, Texture2D& texture)
{
	int x = plane->GetX() - plane->GetWidth()/2;
	int y = plane->GetY() - plane->GetHeight()/2;

	DrawTexture(texture, x, y, RAYWHITE);
}

void RaylibPainter::PaintBullet(Bullet* bullet)
{
	int x = bullet->GetX() - bullet->GetWidth() / 2;
	int y = bullet->GetY() - bullet->GetHeight() / 2;

	DrawTexture(_bullet, x, y, RAYWHITE);
}