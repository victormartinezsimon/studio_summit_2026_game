#include "PainterManager.h"
#include "Painter.h"
#include "sprites.h"
#include "Painter.h"

PainterManager::PainterManager()
{
	_sprites[SPRITE_ID::PLAYER] = sprite_PLAYER;
	_sprites[SPRITE_ID::ENEMY] = sprite_ENEMY;
	_sprites[SPRITE_ID::BULLET] = sprite_BULLET;
	_sprites[SPRITE_ID::TITLE] = sprite_TITLE;
	_sprites[SPRITE_ID::PLAYER_SELECTOR] = sprite_PLAYER_SELECTOR;
	_sprites[SPRITE_ID::START_GAME] = sprite_START_GAME;
	_sprites[SPRITE_ID::ENEMY_SELECTOR] = sprite_ENEMY_SELECTOR;
	_sprites[SPRITE_ID::SHOT_3_TIMES] = sprite_SHOT_3_TIMES;
	_sprites[SPRITE_ID::INCREASE_ORIGIN] = sprite_MORE_SOURCE;
	_sprites[SPRITE_ID::INCRASE_FIRE_RATE] = sprite_MORE_FIRERATE;
	_sprites[SPRITE_ID::GIVE_PENETRATION] = sprite_PENETRATION;
	_sprites[SPRITE_ID::GIVE_EXPLOSION] = sprite_EXPLOSIVE_BULLETS;
	_sprites[SPRITE_ID::FAST_SHOTS] = sprite_FAST_SHOT;
	_sprites[SPRITE_ID::SLOW_SHOTS] = sprite_SLOW;	
	_sprites[SPRITE_ID::GIVE_SHIELD] = sprite_GIVE_SHIELD;	
	_sprites[SPRITE_ID::METEORITE] = sprite_METERORITE;	
	_sprites[SPRITE_ID::EXPLOSION] = sprite_EXPLOSION;
	_sprites[SPRITE_ID::PLAYER_SHIELD] = sprite_SHIELD_PLAYER;
	_sprites[SPRITE_ID::ENEMY_SHIELD] = sprite_SHIELD_ENEMY;


	_sprites[SPRITE_ID::NUMBER_0] = sprite_NUMBER_0;
	_sprites[SPRITE_ID::NUMBER_1] = sprite_NUMBER_1;
	_sprites[SPRITE_ID::NUMBER_2] = sprite_NUMBER_2;
	_sprites[SPRITE_ID::NUMBER_3] = sprite_NUMBER_3;
	_sprites[SPRITE_ID::NUMBER_4] = sprite_NUMBER_4;
	_sprites[SPRITE_ID::NUMBER_5] = sprite_NUMBER_5;
	_sprites[SPRITE_ID::NUMBER_6] = sprite_NUMBER_6;
	_sprites[SPRITE_ID::NUMBER_7] = sprite_NUMBER_7;
	_sprites[SPRITE_ID::NUMBER_8] = sprite_NUMBER_8;
	_sprites[SPRITE_ID::NUMBER_9] = sprite_NUMBER_9;

	_painter = new Painter();

	_sizes[SPRITE_ID::PLAYER] = {PLAYER_WIDTH, PLAYER_HEIGHT};
	_sizes[SPRITE_ID::ENEMY] = {ENEMY_WIDTH, ENEMY_HEIGHT};
	_sizes[SPRITE_ID::BULLET] = {BULLET_WIDTH, BULLET_HEIGHT};
	_sizes[SPRITE_ID::TITLE] = {TITLE_WIDTH, TITLE_HEIGHT};
	_sizes[SPRITE_ID::PLAYER_SELECTOR] = {PLAYER_SELECTOR_WIDTH, PLAYER_SELECTOR_HEIGHT};
	_sizes[SPRITE_ID::START_GAME] = {START_GAME_WIDTH, START_GAME_HEIGHT};
	_sizes[SPRITE_ID::ENEMY_SELECTOR] = {ENEMY_SELECTOR_WIDTH, ENEMY_SELECTOR_HEIGHT};

	_sizes[SPRITE_ID::SHOT_3_TIMES] = {SHOT_3_TIMES_WIDTH, SHOT_3_TIMES_HEIGHT};
	_sizes[SPRITE_ID::INCREASE_ORIGIN] = {MORE_SOURCE_WIDTH, MORE_SOURCE_HEIGHT};
	_sizes[SPRITE_ID::INCRASE_FIRE_RATE] = {MORE_FIRERATE_WIDTH, MORE_FIRERATE_HEIGHT};
	_sizes[SPRITE_ID::GIVE_PENETRATION] = {PENETRATION_WIDTH, PENETRATION_HEIGHT};
	_sizes[SPRITE_ID::GIVE_EXPLOSION] = {EXPLOSIVE_BULLETS_WIDTH, EXPLOSIVE_BULLETS_HEIGHT};
	_sizes[SPRITE_ID::FAST_SHOTS] = {FAST_SHOT_WIDTH, FAST_SHOT_HEIGHT};
	_sizes[SPRITE_ID::SLOW_SHOTS] = {SLOW_WIDTH, SLOW_HEIGHT};
	_sizes[SPRITE_ID::GIVE_SHIELD] = {GIVE_SHIELD_WIDTH, GIVE_SHIELD_HEIGHT};

	_sizes[SPRITE_ID::METEORITE] = {METERORITE_WIDTH, METERORITE_HEIGHT};
	_sizes[SPRITE_ID::EXPLOSION] = {EXPLOSION_WIDTH, EXPLOSION_HEIGHT};
	_sizes[SPRITE_ID::PLAYER_SHIELD] = {SHIELD_PLAYER_WIDTH, SHIELD_PLAYER_HEIGHT};
	_sizes[SPRITE_ID::ENEMY_SHIELD] = {SHIELD_ENEMY_WIDTH, SHIELD_ENEMY_HEIGHT};

	_sizes[SPRITE_ID::NUMBER_0] = {NUMBER_0_WIDTH, NUMBER_0_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_1] = {NUMBER_1_WIDTH, NUMBER_1_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_2] = {NUMBER_2_WIDTH, NUMBER_2_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_3] = {NUMBER_3_WIDTH, NUMBER_3_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_4] = {NUMBER_4_WIDTH, NUMBER_4_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_5] = {NUMBER_5_WIDTH, NUMBER_5_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_6] = {NUMBER_6_WIDTH, NUMBER_6_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_7] = {NUMBER_7_WIDTH, NUMBER_7_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_8] = {NUMBER_8_WIDTH, NUMBER_8_HEIGHT};
	_sizes[SPRITE_ID::NUMBER_9] = {NUMBER_9_WIDTH, NUMBER_9_HEIGHT};
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
		if(d.mask != -1)
		{
			_painter->PaintItem(_sprites.at(d.id), d.width, d.height, d.x, d.y, d.mask);
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

void PainterManager::AddToPaint(SPRITE_ID id, unsigned int width, unsigned int height, int x, int y)
{
	data d;
	d.id = id;
	d.width = width;
	d.height = height;
	d.x = x;
	d.y = y;
	d.mask = -1;

	_toPaint.push_back(d);
}

void PainterManager::AddUIToPaint(SPRITE_ID id, int x, int y)
{
	auto width = _sizes[id].first;
	auto height = _sizes[id].second;
	AddToPaint(id, width, height, x - width/2, y - height /2);
}

void PainterManager::AddToPaintWithAlpha(SPRITE_ID id, unsigned int width, unsigned int height, int x, int y, int maskID)
{
	data d;
	d.id = id;
	d.width = width;
	d.height = height;
	d.x = x;
	d.y = y;
	d.mask = maskID;

	_toPaint.push_back(d);
}
void PainterManager::AddUIToPaintWithAlpha(SPRITE_ID id, int x, int y, int maskID)
{
	auto width = _sizes[id].first;
	auto height = _sizes[id].second;
	AddToPaintWithAlpha(id, width, height, x - width/2, y - height /2, maskID);
}