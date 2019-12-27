#include "server.h"
#include "tcpclientsocket.h"
//#include <QMessageBox>
#include <QDebug>
#include <iostream>

using namespace std;
/**
 * @brief Server::Server
 * @param parent
 * @param port
 * @param includeSender 发送时是否包括发送者本身 0:不包括 1:包括
 * @param heartBeatTime 多少秒没有活动 则自动关闭连接
 */
Server::Server(QObject *parent, int port, int includeSender, int heartBeatTime) :
    QTcpServer(parent)
{
    this->port = port;

    this->include_sender = includeSender;
    this->heart_beat_time = heartBeatTime;


    if(!this->listen(QHostAddress::AnyIPv4,this->port))
    {
        qDebug() << "无法启动服务器：" << this->errorString();

    }else{
        qDebug()<<"成功监听:"<<this->port;
    }
}
/*
 * 当有新的用户连接进来的时候 触发这个函数，参数是socket的描述符
 *  此函数 重构了父类QTcpServer类中的虚函数，当newConnection信号发射时，自动进入此函数
*/
void Server::incomingConnection(qintptr socketDescriptor)
{

    TcpClientSocket *tcpclientsocket = new TcpClientSocket(this->heart_beat_time,this);
    connect(tcpclientsocket,&TcpClientSocket::dataToServer,this,&Server::sendToAllClient);
    //    connect(ui_counter,SIGNAL(signal_DeskNotValid(Server *,int)),tcpclientsocket,SLOT(slotDeskNotValid(Server *,int)));
    //    connect(tcpclientsocket,&TcpClientSocket::aboutToClose,this,&Server::)


    connect(tcpclientsocket,SIGNAL(signal_discon(int)),this,SLOT(slotDisconnected(int)));

    tcpclientsocket->setSocketDescriptor(socketDescriptor);
    // 在服务器端中记录刚刚登陆进来的客户端
    TcpClientSocketList.append(tcpclientsocket);
    qDebug()<<socketDescriptor<<" online";

    qDebug()<<"online number:"<<TcpClientSocketList.size();


}
void Server::slotDisconnected(int descriptor)
{
    for(int i=0;i<TcpClientSocketList.count();i++)
    {
        TcpClientSocket *item = TcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            qDebug()<<descriptor<<" offline";
            TcpClientSocketList.removeAt(i);
            item->deleteLater();

            qDebug()<<"online number:"<<TcpClientSocketList.size();
            return;
        }
    }

}

void Server::sendToAllClient(QByteArray ba,int socketDesc)
{
    for(int i=0;i<TcpClientSocketList.count();i++)
    {
        TcpClientSocket *item = TcpClientSocketList.at(i);

        //是否屏蔽发送者
        if(include_sender!=0 || item->socketDescriptor()!=socketDesc){
//            qDebug()<<include_sender<<item->socketDescriptor()<<socketDesc;
            item->write(ba);
            item->flush();
          //item->waitForBytesWritten();
        }
        item->resetHeatBeatTime(heart_beat_time);
    }
}

