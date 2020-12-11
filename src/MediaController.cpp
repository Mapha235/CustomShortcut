#include "MediaController.hpp"
#include <QProcess>

#include <QDebug>

#include <iostream>
#include <string>
#include <windows.h>
#include <winuser.h>

static bool SystemVolumeIsMute = false;
namespace {
bool MultimediaControls(int hex_code)
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
        return false;
    }
    return true;
}
}

namespace multimedia {
bool PlayPause()
{
    return MultimediaControls(VK_MEDIA_PLAY_PAUSE);
}
bool NextTrack()
{
    return MultimediaControls(VK_MEDIA_NEXT_TRACK);
}
bool PrevTrack()
{
    return MultimediaControls(VK_MEDIA_PREV_TRACK);
}
bool VolumeDown()
{
    QProcess process;

    if (process.startDetached("nircmd mutesysvolume 0")) {
        double v = 655.35 * (-2);
        std::string command = "nircmd changesysvolume ";
        command.append(std::to_string(v));
        process.startDetached(command.c_str());

        return true;
    } else
        return MultimediaControls(VK_VOLUME_DOWN);
}
bool VolumeUp()
{
    QProcess process;

    if (process.startDetached("nircmd mutesysvolume 0")) {
        double v = 655.35 * (2);
        std::string command = "nircmd changesysvolume ";
        command.append(std::to_string(v));
        process.startDetached(command.c_str());
        
        return true;
    } else
        return MultimediaControls(VK_VOLUME_UP);
}
bool Mute()
{
    QProcess process;

    SystemVolumeIsMute = !SystemVolumeIsMute;
    std::cout << SystemVolumeIsMute << std::endl;
    std::string command = "nircmd mutesysvolume ";
    command.append(std::to_string(SystemVolumeIsMute));
    if (process.startDetached(command.c_str())) {
        return true;
    } else {
        return MultimediaControls(VK_VOLUME_MUTE);
    }
}
}
using namespace multimedia;
bool (*mediaFctPtr[])() = { PlayPause, NextTrack, PrevTrack, VolumeDown, VolumeUp, Mute };