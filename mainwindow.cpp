#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyle>
#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setDeviceController();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_IPAddress_textChanged(const QString &arg1)
{
    QString state = "0";
    if(arg1 == "...")
    {
        state = "";
    }
    else
    {
        QHostAddress address(arg1);
        QString state = "0";
        if(QAbstractSocket::IPv4Protocol == address.protocol());
        {
            state = "1";
        }
    }
    ui->IPAddress->setProperty("state",state);
    style()->polish(ui->IPAddress);
}


void MainWindow::on_btnconnect_clicked()
{
    if(_controller.state() == QAbstractSocket::ConnectedState)
    {
        _controller.disconnect();
    }
    else
    {
        auto ip = ui->IPAddress->text();
        auto port = ui->spinBox->value();
        _controller.connectToDevice(ip,port);
    }

}

void MainWindow::device_connected()
{
    ui->lstConsole->addItem("Connected to Device");
    ui->btnconnect->setText("Disconnect");
    ui->groupBox_2->setEnabled(true);
}

void MainWindow::device_disconnected()
{
    ui->lstConsole->addItem("Disconnected from Device");
    ui->btnconnect->setText("Connect");
    ui->groupBox_2->setEnabled(false);
}

void MainWindow::device_stateChanged(QAbstractSocket::SocketState state)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    ui->lstConsole->addItem(metaEnum.valueToKey(state));
}

void MainWindow::device_errorOccurred(QAbstractSocket::SocketError error)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    ui->lstConsole->addItem(metaEnum.valueToKey(error));
}

void MainWindow::setDeviceController()
{
    connect(&_controller,&DeviceController::connected,this,&MainWindow::device_connected);
    connect(&_controller,&DeviceController::disconnected,this,&MainWindow::device_disconnected);
    connect(&_controller,&DeviceController::stateChanged,this,&MainWindow::device_stateChanged);
    connect(&_controller,&DeviceController::errorOccurred,this,&MainWindow::device_errorOccurred);
    connect(&_controller,&DeviceController::dataReady,this,&MainWindow::device_dataReady);
}


void MainWindow::on_btnclear_clicked()
{
    ui->lstConsole->clear();
}

void MainWindow::device_dataReady(QByteArray data)
{
    ui->lstConsole->addItem(QString(data));
}


void MainWindow::on_btnsend_clicked()
{
    auto msg = ui->msgsend->text().trimmed();
    _controller.send(msg);
}

