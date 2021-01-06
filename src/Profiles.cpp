#include "Profiles.hpp"

#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QStringLiteral>

Profiles::Profiles()
{
    this->setTitle(QString("Profiles"));
    this->setAlignment(Qt::AlignHCenter);
    // this->setContentsMargins(3, 3, 3, 3);

    m_save_btn = new QPushButton("Save");

    m_profiles.reserve(5);
    //Create 5 buttons, one for each profile
    for (int i = 0; i < 5; ++i) {
        m_profiles.push_back(new QPushButton(QString::number(i + 1)));
    }

    initUI();
}

Profiles::~Profiles()
{
    delete m_save_btn;

    for (auto profile : m_profiles)
        delete profile;
    m_profiles.clear();
}

void Profiles::initUI()
{
    QGridLayout* layout = new QGridLayout;

    int i = 0;
    for (const auto profile : m_profiles) {
        profile->setEnabled(false);
        layout->addWidget(profile, i, 0);
        i++;
    }

    this->setLayout(layout);
}

void Profiles::signalHandler()
{
    // for (auto profile : m_profiles) {
    //     connect(profile, &QPushButton::clicked, [] { emit })
    // }

    for (unsigned int i = 0; i < 5; ++i) {
        connect(m_profiles[i], &QPushButton::clicked, [&]() { changeProfile(i); });
    }
}

void Profiles::changeProfile(unsigned int profile_id)
{
    QString l_file_name("profile");
    l_file_name.append(QString::number(profile_id));
    l_file_name.append(QString(".json"));
    emit profileChanged(l_file_name);
}

bool Profiles::saveProfile(unsigned int profile_id, SaveFormat format)
{
    return false;
}

bool Profiles::loadProfile(unsigned int profile_id, SaveFormat format)
{
    QString l_file_name("profile");
    l_file_name.append(QString::number(profile_id));
    l_file_name.append(format == Json ? QString(".json") : QString(".dat"));

    QFile loadFile(l_file_name);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    return false;
}

#include "moc_Profiles.cpp"