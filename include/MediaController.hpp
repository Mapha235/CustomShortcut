#pragma once
#include <QProcess>
#include <QObject>
#include <QString>
#include <QStringList>
#include <string>
#include <endpointvolume.h>
#include <windows.h>

class MediaController : public QObject{
    Q_OBJECT
public:
    MediaController();
    ~MediaController();

// public slots:
    void play();
    void pause();
    void skip();
    void volumeDown(float value);
    void volumeUp(float value);
};