#include "CreateShortcutWindow.hpp"
#include "InputManager.hpp"

#include <QColor>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace Transform {
inline QWidget* centerWidget(QWidget* target_widget, QWidget* parent = nullptr)
{
    QWidget* centered = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(centered);
    // layout->setAlignment(Qt::AlignHCenter);
    layout->setAlignment(Qt::AlignBaseline);
    layout->addWidget(target_widget);
    centered->setLayout(layout);
    return centered;
}
inline QPushButton *createQPushButtonWithWordWrap(QWidget *parent, const QString &text)
{
   auto btn = new QPushButton(parent);
   auto label = new QLabel(text,btn);
   label->setWordWrap(true);

   auto layout = new QHBoxLayout(btn);
   layout->addWidget(label,0,Qt::AlignCenter);

   return btn;
}
}

CreateShortcutWindow::CreateShortcutWindow(DWORD device_id, QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_device_id(device_id)
    , m_btn_seq(0)
{
    m_already_pressed_btns = std::vector<bool>(16, 0);

    // this attribute calls the destructor upon closing this widget
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QString("Gamepad ").append(QString::number(m_device_id)));
    m_add_btn = new QPushButton("Add");
    m_cancel_btn = new QPushButton("Cancel");

    m_gamepad_btn_seq = new QTextEdit;
    m_gamepad_btn_seq->setReadOnly(true);
    m_gamepad_btn_seq->setAlignment(Qt::AlignVCenter);
    resetButtonSequence();

    m_action = new QTextEdit;

    m_listener = new QTimer;
    connect(m_listener, &QTimer::timeout, this, &CreateShortcutWindow::registerButtonSequence);
    m_listener->start(1);

    m_action_type_dropdown = new QComboBox;
    m_action_type_dropdown->addItem("Command");
    // m_action_type_dropdown->addItem("Hotkey");
    m_action_type_dropdown->addItem("Media Control");
    m_action_type_dropdown->addItem("Launch Program (.exe)");

    initShortcutDetails();
    initUI();
    signalHandler();
}

void CreateShortcutWindow::signalHandler()
{
    connect(m_add_btn, &QPushButton::clicked, this, &CreateShortcutWindow::bundleResults);
    connect(m_cancel_btn, &QPushButton::clicked, this, &QWidget::close);
    connect(m_action_type_dropdown, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index) { m_shortcut_details->setCurrentIndex(index); });
}

CreateShortcutWindow::~CreateShortcutWindow()
{
    delete m_layout;
    delete m_add_btn;
    delete m_cancel_btn;
    delete m_action_type_dropdown;
    delete m_gamepad_btn_seq;
    delete m_shortcut_details;
    delete m_listener;
}

void CreateShortcutWindow::initUI()
{
    m_layout = new QGridLayout;
    m_layout->addWidget(m_action_type_dropdown, 0, 0, 1, 4);
    m_layout->addWidget(m_gamepad_btn_seq, 1, 0, 1, 2);
    m_layout->addWidget(m_shortcut_details, 1, 2, 1, 2);
    m_layout->addWidget(m_add_btn, 3, 2, 1, 1);
    m_layout->addWidget(m_cancel_btn, 3, 3, 1, 1);
    this->setLayout(m_layout);
}

void CreateShortcutWindow::initShortcutDetails()
{
    m_media = new QComboBox;
    m_media->addItem("Play/Pause");
    m_media->addItem("Next Track");
    m_media->addItem("Previous Track");
    m_media->addItem("Volume Down");
    m_media->addItem("Volume Up");
    m_media->addItem("Mute Volume");

    m_shortcut_details = new QStackedWidget;
    m_file_dialog_btn = new QPushButton("Click to select a file", m_media);
    
    // m_file_dialog_btn->setFixedHeight(50);
    connect(m_file_dialog_btn, &QPushButton::clicked, this, &CreateShortcutWindow::openFileDialog);

    QWidget* centered_btn = Transform::centerWidget(m_file_dialog_btn, m_shortcut_details);
    QWidget* centered_media = Transform::centerWidget(m_media, m_shortcut_details);

    m_shortcut_details->addWidget(m_action);
    m_shortcut_details->addWidget(centered_media);
    m_shortcut_details->addWidget(centered_btn);

    m_shortcut_details->setCurrentIndex(0);
}

