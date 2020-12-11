#pragma once

#include "CustomShortcut.hpp"
#include <vector>
#include <windows.h>
#include <xinput.h>

class Device {
public:
    virtual ~Device() {};
    virtual WORD getInput() = 0;
    virtual DWORD getID() = 0;
    virtual bool isConnected() = 0;
    virtual XINPUT_STATE getState() = 0;
    virtual unsigned int getIndex() = 0;
    virtual std::vector<CustomShortcut*> getShortcuts() = 0;
    virtual void setShortcut(int index, CustomShortcut* shortcut) = 0;
    virtual void removeShortcut(unsigned int index) = 0;
protected:

};