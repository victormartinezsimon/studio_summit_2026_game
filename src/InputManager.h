#pragma once

class InputManager
{
public:
	InputManager();

public:
	int GetInputValue();
    float GetInputValueNormalized();
    float NormalizeValue(int value)const;

private:
    void UpdateFakeController();
    int _fakeControllerValue = 0;
    bool _havekeyboard = false;
    unsigned int fakeIncrease = 10;
};
