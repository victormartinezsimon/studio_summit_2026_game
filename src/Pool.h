#pragma once
#include <list>

#include "GameConfig.h"

template <typename T, unsigned int N>
class Pool
{
public:
	Pool()
	{
		for (std::size_t i = 0; i < N; ++i) {
			T* obj = new T();
			free_.push_back(obj);
			all_.push_back(obj);
		}
	}
	Pool(const Pool&) = delete;
	Pool& operator=(const Pool&) = delete;
	Pool(Pool&&) = delete;
	Pool& operator=(Pool&&) = delete;

	~Pool() {
		for (T* obj : all_) {
			delete obj;
		}
	}

public:
	T* Get()
	{
		if (free_.empty()) {
			throw std::runtime_error("ObjectPool agotado");
		}

		T* obj = free_.front();
		free_.pop_front();
		used_.push_back(obj);
		return obj;
	}
	void Release(T* obj)
	{
		auto it = std::find(used_.begin(), used_.end(), obj);
		if (it == used_.end()) {
			throw std::runtime_error("El objeto no pertenece al pool");
		}

		used_.erase(it);
		free_.push_back(obj);
	}

	

private:
	std::list<T*> free_;
	std::list<T*> used_;
	std::list<T*> all_; 
};
