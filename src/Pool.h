#pragma once
#include <array>
#include <algorithm>
#include <stdexcept>
#include <functional>

#include "GameConfig.h"

class PainterManager;

template <typename T, unsigned int N>
class Pool
{
public:
	Pool()
	{
		for(std::size_t i = 0; i< N; ++i)
		{
			_poolElements[i].SetID(i);
			_used[i] = false;
		}

	}
	Pool(const Pool&) = delete;
	Pool& operator=(const Pool&) = delete;
	Pool(Pool&&) = delete;
	Pool& operator=(Pool&&) = delete;
public:

	int Get()
	{
		for(int i = 0; i< N; ++i)
		{
			if(!_used[i])
			{
				_used[i] = true;
				++_currentUse;

				if( i > _lastInUse)
				{
					_lastInUse = i;
				}

				return i;
			}
		}
		return -1;
	}

	void Release(const T& elem)
	{
		int index = elem.GetID();
		Release(index);
	}

	void Release(int index)
	{
		if(index == -1)
		{
			return;
		}
		_used[index] = false;
		--_currentUse;

		while(_lastInUse >= 0 && !_used[_lastInUse] )
		{
			--_lastInUse;
		}
	}

	void ReturnAll()
	{
		_used.fill(false);
		_currentUse = 0;
		_lastInUse = -1;
		for(T elem: _poolElements)
		{
			elem.SetPositionX(-1000);
			elem.SetPositionY(-1000);
		}
	}

	void for_each_active(std::function<void(T&)> func)
	{
		for(int i = 0; i <= _lastInUse; ++i)
		{
			if(!_used[i]){continue;}
			func(_poolElements[i]);
		}
	}	

	void call_for_element(int id, std::function<void(T&)> func) 
	{
		if(id == -1){return;}
		if(id > _poolElements.size())
		{
			return;
		}
		func(_poolElements[id]);
	}		

	int TotalInUse() const
	{
		return _currentUse;
	}

	void Paint(PainterManager* painter)
	{
		for(int i = 0; i <= _lastInUse; ++i)
		{
			if(!_used[i]){continue;}
			_poolElements[i].Paint(painter);
		}
	}

	T* GetElement(int id)
	{
		if(id < 0 || id >= N)
		{
			return nullptr;
		}
		if(!_used[id]){return nullptr;}
		return &_poolElements[id];
	}

private:
	std::array<T,N> _poolElements; 
	std::array<bool,N> _used; 
	int _currentUse = 0;
	int _lastInUse = -1;
};
