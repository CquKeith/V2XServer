#ifndef SERVER_H
#define SERVER_H
#include <QList>

#include <QObject>
#include <QTcpServer>
#include "tcpclientsocket.h"



class TcpClientSocket;
class Server : public QTcpServer
{
     Q_OBJECT  //添加Q_OBJECT宏用于信号的槽的通信
public:
    Server(QObject *parent,int port,int includeSender=0,int heartBeatTime=120);
    //用于与每一个客户端连接的TcpClientSock
     QList<TcpClientSocket *> TcpClientSocketList;


protected:
    //重载 TcpServer中的这个虚函数，当新连进来一个客户端时自己调用
    void incomingConnection(qintptr socketDescriptor );
private:
    int port;
    int include_sender;
    int heart_beat_time;

private slots:
    void slotDisconnected(int description);

    void sendToAllClient(QByteArray ba,int socketDesc);

};

#endif // SERVER_H
