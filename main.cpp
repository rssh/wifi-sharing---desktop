#include <QtGui/QApplication>
#include "mainwindow.h"
#include <iostream>


void msgHandler(QtMsgType type, const char *msg);

int main(int argc, char *argv[])
{
    qInstallMsgHandler(msgHandler);
    QApplication a(argc, argv);

    MainWindow w;

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        w.showTrayIcon();
    } else {
        w.show();
    }

    int retval;
    while(! w.isDone()) {
       retval = a.exec();
    }
    return retval;
}

void msgHandler(QtMsgType type, const char *msg)
{
    switch (type) {
      case QtDebugMsg:
        std::cerr << "Debug: " <<  msg << std::endl;
        break;
    case QtWarningMsg:
        std::cerr << "Warning: " <<  msg << std::endl;;
        break;
    case QtCriticalMsg:
        std::cerr << "Critical: " << msg << std::endl;
        break;
    case QtFatalMsg:
        std::cerr << "Fatal: " << msg << std::endl;
        abort();
    }
}
