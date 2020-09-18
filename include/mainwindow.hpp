#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>
#include <QIcon>

#include <iostream>
#include <vector>

// #include "Device.hpp"
#include "gamepad.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    static MainWindow* getInstance();
    void buttonHandler();
    void createLayout();
    void initUI();
    void getConnectedDevices();

public slots:
    void listen();
    void deviceDisconnected(DWORD id);

signals:
    // void newDevice(DWORD id);

protected:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

private:
    MainWindow();
    // disallow Copy-Constructur
    MainWindow(const MainWindow&);
    // disallow assignment operator
    MainWindow& operator=(const MainWindow& other);
    ~MainWindow();

    // std::vector<Device*> devices;
    std::vector<Gamepad*> devices;
    QGroupBox *device_box;
    QStackedWidget* scaffold;
    QWidget* main;
    QGridLayout* layout;
    QPushButton* add_button;
};