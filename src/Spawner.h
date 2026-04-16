#pragma once
#include "Pool.h"

class PainterManager;

template <typename T, unsigned int N>
class Spawner : public WorldObject
{
public:
	Spawner(float delaySpawn, PainterManager* painterManager)
	: _delaySpawn(delaySpawn)
	{}

	void SetCallbackConfiguration(std::function<void(T& obj)> configureCallback)
	{
		_configureCallback = configureCallback;
	}
	void SetCallbackUpdate(std::function<void(T& obj)> configureCallback)
	{
		_updateCallback = configureCallback;
	}

public:
	void Update(const float deltaTime)
	{
		_timeSpawn += deltaTime;
		if(_timeSpawn > _delaySpawn)
		{
			_timeSpawn = 0;
			int id = _objects.Get();
			if(id != -1)
			{
				_objects.call_for_element(id, [&](T& obj){_configureCallback(obj);});
			}
		}
		_objects.for_each_active([&](T& obj){UpdateObject(deltaTime, obj);});
	}

	void Paint(PainterManager* painter) override
	{
		_objects.for_each_active([&](T& obj)
        {
			obj.Paint(painter);
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

	void ConfigureSprite(PainterManager* painter) override
	{
		_objects.for_each_active([&](T& obj)
        {
			obj.ConfigureSprite(painter);
        });
	}

private:
	void UpdateObject(const float deltaTime, T& obj)
	{
		obj.Update(deltaTime);

		if(obj.OutOfScreen())
		{
			_objects.Release(obj);
		}

		if(_updateCallback != nullptr)
		{
			_updateCallback(obj);
		}
	}

private:
	Pool<T, N> _objects;
	float _delaySpawn;
	float _timeSpawn = 0;
	std::function<void(T& obj)> _configureCallback = nullptr;
	std::function<void(T& obj)> _updateCallback = nullptr;
};
