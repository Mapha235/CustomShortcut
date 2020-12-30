#include "CustomShortcut.hpp"
#include "InputManager.hpp"
#include "MediaController.hpp"

#include <QApplication>
#include <QDebug>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QKeyEvent>
#include <QMessageBox>
#include <QProcess>
#include <QSizePolicy>
#include <QString>
#include <QTimer>

#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <cstdlib>
#include <iostream>
#include <winuser.h>

CustomShortcut::CustomShortcut(const int owner_id, const int id, QWidget* parent)

    : QGroupBox(parent)
    , m_owner_id(owner_id)
    , m_id(id)
    , m_exec_condition(0)
{
    initUI();
    buttonHandler();
}

CustomShortcut::CustomShortcut(const int owner_id, const Action type, const std::vector<DWORD> btn_seq, QWidget* parent)
    : QGroupBox(parent)
    , m_owner_id(owner_id)
    , m_exec_condition(0)
    , m_threshold(std::accumulate(btn_seq.begin(), btn_seq.end(), 0))
    , m_type(type)
    , m_btn_seq(btn_seq)
    , m_media_type(-1)
{
    initUI();
    buttonHandler();
}

CustomShortcut::~CustomShortcut()
{
    delete m_action_type;
    delete m_btn_seq_line;
    delete m_command_line;
    delete m_delete_btn;
    delete m_layout;
}

void CustomShortcut::initUI()
{
    QString l_btn_seq_text;

    for (const auto& btn : m_btn_seq) {
        l_btn_seq_text += InputManager::translateButtonFromIdToName(btn);
        if (*(m_btn_seq.end() - 1) != btn)
            l_btn_seq_text += " + ";
    }

    m_action_type = new QLineEdit("Hotkey");
    m_btn_seq_line = new QLineEdit(l_btn_seq_text);
    m_command_line = new QLineEdit();
    m_delete_btn = new QPushButton("x");

    m_delete_btn->setFixedSize(QSize(20, 50));

    m_layout = new QGridLayout(this);
    m_layout->addWidget(m_btn_seq_line, 0, 0, 1, 5);
    m_layout->addWidget(m_command_line, 1, 0, 1, 5);
    m_layout->addWidget(m_delete_btn, 0, 5, 2, 1);
    this->setLayout(m_layout);

    m_btn_seq_line->setReadOnly(true);
    m_btn_seq_line->setAlignment(Qt::AlignHCenter);

    m_command_line->setReadOnly(true);
    m_command_line->setAlignment(Qt::AlignHCenter);

    m_action_type->setReadOnly(true);
    m_action_type->setAlignment(Qt::AlignCenter);
}

void CustomShortcut::closeThis()
{
    emit delete_signal(m_id);
    this->close();
}

void CustomShortcut::buttonHandler()
{
    connect(m_delete_btn, &QPushButton::clicked, this, &CustomShortcut::closeThis);
}

void CustomShortcut::change_btn_seq(std::vector<DWORD> seq)
{
    m_btn_seq = seq;
}

void CustomShortcut::execute()
{
    QProcess process;
    bool success(false);

    if (m_type == Action::VolumeControl) {
        // This function pointer is defined inside MediaController.cpp
        bool (*mediaFctPtr[])() = { multimedia::PlayPause, multimedia::NextTrack, multimedia::PrevTrack, multimedia::VolumeDown, multimedia::VolumeUp, multimedia::Mute };
        success = mediaFctPtr[m_media_type]();

    } else if (m_type == Action::Process) {
        success = process.startDetached("\"" + m_command + "\"");
        // if (QProcess::execute(m_command_line->text()) == -2) {
    } else if (m_type == Action::TerminalCommand) {
        success = process.startDetached(m_command);
    }
    if (!success) {
        qDebug() << "Failed to run program.";
        QMessageBox l_error_msg;
        l_error_msg.setIcon(QMessageBox::Critical);
        l_error_msg.setText(QString("Failed to run ").append(m_command).append("."));
        l_error_msg.exec();
    }
}

DWORD CustomShortcut::minThreshold()
{
    return m_threshold;
}

