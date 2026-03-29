#include "SpriteSheetController.h"

SpriteSheetController::SpriteSheetController(unsigned int cols, unsigned int rows, float frameDuration, 
	PainterManager::SPRITE_ID sprite, unsigned int spriteWidth, unsigned int spriteHeight):_cols(cols), _rows(rows), 
	_sprite(sprite), _frameDuration(frameDuration), _timeAcum(0),
	 _totalFrames(cols * rows), _totalDuration(cols * rows * frameDuration),
	 _spriteWidth(spriteWidth), _spriteHeight(spriteHeight),
	 _frameWidth(spriteWidth / cols), _frameHeight(spriteHeight/ rows)
{
	
}

bool SpriteSheetController::Update(const float deltaTime)
{
	if(_frameDuration <0){return false;}

	_timeAcum += deltaTime;

	return _timeAcum > _totalDuration;

}
void SpriteSheetController::Paint(PainterManager* painter, float x, float y)
{
	int frameID = _timeAcum / _frameDuration;
	PaintFrame(painter, x, y, frameID);
}
void SpriteSheetController::PaintFrame(PainterManager* painter, float x, float y, int frameId)
{
	int coordX, coordY;
	GetCoordsForFrame(frameId, coordX, coordY);

	float w, h;
	painter->GetSpriteSize(_sprite, w, h);

	painter->AddToPaint(_sprite, x, y, PainterManager::MASK_ID::FULL, _frameWidth, _frameHeight, coordX, coordY);
}
void SpriteSheetController::GetCoordsForFrame(int frameId, int& coordX, int& coordY)
{
	int row = frameId / _rows;
	int col = frameId % _cols;

	coordX = _frameWidth * col;
	coordY = _frameHeight * row;
}

