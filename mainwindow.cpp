#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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
    trayIcon -> setToolTip(tr("MDS"));

    //connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
    //                         SLOT(isActivated(QSystemTrayIcon::ActivationReason)));

    QMenu* menu = new QMenu();

    QAction* open = new QAction(tr("Open"),this);
    open->setStatusTip(tr("Open main window"));
    connect(open,SIGNAL(triggered()),this,SLOT(isActivated()));
    menu->addAction(open);

    QAction* quit = new QAction(tr("Quit"),this);
    connect(quit, SIGNAL(triggered()), this, SLOT(quit()));
    menu->addAction(quit);

    trayIcon->setContextMenu(menu);

}


void MainWindow::isActivated(QSystemTrayIcon::ActivationReason activationReason) {
    this->show();
}


void MainWindow::closeEvent(QCloseEvent *event)
 {
     if (trayIcon->isVisible()) {
         QMessageBox::information(this, tr("Systray"),
                                  tr("The program will keep running in the "
                                     "system tray. To terminate the program, "
                                     "choose <b>Quit</b> in the context menu "
                                     "that pops up when clicking this program's "
                                     "entry in the system tray."));
         hide();
         event->ignore();
     }
 }




