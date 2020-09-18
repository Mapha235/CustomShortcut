#include "mainwindow.hpp"

MainWindow::MainWindow()
{
    getConnectedDevices();
    // devices = new Device*[XUSER_MAX_COUNT + 1];
    scaffold = new QStackedWidget;
    main = new QWidget;
    add_button = new QPushButton("+");

    device_box = new QGroupBox("Devices");
    QPushButton* refresh_btn = new QPushButton("Refresh");
    QGridLayout* device_box_layout = new QGridLayout;


    for (int i = 0; i < devices.size(); i++) {
    // for(auto device: devices){
        std::string device_name = "Gamepad ";
        device_name.append(std::to_string(i));
        QPushButton *device_btn = new QPushButton(QString(device_name.c_str()));
        // device_btn->setAlignment(Qt::AlignCenter);
        device_box_layout->addWidget(device_btn, 0, i, 1, 1);

        // connect(device_btn, &QPushButton::clicked, [=]{devices.at(i)->vibrate();});
        connect(device_btn, &QPushButton::clicked, [=]{devices.at(i)->vibrate(20000);});
    }

    device_box_layout->addWidget(refresh_btn, 2, 0, 1, device_box_layout->columnCount());
    device_box->setLayout(device_box_layout);

    // connect(refresh_btn, &QPushButton::clicked, [=] { getConnectedDevices(); });
    connect(refresh_btn, &QPushButton::clicked, this ,&MainWindow::getConnectedDevices);

    QComboBox* action_type_dropdown = new QComboBox;

    QLabel* device_label = new QLabel("Device");
    QComboBox* device_selection = new QComboBox;

    QLineEdit* hold_line = new QLineEdit;
    QLineEdit* press_line = new QLineEdit;

    QLabel* col1_label = new QLabel("Action Type");
    QLabel* col2_label = new QLabel("Action");
    QLabel* col3_label = new QLabel("Modifier (Hold)");
    QLabel* col4_label = new QLabel("Key (Press)");
    QLabel* connected_devices_label = new QLabel("Devices:");

    device_label->setAlignment(Qt::AlignCenter);
    connected_devices_label->setAlignment(Qt::AlignCenter);
    col1_label->setAlignment(Qt::AlignCenter);
    col2_label->setAlignment(Qt::AlignCenter);
    col3_label->setAlignment(Qt::AlignCenter);
    col4_label->setAlignment(Qt::AlignCenter);

    action_type_dropdown->addItem(tr("Hotkey"));
    action_type_dropdown->addItem(tr("Launch Program"));
    action_type_dropdown->addItem(tr("Volume Control"));
    action_type_dropdown->addItem(tr("Command"));

    device_selection->addItem("Keyboard");

    layout = new QGridLayout;

    // layout->addWidget(connected_devices_label, 0, 0, 1, 1);
    layout->addWidget(device_box, 0, 0, 1, 5);

    layout->addWidget(device_selection, 2, 0, 1, 1);
    layout->addWidget(action_type_dropdown, 2, 1, 1, 1);

    layout->addWidget(hold_line, 2, 3, 1, 1);
    layout->addWidget(press_line, 2, 4, 1, 1);

    layout->addWidget(device_label, 1, 0, 1, 1);
    layout->addWidget(col1_label, 1, 1, 1, 1);
    layout->addWidget(col2_label, 1, 2, 1, 1);
    layout->addWidget(col3_label, 1, 3, 1, 1);
    layout->addWidget(col4_label, 1, 4, 1, 1);
    layout->addWidget(add_button, 5, 0, 1, 5);
    main->setLayout(layout);
    scaffold->addWidget(main);
    setCentralWidget(scaffold);

    // getConnectedDevices();
}

MainWindow* MainWindow::getInstance()
{
    static MainWindow instance;
    return &instance;
}

MainWindow::~MainWindow()
{
    delete device_box;
    delete add_button;
    delete layout;
    delete main;
    delete scaffold;
}

void MainWindow::initUI()
{
}

void MainWindow::createLayout()
{
}

void MainWindow::buttonHandler()
{
    // connect(add_button, &QPushButton::clicked(), []{});
}

void MainWindow::getConnectedDevices()
{
    DWORD dwResult;
    XINPUT_STATE state;
    std::cout << "HIII" << std::endl;

    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
        // Fills block of memory with zeros

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &state);
        if (dwResult == ERROR_SUCCESS) {
            ZeroMemory(&state, sizeof(XINPUT_STATE));
            // Controller is connected
            // Gamepad* gamepad = new Gamepad(i, state);
            // Gamepad gamepad(i, state);
            devices.push_back(new Gamepad(i, state));
            // delete gamepad;
        } else {
            break;
        }
    }
}

void MainWindow::listen(){

}

void MainWindow::deviceDisconnected(DWORD id){
    devices.erase(devices.begin() + id);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
}

#include "moc_mainwindow.cpp"