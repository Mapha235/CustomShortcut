#include "mainwindow.hpp"
#include "CreateShortcutWindow.hpp"
#include "MediaController.hpp"
#include "RightClickableButton.hpp"

#include <QCborMap>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

MainWindow::MainWindow()
    : m_current_device_index(0)
    , m_shortcuts_count(0)
{
    QSize size = QDesktopWidget().availableGeometry(this).size() * 0.3;
    this->setFixedSize(size.height() * 1.3, size.width() * 0.9);

    m_manager = new InputManager(0);
    getConnectedDevices();
    m_manager->startListener();

    m_scaffold = new QStackedWidget;
    m_main = new QWidget;
    m_add_btn = new QPushButton("+");

    m_device_box = new QGroupBox("Devices");
    m_device_box->setAlignment(Qt::AlignHCenter);

    createDeviceBoxLayout();
    m_device_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_shortcuts_box_layout = new QStackedLayout;
    m_shortcuts_box = new QGroupBox;
    m_shortcuts_box->setAlignment(Qt::AlignHCenter);
    m_shortcuts_box->setFlat(true);

    // set the initial device to the first registered gamepad and display the corresponding name und the qgroupbox
    if (m_devices.size() > 0) {
        m_current_device_index = 0;
        createShortcutsBoxLayout();
        QString title("Shortcuts - ");
        title.append(m_devices.at(0)->getName());
        m_shortcuts_box->setTitle(title);
    } else
        m_shortcuts_box->setTitle("Shortcuts");

    buttonHandler();

    m_profiles = new Profiles;
    // connect(m_profiles, &Profiles::)
    m_profiles->setMaximumWidth(40);

    // create the m_main m_layout for the m_main window
    m_layout = new QGridLayout;

    m_layout->addWidget(m_device_box, 0, 0, 1, 3);
    m_layout->addWidget(m_profiles, 1, 0, 2, 1);
    m_layout->addWidget(m_shortcuts_box, 1, 1, 1, 2);
    // m_layout->addWidget(m_add_btn, 0, 2, 1, 1);
    m_layout->addWidget(m_add_btn, 3, 1, 1, 2);

    m_main->setLayout(m_layout);    
    m_scaffold->addWidget(m_main);
    setCentralWidget(m_scaffold);


    load(Json);
    for (auto device : m_devices) {
        m_shortcuts_count += device->getShortcuts().size();
    }
}

MainWindow* MainWindow::getInstance()
{
    static MainWindow instance;
    return &instance;
}

MainWindow::~MainWindow()
{
    save(Json);

    delete m_manager;
    delete m_add_btn;
    delete m_device_box;
    delete m_shortcuts_box_layout;
    delete m_shortcuts_box;

    delete m_layout;
    delete m_scaffold;

    m_devices.clear();
}

void MainWindow::initUI()
{
}

void MainWindow::createLayout()
{
}

void MainWindow::toTray(){

}

void MainWindow::createDeviceBoxLayout()
{
    QGridLayout* m_device_box_layout = new QGridLayout(m_device_box);

    for (unsigned int i = 0; i < m_devices.size(); i++) {
        m_device_box_layout->addWidget(m_devices[i]->getButton(), 0, i, 1, 1);
    }
    m_device_box->setLayout(m_device_box_layout);
}

void MainWindow::createShortcutsBoxLayout()
{
    m_shortcuts_box_layout->setAlignment(Qt::AlignTop);
    m_shortcuts_box_layout->setSizeConstraint(QLayout::SetMaximumSize);
    for (auto device : m_devices)
        m_shortcuts_box_layout->addWidget(device->getShortcutsBox());
    m_shortcuts_box->setLayout(m_shortcuts_box_layout);
}

void MainWindow::showShortcuts(unsigned int index)
{
    try {
        m_current_device_index = index;
        QString title("Shortcuts - ");
        title.append(m_devices.at(index)->getName());
        m_shortcuts_box->setTitle(title);
        m_shortcuts_box_layout->setCurrentWidget(m_devices.at(index)->getShortcutsBox());

    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

void MainWindow::DeviceBoxHandler()
{
}

void MainWindow::buttonHandler()
{
    connect(m_add_btn, &QPushButton::clicked, this, &MainWindow::openNewShortcutWindow);
    for (auto device : m_devices) {
        connect(device->getButton(), &RightClickableButton::rightClicked, [=] { device->vibrate(25000); });
        connect(device->getButton(), &RightClickableButton::clicked, this, &MainWindow::showShortcuts);
    }
}

void MainWindow::getConnectedDevices()
{
    DWORD dwResult;

    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
        XINPUT_STATE state;

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &state);
        // Fills block of memory with zeros
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        // Controller is connected
        if (dwResult == ERROR_SUCCESS) {
            m_devices.push_back(new Gamepad(i, m_devices.size(), state));
            m_manager->addDevice(*(m_devices.end() - 1));
        }
    }
}

void MainWindow::openNewShortcutWindow()
{
    CreateShortcutWindow* window = new CreateShortcutWindow(m_devices.at(m_current_device_index)->getID());

    connect(window, &CreateShortcutWindow::resultReady, this, &MainWindow::addShortcut);
    window->show();
}

void MainWindow::deviceDisconnected(DWORD id)
{
    m_devices.erase(m_devices.begin() + id);
    std::cout << m_devices.size() << std::endl;
}

void MainWindow::addShortcut(CustomShortcut* shortcut)
{
    try {
        shortcut->setID(m_shortcuts_count);

        m_shortcuts_count++;
        m_devices.at(m_current_device_index)->newShortcut(shortcut);

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void MainWindow::read(const QJsonObject& json)
{
    if (json.contains("gamepads") && json["gamepads"].isArray()) {
        QJsonArray l_gamepadArray = json["gamepads"].toArray();

        for (int i = 0; i < m_devices.size(); ++i) {
            if (i < l_gamepadArray.size()) {
                QJsonObject gamepadObject = l_gamepadArray[i].toObject();
                m_devices[i]->read(gamepadObject);
            }
        }
    }
}

void MainWindow::write(QJsonObject& json) const
{
    QJsonArray l_m_devicesArray;
    for (auto device : m_devices) {
        QJsonObject l_deviceObject;
        device->write(l_deviceObject);
        l_m_devicesArray.append(l_deviceObject);
    }
    json["gamepads"] = l_m_devicesArray;
}

bool MainWindow::save(SaveFormat format) const
{
    QFile saveFile(format == Json
            ? QStringLiteral("save.json")
            : QStringLiteral("save.dat"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject saveObject;
    write(saveObject);
    saveFile.write(format == Json
            ? QJsonDocument(saveObject).toJson()
            : QCborValue::fromJsonValue(saveObject).toCbor());

    return true;
}

bool MainWindow::load(SaveFormat format)
{
    QFile loadFile(format == Json
            ? QStringLiteral("save.json")
            : QStringLiteral("save.dat"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(format == Json
            ? QJsonDocument::fromJson(saveData)
            : QJsonDocument(QCborValue::fromCbor(saveData).toMap().toJsonObject()));

    read(loadDoc.object());
    
    return true;
}

#include "moc_mainwindow.cpp"