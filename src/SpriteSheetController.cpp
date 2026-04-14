#include "SpriteSheetController.h"

SpriteSheetController::SpriteSheetController():_cols(1), _rows(1),_frameDuration(-1), _timeAcum(0),
	 _totalFrames(1), _totalDuration(-1)
	 
{
}

void SpriteSheetController::Configure( const PainterManager* painter, PainterManager::SPRITE_ID sprite, unsigned int cols, unsigned int rows, float frameDuration, bool loop)
{
	_sprite = sprite;
	_cols = cols;
	_rows = rows;
	_frameDuration = frameDuration;
	_timeAcum = 0;
	_totalFrames = cols * rows;
	_totalDuration =cols * rows * frameDuration;
	_fixedFrame = -1;
	_loop = loop;

	if(painter)
	{
		unsigned int w, h;
		painter->GetSpriteSize(_sprite, w,h);
		_frameWidth = (w / _cols);
		_frameHeight = (h/ _rows);
	}
	else
	{
		_frameWidth = -1;
		_frameHeight = -1;
	}
}

void SpriteSheetController::Configure(	const PainterManager* painter, PainterManager::SPRITE_ID sprite, unsigned int cols, unsigned int rows, float frameDuration)
{
	Configure(painter, sprite, cols, rows, frameDuration, false);
}

void SpriteSheetController::Configure(	const PainterManager* painter,  PainterManager::SPRITE_ID sprite)
{
	Configure(painter, sprite, 1, 1, -1, false);
}

void SpriteSheetController::Configure( const PainterManager* painter, const SpriteSheetController* other)
{
	if(other == nullptr)
	{	
		return;
	}
	Configure(painter,other->_sprite, other->_cols, other->_rows, other->_frameDuration, other->_loop);
}


void SpriteSheetController::Reset()
{
	_timeAcum = 0;
}

bool SpriteSheetController::Update(const float deltaTime)
{
	if(_frameDuration <0){return false;}

	_timeAcum += deltaTime;

	if(_timeAcum > _totalDuration)
	{
		if(_loop)
		{
			Reset();
		}
		else
		{
			return true;
		}
	}
	return false;
}

void SpriteSheetController::Paint(PainterManager* painter, float x, float y)const
{
	Paint(painter, x, y, 1.0f);
}

void SpriteSheetController::Paint(PainterManager* painter, float x, float y, float alpha)const
{
	int frameID = _timeAcum / _frameDuration;
	if(_fixedFrame != -1)
	{
		frameID = _fixedFrame;
	}
	PaintFrame(painter, x, y, frameID, alpha);
}
void SpriteSheetController::PaintFrame(PainterManager* painter, float x, float y, int frameId) const
{
	PaintFrame(painter, x, y, frameId, 1.0f);
}
void SpriteSheetController::PaintFrame(PainterManager* painter, float x, float y, int frameId, float alpha) const
{
	int coordX, coordY;
	GetCoordsForFrame(frameId, coordX, coordY);

	painter->AddToPaint(_sprite, x, y, alpha, _frameWidth, _frameHeight, coordX, coordY);
}
void SpriteSheetController::GetCoordsForFrame(int frameId, int& coordX, int& coordY) const
{
	int row = frameId / _cols;
	int col = frameId % _cols;

	coordX = _frameWidth * col;
	coordY = _frameHeight * row;
}

void SpriteSheetController::SetSprite(PainterManager::SPRITE_ID sprite, float width, float height)
{
	_sprite = sprite;
	_frameWidth = width;
	_frameHeight = height;
}

PainterManager::SPRITE_ID SpriteSheetController::GetSprite()const
{
	return _sprite;
}


void SpriteSheetController::GetSize(unsigned int& w, unsigned int& h) const
{
	w = _frameWidth;
	h = _frameHeight;
}

unsigned int SpriteSheetController::GetWidth() const
{
	return _frameWidth;
}
unsigned int SpriteSheetController::GetHeight() const
{
	return _frameHeight;
}

void SpriteSheetController::SetFixedFrame(int value)
{
	_fixedFrame = value;
}
