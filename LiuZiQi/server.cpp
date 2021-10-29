#include "server.h"

Server::Server()
{
    listen(QHostAddress::Any,8899);
}

void Server::incomingConnection(qintptr handle)
{
    qDebug()<<"******";
    sock = new QTcpSocket(this);
    sock->setSocketDescriptor(handle);
    connect(sock,&QTcpSocket::readyRead,this,&Server::Rec);
}

void Server::Rec()
{
    char buffer[1024];
    if(sock->bytesAvailable() > 0)
    sock->read(buffer,sock->bytesAvailable());
    for(int i = 0;buffer[i];i++)
    qDebug()<<(int)buffer[i] - 97;
}
