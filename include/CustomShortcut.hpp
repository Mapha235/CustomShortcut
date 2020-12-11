#pragma once

#include <QEventLoop>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <vector>
#include <windows.h>
#include <xinput.h>

enum class Action { 
    // Hotkey = 0,
    TerminalCommand = 0,
    VolumeControl = 1,
    Process = 2
};

class CustomShortcut : public QGroupBox {
    Q_OBJECT
public:
    // This constructor is used only for reading instantiating a CustomShortcut object read from the save file.
    CustomShortcut(const int owner_id, const int id, QWidget* parent = nullptr);
    // This constructor is used when the user creates a new shortcut via the CreateShortcutWindow.
    CustomShortcut(const int owner_id, const Action type, const std::vector<DWORD> btn_seq, QWidget* parent = nullptr);

    ~CustomShortcut();

    void initUI();
    void buttonHandler();

    void change_btn_seq(std::vector<DWORD> seq);

    /*Returns the sum of all button's pressed.
      Used as a   */
    DWORD minThreshold();

    void asignBtnSeq(std::vector<DWORD>& seq);
    void incrementCondition();
    void resetCondition();

    void showFileNameOnly();

    void setActionText(QString action);
    /*Executes the defined shortcut.*/
    void execute();
    /* Checks that the correct buttons were pressed in the correct order. */
    void checkButtonOrder(std::vector<DWORD>& btns_status);

    void setID(const int id);
    /* Returns the personal ID of the shortcut.*/
    unsigned int getID();

    /* Returns the owner device of this shortcut.*/
    unsigned int getOwnerID();

    void setMultimediaCommand(unsigned int nr);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;


protected:
    // bool eventFilter(QObject *obj, QEvent *event) override;
signals:
    void delete_signal(unsigned int nr);

public slots:
    void setCommand(QString command);
    void closeThis();

private:
    //UI members
    unsigned int m_id;
    DWORD m_owner_id;
    QGridLayout* m_layout;
    QLineEdit* m_action_type;
    QLineEdit* m_btn_seq_line;
    QLineEdit* m_command_line;
    QPushButton* m_delete_btn;

    // functional m_members
    Action m_type;
    QString m_command;
    /* Only used if this shortcut is mapped to a multimedia command.*/
    int m_media_type;
    std::vector<DWORD> m_btn_seq;
    DWORD m_threshold;
    unsigned int m_exec_condition;
};