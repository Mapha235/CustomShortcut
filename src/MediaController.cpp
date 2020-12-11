#include "MediaController.hpp"
#include <QProcess>

#include <QDebug>

#include <iostream>
#include <string>
#include <windows.h>
#include <winuser.h>

static bool SystemVolumeIsMute = false;
namespace {
void MultimediaControls(int hex_code)
{
    INPUT inputs[2];
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = hex_code;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = hex_code;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs)) {
        // OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}
}

namespace multimedia {
void PlayPause()
{
    MultimediaControls(VK_MEDIA_PLAY_PAUSE);
}
void NextTrack()
{
    MultimediaControls(VK_MEDIA_NEXT_TRACK);
}
void PrevTrack()
{
    MultimediaControls(VK_MEDIA_PREV_TRACK);
}
void VolumeDown()
{
    QProcess process;

    if (!process.startDetached("nircmd mutesysvolume 0")) {
        double v = 655.35 * (-2);
        std::string command = "nircmd changesysvolume ";
        command.append(std::to_string(v));
        process.startDetached(command.c_str());
    } else
        MultimediaControls(VK_VOLUME_DOWN);
}
void VolumeUp()
{
    QProcess process;

    if (!process.startDetached("nircmd mutesysvolume 0")) {
        double v = 655.35 * (2);
        std::string command = "nircmd changesysvolume ";
        command.append(std::to_string(v));
        process.startDetached(command.c_str());
    } else
        MultimediaControls(VK_VOLUME_UP);
}
void Mute()
{
    QProcess process;

    SystemVolumeIsMute = !SystemVolumeIsMute;
    std::cout << SystemVolumeIsMute << std::endl;
    std::string command = "nircmd mutesysvolume ";
    command.append(std::to_string(SystemVolumeIsMute));
    if (!process.startDetached(command.c_str())) {
    } else {
        std::cerr << "Mute failed." << std::endl;
        MultimediaControls(VK_VOLUME_MUTE);
    }
}
}
using namespace multimedia;
void (*mediaFctPtr[])() = { PlayPause, NextTrack, PrevTrack, VolumeDown, VolumeUp, Mute };