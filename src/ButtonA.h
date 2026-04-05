#pragma once
#include <functional>
#include <utility>

class ButtonA
{

public:
	ButtonA() {}

public:
	void Update(const float deltaTime, const float currentInputValueNormalized);
	float GetLeftTime() const;

public:
	void SelectInPosition(float duration, std::pair<int, int> optionA, std::pair<int, int> optionB, std::pair<int, int> optionC, std::function<void(int)> callback);
	void SelectInPosition(float duration, std::pair<int, int> optionA, std::pair<int, int> optionB, std::function<void(int)> callback);
	void SelectInPosition(float duration, std::pair<int, int> optionA, std::function<void(int)> callback);

private:
	bool _enabled = false;
	float _duration;
	std::pair<int, int> _optionA;
	std::pair<int, int> _optionB;
	std::pair<int, int> _optionC;
	std::function<void(int)> _callback;
	float _acumTime;
	int _currentSelection = -1;
};