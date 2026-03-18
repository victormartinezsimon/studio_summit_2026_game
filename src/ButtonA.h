#pragma once
#include <functional>
#include <utility>

class ButtonA
{

public:
	ButtonA(){}
public:
	void Update(const float deltaTime, const float currentInputValueNormalized, const float currentInputValue);
	float GetLeftTime() const;

public:
	void SelectInPosition(float duration, std::pair<int, int> optionA,std::pair<int, int> optionB, std::function<void(int)> callback);
	void SelectInPosition(float duration, std::pair<int, int> optionA, std::function<void(int)> callback);

private:
	bool _enabled = false;
	float _duration;
	std::pair<int, int> _optionA;
	std::pair<int, int> _optionB;
	std::function<void(int)> _callback;
	float _acumTime;
};