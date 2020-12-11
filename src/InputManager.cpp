#include "InputManager.hpp"
#include <iostream>

std::vector<std::vector<DWORD>> InputManager::btns_status;
InputManager::InputManager(int device_count)
{
    btns_status = std::vector<std::vector<DWORD>>(device_count, std::vector<DWORD>(16, 0));
    prev_btn_id = std::vector<DWORD>(device_count, 0);
    btn_pressed_count = std::vector<DWORD>(device_count, 0);

    listener = new QTimer(this);
    connect(listener, &QTimer::timeout, this, &InputManager::operate);
}

InputManager::~InputManager()
{
    delete listener;
    for (auto it : btns_status) {
        it.clear();
    }
    btns_status.clear();

    for (auto device : devices) {
        delete device;
    }
    devices.clear();

    btn_pressed_count.clear();
    prev_btn_id.clear();
}

void InputManager::operate()
{
    for (auto device : devices) {
        // QThread thread;
        updateStatus(device);
        checkShortcuts(device);
    }
}

//TODO: Multithreading i.e. one thread for every device
std::vector<DWORD> InputManager::updateStatus(Device* device)
{
    unsigned int controllerIndex = device->getIndex();
    auto btn_id = device->getInput();

    DWORD* prev_btn = &prev_btn_id.at(controllerIndex);
    DWORD* count = &btn_pressed_count.at(controllerIndex);
    std::vector<DWORD>* status = &btns_status.at(controllerIndex);

    try {
        if (btn_id != 0) {
            if (btn_id > *prev_btn)
                *count += 1;
        } else
            *count = 0;

        if (btn_id >= *prev_btn)
            *prev_btn = btn_id;
        else {
            *count -= 1;
            *prev_btn = btn_id;
        }

        for (unsigned int i = 0; i < status->size(); ++i) {
            if ((btn_id & (int)pow(2, i)) != 0) {
                if (status->at(i) == 0)
                    status->at(i) = *count;
            } else
                status->at(i) = 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Fehler in updateStatus()" << '\n';
        std::cerr << e.what() << '\n';
    }

    // for (int i = 0; i < btns_status[0].size(); ++i) {
    //     std::cout << btns_status[0]at(i) << " ";
    //     if (i == btns_status[0].size() - 1)
    //         std::cout << '\n';
    // }
    return btns_status.at(device->getIndex());
}

void InputManager::checkShortcuts(Device* device)
{
    auto& shortcuts = device->getShortcuts();
    // for (auto shortcut : shortcuts) {
    for (int i = 0; i < shortcuts.size(); ++i) {
        if (device->getInput() >= shortcuts[i]->minThreshold()) {
            try {
                shortcuts[i]->checkButtonOrder(btns_status.at(device->getIndex()));
            } catch (const std::exception& e) {
                std::cout << device->getShortcuts().size() << std::endl;
            }

        } else {
            shortcuts[i]->resetCondition();
        }
        // std::cout << device->getInput() << " " << shortcut->minThreshold() << std::endl;
    }
}

void InputManager::addDevice(Device* device)
{
    devices.push_back(device);
    btns_status.push_back(std::vector<DWORD>(16, 0));
    prev_btn_id.push_back(0);
    btn_pressed_count.push_back(0);
}

void InputManager::startListener()
{
    listener->start(1);
}

void InputManager::stopListener()
{
    listener->stop();
}

std::vector<DWORD> InputManager::getButtonStatus(DWORD device_id)
{
    return btns_status.at(device_id);
}

QString InputManager::translateButtonFromIdToName(DWORD button_id)
{
    button_id = log2(button_id);
    QString text;
    switch (button_id) {
    case 0:
        text = "D-PAD_UP";
        break;
    case 1:
        text = "D-PAD_DOWN";
        break;
    case 2:
        text = "D-PAD_LEFT";
        break;
    case 3:
        text = "D-PAD_RIGHT";
        break;
    case 4:
        text = "START";
        break;
    case 5:
        text = "BACK";
        break;
    case 6:
        text = "L3";
        break;
    case 7:
        text = "R3";
        break;
    case 8:
        text = "L1";
        break;
    case 9:
        text = "R1";
        break;
    case 12:
        text = "A";
        break;
    case 13:
        text = "B";
        break;
    case 14:
        text = "X";
        break;
    case 15:
        text = "Y";
        break;
    default:
        break;
    }
    return text;
}

DWORD InputManager::translateButtonFromNameToId(QString button_name)
{
    DWORD id;
    if (button_name == "D-PAD_UP")
        id = 0;
    else if (button_name == "D-PAD_DOWN")
        id = 1;
    else if (button_name == "D-PAD_LEFT")
        id = 2;
    else if (button_name == "D-PAD_RIGHT")
        id = 3;
    else if (button_name == "START")
        id = 4;
    else if (button_name == "BACK")
        id = 5;
    else if (button_name == "L3")
        id = 6;
    else if (button_name == "R3")
        id = 7;
    else if (button_name == "L1")
        id = 8;
    else if (button_name == "R1")
        id = 9;
    else if (button_name == "A")
        id = 12;
    else if (button_name == "B")
        id = 13;
    else if (button_name == "X")
        id = 14;
    else if (button_name == "Y")
        id = 15;
    else
        return -1;

    return pow(2, id);
}

#include "moc_InputManager.cpp"