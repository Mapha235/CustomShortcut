#pragma once

class Device{
public:
    virtual ~Device(){};
    virtual WORD getInput() = 0;
    virtual bool isConnected() = 0;
    virtual XINPUT_STATE getState() = 0;
};