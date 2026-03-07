#include "InputManager.h"

#include "controller.h"
#include "controller.c"

#include <stdexcept>

#include <poll.h>
#include <linux/input.h>

#include "platform.h"

static struct pollfd fds[2];

InputManager::InputManager()
{
    if( controller_open( CONTROLLER_MODE_AUTO ) != CONTROLLER_OK )
    {
        throw std::runtime_error("Cant open controller");
    }
    _fakeControllerValue = 0;

    // Set up keyboard device
	fds[0].fd = SPFindKeyboardDevice();
	fds[0].events = POLLIN;
	
	if (fds[0].fd < 0)
	{
		//printf("Could not find keyboard device. Make sure a keyboard is connected.\n");
		_havekeyboard = false;
	}
}

int InputManager::GetInputValue()
{
    UpdateFakeController();
    
    int     r, val;
    r = controller_read( &val );
    if( r == CONTROLLER_OK )
    {
        return val;
    }
    return _fakeControllerValue;
}
float InputManager::GetInputValueNormalized()
{
    int value = GetInputValue();
    return value / 255.0;
}

void InputManager::UpdateFakeController()
{
    if (_havekeyboard)
    {
        int ret = poll(&fds[0], 1, 0);
        if (ret > 0)
        {
            struct input_event ev;
            if (read(fds[0].fd, &ev, sizeof(struct input_event)) == sizeof(struct input_event))
            {
                if (ev.type == EV_KEY && ev.value == 1)  // Key press
                {
                    if (ev.code == KEY_LEFT)
                    {
                        _fakeControllerValue-= 10;
                    }
                    else
                    {
                        if(ev.code == KEY_RIGHT)
                        {
                            _fakeControllerValue += 10;
                        }
                    }
                }
            }
        }

        _fakeControllerValue = std::max(0, _fakeControllerValue);
        _fakeControllerValue = std::min(255, _fakeControllerValue);
    }
}