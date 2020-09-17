#pragma once
#include <QObject>
#include <iostream>
#include <windows.h>
#include <xinput.h>

#include "device.hpp"

class Gamepad : public QObject, public Device {
Q_OBJECT
public:
    Gamepad(DWORD id);
    ~Gamepad();

    WORD getInput();
    XINPUT_STATE getState();
    bool isConnected();

    DWORD getID();
    void setID(DWORD id);

signals:
    void buttonPressed(WORD button_code);

private:
    DWORD controllerID;
    XINPUT_STATE state;
};