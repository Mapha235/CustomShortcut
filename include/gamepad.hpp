#pragma once

#include <vector>

#include <windows.h>
#include <xinput.h>

#include "CustomShortcut.hpp"
#include "RightClickableButton.hpp"
#include "device.hpp"

// #include <QObject>
#include <QScrollArea>

#include <QTimer>
#include <QVBoxLayout>
#include <QJsonObject>

class Gamepad : public QObject, public Device {
    Q_OBJECT
public:
    Gamepad(DWORD id, unsigned int index, XINPUT_STATE s);
    ~Gamepad();

    WORD getInput() override;
    XINPUT_STATE getState() override;
    bool isConnected() override;

    DWORD getID() override;
    void setID(DWORD id);

    QString getName();
    /*Returns the number of registered shortcuts i.e. the vector's size.*/
    unsigned int getShortcutSize();

    RightClickableButton* getButton();
    /*Returns the vector containing all the registered CustomShortcut objects.*/
    std::vector<CustomShortcut*> getShortcuts() override;
    /*Returns the widget that displays all registered shortcuts.*/
    QScrollArea* getShortcutsBox();

    void newShortcut(CustomShortcut* shortcut);

    void setIndex(unsigned int index);
    unsigned int getIndex() override;

    void setShortcut(int index, CustomShortcut* shortcut);
    void removeShortcut(unsigned int id) override;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

public slots:
    /*Mainly used for controller identification.*/
    void vibrate(int MotorSpeed);

signals:
    void buttonPressed(WORD button_code);
    void disconnected(DWORD id);
    void operate();

protected:
    // bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QString m_name;
    DWORD m_controllerID;
    unsigned int m_index;
    XINPUT_STATE m_state;
    QVBoxLayout* m_layout;
    RightClickableButton* m_rcbutton;
    QScrollArea* m_shortcuts_box;

    std::vector<CustomShortcut*> m_shortcuts;
};