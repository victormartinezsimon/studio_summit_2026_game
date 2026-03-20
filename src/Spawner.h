#pragma once
#include "Pool.h"
#include "PainterManager.h"

template <typename T, unsigned int N>
class Spawner : public WorldObject
{
public:
	Spawner(float delaySpawn, PainterManager* painterManager)
	: _delaySpawn(delaySpawn), _painterManager(painterManager)
	{}

	void SetCallbackConfiguration(std::function<void(T& obj)> configureCallback)
	{
		_configureCallback = configureCallback;
	}

public:
	void Update(const float deltaTime)
	{
		_timeSpawn += deltaTime;
		if(_timeSpawn > _delaySpawn)
		{
			_timeSpawn = 0;
			int id = _objects.Get();
			_objects.call_for_element(id, [&](T& obj){_configureCallback(obj);});
		}
		_objects.for_each_active([&](T& obj){UpdateObject(deltaTime, obj);});
	}

	void Paint(PainterManager::SPRITE_ID sprite)
	{
		 _objects.for_each_active([&](const T& obj)
        {
            float posX, posY;
            obj.GetPaintPosition(posX, posY);
            _painterManager->AddToPaint(sprite, 
                obj.GetWidth(), obj.GetHeight(), posX, posY);
        });
	}

	void Reset()
	{
		_objects.ReturnAll();
		_timeSpawn = 0;
	}

	void for_each_active(std::function<void(T& obj)> fun)
	{
		_objects.for_each_active([&](T& obj)
		{
			fun(obj);
		});
	}

private:
	void UpdateObject(const float deltaTime, T& obj)
	{
		obj.Update(deltaTime);

		if(OutOfScreen(obj))
		{
			_objects.Release(obj);
		}
	}

	bool OutOfScreen(const T& obj)
	{
		float posX, posY;
        obj.GetPaintPosition(posX, posY);
        if(posX + obj.GetWidth() < 0 && obj.GetMoveLeft())
        {   
            return true;
        }

        if(posX > SCREEN_WIDTH && !obj.GetMoveLeft())
        {   
            return true;
        }

		if (posY < 0 )
		{
			return true;
		}

		if(posY > SCREEN_HEIGHT)
		{
			return true;
		}

		return false;
	}

private:
	Pool<T, N> _objects;
	float _delaySpawn;
	float _timeSpawn = 0;
	std::function<void(T& obj)> _configureCallback = nullptr;
	PainterManager* _painterManager;
};
