#include "Profiles.hpp"

#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QStringLiteral>

Profiles::Profiles()
    : m_is_save_mode(false)
    , m_is_highlighted(false)
    , m_current_profile(0)
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
        m_profiles[i]->setContextMenuPolicy(Qt::CustomContextMenu);
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
        connect(m_profiles[i], &QPushButton::clicked, [&, i]() { handleProfile(i); });
    }
    connect(m_save_btn, &QPushButton::clicked, this, &Profiles::changeMode);
}

void Profiles::setCurrentProfile(int profile_id)
{
    if (m_current_profile != profile_id) {
        m_current_profile = profile_id;
        // loadProfile();
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
    qDebug() << "Saved to: " << profile_id + 1;
    QString l_file_name("./profiles/profile");
    l_file_name.append(QString::number(profile_id + 1));
    l_file_name.append(QString(".json"));
    emit newProfile(l_file_name);
}

void Profiles::loadProfile(unsigned int profile_id)
{
    highlight(profile_id, QString("black"));
    emit profileChanged(profile_id);
}

void Profiles::handleProfile(unsigned int profile_id)
{
    if (m_is_save_mode) {
        for (int i = 0; i < 5; ++i) {
            if (i != profile_id)
                setDefaultColor(i);
        }
        m_is_save_mode = false;

        m_save_btn->setText("Save");

        saveProfile(profile_id);
    } else {
        setDefaultColor(m_current_profile);
        loadProfile(profile_id);
    }
    m_current_profile = profile_id;
}

void Profiles::changeMode()
{
    m_is_save_mode = !m_is_save_mode;
    if (m_is_save_mode) {
        m_save_btn->setText("Cancel");
        for (unsigned int i = 0; i < m_profiles.size(); ++i) {
            highlight(i, QString("black"));
        }
    } else {
        m_save_btn->setText("Save");
        for (unsigned int i = 0; i < m_profiles.size(); ++i) {
            if (i != m_current_profile)
                setDefaultColor(i);
        }
    }
}

#include "moc_Profiles.cpp"