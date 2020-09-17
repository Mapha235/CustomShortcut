#include "MediaController.hpp"

MediaController::MediaController() {

}

MediaController::~MediaController() {

}


void MediaController::play(){

}
void MediaController::pause(){

}
void MediaController::skip(){

}

// TODO: use IAudioEndpointVolume API (Windows only) -> endpointsvolume.h already imported
void MediaController::volumeDown(float value){
    // QString program = "nircmd";
    // QStringList args;
    double v = 655.35 * -(value);
    // args << "changevolume" << std::to_string(v).c_str();
    // QProcess *volume = new QProcess(this);
    // volume->start(program, args);
    std::string command = "nircmdc changesysvolume ";
    command.append(std::to_string(v));
    system("nircmd mutesysvolume 0");
    system(command.c_str());
}
void MediaController::volumeUp(float value){
    // QString program = "nircmd";
    // QStringList args;
    double v = 655.35 * (value);
    // args << "changevolume" << std::to_string(v).c_str();
    // QProcess *volume = new QProcess(this);
    // volume->start(program, args);
    std::string command = "nircmdc changesysvolume ";
    command.append(std::to_string(v));
    system("nircmd mutesysvolume 0");
    system(command.c_str());
}

#include "moc_MediaController.cpp"
