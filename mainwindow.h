#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QtGui/QApplication>
#include <QNetworkRequest>

#include <HUpnpCore/HControlPoint>


#include "upnpdeviceslistmodel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showTrayIcon();

    bool isDone() const
    { return done; }

private slots:

    void foreground();

    void quit();

    void rootDeviceOnline(Herqq::Upnp::HClientDevice*);

    void rootDeviceOffline(Herqq::Upnp::HClientDevice*);

    void scanDevices();

    void rescanDevices(bool onlyMobile);

    void activatedDeviceEntry(const QModelIndex & index);


protected:

    void closeEvent(QCloseEvent *event);

private:

    void createTrayIcon();

    void startUpnpMonitoring();



private:
    Ui::MainWindow *ui;

    QSystemTrayIcon* trayIcon;

    Herqq::Upnp::HControlPoint* upnpControlPoint;
    UpnpDevicesListModel*       upnpRootDevices;


    volatile bool done;



};

#endif // MAINWINDOW_H
