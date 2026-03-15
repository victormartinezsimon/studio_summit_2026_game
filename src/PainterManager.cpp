#include "PainterManager.h"
#include "Painter.h"
#include "sprites.h"
#include "Painter.h"

PainterManager::PainterManager()
{
	_sprites[SPRITE_ID::PLAYER] = sprite_PLAYER;
	_sprites[SPRITE_ID::ENEMY] = sprite_ENEMY;
	_sprites[SPRITE_ID::BULLET] = sprite_BULLET;
	_sprites[SPRITE_ID::TITLE] = sprite_void;
	_sprites[SPRITE_ID::TITLE_PLAYER_POSITION] = sprite_MAIN_MENU_POSITION;
	_sprites[SPRITE_ID::START_GAME] = sprite_START_GAME;
	_sprites[SPRITE_ID::IMPROVEMENT_SELECTOR_PLAYER] = sprite_PLAYER_SELECTION;
	_sprites[SPRITE_ID::IMPROVEMENT_SELECTOR_ENEMY] = sprite_ENEMY_SELECTOR;
	_sprites[SPRITE_ID::SHOT_3_TIMES] = sprite_SHOT_3_TIMES;
	_sprites[SPRITE_ID::INCREASE_ORIGIN] = sprite_MORE_SOURCE;
	_sprites[SPRITE_ID::INCRASE_FIRE_RATE] = sprite_MORE_FIRERATE;
	_sprites[SPRITE_ID::GIVE_PENETRATION] = sprite_PENETRATION;
	_sprites[SPRITE_ID::GIVE_EXPLOSION] = sprite_EXPLOSION;
	_sprites[SPRITE_ID::FAST_SHOTS] = sprite_FAST_SHOT;
	_sprites[SPRITE_ID::SLOW_SHOTS] = sprite_SLOW;	
	_sprites[SPRITE_ID::GIVE_SHIELD] = sprite_SHIELD;	
	_painter = new Painter();

	_sizes[SPRITE_ID::PLAYER] = {PLAYER_WIDTH, PLAYER_HEIGHT};
	_sizes[SPRITE_ID::ENEMY] = {ENEMY_WIDTH, ENEMY_HEIGHT};
	_sizes[SPRITE_ID::BULLET] = {BULLET_WIDTH, BULLET_HEIGHT};
	_sizes[SPRITE_ID::TITLE] = {TITLE_WIDTH, TITLE_HEIGHT};
	_sizes[SPRITE_ID::TITLE_PLAYER_POSITION] = {MAIN_MENU_POSITION_WIDTH, MAIN_MENU_POSITION_HEIGHT};
	_sizes[SPRITE_ID::START_GAME] = {START_GAME_WIDTH, START_GAME_HEIGHT};
	_sizes[SPRITE_ID::IMPROVEMENT_SELECTOR_PLAYER] = {PLAYER_SELECTION_WIDTH, PLAYER_SELECTION_HEIGHT};
	_sizes[SPRITE_ID::IMPROVEMENT_SELECTOR_ENEMY] = {ENEMY_SELECTOR_WIDTH, ENEMY_SELECTOR_HEIGHT};

	_sizes[SPRITE_ID::SHOT_3_TIMES] = {SHOT_3_TIMES_WIDTH, SHOT_3_TIMES_HEIGHT};
	_sizes[SPRITE_ID::INCREASE_ORIGIN] = {MORE_SOURCE_WIDTH, MORE_SOURCE_HEIGHT};
	_sizes[SPRITE_ID::INCRASE_FIRE_RATE] = {MORE_FIRERATE_WIDTH, MORE_FIRERATE_HEIGHT};
	_sizes[SPRITE_ID::GIVE_PENETRATION] = {PENETRATION_WIDTH, PENETRATION_HEIGHT};
	_sizes[SPRITE_ID::GIVE_EXPLOSION] = {EXPLOSION_WIDTH, EXPLOSION_HEIGHT};
	_sizes[SPRITE_ID::FAST_SHOTS] = {FAST_SHOT_WIDTH, FAST_SHOT_HEIGHT};
	_sizes[SPRITE_ID::SLOW_SHOTS] = {SLOW_WIDTH, SLOW_HEIGHT};
	_sizes[SPRITE_ID::GIVE_SHIELD] = {SHIELD_WIDTH, SHIELD_HEIGHT};
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
		if(d.id == SPRITE_ID::TITLE)
		{
			_painter->PaintItem(sprite_TITLE, d.width, d.height, d.x, d.y);
		}
		else
		{
			_painter->PaintItem(_sprites.at(d.id), d.width, d.height, d.x, d.y);
		}
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

void PainterManager::AddUIToPaint(SPRITE_ID id, unsigned int x, unsigned int y)
{
	auto width = _sizes[id].first;
	auto height = _sizes[id].second;
	AddToPaint(id, width, height, x - width/2, y - height /2);
}