#pragma once

#include <QGroupBox>
#include <QPushButton>
#include <QString>

#include <vector>

enum SaveFormat {
    Json,
    Binary
};

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
    bool saveProfile(unsigned int profile_id, SaveFormat format);
    bool loadProfile(unsigned int profile_id, SaveFormat format);
    void changeProfile(unsigned int profile_id);


private:
    std::vector<QPushButton*> m_profiles;
    QPushButton* m_save_btn;
};