#include <QApplication>

#include "mainwindow.hpp"

int main(int argc, char** argv)
{
    XINPUT_KEYSTROKE key;
    // while (true) {
    //     auto hh = XInputGetKeystroke(0, 0, &key);
    //     if(key.VirtualKey != 0 && key.Flags != XINPUT_KEYSTROKE_KEYDOWN )
    //         std::cout << key.VirtualKey << std::endl;
    // }

    QApplication app(argc, argv);

    MainWindow* window = MainWindow::getInstance();
    window->show();
    return app.exec();
}