bool CreateShortcutWindow::isAlreadyPressed(int button_id)
{
    return m_already_pressed_btns.at(log2(button_id));
}

void CreateShortcutWindow::resetButtonSequence()
{
    m_btn_seq.clear();
    m_btn = 0;
    // Tells the user that the program is waiting for a button input from the gamepad.
    m_gamepad_btn_seq->setTextColor(QColor(150, 150, 150));
    m_gamepad_btn_seq->setText("Click here to register a button sequence from your gamepad.\nRelease all buttons to reset.");
    std::fill(m_already_pressed_btns.begin(), m_already_pressed_btns.end(), 0);
}

void CreateShortcutWindow::registerButtonSequence()
{
    // Only register a button sequence if the text field has been selected / is in focus.
    if (m_gamepad_btn_seq->hasFocus()) {
        QString current_text = m_gamepad_btn_seq->toPlainText();
        if (!current_text.isEmpty() && current_text.back() == '.') {
            current_text = "";
            m_gamepad_btn_seq->setText(current_text);
        } else {
            current_text += " + ";
        }

        XINPUT_STATE state;
        // this receives the sum of all currently pressed buttons.
        XInputGetState(m_device_id, &state);
        // auto btn_id = state.Gamepad.wButtons;
        if (state.Gamepad.wButtons != 0) {
            m_gamepad_btn_seq->setTextColor(QColor(0, 0, 0));
            if (state.Gamepad.wButtons > m_btn) {
                auto new_button = state.Gamepad.wButtons - m_btn;
                if (!isAlreadyPressed(new_button)) {
                    current_text += InputManager::translateButtonFromIdToName(new_button);
                    m_gamepad_btn_seq->setText(current_text);
                }
            }
            m_btn = state.Gamepad.wButtons;
        } else {
            resetButtonSequence();
        }
        for (unsigned int i = 0; i < m_already_pressed_btns.size(); ++i) {
            if (m_already_pressed_btns[i] == 0) {
                bool pressed = (m_btn & (int)pow(2, i)) != 0;
                m_already_pressed_btns[i] = (m_btn & (int)pow(2, i)) != 0;
            }
        }
    } else {
        return;
    }
}

int CreateShortcutWindow::translateButtons()
{
    QString button_sequence_text = m_gamepad_btn_seq->toPlainText();
    QStringList button_list = button_sequence_text.split(QLatin1Char(' '));
    for (const auto& m_btn : button_list) {
        if (m_btn != "+") {
            DWORD l_btn_id = InputManager::translateButtonFromNameToId(m_btn);
            if (l_btn_id == -1) {
                QMessageBox l_error_msg;
                l_error_msg.setIcon(QMessageBox::Warning);
                l_error_msg.setText("No button sequence has been registered.");
                l_error_msg.exec();
                return -1;
            } else {
                m_btn_seq.push_back(l_btn_id);
            }
        }
    }
    return 0;
}

void CreateShortcutWindow::bundleResults()
{
    if (translateButtons() == -1) {
        return;
    } else {

        Action l_type = static_cast<Action>(m_action_type_dropdown->currentIndex());
        CustomShortcut* l_shortcut = new CustomShortcut(m_device_id, l_type, m_btn_seq);
        l_shortcut->setActionText(m_action_type_dropdown->currentText());

        if (l_type == Action::Process) {
            l_shortcut->setCommand(command);
        } else if (l_type == Action::TerminalCommand) {
            l_shortcut->setCommand(m_action->toPlainText());
        } else {
            l_shortcut->setCommand(m_media->currentText());
            l_shortcut->setMultimediaCommand(m_media->currentIndex());
        }
        emit resultReady(l_shortcut);
        closeWindow();
    }
}

void CreateShortcutWindow::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
        "/home",
        "Executables (*.exe)");
    if (fileName != "") {
        QStringList split_fileName = fileName.split("/");
        m_file_dialog_btn->setText(*(split_fileName.end() - 1));
    }
    command = fileName;
}

void CreateShortcutWindow::closeWindow()
{
    emit closed();
    this->close();
}


#include "moc_CreateShortcutWindow.cpp"