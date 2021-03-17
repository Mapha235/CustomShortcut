#include "Profiles.hpp"

#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QStringLiteral>

Profiles::Profiles()
    : m_is_save_mode(false)
    , m_is_highlighted(false)
    , m_current_profile(-1)
{
    this->setTitle(QString("Profiles"));
    this->setAlignment(Qt::AlignHCenter);

    m_save_btn = new QPushButton("Save");
    m_save_btn->setMaximumWidth(40);

    m_profiles.reserve(5);
    m_profile_used.reserve(5);

    //Create 5 buttons, one for each profile
    for (int i = 0; i < 5; ++i) {
        m_profiles.push_back(new QPushButton(QString::number(i + 1)));
        m_profile_used.push_back(false);
    }

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
        if (!m_profile_used[i])
            // profile->setEnabled(false);

            layout->addWidget(profile, i, 0);
        i++;
    }

    this->setLayout(layout);
}

void Profiles::signalHandler()
{
    for (unsigned int i = 0; i < 5; ++i) {
        connect(m_profiles[i], &QPushButton::clicked, [&, i]() { changeProfile(i); });
    }
    connect(m_save_btn, &QPushButton::clicked, this, &Profiles::changeMode);
}

void Profiles::setCurrentProfile(int profile_id)
{
    if (m_current_profile != profile_id) {
        m_current_profile = profile_id;
        // changeProfile();
    }
}

int Profiles::getCurrentProfile()
{
    return m_current_profile;
}

void Profiles::setDefaultColor(unsigned int profile_id)
{
    m_profiles[profile_id]->setStyleSheet("QPushButton {color: black;}");
}

void Profiles::highlight(unsigned int profile_id, QString& color)
{
    QString stylesheet = QString("QPushButton {color: ");
    stylesheet.append(color);
    stylesheet.append(QString("; font-weight: bold; text-decoration: overline underline;}"));
    m_profiles[profile_id]->setStyleSheet(stylesheet);
}

QPushButton* Profiles::getSaveButton()
{
    return m_save_btn;
}

void Profiles::saveProfile(unsigned int profile_id)
{
    qDebug() << "Saved to: " << profile_id;

    m_profile_used[profile_id] = true;

    for (unsigned int i = 0; i < 5; ++i) {
        m_profiles[i]->disconnect();

        // highlight(profile_id, QString("#00BFFF"));
        // highlight(profile_id, QString("#4169e1"));

        if (i != profile_id)
            setDefaultColor(i);

        connect(m_profiles[i], &QPushButton::clicked, [&, i]() { changeProfile(i); });
    }
    QString l_file_name("profile");
    l_file_name.append(QString::number(profile_id+1));
    l_file_name.append(QString(".json"));
    emit newProfile(l_file_name);
}

bool Profiles::loadProfile(unsigned int profile_id)
{
    QString l_file_name("profile");
    l_file_name.append(QString::number(profile_id+1));
    l_file_name.append(QString(".json"));

    QFile loadFile(l_file_name);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    return false;
}

void Profiles::changeProfile(unsigned int profile_id)
{
    // m_profiles[profile_id]->setStyleSheet("QPushButton {color: black; font-weight: bold; text-decoration: overline underline;}");
    highlight(profile_id, QString("black"));
    // m_profiles[m_current_profile - 1]->setStyleSheet("QPushButton {color: black;}");

    for (int i = 0; i < 5; ++i) {
        if (i != profile_id)
            setDefaultColor(i);
    }

    m_current_profile = profile_id;

    emit profileChanged(profile_id);
}

void Profiles::changeMode()
{
    m_is_save_mode = !m_is_save_mode;
    if (m_is_save_mode)
        m_save_btn->setText("Cancel");
    else {
        m_save_btn->setText("Save");
    }

    for (unsigned int i = 0; i < m_profiles.size(); ++i) {

        highlight(i, QString("black"));
        m_profiles[i]->disconnect();

        connect(m_profiles[i], &QPushButton::clicked, [&, i]() { saveProfile(i); });
    }
}

#include "moc_Profiles.cpp"