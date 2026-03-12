#pragma once
#include <array>
#include <algorithm>
#include <stdexcept>
#include <functional>

#include "GameConfig.h"

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
		for(std::size_t i = 0; i< N; ++i)
		{
			if(!_used[i])
			{
				_used[i] = true;
				++_currentUse;
				return i;
			}
		}
		return -1;
	}

	void Release(const T& elem)
	{
		int index = elem.GetID();
		_used[index] = false;
		--_currentUse;
	}

	void ReturnAll()
	{
		_used.fill(false);
		_currentUse = 0;

	}

	void for_each_active(std::function<void(T&)> func) 
	{
		for(int i = 0; i < N; ++i)
		{
			if(!_used[i]){continue;}
			func(_poolElements[i]);
		}
	}	

	void call_for_element(int id, std::function<void(T&)> func) 
	{
		if(id == -1){return;}
		func(_poolElements[id]);
	}	

	int TotalInUse() const
	{
		return _currentUse;
	}

private:
	std::array<T,N> _poolElements; 
	std::array<bool,N> _used; 
	int _currentUse = 0;
};
