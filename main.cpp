#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        w.showTrayIcon();
    } else {
        w.show();
    }


    return a.exec();
}
