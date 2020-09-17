#pragma once
#include <QWidget>
#include <QHBoxLayout>

class CustomShortcut : public QWidget{
public:
    CustomShortcut();
    ~CustomShortcut();
    void initUI();
private:
    QHBoxLayout *layout;
};