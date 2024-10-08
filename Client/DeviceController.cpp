#include "DeviceController.h"
#include <_mingw_mac.h>

DeviceController::DeviceController(QObject *parent)
    : QObject{parent}
{
    connect(&_socket,&QTcpSocket::connected,this,&DeviceController::connected);
    connect(&_socket,&QTcpSocket::disconnected,this,&DeviceController::disconnected);
    connect(&_socket,&QTcpSocket::errorOccurred,this,&DeviceController::errorOccurred);
    connect(&_socket,&QTcpSocket::stateChanged,this,&DeviceController::socket_stateChanged);
    connect(&_socket,&QTcpSocket::readyRead,this,&DeviceController::socket_readyRead);
}
void DeviceController::connectToDevice(QString ip, int port)
{
    if(_socket.isOpen())
    {
        if(ip == _ip && port == _port)
        {
            return;
        }
        _socket.close();
    }
    _ip = ip;
    _port = port;
    _socket.connectToHost(_ip,_port);
}

QAbstractSocket::SocketState DeviceController::state()
{
    return _socket.state();
}

void DeviceController::disconnect()
{
    _socket.close();
}

void DeviceController::socket_stateChanged(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState)
    {
        _socket.close();
    }
    emit stateChanged(state);
}

void DeviceController::socket_readyRead()
{
    auto data = _socket.readAll();
    emit dataReady(data);
}

void DeviceController::send(QString msg)
{
    _socket.write(msg.toUtf8());
}
