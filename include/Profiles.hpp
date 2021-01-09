#pragma once

#include <QGroupBox>
#include <QPushButton>
#include <QString>
#include <QTimer>

#include <vector>
class Profiles : public QGroupBox {
    Q_OBJECT
public:
    Profiles();
    ~Profiles();

    void initUI();
    void signalHandler();

signals:
    void profileChanged(QString file_name);

public slots:
    bool saveProfile(unsigned int profile_id);
    bool loadProfile(unsigned int profile_id);
    void changeProfile(unsigned int profile_id);

    /*Changes the behavior of the buttons when clicked.*/
    void changeToSaveMode();


    void blink();

private:
    std::vector<QPushButton*> m_profiles;
    std::vector<bool> m_profile_used;
    QPushButton* m_save_btn;

    QTimer* blinker;
    bool m_is_save_mode;
};