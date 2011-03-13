#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QtGui/QApplication>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createTrayIcon();

    void showTrayIcon();

private slots:

    /**
     * called when system tray icon is activated.
     **/
    void isActivated(QSystemTrayIcon::ActivationReason);

    void isActivated()
    { isActivated(QSystemTrayIcon::Unknown); }

    void quit()
    { QApplication::quit(); }


protected:

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    QSystemTrayIcon* trayIcon;
};

#endif // MAINWINDOW_H
