#pragma once

#include <QGridLayout>
#include <QGroupBox>
#include <QGuiApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedLayout>
#include <QStackedWidget>

#include <iostream>
#include <vector>

// #include "Device.hpp"
#include "CustomShortcut.hpp"
#include "InputManager.hpp"
#include "gamepad.hpp"
#include "Profiles.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    static MainWindow* getInstance();
    void buttonHandler();
    void createLayout();
    void initUI();
    void getConnectedDevices();

    void createDeviceBoxLayout();
    void createShortcutsBoxLayout();
    /*  This functions creates a layout for the groupbox that shows the 
        shortcuts of the currently shown device.*/
    void DeviceBoxHandler();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;
    /* Saves the shortcuts into a file (json or binary).*/
    bool save(QString& file_name) const;
    bool load(QString& file_name);

public slots:
    void openNewShortcutWindow();
    void deviceDisconnected(DWORD id);
    void addShortcut(CustomShortcut* shortcut); 
    void showShortcuts(unsigned int id);
    
    //TODO
    void toTray();

private:
    MainWindow();
    // disallow Copy-Constructur
    MainWindow(const MainWindow&);
    // disallow assignment operator
    MainWindow& operator=(const MainWindow& other);
    ~MainWindow();

    InputManager* m_manager;
    std::vector<Gamepad*> m_devices;
    QGroupBox* m_device_box;
    QGroupBox* m_shortcuts_box;
    QStackedLayout* m_shortcuts_box_layout;
    QStackedWidget* m_scaffold;
    QWidget* m_main;
    QGridLayout* m_layout;
    QPushButton* m_add_btn;
    QPushButton* m_toTray_btn;
    Profiles* m_profiles;
    QPushButton* m_save_btn;
    
    DWORD m_current_device_index;
    int m_shortcuts_count;
};