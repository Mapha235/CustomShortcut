#pragma once

#include <vector>

#include <windows.h>
#include <xinput.h>

#include <QString>
#include <QThread>
#include <QTimer>

#include "Device.hpp"
#include "gamepad.hpp"

class Node {
public:
    Node();
    ~Node();
    bool isNeighbor(int node);
    std::vector<int> getNeighbors();

private:
    std::vector<int> neighbors;
    bool isEnd;
};

/* This class monitors all the connected gamepad's buttons.
*/
class InputManager : public QObject {
    Q_OBJECT
public:
    InputManager(int device_count);
    ~InputManager();
    /* Checks which buttons of the specified device are pressed
       and updates the btns_status values accordingly.
    */
    std::vector<DWORD> updateStatus(Device* device);
    /* Checks whether a button sequence mapped to a shortcut has been pressed or not.
       The Shortcut will be executed if a button sequence has been realised.
    */
    void checkShortcuts(Device* device);

    void addDevice(Device* device);
    void startListener();
    void stopListener();

    std::vector<DWORD> getButtonStatus(DWORD device_id);

    /*Translates the received button input to its written name.*/
    static QString translateButtonFromIdToName(DWORD button_id);
    static DWORD translateButtonFromNameToId(QString button_name);
    static std::vector<std::vector<DWORD>> btns_status;
public slots:
    void operate();

private:
    std::vector<Device*> devices;
    std::vector<DWORD> prev_btn_id;
    std::vector<DWORD> btn_pressed_count;
    QTimer* listener;
};