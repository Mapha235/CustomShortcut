#include "gamepad.hpp"

Gamepad::Gamepad(DWORD id)
    : controllerID(id)
{
    // Fills block of memory with zeros
    ZeroMemory(&state, sizeof(XINPUT_STATE));
}

Gamepad::~Gamepad() { }

DWORD Gamepad::getID()
{
    return controllerID;
}

void Gamepad::setID(DWORD id)
{
    controllerID = id;
}

WORD Gamepad::getInput()
{
    auto button = state.Gamepad.wButtons;
    if (button != 0) {
        emit buttonPressed(button);
        return button;
    }
    return 0;
}

XINPUT_STATE Gamepad::getState()
{
    return state;
}

bool Gamepad::isConnected()
{
    return XInputGetState(controllerID, &state) == ERROR_SUCCESS;
}

#include "moc_gamepad.cpp"