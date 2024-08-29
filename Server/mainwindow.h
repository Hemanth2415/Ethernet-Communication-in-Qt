#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MyTcPServer.h"
#include <QStyle>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnstart_clicked();

    void newClientConnected();

    void on_btnclear_clicked();

    void on_btnsend_clicked();

    void clientDisconnected();

    void clientDataReceived(QString msg);

private:
    Ui::MainWindow *ui;
    MyTcPServer *_server;
};
#endif // MAINWINDOW_H
