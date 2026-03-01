#include "TextPainter.h"
#include "Plane.h"
#include "Bullet.h"
#include <iostream>

void TextPainter::Paint()
{
	for (auto&& row : _screen)
	{
		for (auto&& c : row)
		{
			if (c != 0)
			{
				std::cout << c;
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << "\n";
	}
}

void TextPainter::PaintBullet(const Bullet* b)
{
	auto x = b->GetX();
	auto y = b->GetY();
	auto width = b->GetWidth();
	auto height = b->GetHeight();

	for (int row = y; row < y + height; ++row)
	{
		for (int col = x; col < x + width; ++col)
		{
			if (b->GetPlayerTeam())
			{
				_screen[row][col] = 'B';
			}
			else
			{
				_screen[row][col] = 'b';
			}
		}
	}

}
void TextPainter::PaintEnemyPlane(const Plane* p)
{
	auto x = p->GetX();
	auto y = p->GetY();
	auto width = p->GetWidth();
	auto height = p->GetHeight();

	for (int row = y; row < y + height; ++row)
	{
		for (int col = x; col < x + width; ++col)
		{
			if(p->GetPlayerTeam())
			{
				_screen[row][col] = 'P';
			}
			else
			{
				_screen[row][col] = 'p';
			}
		}
	}
}

void TextPainter::ClearScreen()
{
	//magic command to clear the screen and set cursor to top left
	std::cout << "\033[2J\033[H";

	for (auto& row : _screen) {
		std::fill(row.begin(), row.end(), 0);
	}
}
