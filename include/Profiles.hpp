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

    /* Enables the button to be clickable when the corresponding profile has been detected. */
    void enableBtn(unsigned int nr);
signals:
    // void profileChanged(QString& file_name);
    void profileChanged(int profile_id);
    void newProfile(QString& file_name);

public slots:
    /*@param profile_id: 1 <= profile_id <= 5*/
    void saveProfile(unsigned int profile_id);
    /*@param profile_id: 1 <= profile_id <= 5*/
    bool loadProfile(unsigned int profile_id);
    /*@param profile_id: 1 <= profile_id <= 5*/
    void changeProfile(unsigned int profile_id);

    /* Changes the behavior of the buttons when clicked. 
       Clicking a buttons will map a new save file to the clicked button.*/
    void changeToSaveMode();

    void blink();

private:
    std::vector<QPushButton*> m_profiles;
    std::vector<bool> m_profile_used;
    QPushButton* m_save_btn;

    QTimer* blinker;
    bool m_is_save_mode;
};