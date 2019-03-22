#include "tcpclientsocket.h"

#include <QDebug>

TcpClientSocket::TcpClientSocket(QObject *parent)

{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    //有客户端断开连接的时候触发
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

//    socketDescriptor = this->socketDescriptor;
    heart_beat_current = HEART_BEAT_NUMBER;
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=]{
        if(--heart_beat_current <= 0){
            timer->stop();
            this->disconnected();
        }
    });
    timer->start(1000);

}

void TcpClientSocket::dataReceived()
{
    /*
     * 继承自 QIODevice 的方法，返回可用数据的长度
    */
   resetHeatBeatTime();
   while(bytesAvailable()>0)
   {
       QByteArray datagram;
       datagram.resize(bytesAvailable());
       read(datagram.data(),datagram.size());

       emit dataToServer(datagram,this->socketDescriptor());

   }

}

void TcpClientSocket::slotDisconnected()
{
    emit signal_discon(this->socketDescriptor());
}

