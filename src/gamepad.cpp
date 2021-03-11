#include "gamepad.hpp"

#include <QDebug>

#include <QJsonArray>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

Gamepad::Gamepad(DWORD id, unsigned int index, XINPUT_STATE s)
    : m_controllerID(id)
    , m_index(index)
    , m_state(s)
{
    std::string temp_m_name = "Gamepad ";
    temp_m_name.append(std::to_string(index));
    m_name = QString(temp_m_name.c_str());
    m_rcbutton = new RightClickableButton(m_index, m_name);

    m_shortcuts_box = new QScrollArea;
    // Creates the m_layout for the m_shortcuts
    m_layout = new QVBoxLayout(m_shortcuts_box);
    for (unsigned int i = 0; i < m_shortcuts.size(); ++i) {
        m_layout->addWidget(m_shortcuts[i]);
    }
    m_layout->setAlignment(Qt::AlignTop);

    // puts all m_shortcuts into a groupbox
    QGroupBox* temp_shortcuts_box = new QGroupBox(m_shortcuts_box);
    temp_shortcuts_box->setLayout(m_layout);
    temp_shortcuts_box->setFlat(true);

    // makes the groupbox containing the m_shortcuts scrollable
    m_shortcuts_box->setWidgetResizable(true);
    m_shortcuts_box->setWidget(temp_shortcuts_box);
}

Gamepad::~Gamepad()
{
    for (auto shortcut : m_shortcuts) {
        delete shortcut;
    }
    m_shortcuts.clear();
    // emit disconnected(m_controllerID);
}

DWORD Gamepad::getID()
{
    return m_controllerID;
}

void Gamepad::setID(DWORD id)
{
    m_controllerID = id;
}

QString Gamepad::getName()
{
    return m_name;
}

unsigned int Gamepad::getShortcutSize()
{
    return m_shortcuts.size();
}

std::vector<CustomShortcut*> Gamepad::getShortcuts()
{
    return m_shortcuts;
}

WORD Gamepad::getInput()
{
    XInputGetState(m_controllerID, &m_state);
    auto btn_id = m_state.Gamepad.wButtons;
    return btn_id;
}

XINPUT_STATE Gamepad::getState()
{
    return m_state;
}

bool Gamepad::isConnected()
{
    return XInputGetState(m_controllerID, &m_state) == ERROR_SUCCESS;
}

RightClickableButton* Gamepad::getButton()
{
    return m_rcbutton;
}

void Gamepad::vibrate(int MotorSpeed)
{
    _XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = MotorSpeed;
    vibration.wRightMotorSpeed = MotorSpeed;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    XInputSetState(m_controllerID, &vibration);
    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() != 300) {
        end = std::chrono::system_clock::now();
    }
    vibration.wLeftMotorSpeed = 0;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(m_controllerID, &vibration);
}

void Gamepad::setIndex(unsigned int index)
{
    m_index = index;
}

unsigned int Gamepad::getIndex()
{
    return m_index;
}

QScrollArea* Gamepad::getShortcutsBox()
{
    return m_shortcuts_box;
}

void Gamepad::newShortcut(CustomShortcut* shortcut)
{
    connect(shortcut, &CustomShortcut::delete_signal, this, &Gamepad::removeShortcut);

    m_shortcuts.push_back(shortcut);
    m_layout->addWidget(shortcut);
}
void Gamepad::setShortcut(int index, CustomShortcut* shortcut)
{
    m_shortcuts.at(index) = shortcut;
}

void Gamepad::removeShortcut(unsigned int id)
{
    auto it = std::find_if(m_shortcuts.begin(), m_shortcuts.end(), [=](CustomShortcut* t1) { return t1->getID() == id; });
    if (it != m_shortcuts.end()) {
        m_shortcuts.erase(it);
    }
    qDebug() << m_shortcuts.size();
}

void Gamepad::clearShortcuts()
{
    for (auto shortcut : m_shortcuts) {
        delete shortcut;
    }
    m_shortcuts.clear();
}

void Gamepad::read(const QJsonObject& json)
{
    // Necessary for loading another profile during runtime.
    clearShortcuts();

    if (json.contains("name") && json["name"].isString())
        m_name = json["name"].toString();

    if (json.contains("shortcuts") && json["shortcuts"].isArray()) {
        QJsonArray l_shortcutsArray = json["shortcuts"].toArray();

        m_shortcuts.clear();
        m_shortcuts.reserve(l_shortcutsArray.size());

        for (int i = 0; i < l_shortcutsArray.size(); ++i) {
            QJsonObject l_shortcutObject = l_shortcutsArray[i].toObject();
            CustomShortcut* shortcut = new CustomShortcut(m_controllerID, m_shortcuts.size());
            shortcut->read(l_shortcutObject);
            m_shortcuts.push_back(shortcut);
            connect(shortcut, &CustomShortcut::delete_signal, this, &Gamepad::removeShortcut);
        }

        for (auto& shortcut : m_shortcuts) {
            m_layout->addWidget(shortcut);
        }
    }
}

void Gamepad::write(QJsonObject& json) const
{
    json["name"] = m_name;
    QJsonArray l_shortcuts_array;

    for (auto shortcut : m_shortcuts) {
        QJsonObject l_shortcutObj;
        shortcut->write(l_shortcutObj);
        l_shortcuts_array.append(l_shortcutObj);
    }
    json["shortcuts"] = l_shortcuts_array;
}

#include "moc_gamepad.cpp"