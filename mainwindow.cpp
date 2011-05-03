#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QListWidget>
#include <QUrl>
#include <QtNetwork/QNetworkReply>
#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HClientService>
#include <HUpnpCore/HClientDevice>
#include <HUpnpCore/HServiceInfo>
#include <HUpnpCore/HServiceId>
#include <HUpnpCore/HDiscoveryType>
#include <QDesktopServices>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    done=false;
    createTrayIcon();
    startUpnpMonitoring();
}

MainWindow::~MainWindow()
{
    if (ui!=NULL) {
        delete ui;
    }
    if (trayIcon!=NULL) {
        delete trayIcon;
    }
    if (upnpControlPoint!=NULL) {
        delete upnpControlPoint;
    }
}

void MainWindow::showTrayIcon()
{
    trayIcon -> show();
}

void MainWindow::createTrayIcon()
{

    trayIcon = new QSystemTrayIcon(QIcon(":/images/mds_icon_22x22.png"));
    trayIcon -> setToolTip(tr("WiFi share"));

    //connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
    //                         SLOT(isActivated(QSystemTrayIcon::ActivationReason)));

    QMenu* menu = new QMenu();

    QAction* scan = new QAction(tr("Scan"),this);
    scan->setStatusTip(tr("Scan network devices"));
    connect(scan,SIGNAL(triggered()),this,SLOT(scanDevices()));
    menu->addAction(scan);

    QAction* open = new QAction(tr("Open"),this);
    open->setStatusTip(tr("Open main window"));
    connect(open,SIGNAL(triggered()),this,SLOT(foreground()));
    menu->addAction(open);

    QAction* quit = new QAction(tr("Quit"),this);
    connect(quit, SIGNAL(triggered()), this, SLOT(quit()));
    menu->addAction(quit);

    trayIcon->setContextMenu(menu);

}


void MainWindow::foreground()
{
    show();
    raise();
    activateWindow();
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
     if (trayIcon->isVisible()) {
         hide();
         event->ignore();
     }
 }

void MainWindow::quit()
{
    done=true;
    upnpControlPoint->quit();
    QApplication::quit();
}


void MainWindow::startUpnpMonitoring()
{
  //Herqq::HLogger::setTraceLevel(10);
  //Herqq::Upnp::SetLoggingLevel(Herqq::Upnp::Debug);
  upnpControlPoint = new Herqq::Upnp::HControlPoint(this);
  connect(upnpControlPoint,SIGNAL(rootDeviceOnline(Herqq::Upnp::HClientDevice*)),this,SLOT(rootDeviceOnline(Herqq::Upnp::HClientDevice*)));
  connect(upnpControlPoint,SIGNAL(rootDeviceOffline(Herqq::Upnp::HClientDevice*)),this,SLOT(rootDeviceOffline(Herqq::Upnp::HClientDevice*)));
  if (!upnpControlPoint->init()) {
      QMessageBox::warning(this,tr("Can't start upnp"),upnpControlPoint->errorDescription(),QMessageBox::Ok);
      quit();
  }
  upnpRootDevices = new UpnpDevicesListModel();
  ui->listView->setModel(upnpRootDevices);

  // connect actions on clik on list view
  connect(ui->listView,SIGNAL(activated(const QModelIndex&)),this,SLOT(activatedDeviceEntry(const QModelIndex&)));

  //connect options
  connect(ui->onlyPhonesRadioButton,SIGNAL(toggled(bool)),this,SLOT(rescanDevices(bool)));


}


void MainWindow::rootDeviceOnline(Herqq::Upnp::HClientDevice* device)
{
    //std::cerr << "receive root device:" << device->info().friendlyName().toAscii().data() <<
    //             ", embedded size:" << device->embeddedDevices().count() << std::endl;
    //std::cerr << "descrption" << device->description().toUtf8().data() << std::endl;
    upnpRootDevices->addIfApply(device);
}



void MainWindow::rootDeviceOffline(Herqq::Upnp::HClientDevice* device)
{
    upnpRootDevices->remove(device);
}


void MainWindow::scanDevices()
{
    upnpControlPoint->scan(Herqq::Upnp::HDiscoveryType::createDiscoveryTypeForAllResources());
}

void MainWindow::rescanDevices(bool onlyMobile)
{
    upnpRootDevices->clear();
    upnpRootDevices->setOnlyMobile(onlyMobile);
    upnpControlPoint->scan(Herqq::Upnp::HDiscoveryType::createDiscoveryTypeForAllResources());
    Herqq::Upnp::HClientDevices devices = upnpControlPoint->rootDevices();
    for(Herqq::Upnp::HClientDevices::const_iterator it = devices.begin(); it!=devices.end(); ++it) {
        upnpRootDevices->addIfApply(*it);
    }

}



void MainWindow::activatedDeviceEntry(const QModelIndex & index)
{
   int ri = index.row();
   if (ri < upnpRootDevices->rowCount()) {
       QUrl presentationUrl = upnpRootDevices->at(ri).deviceInfo().presentationUrl();
       //std::cerr << "presentation Url is: '" << presentationUrl.toString().toUtf8().data() << "'" << std::endl;
       if (presentationUrl.isValid()) {
           QDesktopServices::openUrl(presentationUrl);
       } else {
           if (!presentationUrl.isEmpty()) {
               QMessageBox::information(this,"Url is not valid",
                                    QString("Url for current device:")+presentationUrl.toString()+QString(" is not valid"),
                                    QMessageBox::Ok);
           }
       }
   }
}



