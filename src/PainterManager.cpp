#include "PainterManager.h"
#include "Painter.h"
#include "sprites.h"
#include "Painter.h"


PainterManager::PainterManager()
{
	//_sprites.insert({SPRITE_ID::PLAYER, sprite_player});
	_sprites[SPRITE_ID::PLAYER] = sprite_player;
	_painter = new Painter();
}

PainterManager::~PainterManager()
{
	delete _painter;
}

void PainterManager::Paint() const
{
	_painter->BeginPaint();

	_painter->PaintBackground();

	for (auto &&d : _toPaint)
	{
		_painter->PaintItem(_sprites.at(d.id), d.width, d.height, d.x, d.y);
	}

	_painter->EndPaint();
}

void PainterManager::ClearListPaint()
{
	_toPaint.clear();
}

void PainterManager::AddToPaint(SPRITE_ID id, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
	data d;
	d.id = id;
	d.width = width;
	d.height = height;
	d.x = x;
	d.y = y;

	_toPaint.push_back(d);
}