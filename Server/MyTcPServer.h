#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <list>

class MyTcPServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcPServer(int port, QObject *parent = nullptr);

    bool isStarted() const;

    void sendToAll(QString msg);


signals:

    void newClientConnected();

    void clientDisconnect();

    void dataReceived(QString msg);

private slots:

    void on_client_Connecting();

    void clientDataReady();

    void clientDisconnected();



private:
    QTcpServer * _server;
    QList<QTcpSocket *>_socketsList;
    bool _isStarted;
};

#endif // MYTCPSERVER_H
