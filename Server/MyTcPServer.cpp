#include "MyTcPServer.h"

MyTcPServer::MyTcPServer(int port, QObject *parent)
    : QObject{parent}
{
    _server = new QTcpServer(this);
    connect(_server,&QTcpServer::newConnection,this,&MyTcPServer::on_client_Connecting);
    _isStarted = _server->listen(QHostAddress::Any,port);
    if(!_isStarted)
    {
        qDebug() << "Server could not connect";
    }
    else
    {
        qDebug() << "Server Started....";
    }
}

bool MyTcPServer::isStarted() const
{
    return _isStarted;
}

void MyTcPServer::sendToAll(QString msg)
{
    foreach(auto socket,_socketsList)
    {
        socket->write(msg.toUtf8());
    }
}

void MyTcPServer::on_client_Connecting()
{
    qDebug() << "A Client is connected to server";
    auto socket = _server->nextPendingConnection();
    connect(socket,&QTcpSocket::readyRead,this,&MyTcPServer::clientDataReady);
    connect(socket,&QTcpSocket::disconnected,this,&MyTcPServer::clientDisconnected);
    _socketsList.append(socket);
    socket->write("Welcome to this Server");
    emit newClientConnected();
}

void MyTcPServer::clientDataReady()
{
    auto socket = qobject_cast<QTcpSocket *>(sender());
    auto data = QString(socket->readAll());
    emit dataReceived(data);
    foreach(auto s,_socketsList)
    {
        if(s!= socket)
        {
            s->write(data.toUtf8());
        }
    }
}

void MyTcPServer::clientDisconnected()
{
    emit clientDisconnect();
}

