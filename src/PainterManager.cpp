#include "PainterManager.h"
#include "Painter.h"
#include "Sprites.h"
#include "Sprites_scaled.h"
#include <algorithm>

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
	_sprites[SPRITE_ID::BIG_BULLETS] = sprite_BIG_BULLETS;	
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
	_sprites[SPRITE_ID::DECREASE_LETTER] = sprite_DECREASE_LETTER;
	_sprites[SPRITE_ID::INCREASE_LETTER] = sprite_INCREASE_LETTER;
	_sprites[SPRITE_ID::ACCEPT_LETTER] = sprite_ACCEPT_LETTER;
	_sprites[SPRITE_ID::NUMBERS] = sprite_NUMBERS;
	_sprites[SPRITE_ID::LETTERS] = sprite_LETTERS;


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
	_sizes[SPRITE_ID::BIG_BULLETS] = {BIG_BULLETS_WIDTH, BIG_BULLETS_HEIGHT};
	_sizes[SPRITE_ID::GIVE_SHIELD] = {GIVE_SHIELD_WIDTH, GIVE_SHIELD_HEIGHT};
	_sizes[SPRITE_ID::METEORITE] = {METEORITE_WIDTH, METEORITE_HEIGHT};
	_sizes[SPRITE_ID::EXPLOSION] = {EXPLOSION_WIDTH, EXPLOSION_HEIGHT};
	_sizes[SPRITE_ID::PLAYER_SHIELD] = {SHIELD_PLAYER_WIDTH, SHIELD_PLAYER_HEIGHT};
	_sizes[SPRITE_ID::ENEMY_SHIELD] = {SHIELD_ENEMY_WIDTH, SHIELD_ENEMY_HEIGHT};
	_sizes[SPRITE_ID::NUMBERS] = {NUMBERS_WIDTH, NUMBERS_HEIGHT};
	_sizes[SPRITE_ID::NEAR_STAR] = {NEAR_STAR_WIDTH, NEAR_STAR_HEIGHT};
	_sizes[SPRITE_ID::MID_STAR] = {MID_STAR_WIDTH, MID_STAR_HEIGHT};
	_sizes[SPRITE_ID::FAR_STAR] = {FAR_STAR_WIDTH, FAR_STAR_HEIGHT};
	_sizes[SPRITE_ID::FINAL_SCORE] = {FINAL_SCORE_WIDTH, FINAL_SCORE_HEIGHT};
	_sizes[SPRITE_ID::RETURN_MENU] = {RETURN_WIDTH, RETURN_HEIGHT};
	_sizes[SPRITE_ID::EXIT_GAME] = {EXIT_WIDTH, EXIT_HEIGHT};
	_sizes[SPRITE_ID::LETTERS] = {LETTERS_WIDTH, LETTERS_HEIGHT};
	_sizes[SPRITE_ID::DECREASE_LETTER] = {DECREASE_LETTER_WIDTH, DECREASE_LETTER_HEIGHT};
	_sizes[SPRITE_ID::INCREASE_LETTER] = {INCREASE_LETTER_WIDTH, INCREASE_LETTER_HEIGHT};
	_sizes[SPRITE_ID::ACCEPT_LETTER] = {ACCEPT_LETTER_WIDTH, ACCEPT_LETTER_HEIGHT};

	_painter = new Painter();

	ScaleDown(sprite_BULLET, BULLET_WIDTH, BULLET_HEIGHT, BULLET_SMALL_WIDTH, BULLET_SMALL_HEIGHT, sprite_bullet_small);
	ScaleUp(sprite_BULLET, BULLET_WIDTH, BULLET_HEIGHT, BULLET_BIG_WIDTH, BULLET_BIG_HEIGHT, sprite_bullet_big);
	_sizes[SPRITE_ID::BULLET_BIG] = {BULLET_BIG_WIDTH, BULLET_BIG_HEIGHT};
	_sizes[SPRITE_ID::BULLET_SMALL] = {BULLET_SMALL_WIDTH, BULLET_SMALL_HEIGHT};
	_sprites[SPRITE_ID::BULLET_BIG] = sprite_bullet_big;
	_sprites[SPRITE_ID::BULLET_SMALL] = sprite_bullet_small;

	ScaleUp(sprite_NUMBERS, NUMBERS_WIDTH, NUMBERS_HEIGHT, NUMBERS_BIG_WIDTH, NUMBERS_BIG_HEIGHT, sprite_numbers_big);
	_sizes[SPRITE_ID::NUMBERS_BIG] = {NUMBERS_BIG_WIDTH, NUMBERS_BIG_HEIGHT};
	_sprites[SPRITE_ID::NUMBERS_BIG] = sprite_numbers_big;
}

