#include "mainwindow.hpp"
#include "CreateShortcutWindow.hpp"
#include "MediaController.hpp"
#include "RightClickableButton.hpp"

#include <QCborMap>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

MainWindow::MainWindow()
    : m_current_device_index(-1)
    , m_shortcuts_count(0)
    , m_cwd(QDir::currentPath())
{
    QSize size = QDesktopWidget().availableGeometry(this).size() * 0.3;
    this->setFixedSize(size.height() * 1.3, size.width() * 0.9);

    m_manager = new InputManager(0);

    m_scaffold = new QStackedWidget;
    m_main = new QWidget;
    m_add_btn = new QPushButton("+");

    m_device_box = new QGroupBox("Devices");

    m_shortcuts_box_layout = new QStackedLayout;
    m_shortcuts_box = new QGroupBox;
    m_shortcuts_box->setAlignment(Qt::AlignHCenter);
    m_shortcuts_box->setFlat(true);

    m_profiles = new Profiles();

    // connect(m_profiles, &Profiles::profileChanged, this, &QMainWindow::load);
    connect(m_profiles, &Profiles::profileChanged, this, &MainWindow::changeProfile);
    connect(m_profiles, &Profiles::newProfile, this, &MainWindow::save);

    if (getConnectedDevices()) {
        m_manager->startListener();
        // m_current_device_index = 0;

        // set the initial device to the first registered gamepad and display the corresponding name und the qgroupbox
        createShortcutsBoxLayout();
        QString title("Shortcuts - ");
        title.append(m_devices.at(0)->getName());
        m_shortcuts_box->setTitle(title);

        load(QString("save.json"));
    } else {
        m_shortcuts_box->setTitle("Shortcuts");
        // m_add_btn->setDisabled(true);
        // m_profiles->setDisabled(true);
    }

    // m_add_btn->setMaximumWidth(40);

    buttonHandler();

    initUI();

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
    saveState();

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
    m_device_box->setAlignment(Qt::AlignHCenter);
    createDeviceBoxLayout();
    m_device_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_profiles->setMaximumWidth(40);
    // create the m_main m_layout for the m_main window
    m_layout = new QGridLayout;

    m_layout->addWidget(m_device_box, 0, 0, 1, 3);
    m_layout->addWidget(m_profiles, 1, 0, 2, 1);
    m_layout->addWidget(m_shortcuts_box, 1, 1, 1, 2);
    m_layout->addWidget(m_profiles->getSaveButton(), 3, 0, 1, 1);
    // m_layout->addWidget(m_add_btn, 1, 3, 3, 1);
    m_layout->addWidget(m_add_btn, 3, 1, 1, 2);

    m_main->setLayout(m_layout);
    m_scaffold->addWidget(m_main);
    setCentralWidget(m_scaffold);
}

void MainWindow::createLayout()
{
}

void MainWindow::toTray()
{
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

bool MainWindow::getConnectedDevices()
{
    DWORD dwResult;
    bool success(false);

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
            success = true;
        }
    }
    return success;
}

void MainWindow::openNewShortcutWindow()
{
    if (m_current_device_index != -1) {
        CreateShortcutWindow* window = new CreateShortcutWindow(m_devices.at(m_current_device_index)->getID());

        connect(window, &CreateShortcutWindow::resultReady, this, &MainWindow::addShortcut);
        window->show();
    }
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

bool MainWindow::saveState() const
{
    if (m_current_device_index != -1) {
        QDir::setCurrent(m_cwd);
        QFile saveFile("save.json");

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return false;
        }
        QJsonObject saveObject;

        saveObject["profile"] = m_profiles->getCurrentProfile() + 1;
        saveObject["device"] = m_current_device_index;
        saveFile.write(QJsonDocument(saveObject).toJson());

        return true;
    }
    return false;
}

bool MainWindow::save(QString& file_name) const
{
    QDir::setCurrent(m_cwd);
    QFile saveFile(file_name);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject saveObject;
    write(saveObject);
    saveFile.write(QJsonDocument(saveObject).toJson());

    return true;
}

bool MainWindow::load(QString& file_name)
{
    QDir::setCurrent(m_cwd);
    QFile loadFile(file_name);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't load " << file_name << ".";
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject save = loadDoc.object();

    int l_current_profile = m_profiles->getCurrentProfile();

    // loads initial state
    if (m_current_device_index < 0) {
        // if (l_current_profile < 0) {
        if (save.contains("profile") && save["profile"].isDouble())
            l_current_profile = save["profile"].toInt() - 1;

        if (save.contains("device") && save["device"].isDouble())
            m_current_device_index = save["device"].toInt();

        /*Fixes a faulty device entry in the save file.*/
        if (m_current_device_index < 0 || m_current_device_index > m_devices.size() - 1)
            m_current_device_index = 0;

        /*Fixes a faulty profile entry in the save file.*/
        if (l_current_profile < 1 || l_current_profile > 4)
            l_current_profile = 0;

        m_profiles->setCurrentProfile(l_current_profile);
        m_profiles->handleProfile(l_current_profile);
    } else {
        read(loadDoc.object());
    }

    return true;
}

void MainWindow::changeProfile(unsigned int profile_id)
{
    if (m_devices.size() > 0) {
        QString l_file_name("./profiles/profile");
        l_file_name.append(QString::number(profile_id + 1));
        l_file_name.append(QString(".json"));

        m_devices[m_current_device_index]->clearShortcuts();

        load(l_file_name);
    }
}

#include "moc_mainwindow.cpp"