#include "PainterManager.h"
#include "Painter.h"
#include "sprites.h"

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
	_sprites[SPRITE_ID::FAST_SHOTS] = sprite_FAST_BULLETS;
	_sprites[SPRITE_ID::SLOW_SHOTS] = sprite_SLOW;	
	_sprites[SPRITE_ID::GIVE_SHIELD] = sprite_GIVE_SHIELD;	
	_sprites[SPRITE_ID::METEORITE] = sprite_METEORITE;	
	_sprites[SPRITE_ID::EXPLOSION] = sprite_EXPLOSION;
	_sprites[SPRITE_ID::PLAYER_SHIELD] = sprite_SHIELD_PLAYER;
	_sprites[SPRITE_ID::ENEMY_SHIELD] = sprite_SHIELD_ENEMY;
	_sprites[SPRITE_ID::NEAR_STAR] = sprite_NEAR_STAR;
	_sprites[SPRITE_ID::MID_STAR] = sprite_MID_STAR;
	_sprites[SPRITE_ID::FAR_STAR] = sprite_FAR_STAR;
	_sprites[SPRITE_ID::FINAL_SCORE] = sprite_FINAL_SCORE;
	_sprites[SPRITE_ID::RETURN_MENU] = sprite_RETURN;
	_sprites[SPRITE_ID::EXIT_GAME] = sprite_EXIT;


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

	_sprites[SPRITE_ID::LETTERS] = sprite_LETTERS;

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
	_sizes[SPRITE_ID::FAST_SHOTS] = {FAST_BULLETS_WIDTH, FAST_BULLETS_HEIGHT};
	_sizes[SPRITE_ID::SLOW_SHOTS] = {SLOW_WIDTH, SLOW_HEIGHT};
	_sizes[SPRITE_ID::GIVE_SHIELD] = {GIVE_SHIELD_WIDTH, GIVE_SHIELD_HEIGHT};

	_sizes[SPRITE_ID::METEORITE] = {METEORITE_WIDTH, METEORITE_HEIGHT};
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
	
	_sizes[SPRITE_ID::NEAR_STAR] = {NEAR_STAR_WIDTH, NEAR_STAR_HEIGHT};
	_sizes[SPRITE_ID::MID_STAR] = {MID_STAR_WIDTH, MID_STAR_HEIGHT};
	_sizes[SPRITE_ID::FAR_STAR] = {FAR_STAR_WIDTH, FAR_STAR_HEIGHT};

	_sizes[SPRITE_ID::FINAL_SCORE] = {FINAL_SCORE_WIDTH, FINAL_SCORE_HEIGHT};
	_sizes[SPRITE_ID::RETURN_MENU] = {RETURN_WIDTH, RETURN_HEIGHT};
	_sizes[SPRITE_ID::EXIT_GAME] = {EXIT_WIDTH, EXIT_HEIGHT};

	_sizes[SPRITE_ID::LETTERS] = {LETTERS_WIDTH, LETTERS_HEIGHT};

}

void PainterManager::GetSpriteSize(SPRITE_ID id, float& width, float& height)const
{
	width = _sizes.at(id).first;
	height = _sizes.at(id).second;
}

float PainterManager::GetWidth(SPRITE_ID id)const
{
	return _sizes.at(id).first;
}
float PainterManager::GetHeight(SPRITE_ID id)const
{
	return _sizes.at(id).second;
}

PainterManager::~PainterManager()
{
	delete _painter;
}

void PainterManager::Paint() const
{
	_painter->BeginPaint();

	_painter->PaintBackground();

	for(int index = _currentIndexToPaint-1; index >= 0; --index)
	{
		float w, h;
		GetSpriteSize(_toPaint[index].id, w, h);
		_painter->PaintItem(_sprites.at(_toPaint[index].id), _toPaint[index].width, _toPaint[index].height, _toPaint[index].x, _toPaint[index].y, 
		_toPaint[index].mask, _toPaint[index].spriteCoordX, _toPaint[index].spriteCoordY,
		w, h );
	}

	_painter->EndPaint();
}

void PainterManager::ClearListPaint()
{
	_currentIndexToPaint = 0;
}

void PainterManager::AddToPaint(SPRITE_ID id, float x, float y)
{
	auto size = _sizes[id];
	AddToPaint(id, x, y, MASK_ID::FULL, size.first, size.second);
}
void PainterManager::AddToPaint(SPRITE_ID id, float x, float y, MASK_ID mask)
{
	auto size = _sizes[id];
	AddToPaint(id, x, y, mask, size.first, size.second);
}

void PainterManager::AddToPaint(SPRITE_ID id, float x, float y, unsigned int width, unsigned int height)
{
	AddToPaint(id, x, y, MASK_ID::FULL, width, height);
}

void PainterManager::AddToPaint(SPRITE_ID id, float x, float y, MASK_ID mask, unsigned int width, unsigned int height)
{
	AddToPaint(id, x, y, mask, width, height, 0,0);
}

void PainterManager::AddToPaint(SPRITE_ID id, float x, float y, MASK_ID mask, unsigned int width, unsigned int height, int spriteCoordX, int spriteCoordY)
{
	if(_currentIndexToPaint < MAX_PAINTED_OBJECTS)
	{
		_toPaint[_currentIndexToPaint].id = id;
		_toPaint[_currentIndexToPaint].width = width;
		_toPaint[_currentIndexToPaint].height = height;
		_toPaint[_currentIndexToPaint].x = x - width/2;
		_toPaint[_currentIndexToPaint].y = y - height/2;
		_toPaint[_currentIndexToPaint].mask = GetMaskID(mask);
		_toPaint[_currentIndexToPaint].spriteCoordX = spriteCoordX;
		_toPaint[_currentIndexToPaint].spriteCoordY = spriteCoordY;
		++_currentIndexToPaint;
	}
}

int PainterManager::GetMaskID(MASK_ID maskID)
{
	switch (maskID)
	{
		case MASK_ID::FULL: return 0;
		case MASK_ID::HALF: return 1;
		case MASK_ID::QUARTER: return 2;
	}
	return 0;
}
