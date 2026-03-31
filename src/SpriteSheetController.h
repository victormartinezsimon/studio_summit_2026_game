#pragma once
#include <functional>
#include "PainterManager.h"

class SpriteSheetController 
{
	public:
		SpriteSheetController();

	public:
		bool Update(const float deltaTime);
		void Paint(PainterManager* painter, float x, float y)const;
		void PaintFrame(PainterManager* painter, float x, float y, int frameId)const;
		void GetCoordsForFrame(int frameId, int& coordX, int& coordY)const;
		void Reset();

		void Configure(	const PainterManager* painter, PainterManager::SPRITE_ID sprite, unsigned int cols, unsigned int rows, float frameDuration);
		void Configure(	const PainterManager* painter,  PainterManager::SPRITE_ID sprite);
		void SetSprite(PainterManager::SPRITE_ID sprite, float width, float height);

		void GetSize(float& w, float& h);

	private:
		unsigned int _cols;
		unsigned int _rows;
		float _frameDuration;
		PainterManager::SPRITE_ID _sprite;
		float _timeAcum;
		int _totalFrames;
		float _totalDuration;
		unsigned int _spriteWidth;
		unsigned int _spriteHeight;
		unsigned int _frameWidth;
		unsigned int _frameHeight;
};
