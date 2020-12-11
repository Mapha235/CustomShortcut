#define QT
// #define GAMEPAD
#ifdef GAMEPAD
#pragma comment(lib, "XInput.lib")

#include <iostream>
#include <string>
#include <vector>

#include <stdlib.h>

#include <thread>
#include <windows.h>
#include <xinput.h>

#include "MediaController.hpp"
#include "gamepad.hpp"

static const WORD XINPUT_Buttons[] = {
    XINPUT_GAMEPAD_A,
    XINPUT_GAMEPAD_B,
    XINPUT_GAMEPAD_X,
    XINPUT_GAMEPAD_Y,
    XINPUT_GAMEPAD_DPAD_UP,
    XINPUT_GAMEPAD_DPAD_DOWN,
    XINPUT_GAMEPAD_DPAD_LEFT,
    XINPUT_GAMEPAD_DPAD_RIGHT,
    XINPUT_GAMEPAD_LEFT_SHOULDER,
    XINPUT_GAMEPAD_RIGHT_SHOULDER,
    XINPUT_GAMEPAD_LEFT_THUMB,
    XINPUT_GAMEPAD_RIGHT_THUMB,
    XINPUT_GAMEPAD_START,
    XINPUT_GAMEPAD_BACK
};

void volume_down(float value);
void volume_up(float value);
void openFileOrganizer();
void openLocker();

int init()
{

    return 0;
}

int main(int argc, char** argv)
{
    _XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = 30000;
    vibration.wRightMotorSpeed = 30000;

    MediaController* media = new MediaController;

    DWORD dwResult;
    XINPUT_STATE state;
    Gamepad gamepad(0, state);

    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
        // Fills block of memory with zeros
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &state);
        if (dwResult == ERROR_SUCCESS) {
            // Controller is connected
            gamepad.setID(i);
        } else {
            break;
        }
    }

    int count = 0;
    bool gameMode = false;
    if (argc > 1)
        gameMode = *argv[1] == '1';

    // XInputGetState updates the Gamepad's state after every iteration
    while (dwResult = XInputGetState(gamepad.getID(), &state) == ERROR_SUCCESS) {
        bool A_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
        bool B_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
        bool X_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
        bool Y_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
        bool RightS_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
        bool LeftS_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
        bool RighT_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
        bool LeftT_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
        bool Back_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
        bool Start_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
        bool DPadR_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
        bool DPadL_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
        bool DPadD_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
        bool DPadU_button_pressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;

        if (state.Gamepad.wButtons != 0)
            std::cout << state.Gamepad.wButtons << std::endl;

        if (gameMode) {
            if (!LeftS_button_pressed) {
                if (LeftT_button_pressed)
                    media->volumeDown(2);
                else if (RighT_button_pressed)
                    media->volumeUp(2);
            }
        } else {
            if (LeftS_button_pressed)
                media->volumeDown(2);
            else if (RightS_button_pressed)
                media->volumeUp(2);
            else if (A_button_pressed) {
                openFileOrganizer();
                std::cout << "hallllloooo" << std::endl;
            } else if (Y_button_pressed) {
                XInputSetState(0, &vibration);
                // openLocker();
            } else if (X_button_pressed)
                openLocker();
        }
        // std::cout << ":)" << std::endl;
        // if (dwResult = XInputGetState(gamepad.getID(), &state) != ERROR_SUCCESS)
        //     isRunning = false;
    }
    delete media;

    return 0;
}

void volume_down(float value)
{
    std::string command = "nircmd changesysvolume ";
    float v = 655.35 * -(value);
    command.append(std::to_string(v));
    system("nircmd mutesysvolume 0");
    system(command.c_str());
}

void volume_up(float value)
{
    std::string command = "nircmd changesysvolume ";
    float v = 655.35 * (value);
    command.append(std::to_string(v));
    system("nircmd mutesysvolume 0");
    system(command.c_str());
}

void openFileOrganizer()
{
    system("C:/Python/python C:/Dev/python/FileOrganizer/src/main.py");
}

void openLocker()
{
    system("explorer \"G:\\Rest\\abi\\CPY_SAVES\\CPY\\one\"");
}
#endif

#ifdef QT
#include <QApplication>

#include "mainwindow.hpp"
#include <iostream>
#include <vector>


void temp()
{
    std::vector<DWORD> shortcuts_graph(16);
}

int main(int argc, char** argv)
{
    XINPUT_KEYSTROKE key;
    // while (true) {
    //     auto hh = XInputGetKeystroke(0, 0, &key);
    //     if(key.VirtualKey != 0 && key.Flags != XINPUT_KEYSTROKE_KEYDOWN )
    //         std::cout << key.VirtualKey << std::endl;
    // }

    QApplication app(argc, argv);

    MainWindow* window = MainWindow::getInstance();
    window->show();
    return app.exec();
}
#endif