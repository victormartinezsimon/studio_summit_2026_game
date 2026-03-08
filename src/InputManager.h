#pragma once

class InputManager
{
public:
	InputManager();

public:
	int GetInputValue();
    float GetInputValueNormalized();

private:
    void UpdateFakeController();
    int _fakeControllerValue = 0;
    bool _havekeyboard = false;
    unsigned int fakeIncrease = 10;
};
