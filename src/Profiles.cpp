#include "Profiles.hpp"

#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QStringLiteral>

Profiles::Profiles()
    : m_is_save_mode(false)
{
    this->setTitle(QString("Profiles"));
    this->setAlignment(Qt::AlignHCenter);
    // this->setContentsMargins(3, 3, 3, 3);

    m_save_btn = new QPushButton("Save");

    m_profiles.reserve(5);
    m_profile_used.reserve(5);

    //Create 5 buttons, one for each profile
    for (int i = 0; i < 5; ++i) {
        m_profiles.push_back(new QPushButton(QString::number(i + 1)));
        m_profile_used.push_back(false);
    }

    blinker = new QTimer();

    initUI();
    signalHandler();
}

Profiles::~Profiles()
{
    delete m_save_btn;

    for (auto profile : m_profiles)
        delete profile;
    m_profiles.clear();
    m_profile_used.clear();
}

void Profiles::initUI()
{
    QGridLayout* layout = new QGridLayout;

    int i = 0;
    for (const auto profile : m_profiles) {
        if(!m_profile_used[i])
            profile->setEnabled(false);

        layout->addWidget(profile, i, 0);
        i++;
    }

    this->setLayout(layout);
}

void Profiles::signalHandler()
{
    for (unsigned int i = 0; i < 5; ++i) {
        connect(m_profiles[i], &QPushButton::clicked, [&, i]() { changeProfile(i + 1); });
    }
    connect(blinker, &QTimer::timeout, this, &Profiles::blink);
}

void Profiles::changeProfile(unsigned int profile_id)
{
    QString l_file_name("profile");
    l_file_name.append(QString::number(profile_id));
    l_file_name.append(QString(".json"));
    // emit profileChanged(l_file_name);
    qDebug() << profile_id;
}

bool Profiles::saveProfile(unsigned int profile_id)
{
    qDebug() << "Saved to: " << profile_id;
    m_profile_used[profile_id] = true;
    
    for(unsigned int i = 0; i < 5; ++i){
        if(!m_profile_used[i])
            m_profiles[i]->setEnabled(false);
    }

    return false;
}

bool Profiles::loadProfile(unsigned int profile_id)
{
    QString l_file_name("profile");
    l_file_name.append(QString::number(profile_id));
    l_file_name.append(QString(".json"));

    QFile loadFile(l_file_name);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    return false;
}

void Profiles::changeToSaveMode()
{
    m_is_save_mode = true;

    bool ok;
    for (auto profile : m_profiles) {
        profile->setEnabled(true);
        profile->disconnect();
        int l_id = profile->text().toInt(&ok) - 1;

        if (!ok)
            qDebug() << "QString to Int Conversion failed.";
        
        connect(profile, &QPushButton::clicked, [=]() { saveProfile(l_id); });
    }
}

void Profiles::blink()
{
    // this->palette()->setColor(QPalette::Button, (m_buttonState) ? m_color1 : m_color2);
    update();
}

#include "moc_Profiles.cpp"