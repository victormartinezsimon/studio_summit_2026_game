#pragma once
#include <vector>
#include <map>
#include <stdint.h>//uing8_t

class Painter;

class PainterManager
{
public:
	enum class SPRITE_ID{PLAYER, ENEMY, BULLET, 
						TITLE, START_GAME,
						PLAYER_SELECTOR, ENEMY_SELECTOR,
						SHOT_3_TIMES,INCREASE_ORIGIN,INCRASE_FIRE_RATE,GIVE_PENETRATION,
						GIVE_EXPLOSION,GIVE_SHIELD,FAST_SHOTS,SLOW_SHOTS,
						NUMBER_0,NUMBER_1,NUMBER_2,NUMBER_3,NUMBER_4,NUMBER_5,NUMBER_6,
						NUMBER_7,NUMBER_8,NUMBER_9,
						METEORITE
					};

private:
	struct data
	{
		SPRITE_ID id;
		unsigned int width;
		unsigned int height;
		unsigned int x;
		unsigned int y;
		unsigned int mask;
	};
public:
	PainterManager();
	~PainterManager();
public:
	void Paint() const;

public:
	void ClearListPaint();
	void AddToPaint(SPRITE_ID id, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
	void AddUIToPaint(SPRITE_ID id, unsigned int x, unsigned int y);

	void AddToPaintWithAlpha(SPRITE_ID id, unsigned int width, unsigned int height, unsigned int x, unsigned int y, int maskID);
	void AddUIToPaintWithAlpha(SPRITE_ID id, unsigned int x, unsigned int y, int maskID);


private:
	Painter* _painter;
	std::vector<data> _toPaint;
	std::map<SPRITE_ID, const uint8_t*> _sprites;
	std::map<SPRITE_ID, std::pair<int, int>> _sizes;
};