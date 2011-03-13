#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createTrayIcon();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (trayIcon!=NULL) {
        delete trayIcon;
    }
}

void MainWindow::showTrayIcon()
{
    trayIcon -> show();
}

void MainWindow::createTrayIcon()
{

    trayIcon = new QSystemTrayIcon(QIcon(":/images/mds_icon_22x22.png"));
    //trayIcon -> setTooltip("MDS Tray Icon");
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
                             SLOT(isActivated(QSystemTrayIcon::ActivationReason)));
}


void MainWindow::isActivated(QSystemTrayIcon::ActivationReason activationReason) {
    this->show();
}