void PainterManager::GetSpriteSize(SPRITE_ID id, unsigned int& width, unsigned int& height)const
{
	width = _sizes.at(id).first;
	height = _sizes.at(id).second;
}

unsigned int PainterManager::GetWidth(SPRITE_ID id)const
{
	return _sizes.at(id).first;
}
unsigned int PainterManager::GetHeight(SPRITE_ID id)const
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
		unsigned int w, h;
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
	AddToPaint(id, x, y, 1.0, size.first, size.second, 0,0);
}
void PainterManager::AddToPaint(SPRITE_ID id, float x, float y, float alpha)
{
	auto size = _sizes[id];
	AddToPaint(id, x, y, alpha, size.first, size.second, 0,0);
}

void PainterManager::AddToPaint(SPRITE_ID id, float x, float y, float alpha, unsigned int width, unsigned int height, int spriteCoordX, int spriteCoordY)
{
	if(_currentIndexToPaint >= MAX_PAINTED_OBJECTS){return;}
	if(x == -1000){return;}
	if(y == -1000){return;}
	if(alpha < 0.05f){return;}

	_toPaint[_currentIndexToPaint].id = id;
	_toPaint[_currentIndexToPaint].width = width;
	_toPaint[_currentIndexToPaint].height = height;
	_toPaint[_currentIndexToPaint].x = x - width/2;
	_toPaint[_currentIndexToPaint].y = y - height/2;
	_toPaint[_currentIndexToPaint].mask = GetMaskID(alpha);
	_toPaint[_currentIndexToPaint].spriteCoordX = spriteCoordX;
	_toPaint[_currentIndexToPaint].spriteCoordY = spriteCoordY;
	++_currentIndexToPaint;
}

int PainterManager::GetMaskID(float alpha)
{
	int level = (int)(alpha * 10.0f + 0.5f);
	if (level < 1) level = 1;
	if (level > 10) level = 10;
	return level * 10;
}


void PainterManager::ScaleUp(const uint8_t* src, int w, int h, int newW, int newH, uint8_t* dst) 
{

    auto get = [&](int x, int y) -> uint8_t {
        x = std::clamp(x, 0, w - 1);
        y = std::clamp(y, 0, h - 1);
        return src[y * w + x];
    };

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint8_t A = get(x,   y-1);
            uint8_t B = get(x-1, y  );
            uint8_t P = get(x,   y  );
            uint8_t C = get(x+1, y  );
            uint8_t D = get(x,   y+1);

            uint8_t E = (B==A && A!=C && B!=D) ? B : P;
            uint8_t F = (A==C && A!=B && C!=D) ? C : P;
            uint8_t G = (B==D && B!=A && D!=C) ? B : P;
            uint8_t H = (D==C && D!=B && C!=A) ? C : P;

            dst[(y*2  ) * newW + (x*2  )] = E;
            dst[(y*2  ) * newW + (x*2+1)] = F;
            dst[(y*2+1) * newW + (x*2  )] = G;
            dst[(y*2+1) * newW + (x*2+1)] = H;
        }
    }
	
}

void PainterManager::ScaleDown(const uint8_t* src, int w, int h, int newW, int newH, uint8_t* dst) 
{
    for (int y = 0; y < newH; y++)
        for (int x = 0; x < newW; x++)
            dst[y * newW + x] = src[(y * 2) * w + (x * 2)];
}