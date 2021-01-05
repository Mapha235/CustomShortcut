#pragma once

#include <QGroupBox>
#include <QPushButton>

#include <vector>

class Profiles : public QGroupBox {

public:
    Profiles();
    ~Profiles();

    void initUI();

private:
    std::vector<QPushButton*> profiles;
};