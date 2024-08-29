#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _server = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnstart_clicked()
{
    if(_server == nullptr)
    {
        auto port = ui->spnserverport->value();
        _server = new MyTcPServer(port);
        connect(_server,&MyTcPServer::newClientConnected,this,&MainWindow::newClientConnected);
        connect(_server,&MyTcPServer::dataReceived,this,&MainWindow::clientDataReceived);
        connect(_server,&MyTcPServer::clientDisconnect,this,&MainWindow::clientDisconnected);
    }
    auto state = (_server->isStarted()? "1" : "0");
    ui->color->setProperty("state",state);
    style()->polish(ui->color);
}

void MainWindow::newClientConnected()
{
    ui->lstConsole->addItem("New Client Connected");
}


void MainWindow::on_btnclear_clicked()
{
    ui->lstConsole->clear();
}


void MainWindow::on_btnsend_clicked()
{
    auto msg = ui->lnmsg->text().trimmed();
    _server->sendToAll(msg);
}

void MainWindow::clientDisconnected()
{
    ui->lstConsole->addItem("Client Disconnected");
}

void MainWindow::clientDataReceived(QString msg)
{
    ui->lstConsole->addItem(msg);
}