void CustomShortcut::checkButtonOrder(std::vector<DWORD>& btns_status)
{
    unsigned int pred_btn_order = 0;
    // if (btns_status.at(log2(m_btn_seq.at(0))) > 0) {
    if (btns_status.at(log2(m_btn_seq.at(0))) == 1) {
        pred_btn_order = btns_status.at(log2(m_btn_seq.at(0)));
        // if (btns_status.at(log2(m_btn_seq.at(0))) > 0) {
        m_exec_condition++;
        for (int i = 1; i < m_btn_seq.size(); ++i) {
            try {
                double temp = log2(m_btn_seq[i]);

                if (btns_status.at(temp) == pred_btn_order + 1) {
                    if (m_exec_condition < m_btn_seq.size() + 1) // this is not really necessary. This just ensures that m_exec_condition does not increment infinitely.
                        m_exec_condition++;
                    pred_btn_order = btns_status.at(temp);
                    // pred_btn_order += 1;
                } else {
                    m_exec_condition = 0;
                    pred_btn_order = 0;
                    break;
                }
            } catch (const std::exception& e) {
                // std::cerr << e.what() << '\n';
                std::cerr << "log2 fehlgeschlagen. Fehlerhafter button wurde registriert." << '\n';
            }
        }
    } else {
        m_exec_condition = 0;
    }

    if (m_exec_condition == m_btn_seq.size()) {
        // QFuture<void> future = QtConcurrent::run(execute);
        execute();
    }
}

void CustomShortcut::asignBtnSeq(std::vector<DWORD>& seq)
{
    m_btn_seq = seq;
}

void CustomShortcut::incrementCondition()
{
    m_exec_condition++;
    if (m_exec_condition == m_btn_seq.size()) {
    }
}

void CustomShortcut::resetCondition()
{
    m_exec_condition = 0;
}

void CustomShortcut::showFileNameOnly()
{
    QStringList split_command = m_command.split("/");
    m_command_line->setText(*(split_command.end() - 1));
}

void CustomShortcut::setCommand(QString command)
{
    m_command = command;
    if (m_type == Action::Process) {
        showFileNameOnly();
    } else {
        m_command_line->setText(command);
    }
}

void CustomShortcut::setActionText(QString action)
{
    m_action_type->setText(action);
}

void CustomShortcut::setID(const int id)
{
    m_id = id;
}

unsigned int CustomShortcut::getID()
{
    return m_id;
}

unsigned int CustomShortcut::getOwnerID()
{
    return m_owner_id;
}

void CustomShortcut::setMultimediaCommand(unsigned int nr)
{
    m_media_type = nr;
}

void CustomShortcut::read(const QJsonObject& json)
{
    if (json.contains("type") && json["type"].isDouble())
        m_type = static_cast<Action>(json["type"].toInt());

    if (json.contains("media_type") && json["media_type"].isDouble())
        m_media_type = json["media_type"].toInt();

    if (json.contains("command") && json["command"].isString()) {
        if (m_type == Action::Process) {
            m_command = json["command"].toString();
            showFileNameOnly();
        } else {
            m_command = json["command"].toString();
            m_command_line->setText(json["command"].toString());
        }
    }

    QString l_btn_seq_text;
    if (json.contains("button_sequence") && json["button_sequence"].isArray()) {
        QJsonArray l_btnArray = json["button_sequence"].toArray();
        m_btn_seq.clear();
        m_btn_seq.reserve(l_btnArray.size());
        for (int i = 0; i < l_btnArray.size(); ++i) {
            DWORD l_btn_id = l_btnArray[i].toString().toULong();
            m_btn_seq.push_back(l_btn_id);

            // Create text for displaying the button sequence in text form
            l_btn_seq_text += InputManager::translateButtonFromIdToName(l_btn_id);
            if (i < l_btnArray.size() - 1)
                l_btn_seq_text += " + ";
        }
        m_btn_seq_line->setText(l_btn_seq_text);
        m_threshold = std::accumulate(m_btn_seq.begin(), m_btn_seq.end(), 0);
    }
}

void CustomShortcut::write(QJsonObject& json) const
{
    json["type"] = static_cast<int>(m_type);
    if (m_type == Action::VolumeControl) {
        json["media_type"] = m_media_type;
    }
    json["command"] = m_command;

    QJsonArray l_btn_seq_array;

    for (auto btn : m_btn_seq) {
        l_btn_seq_array.append(QJsonValue((QString::number(btn))));
    }
    json["button_sequence"] = l_btn_seq_array;
}

#include "moc_CustomShortcut.cpp"