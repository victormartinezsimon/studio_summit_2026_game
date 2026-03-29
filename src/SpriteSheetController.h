#pragma once
#include <functional>
#include "PainterManager.h"

class SpriteSheetController 
{
	public:
		SpriteSheetController(unsigned int cols, unsigned int rows, float frameDuration, PainterManager::SPRITE_ID sprite,
		unsigned int spriteWidth, unsigned int spriteHeight);

	public:
		bool Update(const float deltaTime);
		void Paint(PainterManager* painter, float x, float y);
		void PaintFrame(PainterManager* painter, float x, float y, int frameId);
		void GetCoordsForFrame(int frameId, int& coordX, int& coordY);


	private:
		unsigned int _cols;
		unsigned int _rows;
		float _frameDuration;
		PainterManager::SPRITE_ID _sprite;
		float _timeAcum;
		int _totalFrames;
		int _totalDuration;
		unsigned int _spriteWidth;
		unsigned int _spriteHeight;
		unsigned int _frameWidth;
		unsigned int _frameHeight;
};
