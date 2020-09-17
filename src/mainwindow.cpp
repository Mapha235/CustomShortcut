#include "mainwindow.hpp"

MainWindow::MainWindow()
{
    // devices = new Device*[XUSER_MAX_COUNT + 1];
    scaffold = new QStackedWidget;
    main = new QWidget;
    add_button = new QPushButton("+");
    device_list = new QComboBox;

    QComboBox* action_type_dropdown = new QComboBox;

    QLabel* device_label = new QLabel("Device");
    QComboBox* device_selection = new QComboBox;

    QLineEdit* hold = new QLineEdit;
    QLineEdit* press = new QLineEdit;

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

    layout->addWidget(connected_devices_label, 0, 0, 1, 1);
    layout->addWidget(device_list, 0, 1, 1, 1);

    layout->addWidget(device_selection, 2, 0, 1, 1);
    layout->addWidget(action_type_dropdown, 2, 1, 1, 1);

    layout->addWidget(hold, 2, 3, 1, 1);
    layout->addWidget(press, 2, 4, 1, 1);

    layout->addWidget(device_label, 1, 0, 1, 1);
    layout->addWidget(col1_label, 1, 1, 1, 1);
    layout->addWidget(col2_label, 1, 2, 1, 1);
    layout->addWidget(col3_label, 1, 3, 1, 1);
    layout->addWidget(col4_label, 1, 4, 1, 1);
    layout->addWidget(add_button, 5, 0, 1, 5);
    main->setLayout(layout);
    scaffold->addWidget(main);
    setCentralWidget(scaffold);

    getConnectedDevices();
}

MainWindow* MainWindow::getInstance()
{
    static MainWindow instance;
    return &instance;
}

MainWindow::~MainWindow()
{
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
    Gamepad* gamepad = new Gamepad(static_cast<DWORD>(devices.size()));
    DWORD dwResult = gamepad->isConnected();

    for (DWORD i = devices.size(); i < XUSER_MAX_COUNT + 1; i++) {
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &devices.at(i)->getState());

        if (dwResult == ERROR_SUCCESS) {
            // Controller is connected
            devices.push_back(&gamepad);
            device_list->addItem("Gamepad 1");
        } else {
            break;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
}

#include "moc_mainwindow.cpp"