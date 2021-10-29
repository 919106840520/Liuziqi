#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <qtcpsocket.h>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
public slots:
    void incomingConnection(qintptr handle);
    void Rec();
private:
    QTcpSocket *sock;
};

#endif // SERVER_H
