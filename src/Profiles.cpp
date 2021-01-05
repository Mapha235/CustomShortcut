#include "Profiles.hpp"

#include <QString>
#include <QVBoxLayout>

Profiles::Profiles()
{
    this->setTitle(QString("Profiles"));
    //Create 5 buttons, one for each profile
    for (int i = 0; i < 5; ++i) {
        profiles.push_back(new QPushButton(QString::number(i + 1)));
    }
}

Profiles::~Profiles()
{
    for (auto profile : profiles)
        delete profile;
    profiles.clear();
}

void Profiles::initUI()
{
    QVBoxLayout* layout = new QVBoxLayout;

    for (const auto profile : profiles) {
        profile->setEnabled(false);
        // profile->setStyleSheet(QString::fromUtf8("QPushButton:disabled"
        //                                          "{ color: gray }"));
        layout->addWidget(profile);
    }
}

this->setLayout(layout);
}