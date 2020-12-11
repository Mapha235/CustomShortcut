#pragma once

#include <QComboBox>
#include <QGridLayout>

#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QKeyEvent>
#include <QStackedWidget>

#include <vector>
#include <windows.h>
#include <xinput.h>

#include "CustomShortcut.hpp"
#include "Device.hpp"

class CreateShortcutWindow : public QWidget {
    Q_OBJECT
public:
    CreateShortcutWindow(DWORD device_id, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CreateShortcutWindow();
    /*Connects all used signals with their corresponding slots.*/
    void signalHandler();
    /*Creates the window's layout.*/
    void initUI();
    /*Fills the m_shortcut_details QStackedWidget with the necessary options.*/
    void initShortcutDetails();

    void resetButtonSequence();
    bool isAlreadyPressed(int button_id);
    void bundleResults();

    void openFileDialog();
signals:
    void resultReady(CustomShortcut* shortcut);
    void closed();
public slots:
    void registerButtonSequence();
    int translateButtons();
    void closeWindow();

protected:
    void keyPressEvent(QKeyEvent* key);

private:
    // UI members
    QGridLayout* m_layout;
    QPushButton* m_add_btn;
    QPushButton* m_cancel_btn;
    QComboBox* m_action_type_dropdown;
    QTextEdit* m_gamepad_btn_seq;

    QStackedWidget *m_shortcut_details;
    QTextEdit* m_action;
    QComboBox *m_media;
    QPushButton *m_file_dialog_btn;

    // functional members
    Device* m_device;
    DWORD m_device_id;
    QTimer* m_listener;
    std::vector<DWORD> m_btn_seq;
    DWORD m_btn;
    //TODO use m_deqe
    std::vector<bool> m_already_pressed_btns;
    QString command;
};