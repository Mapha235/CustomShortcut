#include "gamepad.hpp"

Gamepad::Gamepad(DWORD id, XINPUT_STATE s)
    : controllerID(id)
    , state(s)
{
    // Fills block of memory with zeros
    // ZeroMemory(&state, sizeof(XINPUT_STATE));
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

void Gamepad::vibrate(int MotorSpeed)
{
    _XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = MotorSpeed;
    vibration.wRightMotorSpeed = MotorSpeed;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::cout << controllerID << std::endl;

    XInputSetState(controllerID, &vibration);
    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() != 300) {
        end = std::chrono::system_clock::now();
        // std::cout << "Time is" << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " second" << std::endl;
    }
    vibration.wLeftMotorSpeed = 0;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(controllerID, &vibration);
}

#include "moc_gamepad.cpp"