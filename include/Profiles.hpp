#pragma once

#include <QGroupBox>
#include <QPushButton>
#include <QString>

#include <vector>
class Profiles : public QGroupBox {
    Q_OBJECT
public:
    Profiles();
    ~Profiles();

    void initUI();
    void signalHandler();

    void setCurrentProfile(int profile_id);
    int getCurrentProfile();

    void setDefaultColor(unsigned int profile_id);
    /*Highlights a button with the specified color.*/
    void highlight(unsigned int profile_id, QString& color);

    QPushButton* getSaveButton();
signals:
    // void profileChanged(QString& file_name);
    void profileChanged(int profile_id);
    void newProfile(QString& file_name);

public slots:

    void handleProfile(unsigned int profile_id);
    /* Changes the behavior of the buttons when clicked. 
       Clicking a buttons will map a new save file to the clicked button or cancel saving.*/
    void changeMode();

private slots:
    /* @param profile_id: 0 <= profile_id <= 4 */
    void saveProfile(unsigned int profile_id);
    /* @param profile_id: 0 <= profile_id <= 4 */
    void loadProfile(unsigned int profile_id);

private:
    std::vector<QPushButton*> m_profiles;
    std::vector<bool> m_profile_used;
    QPushButton* m_save_btn;

    bool m_is_highlighted;
    bool m_is_save_mode;
    /* 1 <= m_current_profile <= 5 */
    int m_current_profile;
};