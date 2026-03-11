#pragma once
#include <functional>
#include <utility>

class ButtonA
{

public:
	ButtonA(){}
public:
	void Update(const float deltaTime, const float currentInputValueNormalized, const float currentInputValue);
private:
	void UpdateSelectInPosition(const float deltaTime,const float currentInputValueNormalized, const float currentInputValue);
	void UpdateSelectAfterTime(const float deltaTime,const float currentInputValueNormalized, const float currentInputValue);

public:
	void SelectInPosition(float duration, std::pair<int, int> marginA, std::function<void(int)> callback);
	void SelectAfterTime(float duration, std::function<void(int)> callback);

private:
	bool _enabled = false;
	bool _isSelectInPosition = false;
	float _duration;
	std::pair<int, int> _marginA;
	std::function<void(int)> _callback;
	float _acumTime;
};