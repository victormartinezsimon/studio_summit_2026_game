#include "SpriteSheetController.h"

SpriteSheetController::SpriteSheetController():_cols(1), _rows(1),_frameDuration(-1), _timeAcum(0),
	 _totalFrames(1), _totalDuration(-1)
	 
{
	
}

void SpriteSheetController::Configure(	const PainterManager* painter, unsigned int cols, unsigned int rows, float frameDuration, PainterManager::SPRITE_ID sprite)
{
	_sprite = sprite;
	_cols = cols;
	_rows = rows;
	_frameDuration = frameDuration;
	_timeAcum = 0;
	_totalFrames = cols * rows;
	_totalDuration =cols * rows * frameDuration;

	float w, h;
	painter->GetSpriteSize(_sprite, w,h);
	_frameWidth = (w / _cols);
	_frameHeight = (h/ _rows);
}


void SpriteSheetController::Reset()
{
	_timeAcum = 0;
}

bool SpriteSheetController::Update(const float deltaTime)
{
	if(_frameDuration <0){return false;}

	_timeAcum += deltaTime;

	return _timeAcum > _totalDuration;

}
void SpriteSheetController::Paint(PainterManager* painter, float x, float y)const
{
	int frameID = _timeAcum / _frameDuration;
	PaintFrame(painter, x, y, frameID);
}
void SpriteSheetController::PaintFrame(PainterManager* painter, float x, float y, int frameId) const
{
	int coordX, coordY;
	GetCoordsForFrame(frameId, coordX, coordY);

	painter->AddToPaint(_sprite, x, y, PainterManager::MASK_ID::FULL, _frameWidth, _frameHeight, coordX, coordY);
}
void SpriteSheetController::GetCoordsForFrame(int frameId, int& coordX, int& coordY) const
{
	int row = frameId / _cols;
	int col = frameId % _cols;

	coordX = _frameWidth * col;
	coordY = _frameHeight * row;
}

void SpriteSheetController::SetSprite(PainterManager::SPRITE_ID sprite)
{
	_sprite = sprite;
}
