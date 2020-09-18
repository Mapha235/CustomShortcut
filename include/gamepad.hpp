#pragma once
#include <QObject>
#include <iostream>
#include <windows.h>
#include <xinput.h>
#include <chrono>

#include "device.hpp"

class Gamepad : public QObject, public Device {
    Q_OBJECT
public:
    Gamepad(DWORD id, XINPUT_STATE s);
    ~Gamepad();

    WORD getInput();
    XINPUT_STATE getState();
    bool isConnected();

    DWORD getID();
    void setID(DWORD id);

public slots:
    void vibrate(int MotorSpeed);

signals:
    void buttonPressed(WORD button_code);

private:
    DWORD controllerID;
    XINPUT_STATE state;
};